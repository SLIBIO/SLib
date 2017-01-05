#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_IOS)

#include "../../../inc/slib/media/audio_player.h"

#include "../../../inc/slib/core/log.h"
#include "../../../inc/slib/core/ptr.h"
#include "../../../inc/slib/core/platform_apple.h"

#import <Foundation/Foundation.h>
#import <AudioUnit/AudioUnit.h>
#import <AudioToolbox/AudioToolbox.h>
#import <AVFoundation/AVFoundation.h>


@interface _iOS_AVPlayer_Observer : NSObject
{
	AVPlayerStatus mStatus;
	sl_bool mIsPlaying;
	AVPlayer* mAudioPlayer;
	AVPlayerItem* mAudioItem;
	slib::Mutex mLock;
	slib::WeakRef<slib::AudioPlayerControl> mControl;
}

-(id) initWithURL:(NSString* ) url;
-(void) setStatus:(AVPlayerStatus) status;
-(AVPlayerStatus) getStatus;
-(sl_bool) isPlaying;
-(void) pause;
-(void) play;
-(void) stop;
- (void)playerItemDidReachEnd:(NSNotification *)notification;

@end

@implementation _iOS_AVPlayer_Observer
-(id) initWithURL:(NSString *)url control:(slib::AudioPlayerControl*) control
{
	self = [super init];
	mStatus = AVPlayerStatusUnknown;
	mIsPlaying = sl_false;
	
	NSURL* _url = [NSURL URLWithString:url];
	mAudioItem = [AVPlayerItem playerItemWithURL:_url];
	mAudioPlayer = [AVPlayer playerWithPlayerItem:mAudioItem];
	
	if (mAudioPlayer != nil) {
		[mAudioPlayer addObserver:self forKeyPath:@"status" options:0 context:nil];
		[[NSNotificationCenter defaultCenter] addObserver:self
												 selector:@selector(playerItemDidReachEnd:)
													 name:AVPlayerItemDidPlayToEndTimeNotification
												   object:mAudioItem];
	}
	
	mControl = control;
	
	return self;
}

-(void) dealloc
{
	[mAudioPlayer removeObserver:self forKeyPath:@"status"];
	[[NSNotificationCenter defaultCenter] removeObserver:self name:AVPlayerItemDidPlayToEndTimeNotification object:mAudioItem];
}

-(void) setStatus:(AVPlayerStatus)status
{
	mStatus = status;
}

-(AVPlayerStatus) getStatus
{
	return mStatus;
}

-(sl_bool) isPlaying
{
	return mIsPlaying;
}

-(void) play
{
	slib::MutexLocker lock(&mLock);
	if (mStatus == AVPlayerStatusFailed) {
		return;
	}
	if (mStatus == AVPlayerStatusReadyToPlay) {
		[mAudioPlayer play];
	}
	mIsPlaying = sl_true;
}

-(void) pause
{
	slib::MutexLocker lock(&mLock);
	if (mStatus == AVPlayerStatusFailed) {
		return;		
	}
	if (mStatus == AVPlayerStatusReadyToPlay) {
		[mAudioPlayer pause];
	}
	mIsPlaying = sl_false;
}

-(void) stop
{
	slib::MutexLocker lock(&mLock);
	[mAudioItem cancelPendingSeeks];
	[mAudioItem.asset cancelLoading];
	[self processStopped];
}

- (void)playerItemDidReachEnd:(NSNotification *)notification {
	slib::MutexLocker lock(&mLock);
	[self processStopped];
}

-(void)observeValueForKeyPath:(NSString *)keyPath ofObject:(id)object change:(NSDictionary *)change context:(void *)contex
{
	slib::MutexLocker lock(&mLock);
	if (mAudioPlayer != object) {
		return;
	}
	if ([keyPath isEqualToString:@"status"]) {
		[self setStatus:mAudioPlayer.status];
		
		if (mStatus == AVPlayerStatusFailed) {
			[self processStopped];
		} else if (mStatus == AVPlayerStatusReadyToPlay) {
			if (mIsPlaying) {
				[mAudioPlayer play];
			}
		} else if (mStatus == AVPlayerStatusUnknown) {
			mIsPlaying = false;
		}
	}
}

