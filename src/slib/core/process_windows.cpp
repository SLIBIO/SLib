/*
 *   Copyright (c) 2008-2019 SLIBIO <https://github.com/SLIBIO>
 *
 *   Permission is hereby granted, free of charge, to any person obtaining a copy
 *   of this software and associated documentation files (the "Software"), to deal
 *   in the Software without restriction, including without limitation the rights
 *   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *   copies of the Software, and to permit persons to whom the Software is
 *   furnished to do so, subject to the following conditions:
 *
 *   The above copyright notice and this permission notice shall be included in
 *   all copies or substantial portions of the Software.
 *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *   THE SOFTWARE.
 */

#include "slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_WINDOWS)

#include "slib/core/process.h"

#include "slib/core/app.h"
#include "slib/core/file.h"
#include "slib/core/string_buffer.h"
#include "slib/core/platform_windows.h"

#include <process.h>

namespace slib
{

	namespace priv
	{
		namespace process
		{

#if !defined(SLIB_PLATFORM_IS_MOBILE)

			static sl_bool CreatePipe(HANDLE* pRead, HANDLE* pWrite)
			{
				SECURITY_ATTRIBUTES saAttr;
				saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
				saAttr.bInheritHandle = TRUE;
				saAttr.lpSecurityDescriptor = NULL;
				return ::CreatePipe(pRead, pWrite, &saAttr, 0) != 0;
			}
			
			static sl_bool Execute(const StringParam& _pathExecutable, const String* cmds, sl_uint32 nCmds, PROCESS_INFORMATION* pi, STARTUPINFOW* si, sl_bool flagInheritHandles)
			{
				StringCstr16 pathExecutable(_pathExecutable);
				String16 cmd;
				{
					StringBuffer16 sb;
					sb.addStatic(SLIB_UNICODE("\""));
					sb.addStatic(pathExecutable.getData(), pathExecutable.getLength());
					sb.addStatic(SLIB_UNICODE("\""));
					String args = Application::buildCommandLine(cmds, nCmds);
					if (args.isNotEmpty()) {
						sb.addStatic(SLIB_UNICODE(" "));
						sb.add(String16::from(args));
					}
					cmd = sb.merge();
				}
				return CreateProcessW(
					(LPCWSTR)(pathExecutable.getData()),
					(LPWSTR)(cmd.getData()),
					NULL, // process security attributes
					NULL, // thread security attributes
					flagInheritHandles, // handles are inherited,
					NORMAL_PRIORITY_CLASS,
					NULL, // Environment (uses parent's environment)
					NULL, // Current Directory (uses parent's current directory)
					si,
					pi) != 0;
			}

			class ProcessStream : public Stream
			{
			public:
				HANDLE m_hRead;
				HANDLE m_hWrite;

			public:
				ProcessStream()
				{
					m_hRead = INVALID_HANDLE_VALUE;
					m_hWrite = INVALID_HANDLE_VALUE;
				}

				~ProcessStream()
				{
					_close();
				}

			public:
				void close() override
				{
					ObjectLocker lock(this);
					_close();
				}

				sl_int32 read32(void* buf, sl_uint32 size) override
				{
					HANDLE handle = m_hRead;
					if (handle != INVALID_HANDLE_VALUE) {
						if (size == 0) {
							return 0;
						}
						sl_uint32 ret = 0;
						if (ReadFile(handle, buf, size, (DWORD*)&ret, NULL)) {
							if (ret > 0) {
								return ret;
							}
						}
					}
					close();
					return -1;
				}

				sl_int32 write32(const void* buf, sl_uint32 size) override
				{
					HANDLE handle = m_hWrite;
					if (handle != INVALID_HANDLE_VALUE) {
						if (size == 0) {
							return 0;
						}
						sl_uint32 ret = 0;
						if (WriteFile(handle, (LPVOID)buf, size, (DWORD*)&ret, NULL)) {
							if (ret > 0) {
								return ret;
							}
						}
					}
					close();
					return -1;
				}

				void _close()
				{
					if (m_hRead >= 0) {
						CloseHandle(m_hRead);
						m_hRead = INVALID_HANDLE_VALUE;
					}
					if (m_hWrite >= 0) {
						CloseHandle(m_hWrite);
						m_hWrite = INVALID_HANDLE_VALUE;
					}
				}

			};

			class ProcessImpl : public Process
			{
			private:
				HANDLE m_hProcess;
				Ref<ProcessStream> m_stream;

			protected:
				ProcessImpl()
				{
					m_hProcess = INVALID_HANDLE_VALUE;
				}

				~ProcessImpl()
				{
					close();
				}

