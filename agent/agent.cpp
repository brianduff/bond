#include <jvmti.h>
#include <iostream>

static bool inMethod = false;

static bool checkError(jvmtiError errNum) {
  if (errNum == JVMTI_ERROR_NONE) {
    return true;
  }
  std::cerr << "JVMTI: ERROR: " << errNum << "\n";
  return false;
}

jstring getClassName(JNIEnv* env, jclass cls) {
  jclass clazz = env->GetObjectClass(cls);
  jmethodID methodId = env->GetMethodID(clazz, "getName", "()Ljava/lang/String;");
  env->DeleteLocalRef(clazz);

  if (!methodId) {
    return NULL;
  }
  return (jstring) env->CallObjectMethod(cls, methodId);
}

void JNICALL
callbackMethodEntryEvent(jvmtiEnv *jvmti,
            JNIEnv* env,
            jthread thread,
            jmethodID method) {
  if (inMethod) return;
  inMethod = true;
  jclass declaringClassId;
  jvmti->GetMethodDeclaringClass(method, &declaringClassId);

  jstring className = getClassName(env, declaringClassId);
  const char *classNameNative;
  if (className) {
    classNameNative = env->GetStringUTFChars(className, 0);
  } else {
    classNameNative = "<UNKNOWN>";
  }

  char *methodName;
  char *sig;
  char *gsig;
  jvmtiError err = jvmti->GetMethodName(method, &methodName, &sig, &gsig);
  std::cout << "Called " << classNameNative << "." << methodName << sig << "\n";

  env->DeleteLocalRef(declaringClassId);
  if (className) {
    env->ReleaseStringUTFChars(className, classNameNative);
  }
  inMethod = false;
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

  std::cout << "Registered all callbacks. Whee!\n";

  return JNI_OK;
}
