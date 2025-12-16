//
// Created by os on 5/21/24.
//
#include "../h/node.hpp"
#include "../h/MemoryAllocator.hpp"

void *Node::operator new(size_t size) {
    uint64 size_in_blocks = (size + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE;
    if (size_in_blocks * MEM_BLOCK_SIZE - size < 4) size_in_blocks++;
    return MemoryAllocator::allocate(size_in_blocks);
}

void Node::operator delete(void *mem) noexcept {
    MemoryAllocator::deallocate(mem);
}