			public:
				static Ref<ProcessImpl> create(const StringParam& pathExecutable, const String* strArguments, sl_uint32 nArguments)
				{
					HANDLE hStdinRead, hStdinWrite, hStdoutRead, hStdoutWrite;
					if (CreatePipe(&hStdinRead, &hStdinWrite)) {
						SetHandleInformation(hStdinWrite, HANDLE_FLAG_INHERIT, 0);
						if (CreatePipe(&hStdoutRead, &hStdoutWrite)) {
							SetHandleInformation(hStdoutRead, HANDLE_FLAG_INHERIT, 0);
							PROCESS_INFORMATION pi;
							Base::zeroMemory(&pi, sizeof(pi));
							STARTUPINFOW si;
							Base::zeroMemory(&si, sizeof(si));
							si.cb = sizeof(si);
							si.hStdInput = hStdinRead;
							si.hStdOutput = hStdoutWrite;
							si.hStdError = hStdoutWrite;
							si.dwFlags = STARTF_USESTDHANDLES;
							if (Execute(pathExecutable, strArguments, nArguments, &pi, &si, sl_true)) {
								CloseHandle(pi.hThread);
								Ref<ProcessImpl> ret = new ProcessImpl;
								ret->m_hProcess = pi.hProcess;
								Ref<ProcessStream> stream = new ProcessStream;
								stream->m_hRead = hStdoutRead;
								stream->m_hWrite = hStdinWrite;
								ret->m_stream = stream;
								return ret;
							}
							CloseHandle(hStdoutRead);
							CloseHandle(hStdoutWrite);
						}
						CloseHandle(hStdinRead);
						CloseHandle(hStdinWrite);
					}
					return sl_null;
				}

			public:
				void terminate() override
				{
					ObjectLocker lock(this);
					m_stream->close();
					if (m_hProcess != INVALID_HANDLE_VALUE) {
						HANDLE handle = m_hProcess;
						m_hProcess = INVALID_HANDLE_VALUE;
						lock.unlock();
						TerminateProcess(handle, 0);
						CloseHandle(handle);
						m_status = ProcessStatus::Terminated;
					}
				}

				void kill() override
				{
					terminate();
				}

				void wait() override
				{
					ObjectLocker lock(this);
					if (m_hProcess != INVALID_HANDLE_VALUE) {
						HANDLE handle = m_hProcess;
						m_hProcess = INVALID_HANDLE_VALUE;
						lock.unlock();
						DWORD ret = WaitForSingleObject(handle, INFINITE);
						if (ret == WAIT_OBJECT_0) {
							m_status = ProcessStatus::Exited;
							DWORD code = 0;
							if (GetExitCodeProcess(handle, &code)) {
								m_exitStatus = (int)code;
							}
						} else {
							m_status = ProcessStatus::Unknown;
						}
						CloseHandle(handle);
						m_stream->close();
					}
				}

				Ref<Stream> getStream() override
				{
					return Ref<Stream>::from(m_stream);
				}

				void close()
				{
					if (m_hProcess != INVALID_HANDLE_VALUE) {
						CloseHandle(m_hProcess);
						m_hProcess = INVALID_HANDLE_VALUE;
					}
				}

			};

#endif

		}
	}

	using namespace priv::process;

	sl_uint32 Process::getCurrentProcessId()
	{
		return (sl_uint32)(GetCurrentProcessId());
	}

#if !defined(SLIB_PLATFORM_IS_MOBILE)
	Ref<Process> Process::open(const StringParam& pathExecutable, const String* arguments, sl_uint32 nArguments)
	{
		return Ref<Process>::from(ProcessImpl::create(pathExecutable, arguments, nArguments));
	}

	sl_bool Process::run(const StringParam& pathExecutable, const String* strArguments, sl_uint32 nArguments)
	{		
		PROCESS_INFORMATION pi;
		ZeroMemory(&pi, sizeof(pi));

		STARTUPINFOW si;
		ZeroMemory(&si, sizeof(si));
		si.dwFlags = STARTF_USESTDHANDLES;
		si.cb = sizeof(si);

		if (Execute(pathExecutable, strArguments, nArguments, &pi, &si, sl_false)) {
			CloseHandle(pi.hProcess);
			CloseHandle(pi.hThread);
			return sl_true;
		}
		return sl_false;
	}

	void Process::runAsAdmin(const StringParam& pathExecutable, const String* strArguments, sl_uint32 nArguments)
	{
		Windows::ShellExecuteParam param;
		param.runAsAdmin = sl_true;
		param.path = pathExecutable;
		param.params = Application::buildCommandLine(strArguments, nArguments);
		Windows::shellExecute(param);
	}

	sl_bool Process::isAdmin()
	{
		return Windows::isCurrentProcessRunAsAdmin();
	}

	void Process::exec(const StringParam& _pathExecutable, const String* strArguments, sl_uint32 nArguments)
	{
#if defined(SLIB_PLATFORM_IS_WIN32)
		StringCstr pathExecutable(_pathExecutable);
		char* exe = pathExecutable.getData();
		char* args[1024];
		args[0] = exe;
		if (nArguments > 1020) {
			nArguments = 1020;
		}
		for (sl_size i = 0; i < nArguments; i++) {
			args[i + 1] = strArguments[i].getData();
		}
		args[nArguments + 1] = 0;
		_execvp(exe, args);
		::exit(1);
#endif
	}

	void Process::exit(int code)
	{
#if defined(SLIB_PLATFORM_IS_WIN32)
		::exit(code);
#endif
	}

#endif

}

#endif
