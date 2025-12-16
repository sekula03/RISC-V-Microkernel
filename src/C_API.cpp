//
// Created by os on 5/15/24.
//

#include "../h/syscall_c.hpp"
#include "../h/riscv.hpp"

void* mem_alloc(size_t size) {
    uint64 size_in_blocks = (size + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE;
    if (size_in_blocks * MEM_BLOCK_SIZE - size < 4) size_in_blocks++;
    Riscv::w_a1(size_in_blocks);
    Riscv::w_a0(0x01UL);
    __asm__ volatile ("ecall");
    return (void*)Riscv::r_a0();
}

int mem_free(void* mem) {
    Riscv::w_a1((uint64)mem);
    Riscv::w_a0(0x02UL);
    __asm__ volatile ("ecall");
    return (int)Riscv::r_a0();
}

int thread_create(PCB** handle, void(*start_routine)(void*), void* arg) {
    void* thread_stack = mem_alloc(DEFAULT_STACK_SIZE);
    if (!thread_stack) return -2;
    Riscv::w_a4((uint64)thread_stack);
    Riscv::w_a3((uint64)arg);
    Riscv::w_a2((uint64)start_routine);
    Riscv::w_a1((uint64)handle);
    Riscv::w_a0(0x11UL);
    __asm__ volatile ("ecall");
    return (int)Riscv::r_a0();
}

int thread_exit() {
    Riscv::w_a0(0x12UL);
    __asm__ volatile ("ecall");
    return (int)Riscv::r_a0();
}

void thread_dispatch() {
    Riscv::w_a0(0x13UL);
    __asm__ volatile ("ecall");
}

int sem_open(SCC** handle, uint init) {
    Riscv::w_a2((uint64)init);
    Riscv::w_a1((uint64)handle);
    Riscv::w_a0(0x21UL);
    __asm__ volatile ("ecall");
    return (int)Riscv::r_a0();
}

int sem_close(SCC* handle) {
    Riscv::w_a1((uint64)handle);
    Riscv::w_a0(0x22UL);
    __asm__ volatile ("ecall");
    return (int)Riscv::r_a0();
}

int sem_wait(SCC* id) {
    Riscv::w_a1((uint64)id);
    Riscv::w_a0(0x23UL);
    __asm__ volatile ("ecall");
    return (int)Riscv::r_a0();
}

int sem_signal(SCC* id) {
    Riscv::w_a1((uint64)id);
    Riscv::w_a0(0x24UL);
    __asm__ volatile ("ecall");
    return (int)Riscv::r_a0();
}

int sem_timedwait(SCC* id, time_t timeout) {
    if (timeout == 0) return -2;
    Riscv::w_a2((uint64)timeout);
    Riscv::w_a1((uint64)id);
    Riscv::w_a0(0x25UL);
    __asm__ volatile ("ecall");
    return (int)Riscv::r_a0();
}

int sem_trywait(SCC* id) {
    Riscv::w_a1((uint64)id);
    Riscv::w_a0(0x26UL);
    __asm__ volatile ("ecall");
    return (int)Riscv::r_a0();
}

int time_sleep(time_t time) {
    if (time == 0) return 0;
    Riscv::w_a1((uint64)time);
    Riscv::w_a0(0x31UL);
    __asm__ volatile ("ecall");
    return (int)Riscv::r_a0();
}

char getc() {
    Riscv::w_a0(0x41UL);
    __asm__ volatile ("ecall");
    return (char)Riscv::r_a0();
}

void putc(char c) {
    Riscv::w_a1((uint64)c);
    Riscv::w_a0(0x42UL);
    __asm__ volatile ("ecall");
}
