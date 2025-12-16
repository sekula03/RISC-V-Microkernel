#include "../lib/hw.h"

class PCB {
public:
    static PCB* create(void (*body)(void*), void* arg, void* stack, bool mode = true);
    static void yield();
    static PCB* running;
    bool isFinished() const;
    void setFinished();
    static void initialize();
private:
    friend class Riscv;
    friend class SCC;
    friend class List;
    PCB(void (*body)(void*), void* arg, void* stack, bool mode);
    ~PCB();
    static void dispatch();
    static void wrapper();
    static uint64 time_counter;
    struct Context {
        uint64 ra;
        uint64 sp;
        uint64 sepc;
        uint64 sstatus;
    };
    Context context{};
    void (*body)(void*);
    void* arg;
    void* stack;
    uint64 time_slice;
    bool finished;
    int wait_status;
    bool mode;
    static void* operator new(size_t);
    static void* operator new[](size_t);
    static void operator delete(void*) noexcept ;
    static void operator delete[](void*) noexcept ;
    static void contextSwitch(Context*, Context*);
};