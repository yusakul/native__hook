LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := hookread
LOCAL_SRC_FILES := register.c hook_read.c  vector.c
LOCAL_LDFLAGS	:=	 -llog 

include $(BUILD_SHARED_LIBRARY)
