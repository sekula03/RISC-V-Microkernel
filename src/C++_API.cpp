//
// Created by os on 5/14/24.
//

#include "../h/syscall_cpp.hpp"
#include "../h/syscall_c.hpp"

void* operator new(size_t size) {
    return mem_alloc(size);
}

void operator delete(void* mem) noexcept {
    mem_free(mem);
}

void* operator new[](size_t size) {
    return mem_alloc(size);
}

void operator delete[](void* mem) noexcept {
    mem_free(mem);
}