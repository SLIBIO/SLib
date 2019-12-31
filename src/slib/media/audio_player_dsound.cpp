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

#include "slib/media/dsound.h"

#if defined(SLIB_PLATFORM_IS_WIN32)

#include <initguid.h>
#include <Windows.h>
#include <MMSystem.h>
#include <dsound.h>

#include "slib/core/platform_windows.h"
#include "slib/core/log.h"
#include "slib/core/thread.h"

#pragma comment(lib, "dsound.lib")

#define NUM_PLAY_NOTIFICATIONS  2

namespace slib
{

	namespace priv
	{
		namespace dsound
		{

			class AudioPlayerBufferImpl : public AudioPlayerBuffer
			{
			public:
				LPDIRECTSOUND		m_ds;
				LPDIRECTSOUNDBUFFER m_dsBuffer;
				LPDIRECTSOUNDNOTIFY m_dsNotify;
				HANDLE              m_hNotificationEvents[2];

				sl_uint32 m_nSamplesFrame;
				sl_uint32 m_nBufferSize;
				sl_uint32 m_nOffsetNextWrite;
				sl_uint32 m_nNotifySize;

				Ref<Thread> m_thread;
				sl_bool m_flagRunning;
				sl_bool m_flagOpened;

			public:
				AudioPlayerBufferImpl()
				{
					m_nOffsetNextWrite = m_nNotifySize = 0;
					m_ds = NULL;
					m_dsBuffer = NULL;
					m_dsNotify = NULL;
					m_flagOpened = sl_true;
					m_flagRunning = sl_false;
					m_thread.setNull();
				}

				~AudioPlayerBufferImpl()
				{
					release();
				}

			public:
				static void logError(String text)
				{
					LogError("AudioPlayer", text);
				}

				static Ref<AudioPlayerBufferImpl> create(const AudioPlayerBufferParam& param, const GUID& deviceID)
				{
					if (param.channelsCount != 1 && param.channelsCount != 2) {
						return sl_null;
					}
					
					::CoInitializeEx(NULL, COINIT_MULTITHREADED);

					LPDIRECTSOUND ds;
					HRESULT hr = DirectSoundCreate(&deviceID, &ds, NULL);
					if (SUCCEEDED(hr)) {
						hr = ds->SetCooperativeLevel(::GetDesktopWindow(), DSSCL_NORMAL);
						if (SUCCEEDED(hr)) {
							WAVEFORMATEX wf;
							wf.wFormatTag = WAVE_FORMAT_PCM;
							wf.nChannels = param.channelsCount;
							wf.wBitsPerSample = 16;
							wf.nSamplesPerSec = param.samplesPerSecond;
							wf.nBlockAlign = wf.wBitsPerSample * wf.nChannels / 8;
							wf.nAvgBytesPerSec = wf.nSamplesPerSec * wf.nBlockAlign;
							wf.cbSize = 0;

							sl_uint32 samplesPerFrame = wf.nSamplesPerSec * param.frameLengthInMilliseconds / 1000;
							sl_uint32 sizeBuffer = samplesPerFrame * wf.nBlockAlign * 3;
							sl_int32 notifySize = sizeBuffer / NUM_PLAY_NOTIFICATIONS;

							DSBUFFERDESC desc;
							ZeroMemory(&desc, sizeof(DSBUFFERDESC));
							desc.dwSize = sizeof(DSBUFFERDESC);
							desc.dwFlags = DSBCAPS_CTRLPOSITIONNOTIFY | DSBCAPS_GETCURRENTPOSITION2 | DSBCAPS_GLOBALFOCUS;
							desc.dwBufferBytes = sizeBuffer;
							desc.lpwfxFormat = &wf;

							LPDIRECTSOUNDBUFFER dsBuffer;
							HANDLE hNotificationEvents[2];
							hNotificationEvents[0] = ::CreateEventW(NULL, FALSE, FALSE, NULL);
							hNotificationEvents[1] = ::CreateEventW(NULL, FALSE, FALSE, NULL);

							hr = ds->CreateSoundBuffer(&desc, &dsBuffer, NULL);
							if (SUCCEEDED(hr)) {
								LPDIRECTSOUNDNOTIFY dsNotify;
								hr = dsBuffer->QueryInterface(IID_IDirectSoundNotify, (VOID**)&dsNotify);
								if (SUCCEEDED(hr)) {
									DSBPOSITIONNOTIFY   dsPosNotify[NUM_PLAY_NOTIFICATIONS + 1];
									sl_int32 notifyIndex = 0;

									for (notifyIndex = 0; notifyIndex < NUM_PLAY_NOTIFICATIONS; notifyIndex++) {
										dsPosNotify[notifyIndex].dwOffset = (notifySize * notifyIndex) + notifySize - 1;
										dsPosNotify[notifyIndex].hEventNotify = hNotificationEvents[0];
									}
									dsPosNotify[notifyIndex].dwOffset = DSBPN_OFFSETSTOP;
									dsPosNotify[notifyIndex].hEventNotify = hNotificationEvents[1];

									hr = dsNotify->SetNotificationPositions(NUM_PLAY_NOTIFICATIONS + 1, dsPosNotify);
									if (SUCCEEDED(hr)) {
										hr = dsBuffer->Play(0, 0, DSBPLAY_LOOPING);
										if (SUCCEEDED(hr)) {
											Ref<AudioPlayerBufferImpl> ret = new AudioPlayerBufferImpl();
											if (ret.isNotNull()) {
												ret->m_ds = ds;
												ret->m_dsBuffer = dsBuffer;
												ret->m_dsNotify = dsNotify;
												ret->m_nNotifySize = notifySize;
												ret->m_hNotificationEvents[0] = hNotificationEvents[0];
												ret->m_hNotificationEvents[1] = hNotificationEvents[1];
												ret->m_nSamplesFrame = samplesPerFrame;
												ret->m_nBufferSize = sizeBuffer;
												
												ret->_init(param);
												
												if (param.flagAutoStart) {
													ret->start();
												}
												return ret;
											}
										} else {
											logError("Failed to start direct sound looping");
										}
									} else {
										logError("Failed to set dsound notify positions");
									}
								} else {
									logError("Failed to get dsound notify");
								}
								dsBuffer->Release();
							} else {
								logError("Failed to create dsound buffer 8");
							}
							ds->Release();
							CloseHandle(hNotificationEvents[0]);
							CloseHandle(hNotificationEvents[1]);
						} else {
							logError("Direct sound set CooperativeLevel failed.");
						}
					} else {
						if (hr == DSERR_ALLOCATED) {
							logError("Direct sound playback device is already used");
						} else {
							logError("Can not create direct sound playback device");
						}
					}
					return sl_null;
				}

