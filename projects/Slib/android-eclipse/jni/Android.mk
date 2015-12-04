LOCAL_PATH := $(call my-dir)

SRC_BASE = ../../../../src
SLIB_CORE = $(SRC_BASE)/slib/core
SLIB_CRYPTO = $(SRC_BASE)/slib/crypto
SLIB_MATH = $(SRC_BASE)/slib/math
SLIB_NETWORK = $(SRC_BASE)/slib/network
SLIB_GRAPHICS = $(SRC_BASE)/slib/graphics

SLIB_RENDER = $(SRC_BASE)/slib/render
SLIB_UI = $(SRC_BASE)/slib/ui
SLIB_MEDIA = $(SRC_BASE)/slib/media
SLIB_DEVICE = $(SRC_BASE)/slib/device
SLIB_DB = $(SRC_BASE)/slib/db

SLIB_THIRDPARTY = $(SRC_BASE)/thirdparty

include $(CLEAR_VARS)
LOCAL_MODULE := slib
LOCAL_ARM_MODE := arm
ifeq ($(TARGET_ARCH_ABI),armeabi-v7a)
    LOCAL_ARM_NEON := true
endif

SLIB_CORE_FILES := $(wildcard $(LOCAL_PATH)/$(SLIB_CORE)/*.cpp)
SLIB_CRYPTO_FILES := $(wildcard $(LOCAL_PATH)/$(SLIB_CRYPTO)/*.cpp)
SLIB_MATH_FILES := $(wildcard $(LOCAL_PATH)/$(SLIB_MATH)/*.cpp)
SLIB_NETWORK_FILES := $(wildcard $(LOCAL_PATH)/$(SLIB_NETWORK)/*.cpp)
SLIB_GRAPHICS_FILES := $(wildcard $(LOCAL_PATH)/$(SLIB_GRAPHICS)/*.cpp)

SLIB_RENDER_FILES := $(wildcard $(LOCAL_PATH)/$(SLIB_RENDER)/*.cpp)
SLIB_UI_FILES := $(wildcard $(LOCAL_PATH)/$(SLIB_UI)/*.cpp)
SLIB_MEDIA_FILES := $(wildcard $(LOCAL_PATH)/$(SLIB_MEDIA)/*.cpp)
SLIB_DEVICE_FILES := $(wildcard $(LOCAL_PATH)/$(SLIB_DEVICE)/*.cpp)
SLIB_DB_FILES := $(wildcard $(LOCAL_PATH)/$(SLIB_DB)/*.cpp)

LOCAL_SRC_FILES := $(SLIB_CORE_FILES:$(LOCAL_PATH)/%=%) 
LOCAL_SRC_FILES += $(SLIB_CRYPTO_FILES:$(LOCAL_PATH)/%=%) 
LOCAL_SRC_FILES += $(SLIB_MATH_FILES:$(LOCAL_PATH)/%=%) 
LOCAL_SRC_FILES += $(SLIB_NETWORK_FILES:$(LOCAL_PATH)/%=%) 
LOCAL_SRC_FILES += $(SLIB_GRAPHICS_FILES:$(LOCAL_PATH)/%=%) 

LOCAL_SRC_FILES += $(SLIB_RENDER_FILES:$(LOCAL_PATH)/%=%) 
LOCAL_SRC_FILES += $(SLIB_UI_FILES:$(LOCAL_PATH)/%=%) 
LOCAL_SRC_FILES += $(SLIB_MEDIA_FILES:$(LOCAL_PATH)/%=%) 
LOCAL_SRC_FILES += $(SLIB_DEVICE_FILES:$(LOCAL_PATH)/%=%) 
LOCAL_SRC_FILES += $(SLIB_DB_FILES:$(LOCAL_PATH)/%=%) 

LOCAL_SRC_FILES += \
	$(SLIB_THIRDPARTY)/thirdparty_libjpeg.c \
	$(SLIB_THIRDPARTY)/thirdparty_libpng.c \
	$(SLIB_THIRDPARTY)/thirdparty_soil2.c \
	$(SLIB_THIRDPARTY)/thirdparty_zlib.c \
	$(SLIB_THIRDPARTY)/thirdparty_freetype.c \
	$(SLIB_THIRDPARTY)/thirdparty_sqlite3.c
	
ifeq ($(TARGET_ARCH_ABI),armeabi-v7a)
LOCAL_SRC_FILES += \
	$(SLIB_THIRDPARTY)/thirdparty_libopus.c.neon \
	$(SLIB_THIRDPARTY)/thirdparty_libvpx.c.neon \
	$(SLIB_THIRDPARTY)/thirdparty_libvpx_enc.c.neon \
	$(SLIB_THIRDPARTY)/thirdparty_libvpx_dec.c.neon
else
LOCAL_SRC_FILES += \
	$(SLIB_THIRDPARTY)/thirdparty_libopus.c \
	$(SLIB_THIRDPARTY)/thirdparty_libvpx.c \
	$(SLIB_THIRDPARTY)/thirdparty_libvpx_enc.c \
	$(SLIB_THIRDPARTY)/thirdparty_libvpx_dec.c
endif	

include $(BUILD_STATIC_LIBRARY)
