//
// Created by os on 5/19/24.
//

#include "../h/Scheduler.hpp"
#include "../h/riscv.hpp"
#include "../h/PCB.hpp"
#include "../h/MemoryAllocator.hpp"
#include "../h/syscall_c.hpp"

PCB* PCB::running = nullptr;
uint64 PCB::time_counter = 0;

PCB* PCB::create(void (*body)(void *), void *arg, void *stack, bool mode) {
    return new PCB(body, arg, stack, mode);
}

PCB::PCB(void (*body)(void *), void *arg, void *stack, bool mode) {
    this->body = body;
    this->arg = arg;
    this->stack = stack;
    this->time_slice = DEFAULT_TIME_SLICE;
    this->finished = false;
    this->wait_status = 0;
    this->mode = mode;
    this->context.ra = (uint64)wrapper;
    this->context.sp = stack?(uint64)((uint8*)stack + DEFAULT_STACK_SIZE):0;
    this->context.sepc = 0;
    this->context.sstatus = 0;
    if (body != nullptr) Scheduler::put(this);
}

PCB::~PCB() {
    mem_free(stack);
}

void PCB::yield() {
    Riscv::w_a0(0x13UL);
    __asm__ volatile("ecall");
}

void PCB::dispatch() {
    if (!running->finished && running->wait_status != -3) Scheduler::put(running);
    running->context.sepc = Riscv::r_sepc();
    running->context.sstatus = Riscv::r_sstatus();
    PCB* oldR = running;
    if (oldR->finished) delete running;
    running = Scheduler::get();
    contextSwitch(&oldR->context, &running->context);
    Riscv::w_sepc(running->context.sepc);
    Riscv::w_sstatus(running->context.sstatus);
}

void PCB::wrapper() {
    if (running->mode) Riscv::mc_sstatus(Riscv::SSTATUS_SPP);
    Riscv::new_thread_start();
    running->body(running->arg);
    running->finished = true;
    yield();
}

void* PCB::operator new(size_t size) {
    uint64 size_in_blocks = (size + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE;
    if (size_in_blocks * MEM_BLOCK_SIZE - size < 4) size_in_blocks++;
    return MemoryAllocator::allocate(size_in_blocks);
}

void* PCB::operator new[](size_t size) {
    uint64 size_in_blocks = (size + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE;
    if (size_in_blocks * MEM_BLOCK_SIZE - size < 4) size_in_blocks++;
    return MemoryAllocator::allocate(size_in_blocks);
}

void PCB::operator delete(void *mem) noexcept {
    MemoryAllocator::deallocate(mem);
}

void PCB::operator delete[](void *mem) noexcept {
    MemoryAllocator::deallocate(mem);
}

bool PCB::isFinished() const { return finished; }

void PCB::setFinished() { finished = true; }

void PCB::initialize() {
    running = nullptr;
    time_counter = 0;
}