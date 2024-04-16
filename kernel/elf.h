// Format of an ELF executable file

#define ELF_MAGIC 0x464C457FU  // "\x7FELF" in little endian

// File header
struct elfhdr {
  uint magic;  // must equal ELF_MAGIC
  uchar elf[12];
  ushort type;
  ushort machine;
  uint version;
  uint64 entry;
  uint64 phoff; //program header table offset
  uint64 shoff; //section header table offset
  uint flags;
  ushort ehsize; //elf header size
  ushort phentsize; // program header table entry size
  ushort phnum; // program header table entry count
  ushort shentsize; // section header table entry size
  ushort shnum; // section header table entry count
  ushort shstrndx; // section header string table index
};

// Program section header
struct proghdr {
  uint32 type;
  uint32 flags;
  uint64 off;
  uint64 vaddr;
  uint64 paddr;
  uint64 filesz; // size of this segment in the elf file. can also see it as the number of bytes starting from off to read the segment from the elf file
  uint64 memsz; //size of this segment in memory, it can be larger than the filesz
  uint64 align;
};

// Values for Proghdr type
#define ELF_PROG_LOAD           1

// Flag bits for Proghdr flags
#define ELF_PROG_FLAG_EXEC      1
#define ELF_PROG_FLAG_WRITE     2
#define ELF_PROG_FLAG_READ      4
