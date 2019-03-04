#ifndef _SO_H_
#define _SO_H_

#define ELF32_R_SYM(x) ((x) >> 8)
#define ELF32_R_TYPE(x) ((x) & 0xff)

#define SOINFO_NAME_LEN 128

typedef int int32_t;
typedef unsigned int size_t;
typedef unsigned short int uint16_t;
typedef unsigned int uint32_t;

typedef uint32_t Elf32_Addr;
typedef uint16_t Elf32_Half;
typedef uint32_t Elf32_Off;
typedef int32_t Elf32_Sword;
typedef uint32_t Elf32_Word;

typedef struct elf32_sym {
	Elf32_Word st_name;
	Elf32_Addr st_value;
	Elf32_Word st_size;
	unsigned char st_info;
	unsigned char st_other;
	Elf32_Half st_shndx;
} Elf32_Sym;

typedef struct elf32_rel {
	Elf32_Addr r_offset;
	Elf32_Word r_info;
} Elf32_Rel;

typedef struct dynamic{
 Elf32_Sword d_tag;
 union{
 Elf32_Sword d_val;
 Elf32_Addr d_ptr;
 } d_un;
} Elf32_Dyn;
/*
 * padding
 *20 + 20 + 4
 *padding1
 *20
 * */
typedef struct soinfo {
	char name[SOINFO_NAME_LEN];
	char padding[4];
	size_t phnum;
	Elf32_Addr entry;
	Elf32_Addr base;
	unsigned size;

	char padding1[4];
	Elf32_Dyn* dynamic;
	char padding3[16];

	const char* strtab;
	Elf32_Sym* symtab;

	char padding2[20];

	Elf32_Rel* plt_rel;
	size_t plt_rel_count;

	Elf32_Rel* rel;
	size_t rel_count;

} SoInfo;

#endif
