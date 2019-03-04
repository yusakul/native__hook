#include "so.h"
#include "hook_read.h"
#include "log.h"
#include <dlfcn.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <errno.h>
#include "vector.h"

typedef int (*READ_FUNC)(int, void*, size_t);
int my_read(int fd, void *buf, size_t size);
static unsigned int read_reloc;
static READ_FUNC old_read;

static int hooked;

static int key;

int my_read(int fd, void *buf, size_t size) {
	int sz = 0;
	LOGI("hook read success,fd:%d,buf:%p,size:%d", fd, buf, size);

//	sz = read_n_bytes_from_java(size, buf, key);
	sz = read_from_array(size, buf, key);
	LOGI("read n bytes:%d", sz);

	if (sz > 0) {
		return sz;
	}

	sz = old_read(fd, buf, size);
	LOGI("read from old read:%d", sz);

	return sz;
}
/*
 * 会修改附近4096个字节的值
 * */
void mmprotect(unsigned long addr) {
	unsigned long start;
	int pagesize = getpagesize();
	start = (addr / pagesize) * pagesize;
	LOGI(
			"try to protect,start:%lx,addr:%lx,pagesize:%d", start, addr, pagesize);
	int ret = mprotect(start, 4096, PROT_EXEC | PROT_READ | PROT_WRITE);
	if (ret < 0) {
		LOGW("mprotect failed,%s", strerror(errno));
	}
}

static int modify_rel_one(int i, Elf32_Addr base, const char* strtab,
		Elf32_Sym* symtab, Elf32_Rel* rel) {
	unsigned int type = ELF32_R_TYPE(rel->r_info);
	unsigned int sym_idx = ELF32_R_SYM(rel->r_info);
	unsigned reloc = (unsigned) (rel->r_offset + base);
	char *sym_name = (char*) (strtab + symtab[sym_idx].st_name);

	if (!strcmp(sym_name, "read")) {
		LOGI("modify reloc to:%p", my_read);
		//reloc中，是地址，改地址是存放的函数地址值
		read_reloc = reloc;
		unsigned int * va = (unsigned int*) reloc;
		old_read = *va;
		LOGI("reloc addr value:%0x", *va);
		mmprotect(reloc);
		*va = my_read;
		LOGI("my read new location:%p", *va);
		hooked = 1;
		return 1;
	}
	return 0;
}

static void modify_rel(Elf32_Addr base, const char* strtab, Elf32_Sym* symtab,
		Elf32_Rel* rel_start, size_t size) {
	int i;
	unsigned long start = (unsigned long) rel_start;
	for (i = 0; i < size; i++) {
		Elf32_Rel* rel = (Elf32_Rel*) (start + i * 8);
		int ret = modify_rel_one(i, base, strtab, symtab, rel);
		if (ret)
			break;
	}
}

/*
 * 指定的hook so
 * */

static void start(const char *soname) {
	void *handle = dlopen(soname, RTLD_LAZY);
	if (handle == NULL) {
		LOGE("load libcrypto.so failed!");
		return;
	}
	SoInfo* info = (SoInfo*) handle;

	modify_rel(info->base, info->strtab, info->symtab, info->plt_rel,
			info->plt_rel_count);

	dlclose(handle);
}

void hook_read_bind(JNIEnv* env, jobject thiz, jint k) {
	LOGI("hook_read_bind,key:%d", k);
	key = k;
	if (!hooked)
		start("libjavacore.so");
}

void hook_read_unbind(JNIEnv* env, jobject thiz) {
	LOGI("hook_read_unbind");
	if (hooked){
		unsigned int * va = (unsigned int*) read_reloc;
		*va = old_read;
		hooked = 0;
	}
}

