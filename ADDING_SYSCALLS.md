# Adding Syscalls to XV6

This Description assumes that my merge request "1f0c05e16b1df49596063dd11e40c16188973405" was accepted by the upstream.


## Checklist

- update kernel/syscall.h to contain the new syscall name. Example:
  - #define SYS_freepg 22
- update kernel/syscall.c. Example:
  - extern uint64 sys_freepg(void);
  - [SYS_freepg]  sys_freepg,
- update kernel/defs.h. Example:
  - Below //kalloc.c  -> See assumes the the function freepg() is defined in kalloc.c
  - int             freepg(void);
- update kernel/sysproc.c. Example:
  - uint64
  - sys_freepg(void) { return freepg()};
- update user/user.h. Example:
- Below // system calls
  - int freepg(void);

Remark: If my request is not accepted you have to addionaly update user/usys.pl to reflect the changes. 

## Ideas

- It would be good if we could reduce the places which are required to add a new system call. Ideally only 1-2 Files (kernel and user) should be updated. Best would be only one file.






