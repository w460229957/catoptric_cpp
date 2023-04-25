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
  // Retrive the Blockmode field from the java object
  jclass jController = env->GetObjectClass(java_this);

  jfieldID jBlockMode = env->GetFieldID(jController, "Block_mode", "Z");

  if (jBlockMode == NULL)
  {
    std::cout << "Failed to find the field named \"Block_mode\" in the java class \"ControllerNative\"\n";
    std::cout << "Please check your Java class ControllerNative ...." << std::endl;
  }
  
  jboolean jBlockValue = env->GetBooleanField(java_this, jBlockMode);
  // If the blockmode is set, then block the thread until the surface is finished moving the mirror
  try
  {
    if (jBlockValue == JNI_TRUE)
    {
      surface.moveMirror(row, col, motor, direction, step);
      surface.update();
    }
    else
    {
      PendingCommands.push([row, col, motor, direction, step](CatoptricSurface *that)
                           {                
            that->moveMirror(row, col, motor, direction, step);
            that->update(); });
    }
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
JNIEXPORT void JNICALL Java_controller_ControllerNative_reset(JNIEnv *, jobject)
{
  surface.reset(REGULAR_RESET);
  surface.update();
}

/*
 * Class:     controller_ControllerNative
 * Method:    test
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_controller_ControllerNative_test(JNIEnv *, jobject)
{
  surface.reset(TEST_RESET);
  surface.update();
}

/*
 * Class:     controller_ControllerNative
 * Method:    quit
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_controller_ControllerNative_quit(JNIEnv *, jobject)
{
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