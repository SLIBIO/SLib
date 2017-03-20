/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "../../../inc/slib/media/dsound.h"

#if defined(SLIB_PLATFORM_IS_WIN32)

#include <initguid.h>
#include <Windows.h>
#include <MMSystem.h>
#include <dsound.h>

#include "../../../inc/slib/core/platform_windows.h"
#include "../../../inc/slib/core/log.h"
#include "../../../inc/slib/core/thread.h"

#pragma comment(lib, "dsound.lib")

namespace slib
{

	class _DirectSound_AudioRecorder : public AudioRecorder
	{
	public:
		IDirectSoundCapture8* m_device;
		IDirectSoundCaptureBuffer8* m_buffer;
		sl_uint32 m_nSamplesFrame;
		HANDLE m_events[3];
		Ref<Thread> m_thread;
		sl_bool m_flagRunning;
		sl_bool m_flagOpened;

	public:
		_DirectSound_AudioRecorder()
		{
			m_flagOpened = sl_true;
			m_flagRunning = sl_false;
			m_events[2] = ::CreateEventW(NULL, FALSE, FALSE, NULL);
		}

		~_DirectSound_AudioRecorder()
		{
			release();
		}
		
	public:
		static void logError(String text)
		{
			LogError("AudioRecorder", text);
		}
		
		static Ref<_DirectSound_AudioRecorder> create(const AudioRecorderParam& param)
		{
			if (param.channelsCount != 1 && param.channelsCount != 2) {
				return sl_null;
			}
			
			::CoInitializeEx(NULL, COINIT_MULTITHREADED);
			
			String deviceID = param.deviceId;
			GUID guid;
			if (deviceID.isEmpty()) {
				guid = DSDEVID_DefaultCapture;
			} else {
				ListLocker<DeviceProperty> props(queryDeviceInfos());
				sl_size i = 0;
				for (; i < props.count; i++) {
					if (deviceID == props[i].szGuid) {
						guid = props[i].guid;
						break;
					}
				}
				if (i == props.count) {
					logError("Failed to find capture device");
					return sl_null;
				}
			}
			
			LPDIRECTSOUNDCAPTURE8 device = NULL;
			HRESULT hr = DirectSoundCaptureCreate8(&guid, &device, NULL);
			if (FAILED(hr)) {
				if (hr == DSERR_ALLOCATED) {
					logError("Dsound capture device is already used");
				} else {
					logError("Can not create dsound capture device");
				}
				return sl_null;
			}
			
			WAVEFORMATEX wf;
			wf.wFormatTag = WAVE_FORMAT_PCM;
			wf.nChannels = param.channelsCount;
			wf.wBitsPerSample = 16;
			wf.nSamplesPerSec = param.samplesPerSecond;
			wf.nBlockAlign = wf.wBitsPerSample * wf.nChannels / 8;
			wf.nAvgBytesPerSec = wf.nSamplesPerSec * wf.nBlockAlign;
			wf.cbSize = 0;
			
			sl_uint32 samplesPerFrame = wf.nSamplesPerSec * param.frameLengthInMilliseconds / 1000 * param.channelsCount;
			sl_uint32 sizeBuffer = samplesPerFrame * wf.nBlockAlign * 2;
			DSCBUFFERDESC desc;
			desc.dwSize = sizeof(desc);
			desc.dwBufferBytes = sizeBuffer;
			desc.dwReserved = 0;
			desc.lpwfxFormat = &wf;
			DSCEFFECTDESC effects[2];
			if (0) { // use voice mode
				desc.dwFlags = DSCBCAPS_CTRLFX;
				desc.dwFXCount = 2;
				desc.lpDSCFXDesc = effects;
				effects[0].dwSize = sizeof(DSCEFFECTDESC);
				effects[0].dwFlags = DSCFX_LOCSOFTWARE;
				effects[0].guidDSCFXClass = GUID_DSCFX_CLASS_AEC;
				effects[0].guidDSCFXInstance = GUID_DSCFX_SYSTEM_AEC;
				effects[0].dwReserved1 = 0;
				effects[0].dwReserved2 = 0;
				effects[1].dwSize = sizeof(DSCEFFECTDESC);
				effects[1].dwFlags = DSCFX_LOCSOFTWARE;
				effects[1].guidDSCFXClass = GUID_DSCFX_CLASS_NS;
				effects[1].guidDSCFXInstance = GUID_DSCFX_SYSTEM_NS;
				effects[1].dwReserved1 = 0;
				effects[1].dwReserved2 = 0;
			} else {
				desc.dwFlags = 0;
				desc.dwFXCount = 0;
				desc.lpDSCFXDesc = NULL;
			}
			
			HANDLE hEvent0 = ::CreateEventW(NULL, FALSE, FALSE, NULL);
			HANDLE hEvent1 = ::CreateEventW(NULL, FALSE, FALSE, NULL);
			
			IDirectSoundCaptureBuffer* dsbuf;
			IDirectSoundCaptureBuffer8* buffer = sl_null;
			hr = device->CreateCaptureBuffer(&desc, &dsbuf, NULL);
			if (SUCCEEDED(hr)) {
				dsbuf->QueryInterface(IID_IDirectSoundCaptureBuffer8, (LPVOID*)&buffer);
				dsbuf->Release();
				if (buffer) {
					IDirectSoundNotify8* notify;
					hr = buffer->QueryInterface(IID_IDirectSoundNotify8, (LPVOID*)&notify);
					if (SUCCEEDED(hr)) {
						DSBPOSITIONNOTIFY pos[2];
						pos[0].dwOffset = (sizeBuffer / 2) - 1;
						pos[0].hEventNotify = hEvent0;
						pos[1].dwOffset = sizeBuffer - 1;
						pos[1].hEventNotify = hEvent1;
						hr = notify->SetNotificationPositions(2, pos);
						if (SUCCEEDED(hr)) {
							hr = buffer->Start(DSCBSTART_LOOPING);
							if (SUCCEEDED(hr)) {
								Ref<_DirectSound_AudioRecorder> ret = new _DirectSound_AudioRecorder();
								if (ret.isNotNull()) {
									ret->m_device = device;
									ret->m_buffer = buffer;
									ret->m_events[0] = hEvent0;
									ret->m_events[1] = hEvent1;
									ret->m_nSamplesFrame = samplesPerFrame;
									
									ret->_init(param);
									
									if (param.flagAutoStart) {
										ret->start();
									}
									return ret;
								}
							} else {
								logError("Failed to start dsound looping");
							}
						} else {
							logError("Failed to set dsound notify postions");
						}
						notify->Release();
					} else {
						logError("Failed to get dsound notify");
					}
					buffer->Release();
				} else {
					logError("Failed to get dsound buffer 8");
				}
			} else {
				logError("Failed to create dsound buffer");
			}
			
			device->Release();
			CloseHandle(hEvent0);
			CloseHandle(hEvent1);
			
			return sl_null;
		}
		
