#include <jni.h>
#include <assert.h>
#include <stdio.h>

#include "hook_read.h"
#include "vector.h"


/*
 * 为某一个类注册本地方法
 * */
static int registerNativeMethods(JNIEnv* env,const char* className,
		JNINativeMethod* gMethods,int numMethods)
{
	jclass clazz;
	clazz = (*env)->FindClass(env,className);
	if (clazz == NULL)
	{
		return JNI_FALSE;
	}

	if ((*env)->RegisterNatives(env,clazz,gMethods,numMethods) < 0){
		return JNI_FALSE;
	}

	return JNI_TRUE;
}

/**
* 方法对应表
*/
static const char* hookreadClassName = "com/yusakul/nativehook/HookRead";
static JNINativeMethod hookreadMethods[] = {
		{"init", "()V", hook_read_bind},//初始化
		{"uninit", "()V", hook_read_unbind},//解绑
		{"purArray", "([B)V", put_array},//解绑
		{"deleteArray", "()V", delete_array},//解绑
};

/*
* 为所有类注册本地方法
*/
static int registerNatives(JNIEnv* env) {
	int ret;
	//
    ret =  registerNativeMethods(env, hookreadClassName, hookreadMethods,
            sizeof(hookreadMethods) / sizeof(hookreadMethods[0]));

    return ret;
}

/*
* System.loadLibrary("lib")时调用
* 如果成功返回JNI版本, 失败返回-1
*/
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved) {
	 JNIEnv* env = NULL;
    jint result = -1;

    if ((*vm)->GetEnv(vm, (void**) &env, JNI_VERSION_1_4) != JNI_OK) {
        return -1;
    }
    assert(env != NULL);

    if (!registerNatives(env)) {//注册
        return -1;
    }
    //成功
    result = JNI_VERSION_1_6;

    return result;
}
