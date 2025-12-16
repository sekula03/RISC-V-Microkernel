#include "../lib/hw.h"

class MemoryAllocator {
public:
    static void* allocate(size_t);
    static int deallocate(void*);
    static void initialize();
private:
    virtual ~MemoryAllocator() = 0;
    struct FF {
        uint64 size;
        struct FF* next;
        struct FF* prev;
    };
    static FF* mem_head;
};