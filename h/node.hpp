#include "../lib/hw.h"
class PCB;
class SCC;

class Node {
public:
    explicit Node(PCB* pcb, time_t time = 0): pcb(pcb),
        next(nullptr), time(time){};
    static void* operator new(size_t);
    static void operator delete(void*) noexcept;
private:
    friend class List;
    PCB* pcb;
    Node* next;
    uint64 time;
};