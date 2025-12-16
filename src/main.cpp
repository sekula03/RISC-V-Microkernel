//
// Created by os on 5/11/24.
//

#include "../h/riscv.hpp"
#include "../h/syscall_cpp.hpp"
#include "../h/syscall_c.hpp"
#include "../h/Scheduler.hpp"
#include "../h/PCB.hpp"
#include "../h/SCC.hpp"
#include "../h/CCC.hpp"
#include "../h/MemoryAllocator.hpp"
#include "../h/printing.hpp"

void userMain();

void wrapper(void*) {
    userMain();
}

void main() {
    Riscv::w_stvec((uint64)&Riscv::interruptRoutineWrapper);
    MemoryAllocator::initialize();
    PCB::initialize();
    Scheduler::initialize();
    CCC::initialize();

    PCB::running = PCB::create(nullptr, nullptr, nullptr, false);

    PCB* console = PCB::create(CCC::controller_data_transfer, nullptr, mem_alloc(DEFAULT_STACK_SIZE), false);

    PCB* userProgram;
    thread_create(&userProgram, wrapper, nullptr);

    Riscv::ms_sstatus(Riscv::SSTATUS_SIE);

    while (!userProgram->isFinished()) {
        thread_dispatch();
    }

    console->setFinished();

    CCC::drain();


    Scheduler::finish();
    CCC::finish();
    __asm__ volatile("li t0, 0x5555");
    __asm__ volatile("li t1, 0x100000");
    __asm__ volatile("sw t0, 0(t1)");
    // page 103 privileged manual
}