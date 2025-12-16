#include "../lib/hw.h"
class List;
class PCB;
class SCC;

class Scheduler {
public:
    static PCB* get();
    static void put(PCB*);
    static void initialize();
    static void awaken();
    static void put_sleep(PCB*, time_t);
    static void finish();
private:
    virtual ~Scheduler() = 0;
    static List* ready;
    static List* sleeping;
};