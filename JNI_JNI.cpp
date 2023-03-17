#include <jni.h>       // JNI header provided by JDK
#include <iostream>    // C++ standard IO header
#include <stdio.h>
#include "JNI_JNI.h"  // Generated
#include "./JNI-Interface/service/JNI_service.hpp" //Servce Interface definition
#include "./JNI-Interface/service/JNI_service_impTest.hpp" //Implementation of the servce interface

using namespace std;


JNI_service_impTest * service = new JNI_service_impTest();

JNIEXPORT void JNICALL Java_JNI_JNI_test(JNIEnv *env, jobject thisObj) {
    service->test();
    return;
}

JNIEXPORT void JNICALL Java_JNI_JNI_quit(JNIEnv *env, jobject thisObj) {
    service->quit();
    return;
}

JNIEXPORT void JNICALL Java_JNI_JNI_reset(JNIEnv *env, jobject thisObj) {
    service->reset();
    return;
}

JNIEXPORT void JNICALL Java_JNI_JNI_move(JNIEnv *, jobject thisObj, jint row, jint col,jint motor,jint direction,jint step) {
    // create a catoptric surface object, and index into each row
    // call the stepMotor function
    service->move({row,col,motor,direction,step});
    return;
}