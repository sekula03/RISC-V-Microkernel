//
// Created by os on 5/28/24.
//

#include "../h/syscall_c.hpp"
#include "../h/syscall_cpp.hpp"

PeriodicThread::PeriodicThread(time_t period) {
    this->period = period;
}

void PeriodicThread::terminate() {
    period = 0;
}

void PeriodicThread::run() {
    while (period != 0) {
        sleep(period);
        periodicActivation();
    }
}