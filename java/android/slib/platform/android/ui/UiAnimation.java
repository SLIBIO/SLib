package slib.platform.android.ui;

import android.view.View;
import android.animation.Animator;
import android.animation.ValueAnimator;
import android.animation.TimeInterpolator;

import slib.platform.android.Logger;

public class UiAnimation {

	public static TimeInterpolator getInterpolator(int curve) {
		TimeInterpolator interpolator = null;
		switch (curve) {
			case 1: // EaseInOut
				interpolator = new android.view.animation.AccelerateDecelerateInterpolator();
				break;
			case 2: // EaseIn
				interpolator = new android.view.animation.AccelerateInterpolator();
				break;
			case 3: // EaseOut
				interpolator = new android.view.animation.DecelerateInterpolator();
				break;
			case 5: // Bounce
				interpolator = new android.view.animation.BounceInterpolator();
				break;
			case 6: // Anticipate
				interpolator = new android.view.animation.AnticipateInterpolator();
				break;
			case 7: // Overshoot
				interpolator = new android.view.animation.OvershootInterpolator();
				break;
			default: // Linear
				interpolator = new android.view.animation.LinearInterpolator();
				break;
		}
		return interpolator;
	}

	private static native void nativeOnStop(long id);

	public static float interpolateValue(float f, float s, float e) {
		return s * (1 - f) + e * f;
	}

	static final float EPSILON = 0.000001f;

	long mId;
	ValueAnimator mAnimator;
	View mView;
	boolean mIsViewShown = false;
	boolean mIsStopped = false;

	Animator.AnimatorListener mListener = new Animator.AnimatorListener() {
		@Override
		public void onAnimationStart(Animator animation) {
		}

		@Override
		public void onAnimationEnd(Animator animation) {
			if (mIsStopped) {
				return;
			}
			_stop();
		}

		@Override
		public void onAnimationCancel(Animator animation) {
			if (mIsStopped) {
				return;
			}
			_stop();
		}

		@Override
		public void onAnimationRepeat(Animator animation) {
		}
	};

	public static UiAnimation start(View view, long id, float duration, float delay,
	                                int curve, int repeatCount, boolean flagReverse,
	                                final float ax, final float ay,
	                                final boolean flagTranslate, final float txStart, final float tyStart, final float txEnd, final float tyEnd,
	                                final boolean flagScale, final float sxStart, final float syStart, final float sxEnd, final float syEnd,
	                                final boolean flagRotate, final float rotateStart, final float rotateEnd,
	                                final boolean flagAlpha, final float alphaStart, final float alphaEnd) {


		try {

			final UiAnimation animation = new UiAnimation();

			animation.mId = id;
			animation.mView = view;

			final boolean flagAnchor = (flagRotate || flagScale) && (Math.abs(ax) > EPSILON || Math.abs(ay) > EPSILON);

			final ValueAnimator animator = ValueAnimator.ofFloat(0, 1);
			animator.addUpdateListener(new ValueAnimator.AnimatorUpdateListener() {

				public void onAnimationUpdate(ValueAnimator va) {
					if (animation.mIsStopped) {
						return;
					}
					View view = animation.mView;
					if (view == null) {
						return;
					}
					if (animation.mIsViewShown) {
						if (!(view.isShown())) {
							animation.mIsViewShown = false;
							animation._stop();
							animator.cancel();
							return;
						}
					} else {
						if (view.isShown()) {
							animation.mIsViewShown = true;
						}
					}
					final float f = (Float) (va.getAnimatedValue());
					if (flagTranslate || flagRotate || flagScale) {
						float r = 0;
						if (flagRotate) {
							r = interpolateValue(f, rotateStart, rotateEnd);
							float rd = (float) (r * 180 / Math.PI);
							if (Math.abs(view.getRotation() - rd) > EPSILON) {
								view.setRotation(rd);
							}
						} else {
							r = rotateStart;
						}
						float sx = 0, sy = 0;
						if (flagScale) {
							sx = interpolateValue(sx, sxStart, sxEnd);
							if (Math.abs(view.getScaleX() - sx) > EPSILON) {
								view.setScaleX(sx);
							}
							sy = interpolateValue(sy, syStart, syEnd);
							if (Math.abs(view.getScaleY() - sy) > EPSILON) {
								view.setScaleY(sy);
							}
						} else {
							sx = sxStart;
							sy = syStart;
						}
						if (flagTranslate || flagAnchor) {
							float tx = 0, ty = 0;
							if (flagTranslate) {
								tx = interpolateValue(f, txStart, txEnd);
								ty = interpolateValue(f, tyStart, tyEnd);
							} else {
								tx = txStart;
								ty = tyStart;
							}
							if (flagAnchor) {
								double cr = Math.cos(r);
								double sr = Math.sin(r);
								tx = (float) ((-ax * cr + ay * sr) * sx + tx + ax);
								ty = (float) ((-ax * sr - ay * cr) * sy + ty + ay);
							}
							if (Math.abs(tx - view.getTranslationX()) > EPSILON) {
								view.setTranslationX(tx);
							}
							if (Math.abs(ty - view.getTranslationY()) > EPSILON) {
								view.setTranslationY(ty);
							}
						}
					}
					if (flagAlpha) {
						float alpha = interpolateValue(f, alphaStart, alphaEnd);
						view.setAlpha(alpha);
					}
				}
			});
			animator.addListener(animation.mListener);

			animator.setInterpolator(getInterpolator(curve));
			animator.setDuration((int) (duration * 1000));
			animator.setStartDelay((int) (delay * 1000));
			if (flagReverse) {
				animator.setRepeatMode(ValueAnimator.REVERSE);
				animator.setRepeatCount(1);
			}
			if (repeatCount < 0) {
				animator.setRepeatCount(ValueAnimator.INFINITE);
			} else {
				if (repeatCount > 0) {
					animator.setRepeatCount(repeatCount);
				}
			}

			if (UiThread.isUiThread()) {
				animator.start();
			} else {
				view.post(new Runnable() {
					public void run() {
						try {
							animator.start();
						} catch (Exception e) {
							Logger.exception(e);
						}
					}
				});
			}

			animation.mAnimator = animator;

			return animation;

		} catch (Exception e) {
			Logger.exception(e);
		}

		return null;

	}

	void stop() {
		if (mIsStopped) {
			return;
		}
		_stop();
		if (UiThread.isUiThread()) {
			mAnimator.cancel();
		} else {
			UiThread.post(new Runnable() {
				public void run() {
					try {
						mAnimator.cancel();
					} catch (Exception e) {
						Logger.exception(e);
					}
				}
			});
		}
	}

	private void _stop()
	{
		mIsStopped = true;
		mView = null;
		nativeOnStop(mId);
	}
}
