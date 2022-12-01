#include <jni.h>       // JNI header provided by JDK
#include <iostream>    // C++ standard IO header
#include <stdio.h>
#include "JNI_JNI.h"  // Generated
using namespace std;

JNIEXPORT void JNICALL Java_JNI_JNI_test(JNIEnv *env, jobject thisObj) {
    cout << "Test!" << endl;
    return;
}

JNIEXPORT void JNICALL Java_JNI_JNI_quit(JNIEnv *env, jobject thisObj) {
    cout << "Quit!" << endl;
    return;
}

JNIEXPORT void JNICALL Java_JNI_JNI_reset(JNIEnv *env, jobject thisObj) {
    cout << "Reset!" << endl;
    return;
}

JNIEXPORT void JNICALL Java_JNI_JNI_move(JNIEnv *, jobject thisObj, jint motor, jint position, jint row, jint col, jint surface) {
    char str[100];
    sprintf(str, "motor: %d \n position: %d \n row: %d \n col: %d \n, surface: %d", motor, position, row, col, surface);
    cout << str << endl;
    return;
}