		void release()
		{
			ObjectLocker lock(this);
			if (!m_flagOpened) {
				return;
			}
			stop();
			m_flagOpened = sl_false;
			m_buffer->Release();
			m_buffer = sl_null;
			m_device->Release();
			m_device = sl_null;
			for (int i = 0; i < 3; i++) {
				CloseHandle(m_events[i]);
			}
		}
		
		sl_bool isOpened()
		{
			return m_flagOpened;
		}
		
		void start()
		{
			ObjectLocker lock(this);
			if (!m_flagOpened) {
				return;
			}
			if (m_flagRunning) {
				return;
			}
			m_thread = Thread::start(SLIB_FUNCTION_CLASS(_DirectSound_AudioRecorder, run, this));
			if (m_thread.isNotNull()) {
				m_flagRunning = sl_true;
			}
		}
		
		void stop()
		{
			ObjectLocker lock(this);
			if (!m_flagOpened) {
				return;
			}
			if (!m_flagRunning) {
				return;
			}
			m_flagRunning = sl_false;
			m_thread->finish();
			::SetEvent(m_events[2]);
			m_thread->finishAndWait();
			m_thread.setNull();
		}
		
		sl_bool isRunning()
		{
			return m_flagRunning;
		}
		
		struct DeviceProperty {
			GUID guid;
			String szGuid;
			String name;
			String description;
		};
		static List<DeviceProperty> queryDeviceInfos()
		{
			List<DeviceProperty> list;
			HRESULT hr = DirectSoundCaptureEnumerateW(DeviceEnumProc, (VOID*)&list);
			if (FAILED(hr)) {
				logError("Can not query device info");
			}
			return list;
		}
		
		static BOOL CALLBACK DeviceEnumProc(LPGUID lpGUID, LPCWSTR lpszDesc, LPCWSTR lpszDrvName, LPVOID lpContext)
		{
			List<DeviceProperty>& list = *((List<DeviceProperty>*)lpContext);
			DeviceProperty prop;
			if (lpGUID) {
				prop.guid = *lpGUID;
				prop.szGuid = Windows::getStringFromGUID(*lpGUID);
				prop.name = (sl_char16*)lpszDrvName;
				prop.description = (sl_char16*)lpszDesc;
				list.add_NoLock(prop);
			}
			return TRUE;
		}
		
		void onFrame(int no)
		{
			sl_uint32 offset = 0;
			sl_uint32 size = m_nSamplesFrame * 2;
			if (no) {
				offset = size;
			}
			LPVOID buf = NULL;
			DWORD dwSize = 0;
			m_buffer->Lock(offset, size, &buf, &dwSize, NULL, NULL, 0);
			if (buf && dwSize) {
				sl_uint32 n;
				if (dwSize > size) {
					n = size / 2;
				} else {
					n = dwSize / 2;
				}
				_processFrame((sl_int16*)buf, n);
				m_buffer->Unlock(buf, dwSize, NULL, NULL);
			}
		}
		
		void run()
		{
			::CoInitializeEx(NULL, COINIT_MULTITHREADED);
			while (Thread::isNotStoppingCurrent()) {
				DWORD dwWait = ::WaitForMultipleObjects(3, m_events, FALSE, INFINITE);
				if (dwWait >= WAIT_OBJECT_0 && dwWait < WAIT_OBJECT_0 + 2) {
					onFrame(dwWait - WAIT_OBJECT_0);
				}
			}
		}
	};

	Ref<AudioRecorder> DirectSound::createRecorder(const AudioRecorderParam& param)
	{
		return _DirectSound_AudioRecorder::create(param);
	}

	List<AudioRecorderInfo> DirectSound::getRecordersList()
	{
		List<AudioRecorderInfo> ret;
		ListElements<_DirectSound_AudioRecorder::DeviceProperty> props(_DirectSound_AudioRecorder::queryDeviceInfos());
		for (sl_size i = 0; i < props.count; i++) {
			_DirectSound_AudioRecorder::DeviceProperty& prop = props[i];
			AudioRecorderInfo info;
			info.id = prop.szGuid;
			info.name = prop.name;
			info.description = prop.description;
			ret.add_NoLock(info);
		}
		return ret;
	}

}

#else

namespace slib
{

	Ref<AudioRecorder> DirectSound::createRecorder(const AudioRecorderParam& param)
	{
		return sl_null;
	}

	List<AudioRecorderInfo> DirectSound::getRecordersList()
	{
		return sl_null;
	}

}

#endif
