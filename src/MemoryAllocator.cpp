//
// Created by os on 5/11/24.
//

#include "../h/MemoryAllocator.hpp"

MemoryAllocator::FF* MemoryAllocator::mem_head = nullptr;

void MemoryAllocator::initialize() {
    mem_head = (FF*)HEAP_START_ADDR;
    mem_head->size = (uint8*)HEAP_END_ADDR - (uint8*)HEAP_START_ADDR;
    mem_head->next = nullptr;
    mem_head->prev = nullptr;
}

void* MemoryAllocator::allocate(size_t size) {
    if (!mem_head || !size) return nullptr;
    FF* curr = mem_head;
    uint64 real_size = size * MEM_BLOCK_SIZE;
    while(curr && curr->size < real_size) curr = curr->next;
    if (!curr) return nullptr;
    if (curr->size - real_size < MEM_BLOCK_SIZE) {
        real_size = curr->size;
        if (curr->prev) curr->prev->next = curr->next;
        if (curr->next) curr->next->prev = curr->prev;
        if (curr == mem_head) mem_head = curr->next;
    }
    else {
        auto* new_frag = (FF*)((uint8*)curr + real_size);
        new_frag->size = curr->size - real_size;
        new_frag->prev = curr->prev;
        new_frag->next = curr->next;
        if (curr->prev) curr->prev->next = new_frag;
        if (curr->next) curr->next->prev = new_frag;
        if (curr == mem_head) {
            mem_head = new_frag;
            new_frag->prev = nullptr;
        }
    }
    *(uint32*)curr = real_size / MEM_BLOCK_SIZE;
    return (uint8*)curr + 4;
}

int MemoryAllocator::deallocate(void *mem) {
    if (!mem) return -1;
    if ((uint8 *) HEAP_START_ADDR > (uint8 *) mem || (uint8 *) HEAP_END_ADDR <= (uint8 *) mem)
        return -2;
    uint8 *frag_start = (uint8*)mem - 4;
    uint64 frag_size = *(uint32 *)frag_start * MEM_BLOCK_SIZE;
    if ((uint64)((uint8 *) HEAP_END_ADDR - frag_start) < frag_size)
        return -3;
    if (!mem_head) {
        mem_head = (FF*)frag_start;
        mem_head->next = nullptr;
        mem_head->prev = nullptr;
        mem_head->size = frag_size;
    }
    else {
        FF *after = mem_head, *new_frag = (FF*)frag_start;
        while (after->next && (uint8*)after->next < frag_start)
            after = after->next;
        if ((uint8*)after > frag_start) {
            if ((uint8*)after < frag_start + frag_size)
                return -4;
            if ((uint8*)after == frag_start + frag_size) {
                new_frag->next = after->next;
                if (after->next) after->next->prev = new_frag;
                new_frag->size = frag_size + after->size;
            }
            else {
                new_frag->next = after;
                after->prev = new_frag;
                new_frag->size = frag_size;
            }
            mem_head = new_frag;
            new_frag->prev = nullptr;
        }
        else if (after->next) {
            if ((uint8*)after + after->size > frag_start || frag_start + frag_size > (uint8*)after->next)
                return -5;
            if (frag_start + frag_size == (uint8*)after->next) {
                new_frag->next = after->next->next;
                if (after->next->next) after->next->next->prev = new_frag;
                new_frag->size = frag_size + after->next->size;
            }
            else {
                new_frag->next = after->next;
                after->next->prev = new_frag;
                new_frag->size = frag_size;
            }
            if ((uint8*)after + after->size == frag_start) {
                after->next = new_frag->next;
                if (new_frag->next) new_frag->next->prev = after;
                after->size += new_frag->size;
            }
            else {
                after->next = new_frag;
                new_frag->prev = after;
            }
        }
        else {
            if ((uint8*)after + after->size > frag_start)
                return -6;
            if ((uint8*)after + after->size == frag_start)
                after->size += frag_size;
            else {
                after->next = new_frag;
                new_frag->prev = after;
                new_frag->size = frag_size;
            }
        }
    }
    return 0;
}