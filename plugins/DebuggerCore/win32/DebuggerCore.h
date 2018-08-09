/*
Copyright (C) 2006 - 2015 Evan Teran
                          evan.teran@gmail.com

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef DEBUGGERCORE_20090529_H_
#define DEBUGGERCORE_20090529_H_

#include "DebuggerCoreBase.h"
#include "IRegion.h"
#include "Module.h"
#include <QSet>

namespace DebuggerCorePlugin {

class DebuggerCore : public DebuggerCoreBase {
	Q_OBJECT
    Q_PLUGIN_METADATA(IID "edb.IDebugger/1.0")
	Q_INTERFACES(IDebugger)
	Q_CLASSINFO("author", "Evan Teran")
	Q_CLASSINFO("url", "http://www.codef00.com")

public:
	DebuggerCore();
	~DebuggerCore() override;

public:
	bool has_extension(quint64 ext) const override;
	edb::address_t page_size() const override;
	std::size_t pointer_size() const override {
		return sizeof(void*);
	};
	std::shared_ptr<IDebugEvent> wait_debug_event(int msecs) override;
	Status attach(edb::pid_t pid) override;
	Status detach() override;
	void kill() override;
	void pause() override;
	void resume(edb::EVENT_STATUS status) override;
	void step(edb::EVENT_STATUS status) override;
	void get_state(State *state) override;
	void set_state(const State &state) override;
	Status open(const QString &path, const QString &cwd, const QList<QByteArray> &args, const QString &tty) override;

	MeansOfCapture last_means_of_capture() const override {
		qDebug("TODO: Implement DebuggerCore::last_means_of_capture");
		return MeansOfCapture::NeverCaptured;
	}

	bool read_pages(edb::address_t address, void *buf, std::size_t count) override;
	bool read_bytes(edb::address_t address, void *buf, std::size_t len) override;
	bool write_bytes(edb::address_t address, const void *buf, std::size_t len) override;
	int sys_pointer_size() const override;
	QMap<qlonglong, QString> exceptions() const override;
	QString exceptionName(qlonglong value) override {
		qDebug("TODO: Implement DebuggerCore::exceptionName"); return "";
	}
	
	qlonglong exceptionValue(const QString &name) override {
		qDebug("TODO: Implement DebuggerCore::exceptionValue"); return 0;
	}

public:
	// thread support stuff (optional)
	QList<edb::tid_t> thread_ids() const override   { return threads_.toList(); }
	edb::tid_t active_thread() const override       { return active_thread_; }
	void set_active_thread(edb::tid_t tid) override { Q_ASSERT(threads_.contains(tid)); active_thread_ = tid; }

public:
	QList<std::shared_ptr<IRegion>> memory_regions() const;
	edb::address_t process_code_address() const;
	edb::address_t process_data_address() const;

public:
	// process properties
	QList<QByteArray> process_args(edb::pid_t pid) const override;
	QString process_exe(edb::pid_t pid) const override;
	QString process_cwd(edb::pid_t pid) const override;
	edb::pid_t parent_pid(edb::pid_t pid) const override;
	QDateTime process_start(edb::pid_t pid) const override;
	quint64 cpu_type() const override;

	CPUMode cpu_mode() const override {
		qDebug("TODO: Implement DebuggerCore::cpu_mode");
		return CPUMode::Unknown;
	}

public:
	IState *create_state() const override;

private:
	QMap<edb::pid_t, std::shared_ptr<IProcess> > enumerate_processes() const override;
	QList<Module> loaded_modules() const override;

public:
	QString stack_pointer() const override;
	QString frame_pointer() const override;
	QString instruction_pointer() const override;
	QString flag_register() const override {
		qDebug("TODO: Implement DebuggerCore::flag_register");
		return "";
	}

public:
	QString format_pointer(edb::address_t address) const override;

public:
	IProcess *process() const override {
		qDebug("TODO: Implement DebuggerCore::process");
		return nullptr;
	}

public:
	// NOTE: win32 only stuff here!
	edb::address_t start_address;
	edb::address_t image_base;

private:
	bool attached() { return DebuggerCoreBase::attached() && process_handle_ != 0; }

private:
	edb::address_t   page_size_;
	HANDLE           process_handle_;
	QSet<edb::tid_t> threads_;
	edb::tid_t       active_thread_;
};

}

#endif
