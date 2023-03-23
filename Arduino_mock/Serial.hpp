#pragma once

class Serial{
private:
    int channel;
public:
    Serial();
    ~Serial();
    unsigned char read();
    void write(unsigned char c);
    void write(const char *str);
    void write(const char *buffer, int size);
    void print(const char *str);
    void start_listening();
    void flush();
};