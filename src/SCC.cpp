//
// Created by os on 5/28/24.
//
#include "../h/SCC.hpp"
#include "../h/list.hpp"
#include "../h/MemoryAllocator.hpp"
#include "../h/PCB.hpp"
#include "../h/Scheduler.hpp"

void* SCC::operator new(size_t size) {
    uint64 size_in_blocks = (size + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE;
    if (size_in_blocks * MEM_BLOCK_SIZE - size < 4) size_in_blocks++;
    return MemoryAllocator::allocate(size_in_blocks);
}

void* SCC::operator new[](size_t size) {
    uint64 size_in_blocks = (size + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE;
    if (size_in_blocks * MEM_BLOCK_SIZE - size < 4) size_in_blocks++;
    return MemoryAllocator::allocate(size_in_blocks);
}

void SCC::operator delete(void *mem) noexcept {
    MemoryAllocator::deallocate(mem);
}

void SCC::operator delete[](void *mem) noexcept {
    MemoryAllocator::deallocate(mem);
}

SCC::SCC(uint init) {
    value = init;
    queue = new List();
}

SCC *SCC::create(uint init) {
    return new SCC(init);
}

void SCC::close() {
    PCB* t = queue->get();
    while(t != nullptr) {
        if (t->wait_status == -3) {
            t->wait_status = -1;
            Scheduler::put(t);
        }
        t = queue->get();
    }
    delete this;
}

void SCC::wait() {
    if (--value < 0)  {
        PCB::running->wait_status = -3;
        queue->put(PCB::running);
        PCB::time_counter = 0;
        PCB::dispatch();
    }
    else PCB::running->wait_status = 0;
}

void SCC::signal() {
    if (++value <= 0) {
        PCB* t = queue->get();
        while (t && t->wait_status != -3) {
            value++;
            t = queue->get();
        }
        if (t) {
            t->wait_status = 0;
            Scheduler::put(t);
        }
    }
}

void SCC::timedwait(time_t timeout) {
    if (--value < 0)  {
        PCB::running->wait_status = -3;
        Scheduler::put_sleep(PCB::running, timeout);
        queue->put(PCB::running);
        PCB::time_counter = 0;
        PCB::dispatch();
    }
    else PCB::running->wait_status = 0;
}

int SCC::trywait() {
    if (value <= 0)
        PCB::running->wait_status = 1;
    else {
        value--;
        PCB::running->wait_status = 0;
    }
    return PCB::running->wait_status;
}

SCC::~SCC() {
    delete queue;
};