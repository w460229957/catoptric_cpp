#include <jni.h>                                   // JNI header provided by JDK
#include "../header/controller_ControllerNative.h" // Generated
#include <iostream>                                // C++ standard IO header
#include <stdio.h>
#include "../header/CatoptricController.hpp"
#include "../header/CatoptricSurface.hpp"
#include "../header/CommandQueue.hpp"
#include "../header/CatoptricRow.hpp"
#include <string>
#include <future>
#include <functional>

static CatoptricSurface surface;

CommandQueue<std::function<void(CatoptricSurface *)>> PendingCommands;

JNIEXPORT void JNICALL Java_controller_ControllerNative_moveMirror(JNIEnv *env, jobject java_this, jint row, jint col, jint motor, jint direction, jint step)
{
  try
  {
      PendingCommands.push(
        [row, col, motor, direction, step](CatoptricSurface *that){
            that->moveMirror(row, col, motor, direction, step);
            that->update();
        });          
  }
  catch (const std::invalid_argument &e)
  {
    std::cout << "Exception caught in c++ native code" << std::endl;
    jclass java_exec = (env->FindClass("java/lang/IndexOutOfBoundsException"));
    if (env != NULL)
      env->ThrowNew(java_exec, e.what());
    else
    {
      std::cout << "Failed to find the class type of IndexOutOfBoundException class" << std::endl;
      std::cout << "Please check your JDK integrity...." << std::endl;
    }
  }
}

/*
 * Class:     controller_ControllerNative
 * Method:    resetInitializing Catoptric Row 
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_controller_ControllerNative_reset(JNIEnv *, jobject){
    PendingCommands.push([](CatoptricSurface* that){
    that->reset(0);
    that->update();
  }); 
}

/*
 * Class:     controller_ControllerNative
 * Method:    test
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_controller_ControllerNative_test(JNIEnv *, jobject)
{
  PendingCommands.push(
    [](CatoptricSurface* that){
      that->reset(1);
      that->update();
    }
  ); 
}

/*
 * Class:     controller_ControllerNative
 * Method:    quit
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_controller_ControllerNative_quit(JNIEnv *, jobject)
{
  PendingCommands.push(
    [](CatoptricSurface * that){
      std::cout<<"Quitting...(Not perfect)" <<std::endl;
      std::exit(0);
    }
  );
  return;
}

/*
 * Class:     controller_ControllerNative
 * Method:    getPendingMessages
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_controller_ControllerNative_getPendingMessages(JNIEnv *, jobject)
{
  return;
}