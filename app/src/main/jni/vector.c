#include "vector.h"
#include "log.h"
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

static pthread_key_t key;

pthread_once_t once = PTHREAD_ONCE_INIT;

struct local {
	int total;
	int pointer;
	char* vector;
};
void delete();
void dest (void *value)
{
    // Release storage pointed to by 'value'
	delete();
}

void oonce()
{
	LOGE("This is run only once!");
//	pthread_key_create (&key, dest);
	pthread_key_create (&key, NULL);
}


void put(char* buffer, int size) {
	struct local* pll =(struct local*)malloc(sizeof(struct local));
	pll->vector = buffer;
	pll->total = size;
	pll->pointer = 0;

//	pid_t threadid = gettid();
	pthread_once(&once,oonce);
//	LOGE("threadid:%d,try to put vector:%p,ptheadkey:%d,local addr:%p",threadid, buffer,key,pll);

//	LOGI("threadid:%d,total:%d,pointer:%d,size:%d,vector addr:%p",threadid, pll->total, pll->pointer, size, pll->vector);
	pthread_setspecific(key, pll);

//	pll = (struct local*) pthread_getspecific(key);
//	LOGI("iii threadid:%d,total:%d,pointer:%d,size:%d,vector addr:%p,pll:%p",threadid, pll->total, pll->pointer, size, pll->vector,pll);
}

void delete() {
	pid_t threadid = gettid();
	struct local* pll = (struct local*) pthread_getspecific(key);
	LOGE("threadid:%d,try to free vector:%p",threadid, pll->vector);
	free(pll->vector);
//	pthread_key_delete(key);
}

void put_array(JNIEnv* env, jobject thiz, jbyteArray array) {
	LOGI("put_array:%0x", array);
	jsize size = (*env)->GetArrayLength(env, array);
	jbyte *jbarray = (jbyte *) malloc(size * sizeof(jbyte));

	(*env)->GetByteArrayRegion(env, array, 0, size, jbarray);
	char* dDate = (char*) jbarray;
	//
	put(dDate, size);
}

void delete_array(JNIEnv* env, jobject thiz) {
	LOGI("hook_read_unbind");
	delete();

}

int read_from_array(int size, char* buffer, int kk) {

	pid_t threadid = gettid();
	struct local* pll = (struct local*) pthread_getspecific(key);

//	LOGI("read_from_array,threadid:%d,ptheadkey:%d,local:%p",threadid,key,pll);
	if (pll == NULL){
		LOGE("thread local is null");
		return -1;
	}
//	LOGI("threadid:%d,total:%d,pointer:%d,size:%d,vector addr:%p",threadid, pll->total, pll->pointer, size, pll->vector);
	if (pll->pointer >= pll->total)
		return -1;
	if (pll->pointer + size > pll->total) {
		size = pll->total - pll->pointer;
	}
	memcpy(buffer, pll->vector + pll->pointer, size);
	pll->pointer += size;

	return size;
}
