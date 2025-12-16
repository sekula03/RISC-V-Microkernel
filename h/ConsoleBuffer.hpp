#include "../lib/hw.h"
class SCC;

class ConsoleBuffer {
private:
    static const int capacity = 256;
    char *buffer;
    int head, tail;
    SCC* spaceAvailable;
    SCC* itemAvailable;
    SCC* mutexHead;
    SCC* mutexTail;
public:
    ConsoleBuffer();
    ~ConsoleBuffer();
    void put(char);
    char get();
    void put_s(char);
    char get_s();
    bool empty();
    static void* operator new(size_t);
    static void operator delete(void*) noexcept;
};