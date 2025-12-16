#include "../lib/hw.h"
class Node;
class PCB;
class SCC;

class List {
public:
    List(): head(nullptr), tail(nullptr) {}
    PCB* get();
    void put(PCB*);
    void awaken();
    void put_sleep(PCB*, time_t);
    static void* operator new(size_t);
    static void operator delete(void*) noexcept;
    ~List();
private:
    Node *head, *tail;
};