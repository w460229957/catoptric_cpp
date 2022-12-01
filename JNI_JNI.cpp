#include <jni.h>       // JNI header provided by JDK
#include <iostream>    // C++ standard IO header
#include "JNI_JNI.h"  // Generated
using namespace std;

JNIEXPORT void JNICALL Java_JNI_JNI_test(JNIEnv *env, jobject thisObj) {
    cout << "Hello World from C++!" << endl;
    return;
}