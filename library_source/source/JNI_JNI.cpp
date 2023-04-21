#include <jni.h>       // JNI header provided by JDK
#include "../header/JNI_JNI.h"  // Generated
#include <iostream>    // C++ standard IO header
#include <stdio.h>
#include "../header/CatoptricController.hpp"
#include "../header/CatoptricSurface.hpp"
using namespace std;


std::shared_ptr<CatoptricSurface> controller{new CatoptricSurface()};

JNIEXPORT void JNICALL Java_JNI_JNI_test(JNIEnv *env, jobject thisObj) {
    if (controller == nullptr) {
        std::cout << "Initializing the controller" << std::endl;
        controller.reset(new CatoptricSurface());
    }
    controller->reset(TEST_RESET);
    return;
}

JNIEXPORT void JNICALL Java_JNI_JNI_quit(JNIEnv *env, jobject thisObj) {
    controller = nullptr;
    return;
}

JNIEXPORT void JNICALL Java_JNI_JNI_reset(JNIEnv *env, jobject thisObj) {
    if (controller == nullptr) {
        std::cout << "Initializing the controller" << std::endl;
        controller.reset(new CatoptricSurface());
    }
    controller->reset(REGULAR_RESET);
    return;
}

JNIEXPORT void JNICALL Java_JNI_JNI_move(JNIEnv *, jobject thisObj, jint row, jint col,jint motor,jint direction,jint step) {
    // create a catoptric surface object, and index into each row
    // call the stepMotor function
    if (controller == nullptr) {
        std::cout << "Initializing the controller" << std::endl;
        controller.reset(new CatoptricSurface());
    }
    controller->moveMirror(row, col, motor, direction, step);
    return;
}