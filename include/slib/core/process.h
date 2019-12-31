/*
 *   Copyright (c) 2008-2018 SLIBIO <https://github.com/SLIBIO>
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

#ifndef CHECKHEADER_SLIB_CORE_PROCESS
#define CHECKHEADER_SLIB_CORE_PROCESS

#include "definition.h"

#include "string.h"
#include "io.h"

namespace slib
{
	
	enum class ProcessStatus
	{
		Running = 0,
		Exited = 1,
		Terminated = 2,
		Killed = 3,
		Unknown = 4
	};
	
	class SLIB_EXPORT Process : public Object
	{
		SLIB_DECLARE_OBJECT
		
	public:
		Process();
		
		~Process();
		
	public:
		static sl_uint32 getCurrentProcessId();
		
		static Ref<Process> open(const StringParam& pathExecutable, const String* arguments, sl_uint32 nArguments);

		static Ref<Process> open(const StringParam& pathExecutable, const ListParam<String>& arguments);

		static sl_bool run(const StringParam& pathExecutable, const String* arguments, sl_uint32 nArguments);

		static sl_bool run(const StringParam& pathExecutable, const ListParam<String>& arguments);

		static void runAsAdmin(const StringParam& pathExecutable, const String* arguments, sl_uint32 nArguments);
		
		static void runAsAdmin(const StringParam& pathExecutable, const ListParam<String>& arguments);
		
		// check administrative privilege (effective root user on Unix)
		static sl_bool isAdmin();
		
		static void exec(const StringParam& pathExecutable, const String* arguments, sl_uint32 nArguments);

		static void exec(const StringParam& pathExecutable, const ListParam<String>& arguments);
		
		static void exit(int code);
		
	public:
		virtual void terminate() = 0;

		virtual void kill() = 0;

		virtual void wait() = 0;
		
		virtual Ref<Stream> getStream() = 0;
		
	public:
		ProcessStatus getStatus();
		
		int getExitStatus();
		
	protected:
		ProcessStatus m_status;
		int m_exitStatus;
		
	};

}

#endif