-(void)processStopped
{
	mIsPlaying = sl_false;
	slib::Ref<slib::AudioPlayerControl> control = mControl;
	if (control.isNotNull()) {
		control->_removeFromMap();
	}
}

@end

SLIB_MEDIA_NAMESPACE_BEGIN

class _iOS_AudioPlayerControl : public AudioPlayerControl
{
public:
	_iOS_AudioPlayerControl()
	{
	}
	
	_iOS_AudioPlayerControl(const String& url)
	{
		SLIB_REFERABLE_CONSTRUCTOR
		mAVObserver = [[_iOS_AVPlayer_Observer alloc] initWithURL:Apple::getNSStringFromString(url) control:this];
	}
	
	_iOS_AudioPlayerControl(const Memory& data)
	{
	}
	
	~_iOS_AudioPlayerControl()
	{
		NSLog(@"completed");
	}
	
public:
	void start()
	{
		ObjectLocker lock(this);
		if (mAVObserver != nil) {
			[mAVObserver play];
		}
	}
	
	void pause()
	{
		ObjectLocker lock(this);
		if (mAVObserver != nil) {
			[mAVObserver pause];
		}
	}
	
	void stop()
	{
		ObjectLocker lock(this);
		if (mAVObserver != nil) {
			[mAVObserver stop];
		}
	}
	
	sl_bool isRunning()
	{
		ObjectLocker lock(this);
		if (mAVObserver != nil) {
			return [mAVObserver isPlaying];
		}
		return sl_false;
	}
	
private:
	_iOS_AVPlayer_Observer* mAVObserver;
};


class _iOS_AudioPlayerBuffer : public AudioPlayerBuffer
{
public:
	sl_bool m_flagInitialized;
	sl_bool m_flagOpened;
	sl_bool m_flagRunning;
	
	AudioComponentInstance m_audioUnitOutput;
	AudioConverterRef m_converter;
	
	AudioStreamBasicDescription m_formatSrc;
	AudioStreamBasicDescription m_formatDst;

public:
	_iOS_AudioPlayerBuffer()
	{
		m_flagInitialized = sl_false;
		m_flagOpened = sl_true;
		m_flagRunning = sl_false;
	}

	~_iOS_AudioPlayerBuffer()
	{
		release();
	}
	
public:
	static void logError(String text)
	{
		SLIB_LOG_ERROR("AudioPlayer", text);
	}
	
