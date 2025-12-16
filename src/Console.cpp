//
// Created by os on 5/28/24.
//
#include "../h/syscall_c.hpp"
#include "../h/syscall_cpp.hpp"

void Console::putc(char c) {
    ::putc(c);
}

char Console::getc() {
    return ::getc();
}

