//
// Created by os on 5/19/24.
//

#include "../h/Scheduler.hpp"
#include "../h/list.hpp"

List* Scheduler::ready = nullptr;
List* Scheduler::sleeping = nullptr;

PCB* Scheduler::get() {
    return ready->get();
}

void Scheduler::put(PCB* pcb) {
    ready->put(pcb);
}

void Scheduler::initialize() {
    ready = new List;
    sleeping = new List;
}

void Scheduler::put_sleep(PCB *pcb, time_t time) {
    sleeping->put_sleep(pcb, time);
}

void Scheduler::awaken() {
    sleeping->awaken();
}

void Scheduler::finish() {
    delete ready;
    delete sleeping;
}