	static Ref<_iOS_AudioPlayerBuffer> create(const AudioPlayerBufferParam& param)
	{
		Ref<_iOS_AudioPlayerBuffer> ret;
		
		if (param.channelsCount != 1 && param.channelsCount != 2) {
			return ret;
		}
		
		AudioComponentDescription desc;
		desc.componentType = kAudioUnitType_Output;
		desc.componentSubType = kAudioUnitSubType_RemoteIO;
		desc.componentManufacturer = kAudioUnitManufacturer_Apple;
		desc.componentFlags = 0;
		desc.componentFlagsMask = 0;
		
		AudioComponent comp = AudioComponentFindNext(NULL, &desc);
		if (! comp) {
			logError("Failed to find audio component");
			return ret;
		}
		
		OSStatus result;
		AudioComponentInstance audioUnitOutput;
		result = AudioComponentInstanceNew(comp, &audioUnitOutput);
		if (result == noErr) {
			UInt32 enableOutput = 1;
			AudioUnitElement bus0 = 0;
			result = AudioUnitSetProperty(audioUnitOutput,
										  kAudioOutputUnitProperty_EnableIO,
										  kAudioUnitScope_Output,
										  bus0, // output bus
										  &enableOutput,
										  sizeof(enableOutput));
			if (result == noErr) {
				
				AudioStreamBasicDescription formatDst;
				UInt32 size = sizeof(formatDst);
				
				result = AudioUnitGetProperty(audioUnitOutput, kAudioUnitProperty_StreamFormat, kAudioUnitScope_Input, bus0, &formatDst, &size);
				if (result == noErr) {
					
#if defined(SLIB_PLATFORM_IS_IOS_SIMULATOR)
					formatDst.mSampleRate = 44100;
#else
					formatDst.mSampleRate = [[AVAudioSession sharedInstance] sampleRate];
#endif
					
					AudioStreamBasicDescription formatSrc;
					formatSrc.mFormatID = kAudioFormatLinearPCM;
					formatSrc.mFormatFlags = kLinearPCMFormatFlagIsSignedInteger | kLinearPCMFormatFlagIsPacked;
					formatSrc.mSampleRate = param.samplesPerSecond;
					formatSrc.mBitsPerChannel = 16;
					formatSrc.mChannelsPerFrame = (UInt32)(param.channelsCount);
					formatSrc.mBytesPerFrame = formatSrc.mChannelsPerFrame * formatSrc.mBitsPerChannel / 8;
					formatSrc.mFramesPerPacket = 1;
					formatSrc.mBytesPerPacket = formatSrc.mBytesPerFrame * formatSrc.mFramesPerPacket;
					
					AudioConverterRef converter;
					if (AudioConverterNew(&formatSrc, &formatDst, &converter) == noErr) {
						
						ret = new _iOS_AudioPlayerBuffer();
						
						if (ret.isNotNull()) {
							
							ret->m_audioUnitOutput = audioUnitOutput;
							ret->m_converter = converter;
							ret->m_formatSrc = formatSrc;
							ret->m_formatDst = formatDst;
							
							ret->m_queue.setQueueSize(param.samplesPerSecond * param.bufferLengthInMilliseconds / 1000 * param.channelsCount);
							ret->m_nChannels = param.channelsCount;
							ret->m_listener = param.listener;
							ret->m_event = param.event;
							
							AURenderCallbackStruct cs;
							cs.inputProc = CallbackOutput;
							cs.inputProcRefCon = ret.ptr;
							
							result = AudioUnitSetProperty(audioUnitOutput,
														  kAudioUnitProperty_SetRenderCallback,
														  kAudioUnitScope_Input,
														  bus0,
														  &cs,
														  sizeof(cs));
							if (result == noErr) {
								
								result = AudioUnitInitialize(audioUnitOutput);
								if (result == noErr) {
									ret->m_flagInitialized = sl_true;
									if (param.flagAutoStart) {
										ret->start();
									}
									return ret;
								} else {
									logError("Failed to initialize audio unit");
								}
								
							} else {
								logError("Failed to set callback");
							}
							
							return Ref<_iOS_AudioPlayerBuffer>::null();

						}
						
						AudioConverterDispose(converter);
						
					} else {
						logError("Failed to create audio converter");
					}
					
				} else {
					logError("Failed to get stream format");
				}
			} else {
				logError("Failed to enable output");
			}
			
			AudioComponentInstanceDispose(audioUnitOutput);
			
		} else {
			logError("Failed to create audio component instance");
		}

		return ret;
	}
	
	void release()
	{
		ObjectLocker lock(this);
		if (!m_flagOpened) {
			return;
		}
		stop();
		m_flagOpened = sl_false;
		if (m_flagInitialized) {
			AudioUnitUninitialize(m_audioUnitOutput);
		}
		AudioComponentInstanceDispose(m_audioUnitOutput);
		AudioConverterDispose(m_converter);
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
		if (AudioOutputUnitStart(m_audioUnitOutput) != noErr) {
			logError("Failed to start audio unit");
		}
		m_flagRunning = sl_true;
	}
	
	void pause()
	{
		ObjectLocker lock(this);
		if (!m_flagOpened) {
			return;
		}
		if (!m_flagRunning) {
			return;
		}
		m_flagRunning = sl_false;
		if (AudioOutputUnitStop(m_audioUnitOutput) != noErr) {
			logError("Failed to stop audio unit");
		}
	}
	
	void stop()
	{
		pause();
	}
	
	sl_bool isRunning()
	{
		return m_flagRunning;
	}
	