				void release() override
				{
					ObjectLocker lock(this);
					if (!m_flagOpened) {
						return;
					}
					stop();
					m_flagOpened = sl_false;
					m_dsBuffer->Release();
					m_dsBuffer = NULL;

					m_dsNotify->Release();
					m_dsNotify = NULL;

					m_ds->Release();
					m_ds = NULL;

					for (int i = 0; i < 2; i++) {
						CloseHandle(m_hNotificationEvents[i]);
					}
				}

				sl_bool isOpened() override
				{
					return m_flagOpened;
				}

				void start() override
				{
					ObjectLocker lock(this);
					if (!m_flagOpened) {
						return;
					}
					if (m_flagRunning) {
						return;
					}
					m_thread = Thread::start(SLIB_FUNCTION_MEMBER(AudioPlayerBufferImpl, run, this));
					if (m_thread.isNotNull()) {
						m_flagRunning = sl_true;
					}
				}

				void stop() override
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
					::SetEvent(m_hNotificationEvents[1]);
					m_thread->finishAndWait();
					m_thread.setNull();
				}

				sl_bool isRunning() override
				{
					return m_flagRunning;
				}

				void run()
				{
					::CoInitializeEx(NULL, COINIT_MULTITHREADED);
					Ref<Thread> thread = Thread::getCurrent();
					while (thread.isNull() || thread->isNotStopping()) {
						DWORD dwWait = ::WaitForMultipleObjects(2, m_hNotificationEvents, FALSE, INFINITE);
						if (dwWait == WAIT_OBJECT_0) {
							onFrame();
						} else {
							m_dsBuffer->Stop();
						}
					}
				}

				void onFrame()
				{
					VOID* pbBuffer = NULL;
					DWORD bufferSize;

					HRESULT hr = m_dsBuffer->Lock(m_nOffsetNextWrite, m_nNotifySize, &pbBuffer, &bufferSize, NULL, NULL, 0L);
					if (SUCCEEDED(hr)) {
						sl_uint32 nSamples = bufferSize / 2;
						sl_int16* s = (sl_int16*)pbBuffer;
						
						_processFrame(s, nSamples);
						
						m_nOffsetNextWrite += bufferSize;
						m_nOffsetNextWrite %= m_nBufferSize;
						m_dsBuffer->Unlock(pbBuffer, bufferSize, NULL, NULL);
					}
				}

			};

			class AudioPlayerImpl : public AudioPlayer
			{
			public:
				GUID m_deviceID;

			public:
				AudioPlayerImpl()
				{
				}

				~AudioPlayerImpl()
				{
				}

			public:
				static void logError(String text)
				{
					LogError("AudioPlayer", text);
				}

				static Ref<AudioPlayerImpl> create(const AudioPlayerParam& param)
				{
					Ref<AudioPlayerImpl> ret = new AudioPlayerImpl();
					GUID gid;
					String deviceID = param.deviceId;
					if (deviceID.isEmpty()) {
						gid = DSDEVID_DefaultPlayback;
					} else {
						ListLocker<DeviceProperty> props(queryDeviceInfos());
						sl_size i = 0;
						for (; i < props.count; i++) {
							if (deviceID == props[i].szGuid) {
								gid = props[i].guid;
								break;
							}
						}
						if (i == props.count) {
							logError("Failed to find capture device");
						}
					}
					ret->m_deviceID = gid;
					return ret;
				}

				Ref<AudioPlayerBuffer> createBuffer(const AudioPlayerBufferParam& param) override
				{
					return AudioPlayerBufferImpl::create(param, m_deviceID);
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
					HRESULT hr = DirectSoundEnumerateW(DeviceEnumProc, (VOID*)&list);
					if (FAILED(hr)) {
						logError("Can not query player device info");
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
						prop.name = String::create(lpszDrvName);
						prop.description = String::create(lpszDesc);
						list.add_NoLock(prop);
					}
					return TRUE;
				}
			};

		}
	}

	using namespace priv::dsound;

	Ref<AudioPlayer> DirectSound::createPlayer(const AudioPlayerParam& param)
	{
		return AudioPlayerImpl::create(param);
	}

	List<AudioPlayerInfo> DirectSound::getPlayersList()
	{
		List<AudioPlayerInfo> ret;
		ListElements<AudioPlayerImpl::DeviceProperty> props(AudioPlayerImpl::queryDeviceInfos());
		for (sl_size i = 0; i < props.count; i++) {
			AudioPlayerImpl::DeviceProperty& prop = props[i];
			AudioPlayerInfo info;
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

	Ref<AudioPlayer> DirectSound::createPlayer(const AudioPlayerParam& param)
	{
		return sl_null;
	}

	List<AudioPlayerInfo> DirectSound::getPlayersList()
	{
		return sl_null;
	}

}

#endif
