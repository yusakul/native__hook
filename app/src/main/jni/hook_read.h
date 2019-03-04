#ifndef _HOOK_READ_H_
#define _HOOK_READ_H_
#include <jni.h>
void hook_read_bind(JNIEnv* env, jobject thiz,jint i);
void hook_read_unbind(JNIEnv* env, jobject thiz);

#endif
