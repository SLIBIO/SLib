package slib.platform.android.media;

import android.content.Context;
import android.content.res.AssetFileDescriptor;
import android.content.res.AssetManager;
import android.graphics.SurfaceTexture;
import android.media.MediaPlayer;
import android.view.Surface;
import slib.platform.android.Logger;

public class SMediaPlayer implements MediaPlayer.OnCompletionListener, MediaPlayer.OnPreparedListener {

	private MediaPlayer mPlayer;
	private long mInstance = 0;

	public static SMediaPlayer openUrl(String url) {
		try {
			MediaPlayer player = new MediaPlayer();
			player.setDataSource(url);
			return new SMediaPlayer(player);
		} catch (Exception e) {
			Logger.exception(e);
			return null;
		}
	}

	public static SMediaPlayer openAsset(Context context, String fileName) {
		try {
			AssetManager assets = context.getAssets();
			AssetFileDescriptor fd = assets.openFd(fileName);
			MediaPlayer player = new MediaPlayer();
			player.setDataSource(fd.getFileDescriptor(), fd.getStartOffset(), fd.getLength());
			return new SMediaPlayer(player);
		} catch (Exception e) {
			Logger.exception(e);
			return null;
		}
	}
	
	private SMediaPlayer(MediaPlayer player) {
		mPlayer = player;
		player.setOnCompletionListener(this);
		player.setOnPreparedListener(this);
		mPlayer.prepareAsync();
	}
	
	public void setInstance(long instance) {
		mInstance = instance;
	}
	
	public void start() {
		try {
			mPlayer.start();
		} catch (Exception e) {
			Logger.exception(e);
		}
	}

	public void pause() {
		try {
			mPlayer.pause();
		} catch (Exception e) {
			Logger.exception(e);
		}
	}

	public void stop() {
		try {
			mPlayer.stop();
		} catch (Exception e) {
			Logger.exception(e);
		}
	}

	public boolean isPlaying() {
		try {
			return mPlayer.isPlaying();
		} catch (Exception e) {
			Logger.exception(e);
		}
		return false;
	}
	
	public void setLooping(boolean flag) {
		try {
			mPlayer.setLooping(flag);
		} catch (Exception e) {
			Logger.exception(e);
		}
	}
	
	private SurfaceTexture mTexture;
	private Surface mSurface;
	private boolean mFlagUpdatedVideo = false;
	private float[] mTextureMatrix = new float[16];
	
	public boolean renderVideo(int textureName, boolean flagResetTexture) {
		try {
			if (flagResetTexture || mTexture == null) {
				mTexture = new SurfaceTexture(textureName);
				mTexture.setOnFrameAvailableListener(new SurfaceTexture.OnFrameAvailableListener() {					
					@Override
					public void onFrameAvailable(SurfaceTexture surfaceTexture) {
						if (surfaceTexture == mTexture) {
							mFlagUpdatedVideo = true;
						}
					}
				});
				mSurface = new Surface(mTexture);
				mPlayer.setSurface(mSurface);
				mSurface.release();			
				mFlagUpdatedVideo = false;
				return false;
			}
			if (mFlagUpdatedVideo) {
				mFlagUpdatedVideo = false;
				mTexture.updateTexImage();
				mTexture.getTransformMatrix(mTextureMatrix);
				return true;
			}
		} catch (Exception e) {
			Logger.exception(e);
		}
		return false;
	}

	private static native void nativeOnCompleted(long instance);
	
	@Override
	public void onCompletion(MediaPlayer mp) {
		nativeOnCompleted(mInstance);
	}
	
	private static native void nativeOnPrepared(long instance);
	
	@Override
	public void onPrepared(MediaPlayer mp) {
		nativeOnPrepared(mInstance);
	}
	
}