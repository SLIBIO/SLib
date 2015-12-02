package slib.platform.android.ui.view;

import javax.microedition.khronos.egl.EGL10;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.egl.EGLDisplay;

import android.opengl.GLSurfaceView;

/*
 *  Multisampling will probably slow down the speed.
 */
public class MultisampleConfigChooser implements GLSurfaceView.EGLConfigChooser {

	@Override
	public EGLConfig chooseConfig(EGL10 egl, EGLDisplay display) {

		EGLConfig[] configs = new EGLConfig[1];
		int[] configsCount = new int[1];
		int[] configSpec;

		configSpec = new int[] {
				EGL10.EGL_RED_SIZE, 8, 
				EGL10.EGL_GREEN_SIZE, 8, 
				EGL10.EGL_BLUE_SIZE, 8, 
				EGL10.EGL_ALPHA_SIZE, 8,
				EGL10.EGL_DEPTH_SIZE, 24,
				EGL10.EGL_STENCIL_SIZE,	8,
				// Requires that setEGLContextClientVersion(2) is called on the view.
				EGL10.EGL_RENDERABLE_TYPE, 4 /* EGL_OPENGL_ES2_BIT */,
				EGL10.EGL_SAMPLE_BUFFERS, 1 /* true */,
				EGL10.EGL_SAMPLES, 2,
				EGL10.EGL_NONE };
		if (egl.eglChooseConfig(display, configSpec, configs, 1, configsCount)) {
			if (configsCount[0] == 1) {
				return configs[0];
			}
		}

		configSpec = new int[] { 
				EGL10.EGL_RED_SIZE, 5,
				EGL10.EGL_GREEN_SIZE, 6,
				EGL10.EGL_BLUE_SIZE, 5,
				EGL10.EGL_DEPTH_SIZE, 16,
				EGL10.EGL_STENCIL_SIZE, 8,
				// Requires that setEGLContextClientVersion(2) is called on the view.
				EGL10.EGL_RENDERABLE_TYPE, 4 /* EGL_OPENGL_ES2_BIT */,
				EGL10.EGL_SAMPLE_BUFFERS, 1 /* true */,
				EGL10.EGL_SAMPLES, 2,
				EGL10.EGL_NONE };
		if (egl.eglChooseConfig(display, configSpec, configs, 1, configsCount)) {
			if (configsCount[0] == 1) {
				return configs[0];
			}
		}

		// No normal multisampling config was found. Try to create a
		// converage multisampling configuration, for the nVidia Tegra2.
		// See the EGL_NV_coverage_sample documentation.
		final int EGL_COVERAGE_BUFFERS_NV = 0x30E0;
		final int EGL_COVERAGE_SAMPLES_NV = 0x30E1;

		configSpec = new int[] {
				EGL10.EGL_RED_SIZE, 8, 
				EGL10.EGL_GREEN_SIZE, 8, 
				EGL10.EGL_BLUE_SIZE, 8,
				EGL10.EGL_ALPHA_SIZE, 8,
				EGL10.EGL_DEPTH_SIZE, 24,
				EGL10.EGL_STENCIL_SIZE, 8,
				EGL10.EGL_RENDERABLE_TYPE, 4 /* EGL_OPENGL_ES2_BIT */,
				EGL_COVERAGE_BUFFERS_NV, 1 /* true */,
				EGL_COVERAGE_SAMPLES_NV, 2, // always 5 in practice on tegra 2
				EGL10.EGL_NONE };
		if (egl.eglChooseConfig(display, configSpec, configs, 1, configsCount)) {
			if (configsCount[0] == 1) {
				return configs[0];
			}
		}

		configSpec = new int[] {
				EGL10.EGL_RED_SIZE, 5, 
				EGL10.EGL_GREEN_SIZE, 6,
				EGL10.EGL_BLUE_SIZE, 5,
				EGL10.EGL_DEPTH_SIZE, 16,
				EGL10.EGL_STENCIL_SIZE, 8,
				EGL10.EGL_RENDERABLE_TYPE, 4 /* EGL_OPENGL_ES2_BIT */,
				EGL_COVERAGE_BUFFERS_NV, 1 /* true */,
				EGL_COVERAGE_SAMPLES_NV, 2, // always 5 in practice on tegra 2
				EGL10.EGL_NONE };
		if (egl.eglChooseConfig(display, configSpec, configs, 1, configsCount)) {
			if (configsCount[0] == 1) {
				return configs[0];
			}
		}

		// Give up, try without multisampling.
		configSpec = new int[] {
				EGL10.EGL_RED_SIZE, 8,
				EGL10.EGL_GREEN_SIZE, 8, 
				EGL10.EGL_BLUE_SIZE, 8,
				EGL10.EGL_ALPHA_SIZE, 8,
				EGL10.EGL_DEPTH_SIZE, 24, 
				EGL10.EGL_STENCIL_SIZE, 8,
				// Requires that setEGLContextClientVersion(2) is called on the view.
				EGL10.EGL_RENDERABLE_TYPE, 4 /* EGL_OPENGL_ES2_BIT */,
				EGL10.EGL_NONE };
		if (egl.eglChooseConfig(display, configSpec, configs, 1, configsCount)) {
			if (configsCount[0] == 1) {
				return configs[0];
			}
		}

		configSpec = new int[] {
				EGL10.EGL_RED_SIZE, 5,
				EGL10.EGL_GREEN_SIZE, 6, 
				EGL10.EGL_BLUE_SIZE, 5,
				EGL10.EGL_DEPTH_SIZE, 16,
				EGL10.EGL_STENCIL_SIZE, 8,
				// Requires that setEGLContextClientVersion(2) is called on the view.
				EGL10.EGL_RENDERABLE_TYPE, 4 /* EGL_OPENGL_ES2_BIT */,
				EGL10.EGL_NONE };
		if (egl.eglChooseConfig(display, configSpec, configs, 1, configsCount)) {
			if (configsCount[0] == 1) {
				return configs[0];
			}
		}
		throw new IllegalArgumentException("eglChooseConfig failed");
	}

}
