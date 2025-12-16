//
// Created by os on 5/15/24.
//
#include "../h/list.hpp"
#include "../h/Scheduler.hpp"
#include "../h/node.hpp"
#include "../h/MemoryAllocator.hpp"
#include "../h/SCC.hpp"
#include "../h/PCB.hpp"


void List::put(PCB *pcb) {
    if (pcb == nullptr) return;
    Node* new_node = new Node(pcb);
    if (head) tail->next = new_node;
    else head = new_node;
    tail = new_node;
}

PCB* List::get() {
    if (!head) return nullptr;
    Node* tmp = head;
    PCB* ret_val = head->pcb;
    head = head->next;
    delete tmp;
    if (!head) tail = nullptr;
    return ret_val;
}

void *List::operator new(size_t size) {
    uint64 size_in_blocks = (size + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE;
    if (size_in_blocks * MEM_BLOCK_SIZE - size < 4) size_in_blocks++;
    return MemoryAllocator::allocate(size_in_blocks);
}

void List::operator delete(void *mem) noexcept {
    MemoryAllocator::deallocate(mem);
}

void List::put_sleep(PCB *pcb, time_t time) {
    if (head == nullptr) {
        head = new Node(pcb, time);
    }
    else {
        if (head->time > time) {
            Node* new_node = new Node(pcb, time);
            new_node->next = head;
            head->time -= time;
            head = new_node;
        }
        else {
            Node *curr = head;
            time_t total = head->time;
            while (curr->next && curr->next->time + total <= time) {
                curr = curr->next;
                total += curr->time;
            }
            Node* new_node = new Node(pcb, time - total);
            new_node->next = curr->next;
            if (curr->next) {
                total += curr->next->time;
                curr->next->time = total - time;
            }
            curr->next = new_node;
        }
    }
}

void List::awaken() {
    if (head && --(head->time) == 0)
        while (head && head->time == 0) {
            if (head->pcb->wait_status == -3) {
                head->pcb->wait_status = -2;
                Scheduler::put(head->pcb);
            }
            Node* tmp = head->next;
            delete head;
            head = tmp;
        }
}

List::~List() {
    Node* curr;
    while (head) {
        curr = head->next;
        delete head;
        head = curr;
    }
}