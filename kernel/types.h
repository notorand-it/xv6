typedef unsigned int   uint;
typedef unsigned short ushort;
typedef unsigned char  uchar;

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int  uint32;
typedef unsigned long uint64;

typedef uint64 pde_t;

enum prstate { PI_UNUSED, PI_USED, PI_SLEEPING, PI_RUNNABLE, PI_RUNNING, PI_ZOMBIE };

struct procinfo{
    int pid;
    char name[16];
    enum prstate state;
    int ppid;
};