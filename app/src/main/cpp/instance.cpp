#include <jni.h>
#include <string>

#include <android/log.h>

#define  LOG_TAG    "nate"
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
extern "C"
JNIEXPORT void JNICALL
Java_com_nate_ndkdemo_Instance_callJavaStaticMethod(JNIEnv *env, jclass clazz) {
    //查找指定的类
    jclass instance = env->FindClass("com/nate/ndkdemo/JniInstance");
    if (instance == nullptr) {
        return;
    }
    // 查找指定的方法
    jmethodID call_static_method = env->GetStaticMethodID(instance, "callStaticMethod",
                                                          "(Ljava/lang/String;)V");
    if (call_static_method == nullptr) {
        return;
    }
    //创建Java字符串
    jstring data = env->NewStringUTF("from c++!");
    //调用java静态方法
    env->CallStaticVoidMethod(instance, call_static_method, data);
    //删除本地引用
    env->DeleteLocalRef(instance);
    env->DeleteLocalRef(data);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_nate_ndkdemo_Instance_callJavaInstanceMethod(JNIEnv *env, jclass cls) {
    //查找指定的类
    jclass clazz = env->FindClass("com/nate/ndkdemo/JniInstance");
    if (clazz == nullptr) {
        return;
    }
    // 查找构造方法
    jmethodID constructor = env->GetMethodID(clazz, "<init>", "()V");

    if (constructor == nullptr) {
        return;
    }

    jmethodID instanceMethod = env->GetMethodID(clazz, "callJavaInstanceMethod",
                                                "(Ljava/lang/String;)V");
    if (instanceMethod == nullptr) {
        return;
    }
    //创建java对象
    jobject instance = env->NewObject(clazz, constructor);
    jstring data = env->NewStringUTF("from c++ string");
    env->CallVoidMethod(instance, instanceMethod, data);
    env->DeleteLocalRef(clazz);
    env->DeleteLocalRef(data);
    env->DeleteLocalRef(instance);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_nate_ndkdemo_Instance_setAge(JNIEnv *env, jclass clazz, jint age) {
    jfieldID ageField = env->GetStaticFieldID(clazz, "AGE", "I");
    env->SetStaticIntField(clazz, ageField, age);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_nate_ndkdemo_Instance_setName(JNIEnv *env, jobject thiz, jstring name) {
    jclass clazz = env->GetObjectClass(thiz);
    if (clazz == nullptr) {
        return;
    }
    jfieldID nameField = env->GetFieldID(clazz, "name", "Ljava/lang/String;");
    env->SetObjectField(thiz, nameField, name);
}
extern "C"
JNIEXPORT jint JNICALL
Java_com_nate_ndkdemo_Instance_sum(JNIEnv *env, jclass clazz, jintArray data) {
    int length = env->GetArrayLength(data);
    int result = 0;
    int index = 0;
    int *nativeArray = (jint *) malloc(sizeof(jint) * length);

    memset(nativeArray, 0, sizeof(int));
    env->GetIntArrayRegion(data, 0, length, nativeArray);
    for (; index < length; index++) {
        result += nativeArray[index];
        LOGE("data = %d result=%d", nativeArray[index], result);
    }

    free(nativeArray);
    return result;

//    int *array = env->GetIntArrayElements(data, NULL);
//    int index = 0;
//    for (; index < length; ++index) {
//        result += array[index];
//        LOGE("data = %d result=%d", array[index], result);
//    }
//    env->ReleaseIntArrayElements(data, array, NULL);
    return result;
}
extern "C"
JNIEXPORT jintArray JNICALL
Java_com_nate_ndkdemo_Instance_array(JNIEnv *env, jclass clazz, jint length, jint data) {
    jintArray array = env->NewIntArray(length);
    if (array == nullptr) {
        return nullptr;
    }
    int *c_array = env->GetIntArrayElements(array, NULL);
    int index = 0;
    for (; index < length; ++index) {
        c_array[index] = data;
    }
    env->ReleaseIntArrayElements(array, c_array, NULL);
    return array;
}
extern "C"
JNIEXPORT void JNICALL
Java_com_nate_ndkdemo_Instance_log(JNIEnv *env, jclass clazz, jstring data) {
    const char *log = env->GetStringUTFChars(data, 0);
    LOGE("log = %s ", log);
    env->ReleaseStringUTFChars(data, log);
}

void willCrash() {
    int i = 10;
    int y = i / 0;
}

extern "C"
JNIEXPORT jobjectArray JNICALL
Java_com_nate_ndkdemo_Instance_setStringArray(JNIEnv *env, jclass clazz, jint length,
                                              jstring context) {
    jclass stringClass = env->FindClass("java/lang/String");
    jobjectArray jobjectArray = env->NewObjectArray(length, stringClass, NULL);
    int a = 1 / 0;
    int index = 0;
    for (; index < length; index++) {
        const char *c_data = env->GetStringUTFChars(context, NULL);
        jstring data = env->NewStringUTF(c_data);
        env->SetObjectArrayElement(jobjectArray, index, data);
        env->ReleaseStringUTFChars(data, c_data);
    }

    return jobjectArray;

}extern "C"
JNIEXPORT void JNICALL
Java_com_nate_ndkdemo_Instance_localVariable(JNIEnv *env, jclass clazz, jint size) {
    int index = 0;
    for (; index < size; ++index) {
        jclass stringClass = env->FindClass("java/lang/String");
        env->DeleteLocalRef(stringClass);
//
    }
}