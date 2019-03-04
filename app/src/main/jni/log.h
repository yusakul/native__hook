#ifndef  _LOG_H_
#define _LOG_H_
#include <android/log.h>

#define HOOKREAD "hookread"

#define LOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, HOOKREAD, __VA_ARGS__)
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG , HOOKREAD, __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO  , HOOKREAD, __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN  , HOOKREAD, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR  , HOOKREAD, __VA_ARGS__)

#define ALOG(TAG,...) __android_log_print(ANDROID_LOG_ERROR  , TAG, __VA_ARGS__)

#endif
