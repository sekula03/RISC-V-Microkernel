//
// Created by os on 5/11/24.
//
#include "../h/riscv.hpp"
#include "../h/printing.hpp"
#include "../h/MemoryAllocator.hpp"
#include "../h/PCB.hpp"
#include "../h/Scheduler.hpp"
#include "../h/SCC.hpp"
#include "../h/CCC.hpp"

void Riscv::interruptRoutine() {
    uint64 volatile scause = r_scause();
    uint volatile op_code = r_a0();
    uint64 volatile operand[4] = {r_a1(), r_a2(), r_a3(), r_a4()};
    if (scause == 0x8000000000000001UL) {
        //TIMER
        mc_sip(SIP_SSIP);
        PCB::time_counter++;
        Scheduler::awaken();
        if (PCB::time_counter >= PCB::running->time_slice) {
            PCB::time_counter = 0;
            PCB::dispatch();
        }
    }
    else if (scause == 0x8000000000000009UL) {
        //CONSOLE
        //mc_sip(Riscv::SIP_SEIP);
        int irq = plic_claim();
        if (irq == 0x0a)  {
            volatile char console_status = *(char*)CONSOLE_STATUS;
            uint8 counter = 0;
            char data;
            while ((console_status & CONSOLE_RX_STATUS_BIT) && counter < CCC::MAX_TRANSFER) {
                data = *(char*)CONSOLE_RX_DATA;
                CCC::putc_controller(data);
                counter++;
                console_status = *(char*)CONSOLE_STATUS;
            }
        }
        plic_complete(irq);
    }
    else if (scause == 0x0000000000000008UL || scause == 0x0000000000000009UL) {
        //SYSCALL
        if (op_code == 0x01UL) {
            //mem_alloc
            __asm__ volatile("sd %0, 80(s0)" : : "r"(MemoryAllocator::allocate(operand[0])));
        }
        else if (op_code == 0x02UL) {
            //mem_free
            __asm__ volatile("sd %0, 80(s0)" : : "r"(MemoryAllocator::deallocate((void*)operand[0])));
        }
        else if (op_code == 0x11UL) {
            //thread_create
            PCB* handle = PCB::create((void (*)(void*))operand[1], (void*)operand[2], (void*)operand[3]);
            *(PCB**)operand[0] = handle;
            __asm__ volatile("sd %0, 80(s0)" : : "r"(handle?0:-1));
        }
        else if (op_code == 0x12UL) {
            //thread_exit
            PCB::running->finished = true;
            PCB::time_counter = 0;
            PCB::dispatch();
            __asm__ volatile("sd %0, 80(s0)" : : "r"(0));
        }
        else if (op_code == 0x13UL) {
            //thread_dispatch
            PCB::time_counter = 0;
            PCB::dispatch();
        }
        else if (op_code == 0x21UL) {
            //sem_open
            SCC* handle = SCC::create((uint64)operand[1]);
            *(SCC**)operand[0] = handle;
            __asm__ volatile("sd %0, 80(s0)" : : "r"(handle?0:-1));
        }
        else if (op_code == 0x22UL) {
            //sem_close
            ((SCC*)operand[0])->close();
            __asm__ volatile("sd %0, 80(s0)" : : "r"(0));
        }
        else if (op_code == 0x23UL) {
            //sem_wait
            ((SCC*)operand[0])->wait();
            __asm__ volatile("sd %0, 80(s0)" : : "r"(PCB::running->wait_status));
        }
        else if (op_code == 0x24UL) {
            //sem_signal
            ((SCC*)operand[0])->signal();
            __asm__ volatile("sd %0, 80(s0)" : : "r"(0));
        }
        else if (op_code == 0x25UL) {
            //sem_timedwait
            ((SCC*)operand[0])->timedwait((uint64)operand[1]);
            __asm__ volatile("sd %0, 80(s0)" : : "r"(PCB::running->wait_status));
        }
        else if (op_code == 0x26UL) {
            //sem_trywait
            ((SCC*)operand[0])->trywait();
            __asm__ volatile("sd %0, 80(s0)" : : "r"(PCB::running->wait_status));
        }
        else if (op_code == 0x31UL) {
            //time_sleep
            PCB::time_counter = 0;
            PCB::running->wait_status = -3;
            Scheduler::put_sleep(PCB::running, (uint64)operand[0]);
            PCB::dispatch();
        }
        else if (op_code == 0x41UL) {
            //getc
            __asm__ volatile("sd %0, 80(s0)" : : "r"(CCC::getc_user()));
        }
        else if (op_code == 0x42UL) {
            //putc
            CCC::putc_user((char)operand[0]);
        }
        w_sepc(r_sepc() + 4);
    }
    else {
        //MISCELLANEOUS
        printString("SEPC: ");
        printInt(Riscv::r_sepc());
        printString("\n");
        printString("SCAUSE: ");
        printInt(scause);
        printString("\n");
        CCC::drain();
        __asm__ volatile("li t0, 0x5555");
        __asm__ volatile("li t1, 0x100000");
        __asm__ volatile("sw t0, 0(t1)");
    }
}

void Riscv::new_thread_start() {
    __asm__ volatile("csrw sepc, ra");
    __asm__ volatile("sret");
}