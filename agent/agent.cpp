#include <jvmti.h>
#include <iostream>

static bool checkError(jvmtiError errNum) {
  if (errNum == JVMTI_ERROR_NONE) {
    return true;
  }
  std::cerr << "JVMTI: ERROR: " << errNum << "\n";
  return false;
}

void JNICALL
callbackMethodEntryEvent(jvmtiEnv *jvmti,
            JNIEnv* jni_env,
            jthread thread,
            jmethodID method) {
  char *methodName;
  char *sig;
  char *gsig;
  if (!checkError(jvmti->GetMethodName(method, &methodName, &sig, &gsig))) {
    std::cout << "Called " << methodName << "\n";
  }
}

JNIEXPORT jint JNICALL 
Agent_OnLoad(JavaVM *vm, char *options, void *reserved) {
  std::cout << "Agent onload\n";

  jvmtiEnv *jvmti;

  jint result = vm->GetEnv((void **) &jvmti, JVMTI_VERSION_1_2);
  if (result != JNI_OK) {
    std::cerr << "Unable to access JVMTI!!!";
  }

  jvmtiCapabilities capabilities;
  (void) memset(&capabilities, 0, sizeof(jvmtiCapabilities));

  capabilities.can_generate_method_entry_events = 1;

  if (!checkError(jvmti->AddCapabilities(&capabilities))) {
    std::cout << "Failed to add capabilities\n";
    return JNI_ERR;
  }

  if (!checkError(jvmti->SetEventNotificationMode(JVMTI_ENABLE, JVMTI_EVENT_METHOD_ENTRY, (jthread) NULL))) {
    std::cout << "Failed to enable method entry notifications\n";
    return JNI_ERR;
  }

  jvmtiEventCallbacks eventCallbacks;
  (void) memset(&eventCallbacks, 0, sizeof(eventCallbacks));
  eventCallbacks.MethodEntry = &callbackMethodEntryEvent;

  if (!checkError(jvmti->SetEventCallbacks(&eventCallbacks, (jint) sizeof(eventCallbacks)))) {
    std::cout << "Failed to set event callbacks\n";
    return JNI_ERR;    
  }

  return JNI_OK;
}