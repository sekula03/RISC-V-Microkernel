class ConsoleBuffer;

class CCC {
public:
    static const char EOF = -1;
    static const int MAX_TRANSFER = 64;

    static void initialize();
    static void finish();
    static char getc_user();
    static void putc_user(char);
    static char getc_controller();
    static void putc_controller(char);

    static void controller_data_transfer(void*);

    static void drain();
private:
    virtual ~CCC() = 0;
    static ConsoleBuffer* input;
    static ConsoleBuffer* output;
};