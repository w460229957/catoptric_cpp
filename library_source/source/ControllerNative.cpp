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
//Launch a thread to execute the pending commands. do not detach the thread
//because we need to wait for the thread to finish before we quit the program
static int exit_flag = false;
std::thread CommandThread([]() {
  while (!exit_flag)
  {
    try{
      auto command = PendingCommands.wait_and_pop();
      (*command)(&surface);
    }
    catch(const std::exception& e){
      std::cout << "Exception caught in c++ native code" << std::endl;
      std::cout << e.what() << std::endl;
    }
  }
});

JNIEXPORT void JNICALL Java_controller_ControllerNative_moveMirror(JNIEnv *env, jobject java_this, jint row, jint col, jint motor, jint direction, jint step)
{
    PendingCommands.push(
      [row, col, motor, direction, step](CatoptricSurface *that){
          that->moveMirror(row, col, motor, direction, step);
          that->update();
      });          
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
JNIEXPORT void JNICALL Java_controller_ControllerNative_quit(JNIEnv * env , jobject java_this)
{
  std::cout<<"Quitting...(Not perfect)" <<std::endl;
  exit_flag = true;
  PendingCommands.push([](CatoptricSurface* that){
    return;
  });
  CommandThread.join();
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