//
// Created by os on 5/30/24.
//

#include "../h/ConsoleBuffer.hpp"
#include "../h/SCC.hpp"
#include "../h/MemoryAllocator.hpp"
#include "../h/CCC.hpp"
#include "../h/syscall_c.hpp"

ConsoleBuffer::ConsoleBuffer() : head(0), tail(0) {
    itemAvailable = SCC::create(0);
    spaceAvailable = SCC::create(capacity);
    mutexHead = SCC::create(1);
    mutexTail = SCC::create(1);
    buffer = (char*) mem_alloc(ConsoleBuffer::capacity);
}

ConsoleBuffer::~ConsoleBuffer() {
    itemAvailable->close();
    spaceAvailable->close();
    mutexHead->close();
    mutexTail->close();
    mem_free(buffer);
}

void ConsoleBuffer::put(char val) {
    spaceAvailable->wait();
    mutexTail->wait();
    buffer[tail] = val;
    tail = (tail + 1) % capacity;
    mutexTail->signal();
    itemAvailable->signal();
}

char ConsoleBuffer::get() {
    itemAvailable->wait();
    mutexHead->wait();
    char ret = buffer[head];
    head = (head + 1) % capacity;
    mutexHead->signal();
    spaceAvailable->signal();
    return ret;
}

void ConsoleBuffer::put_s(char val) {
    if (spaceAvailable->trywait() == 1) return;
    mutexTail->wait();
    buffer[tail] = val;
    tail = (tail + 1) % capacity;
    mutexTail->signal();
    itemAvailable->signal();
}

char ConsoleBuffer::get_s() {
    if (itemAvailable->trywait() == 1) return CCC::EOF;
    mutexHead->wait();
    char ret = buffer[head];
    head = (head + 1) % capacity;
    mutexHead->signal();
    spaceAvailable->signal();
    return ret;
}

void* ConsoleBuffer::operator new(size_t size) {
    uint64 size_in_blocks = (size + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE;
    if (size_in_blocks * MEM_BLOCK_SIZE - size < 4) size_in_blocks++;
    return MemoryAllocator::allocate(size_in_blocks);
}

void ConsoleBuffer::operator delete(void *mem) noexcept {
    MemoryAllocator::deallocate(mem);
}

bool ConsoleBuffer::empty() {
    return (head == tail)?true:false;
}