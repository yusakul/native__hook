#ifndef _VECTOR_H_
#define _VECTOR_H_
#include <jni.h>

void put_array(JNIEnv* env, jobject thiz, jbyteArray array);
void delete_array(JNIEnv* env, jobject thiz);

int read_from_array(int n, char* buffer, int key);

#endif
