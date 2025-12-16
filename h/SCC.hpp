#include "../lib/hw.h"
class List;
class PCB;

class SCC {
public:
    static SCC* create(uint);
    void close();
    void wait();
    void signal();
    void timedwait(time_t);
    int trywait();
private:
    explicit SCC(uint);
    ~SCC();
    static void* operator new(size_t);
    static void* operator new[](size_t);
    static void operator delete(void*) noexcept ;
    static void operator delete[](void*) noexcept ;
    int value;
    List* queue;
};