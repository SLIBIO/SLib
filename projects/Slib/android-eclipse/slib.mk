
include $(CLEAR_VARS)
LOCAL_MODULE	:= slib
ifeq ($(TARGET_ARCH_ABI),armeabi-v7a)
LOCAL_SRC_FILES := $(SLIB_BASE)/projects/Slib/android-eclipse/obj/local/armeabi-v7a/libslib.a
endif
ifeq ($(TARGET_ARCH_ABI),arm64-v8a)
LOCAL_SRC_FILES := $(SLIB_BASE)/projects/Slib/android-eclipse/obj/local/arm64-v8a/libslib.a
endif
include $(PREBUILT_STATIC_LIBRARY)

ifeq ($(TARGET_ARCH_ABI),armeabi-v7a)
include $(CLEAR_VARS)
LOCAL_MODULE	:= vpxarmv7_asm
LOCAL_SRC_FILES := $(SLIB_BASE)/thirdparty/libvpx/vpx1.4/platforms/armv7_android/libvpxarmv7_asm.a
include $(PREBUILT_STATIC_LIBRARY)
endif
