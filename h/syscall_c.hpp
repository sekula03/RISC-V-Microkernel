#include "../lib/hw.h"

class PCB;
class SCC;

void* mem_alloc(size_t);

int mem_free(void*);

int thread_create(PCB**, void(*)(void*), void*);

int thread_exit();

void thread_dispatch();

int sem_open(SCC**, uint);

int sem_close(SCC*);

int sem_wait(SCC*);

int sem_signal(SCC*);

int sem_timedwait(SCC*, time_t);

int sem_trywait(SCC*);

int time_sleep(time_t);

char getc();

void putc(char);