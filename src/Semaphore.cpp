//
// Created by os on 5/28/24.
//

#include "../h/syscall_cpp.hpp"
#include "../h/syscall_c.hpp"

Semaphore::Semaphore(unsigned int init) {
    sem_open(&myHandle, init);
}

Semaphore::~Semaphore() {
    sem_close(myHandle);
}

int Semaphore::wait() {
    return sem_wait(myHandle);
}

int Semaphore::signal() {
    return sem_signal(myHandle);
}

int Semaphore::timedWait(time_t t) {
    return sem_timedwait(myHandle, t);
}

int Semaphore::tryWait() {
    return sem_trywait(myHandle);
}