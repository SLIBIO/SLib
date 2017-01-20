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

	public static boolean start(final View view, final long id, float duration, float delay,
	                            int curve, boolean flagRepeat, boolean flagReverse,
	                            final float ax, final float ay,
	                            final boolean flagTranslate, final float txStart, final float tyStart, final float txEnd, final float tyEnd,
	                            final boolean flagScale, final float sxStart, final float syStart, final float sxEnd, final float syEnd,
	                            final boolean flagRotate, final float rotateStart, final float rotateEnd,
	                            final boolean flagAlpha, final float alphaStart, final float alphaEnd) {


		try {

			final boolean flagAnchor = (flagRotate || flagScale) && (Math.abs(ax) > EPSILON || Math.abs(ay) > EPSILON);

			final ValueAnimator animator = ValueAnimator.ofFloat(0, 1);

			animator.addUpdateListener(new ValueAnimator.AnimatorUpdateListener() {
				public void onAnimationUpdate(ValueAnimator animation) {
					final float f = (float)(animation.getAnimatedValue());
					if (flagTranslate || flagRotate || flagScale) {
						float r = 0;
						if (flagRotate) {
							r = interpolateValue(f, rotateStart, rotateEnd);
							float rd = (float)(r * 180 / Math.PI);
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
								tx = (float)((- ax * cr + ay * sr) * sx + tx + ax);
								ty = (float)((- ax * sr - ay * cr) * sy + ty + ay);
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

			animator.addListener(new Animator.AnimatorListener() {
				@Override
				public void onAnimationStart(Animator animation) {
				}

				@Override
				public void onAnimationEnd(Animator animation) {
					nativeOnStop(id);
				}

				@Override
				public void onAnimationCancel(Animator animation) {
				}

				@Override
				public void onAnimationRepeat(Animator animation) {
				}
			});

			animator.setInterpolator(getInterpolator(curve));
			animator.setDuration((int)(duration * 1000));
			animator.setStartDelay((int)(delay * 1000));
			if (flagReverse) {
				animator.setRepeatMode(ValueAnimator.REVERSE);
				animator.setRepeatCount(1);
			}
			if (flagRepeat) {
				animator.setRepeatCount(ValueAnimator.INFINITE);
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

			return true;

		} catch (Exception e) {
			Logger.exception(e);
		}

		return false;

	}

}
