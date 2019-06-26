LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := libmpg123
LOCAL_ARM_MODE  := arm
LOCAL_CFLAGS    += -O3 -Wall -DHAVE_CONFIG_H  \
				   -fomit-frame-pointer -funroll-all-loops -finline-functions -ffast-math

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/src

LOCAL_CFLAGS += \
	-DACCURATE_ROUNDING \
	-DNO_REAL \
	-DNO_32BIT

ifeq ($(TARGET_ARCH_ABI),armeabi-v7a)
	LOCAL_CFLAGS += -mfloat-abi=softfp -mfpu=neon -DOPT_NEON -DREAL_IS_FLOAT
	LOCAL_SRC_FILES := \
		src/check_neon.S \
		src/compat.c \
		src/dct64.c \
		src/dither.c \
		src/equalizer.c \
		src/feature.c \
		src/format.c \
		src/frame.c \
		src/icy.c \
		src/icy2utf8.c \
		src/id3.c \
		src/index.c \
		src/layer1.c \
		src/layer2.c \
		src/layer3.c \
		src/libmpg123.c \
		src/ntom.c \
		src/optimize.c \
		src/parse.c \
		src/readers.c \
		src/stringbuf.c \
		src/synth.c \
		src/synth_8bit.c \
		src/tabinit.c \
		src/synth_neon.S \
	    src/synth_neon_accurate.S \
		src/synth_stereo_neon.S \
		src/synth_stereo_neon_accurate.S \
		src/dct64_neon.S \
		src/dct64_neon_float.S \
		src/dct36_neon.S \
        src/synth_arm_accurate.S
endif

ifeq ($(TARGET_ARCH_ABI),arm64-v8a)
	LOCAL_CFLAGS += -mfloat-abi=softfp -mfpu=neon -DOPT_NEON64 -DREAL_IS_FLOAT
	LOCAL_SRC_FILES := \
		src/check_neon.S \
		src/compat.c \
		src/dct64.c \
		src/dither.c \
		src/equalizer.c \
		src/feature.c \
		src/format.c \
		src/frame.c \
		src/icy.c \
		src/icy2utf8.c \
		src/id3.c \
		src/index.c \
		src/layer1.c \
		src/layer2.c \
		src/layer3.c \
		src/libmpg123.c \
		src/ntom.c \
		src/optimize.c \
		src/parse.c \
		src/readers.c \
		src/stringbuf.c \
		src/synth.c \
		src/synth_8bit.c \
		src/tabinit.c \
		src/synth_neon64.S \
		src/synth_neon64_accurate.S \
		src/synth_stereo_neon64.S \
		src/synth_stereo_neon64_accurate.S \
		src/dct64_neon64.S \
		src/dct64_neon64_float.S \
		src/dct36_neon64.S
endif

ifeq ($(TARGET_ARCH_ABI),x86)
	LOCAL_CFLAGS     := -DACCURATE_ROUNDING -DHAVE_STRERROR -DOPT_SSE -Wno-int-to-pointer-cast -Wno-pointer-to-int-cast -ffast-math
	LOCAL_SRC_FILES := \
		src/compat.c \
		src/dct64.c \
		src/dither.c \
		src/equalizer.c \
		src/feature.c \
		src/format.c \
		src/frame.c \
		src/icy.c \
		src/icy2utf8.c \
		src/id3.c \
		src/index.c \
		src/layer1.c \
		src/layer2.c \
		src/layer3.c \
		src/libmpg123.c \
		src/ntom.c \
		src/optimize.c \
		src/parse.c \
		src/readers.c \
		src/stringbuf.c \
		src/synth.c \
		src/synth_8bit.c \
		src/tabinit.c \
		src/synth_real.c \
		src/synth_s32.c \
		src/synth_sse.S \
		src/synth_sse_accurate.S \
		src/synth_sse_float.S \
		src/synth_sse_s32.S \
		src/synth_stereo_sse_accurate.S \
		src/synth_stereo_sse_float.S \
		src/synth_stereo_sse_s32.S \
		src/dct64_i386.c \
		src/dct36_sse.S \
		src/dct64_sse.S \
		src/dct64_sse_float.S \
		src/tabinit_mmx.S
	LOCAL_LDLIBS += -Wl,--no-warn-shared-textrel
endif

ifeq ($(TARGET_ARCH_ABI),x86_64)
	LOCAL_CFLAGS := -DACCURATE_ROUNDING  -DHAVE_STRERROR -DOPT_X86_64 -Wno-int-to-pointer-cast -Wno-pointer-to-int-cast -ffast-math
	LOCAL_SRC_FILES := \
		src/compat.c \
		src/dct64.c \
		src/dither.c \
		src/equalizer.c \
		src/feature.c \
		src/format.c \
		src/frame.c \
		src/icy.c \
		src/icy2utf8.c \
		src/id3.c \
		src/index.c \
		src/layer1.c \
		src/layer2.c \
		src/layer3.c \
		src/libmpg123.c \
		src/ntom.c \
		src/optimize.c \
		src/parse.c \
		src/readers.c \
		src/stringbuf.c \
		src/synth.c \
		src/synth_8bit.c \
		src/tabinit.c \
		src/synth_real.c \
		src/synth_s32.c \
		src/getcpuflags_x86_64.S \
		src/synth_x86_64.S \
		src/synth_x86_64_s32.S \
		src/synth_x86_64_accurate.S \
		src/synth_x86_64_float.S \
		src/synth_stereo_x86_64_float.S \
		src/synth_stereo_x86_64.S \
		src/synth_stereo_x86_64_s32.S \
		src/synth_stereo_x86_64_accurate.S \
		src/dct36_x86_64.S \
		src/dct64_x86_64.S \
		src/dct64_x86_64_float.S
endif

include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_ARM_MODE  := arm
LOCAL_MODULE    := mpg123plug
LOCAL_SRC_FILES := mpg123-jni.cpp
LOCAL_SHARED_LIBRARIES := libmpg123
LOCAL_CFLAGS += -O3
LOCAL_LDLIBS := -llog
include $(BUILD_SHARED_LIBRARY) 
