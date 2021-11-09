#include <sys/syscall.h>

long syscall(long number, ...);

void _start()
{
  static const char Message[] = "Hello, World!\n";
  syscall(SYS_write, 1, Message, sizeof(Message)-1);
  syscall(SYS_exit, 0);
}

