#define CPUSTACKSIZE  4 // Stacksize in KB per CPU - Currently leave on 4 KB this is xv6 default
#define NPROC        64  // maximum number of processes
#define NOFILE       16  // open files per process
#define NFILE       100  // open files per system
#define NINODE       50  // maximum number of active i-nodes
#define NDEV         10  // maximum major device number
#define ROOTDEV       1  // device number of file system root disk
#define MAXARG       32  // max exec arguments
#define MAXOPBLOCKS  10  // max # of blocks any FS op writes
#define LOGSIZE      (MAXOPBLOCKS*3)  // max data blocks in on-disk log
#define NBUF         (MAXOPBLOCKS*3)  // size of disk block cache
#define FSSIZE       2000  // size of file system in blocks
#define MAXPATH      128   // maximum file path name
#define USERSTACK    1     // user stack pages

// Defined in Makefile, so it is dynamic based on the config for qemu
//#define NCPU          8  // maximum number of CPUs
//#define MAXMEM       128   // max Memory in megabyte