int open(const char *pathname, int flags) {
  int fd;

  asm("movq $0x2, %%rax\n\t"
      "movq $0644, %%rdx\n\t"
      "movq %1, %%rdi\n\t"
      "movl %2, %%esi\n\t"
      "syscall\n\t"
      "mov %%eax, %0"
      : "=rm"(fd)
      : "rm"(pathname), "rm"(flags)
      : "memory");

  return fd;
}
