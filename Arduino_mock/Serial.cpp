#include "Serial.hpp"
#include <asm-generic/errno-base.h>
#include <cstdlib>
#include <cstring>
#include <string>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>

Serial::Serial() {
    //Create a named pipe and store the file descriptor to channel
    this->channel = mkfifo("/tmp/serial", 0666);
    
    if (this->channel != 0 && errno != EEXIST) {
        std::cout << "Error creating named pipe" << std::endl;
        abort();
    }
    
    this->channel = open("/tmp/serial", O_RDWR);

    if (this->channel == -1) {
        std::cout << "Error opening named pipe" << std::endl;
        abort();
    }

    std::cout << "Serial channel opened" << std::endl;
}


Serial::~Serial() {
    unlink("/tmp/serial");
}


unsigned char Serial::read() {
    unsigned char c;
    int n;
    do{
        int n = ::read(this->channel, &c, 1);
    }while(n !=0 && errno == EINTR);

    if (n != 0) {
        std::cout << "Error reading from named pipe" << std::endl;
        abort();
    }
    return c;
}


void Serial::write(unsigned char c) {
    int n;
    do{
        n = ::write(this->channel, &c, 1);
    }while(n !=0 && errno == EINTR);

    if (n != 0) {
        std::cout << "Error writing to named pipe" << std::endl;
        abort();
    }
}


void Serial::write(const char *str) {
    int n;
    do{
        n = ::write(this->channel, str, strlen(str));
    }while(n !=0 && errno == EINTR);

    if (n != 0) {
        std::cout << "Error writing to named pipe" << std::endl;
        abort();
    }
}


void Serial::write(const char *buffer, int size) {
    int n;
    do{
        n = ::write(this->channel, buffer, size);
    }while(n !=0 && errno == EINTR);

    if (n != 0) {
        std::cout << "Error writing to named pipe" << std::endl;
        abort();
    }
}