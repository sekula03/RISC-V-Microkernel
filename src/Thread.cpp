//
// Created by os on 5/19/24.
//
#include "../h/syscall_cpp.hpp"
#include "../h/syscall_c.hpp"
#include "../h/PCB.hpp"

Thread::Thread(void (*body)(void *), void *arg):
    myHandle(nullptr), body(body), arg(arg) {}

Thread::Thread(): myHandle(nullptr), body(nullptr), arg(nullptr) {}

int Thread::start() {
    return body?thread_create(&myHandle, body, arg):thread_create(&myHandle, wrapper, this);
}

void Thread::dispatch() {
    thread_dispatch();
}

int Thread::sleep(time_t time) {
    return time_sleep(time);
}

Thread::~Thread() = default;

void Thread::wrapper(void *obj) {
    ((Thread*)obj)->run();
}