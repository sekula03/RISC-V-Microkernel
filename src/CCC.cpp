//
// Created by os on 5/28/24.
//
#include "../h/CCC.hpp"
#include "../h/ConsoleBuffer.hpp"
#include "../h/PCB.hpp"

ConsoleBuffer* CCC::input = nullptr;
ConsoleBuffer* CCC::output = nullptr;

void CCC::putc_user(char c) {
    output->put(c);
}

char CCC::getc_user() {
    return input->get();
}

void CCC::putc_controller(char c) {
    input->put_s(c);
}

char CCC::getc_controller() {
    return output->get_s();
}

void CCC::initialize() {
    input = new ConsoleBuffer;
    output = new ConsoleBuffer;
}

void CCC::finish() {
    delete input;
    delete output;
}

void CCC::controller_data_transfer(void*) {
    while(true) {
        char c;
        char console_status;
        console_status = *(char *) CONSOLE_STATUS;
        uint8 counter = 0;
        while ((console_status & CONSOLE_TX_STATUS_BIT) && counter < CCC::MAX_TRANSFER) {
            c = getc_controller();
            if (c == EOF) break;
            *(char *) CONSOLE_TX_DATA = c;
            counter++;
            console_status = *(char *) CONSOLE_STATUS;
        }
        PCB::yield();
    }
}

void CCC::drain() {
    while(!output->empty()) {
        char c;
        char console_status;
        console_status = *(char *) CONSOLE_STATUS;
        while (console_status & CONSOLE_TX_STATUS_BIT) {
            c = getc_controller();
            if (c == EOF) break;
            *(char *) CONSOLE_TX_DATA = c;
            console_status = *(char *) CONSOLE_STATUS;
        }
    }
}