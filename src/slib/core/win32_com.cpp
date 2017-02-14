#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_WINDOWS)

#include "../../../inc/slib/core/win32_com.h"

namespace slib
{

	Memory Win32_COM::readAllBytesFromStream(IStream* stream)
	{
		Memory ret;
		HRESULT hr;

		STATSTG stat;
		Base::zeroMemory(&stat, sizeof(stat));

		if (ret.isEmpty()) {
			hr = stream->Stat(&stat, STATFLAG_NONAME);
			if (hr == S_OK) {
				LARGE_INTEGER n;
				n.QuadPart = 0;
				stream->Seek(n, STREAM_SEEK_SET, NULL);
				ULONG size = (ULONG)(stat.cbSize.QuadPart);
				if (size > 0) {
					ret = Memory::create(size);
					if (ret.isNotEmpty()) {
						ULONG nRead = 0;
						hr = stream->Read(ret.getData(), size, &nRead);
						if (hr == S_OK && nRead == size) {
							return ret;
						}
					}
					ret.setNull();
				}
			}
		}
		return ret;
	}

}

#endif
