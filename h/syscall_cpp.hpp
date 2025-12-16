#include "../lib/hw.h"
class PCB;
class SCC;

void* operator new(size_t);

void operator delete(void*) noexcept;

void* operator new[](size_t);

void operator delete[](void*) noexcept;


class Thread {
public:
    Thread (void (*body)(void*), void* arg);
    virtual ~Thread ();
    int start ();
    static void dispatch ();
    static int sleep (time_t);
protected:
    Thread ();
    virtual void run () {}
private:
    PCB* myHandle;
    void (*body)(void*);
    void* arg;
    static void wrapper(void*);
};


class Semaphore {
public:
    Semaphore (unsigned init = 1);
    virtual ~Semaphore ();
    int wait ();
    int signal ();
    int timedWait (time_t);
    int tryWait();
private:
    SCC* myHandle;
};


class PeriodicThread : public Thread {
public:
    void terminate ();
protected:
    PeriodicThread (time_t period);
    virtual void periodicActivation () {}
private:
    void run() override;
    time_t period;
};


class Console {
public:
    static char getc ();
    static void putc (char);
};