	SafeArray<sl_int16> m_dataConvert;
	void onConvert(sl_uint32 nFrames, AudioBufferList* data)
	{
		sl_uint32 nChannels = m_nChannels;
		sl_uint32 nSamples = nFrames * nChannels;
		
		Array<sl_int16> dataConvert = _getProcessData(nSamples);
		m_dataConvert = dataConvert;
		if (dataConvert.isNull()) {
			return;
		}
		sl_int16* s = dataConvert.getData();
		_processFrame(s, nSamples);
		
		data->mBuffers[0].mDataByteSize = (UInt32)nSamples * 2;
		data->mBuffers[0].mData = s;
		data->mBuffers[0].mNumberChannels = (UInt32)nChannels;

	}
	
	static OSStatus ConverterProc(AudioConverterRef               inAudioConverter,
								  UInt32*                         ioNumberDataPackets,
								  AudioBufferList*                ioData,
								  AudioStreamPacketDescription**  outDataPacketDescription,
								  void*                           inUserData)
	{
		_iOS_AudioPlayerBuffer* object = (_iOS_AudioPlayerBuffer*)inUserData;
		object->onConvert(*ioNumberDataPackets, ioData);
		return noErr;
	}
	
	OSStatus onFrame(UInt32 nFrames, AudioBufferList* data)
	{
		UInt32 sizeFrame = nFrames;
		OSStatus result = AudioConverterFillComplexBuffer(m_converter, ConverterProc, this, &sizeFrame, data, NULL);
		return result;
	}
	
	static OSStatus CallbackOutput(void                        *inRefCon,
								   AudioUnitRenderActionFlags  *ioActionFlags,
								   const AudioTimeStamp        *inTimeStamp,
								   UInt32                      inBusNumber,
								   UInt32                      inNumberFrames,
								   AudioBufferList             *ioData)
	{
		_iOS_AudioPlayerBuffer* object = (_iOS_AudioPlayerBuffer*)(inRefCon);
		if (object) {
			return object->onFrame(inNumberFrames, ioData);
		} else {
			return 500;
		}
	}

};

class _iOS_AudioPlayer : public AudioPlayer
{
public:
	_iOS_AudioPlayer()
	{
	}
	
	~_iOS_AudioPlayer()
	{
	}
	
public:
	static void logError(String text)
	{
		SLIB_LOG_ERROR("AudioPlayer", text);
	}
	
	static Ref<_iOS_AudioPlayer> create(const AudioPlayerParam& param)
	{
		Ref<_iOS_AudioPlayer> ret = new _iOS_AudioPlayer();
		return ret;
	}
	
	Ref<AudioPlayerBuffer> createBuffer(const AudioPlayerBufferParam& param)
	{
		return _iOS_AudioPlayerBuffer::create(param);
	}
	
	Ref<AudioPlayerControl> _openNative(const AudioPlayerOpenParam& param);
};

Ref<AudioPlayer> AudioPlayer::create(const AudioPlayerParam& param)
{
	return _iOS_AudioPlayer::create(param);
}

List<AudioPlayerInfo> AudioPlayer::getPlayersList()
{
	AudioPlayerInfo ret;
	SLIB_STATIC_STRING(s, "Internal Speaker");
	ret.name = s;
	return List<AudioPlayerInfo>::createFromElement(ret);
}

Ref<AudioPlayerControl> _iOS_AudioPlayer::_openNative(const AudioPlayerOpenParam& param)
{
	if (param.data.isNotNull() && param.data.getSize() > 0) {
		Ref<AudioPlayerControl> ret = new _iOS_AudioPlayerControl(param.data);
		if (param.flagAutoStart) {
			ret->start();
		}
		return ret;
	} else if (param.url.isNotNull() && param.url.getLength() > 0) {
		Ref<AudioPlayerControl> ret = new _iOS_AudioPlayerControl(param.url);
		if (param.flagAutoStart) {
			ret->start();
		}
		return ret;
	} else {
		return sl_null;
	}
}

SLIB_MEDIA_NAMESPACE_END

#endif
