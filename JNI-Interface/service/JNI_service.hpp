#pragma once 

/**
 * @file JNI_service.h
 * @brief defines @interface JNI_service, which is used to specify the interface between the JNI_JNI.cpp and the actual implementation.
 * @author Zhengyuan Zhang
*/
#include <jni.h>
#include <initializer_list>

class JNI_service{
public:
    virtual void move(std::vector<int>) = 0;
    virtual void reset() = 0;
    virtual void test() = 0;
    virtual void quit() = 0;
};


