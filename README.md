# JNI教程



# JNI数据类型和类型签名

## 数据类型

JNI的数据类型包括：**基本类型**和**引用类型**。这一点和Java的语言特性一致，基本类型包括jboolean、jchar、jint、jlong、jbyte、jshort、jfloat、jdouble、void，与Java类型的对应关系如下：

基本类型

| JNI类型  | Java类型 |      描述      |
| -------- | -------- | :------------: |
| jboolean | boolean  | 无符号8位整型  |
| jbyte    | byte     | 有符号8位整型  |
| jchar    | char     | 无符号16位整型 |
| jshort   | short    | 有符号16位整型 |
| jint     | int      |    32位整型    |
| jlong    | long     |    64位整型    |
| jfloat   | float    |    32位整型    |
| jdouble  | double   |    64位整型    |
| void     | void     |     无类型     |

JNI中引用类型主要有类、对象和数组，这点也上符合Java的语法规范，对应的关系如下：

| JNI 类型      | Java引用类型 | 描述        |
| ------------- | ------------ | ----------- |
| jobject       | Object       | Object类型  |
| jclass        | Class        | Class类型   |
| jstring       | String       | String类型  |
| jobjectArray  | Object[]     | 对象数组    |
| jbooleanArray | boolean[]    | boolean数组 |
| jbyteArray    | byte[]       | byte数组    |
| jcharArray    | char[]       | char数组    |
| jshortArray   | short[]      | short数组   |
| jintArray     | int[]        | int数组     |
| jlongArray    | long[]       | long数组    |
| jfloatArray   | float[]      | float数组   |
| jdoubleArray  | double[]     | double数组  |
| jthrowable    | Throwable    | Throwable   |



# 常用命令

## javah -jni自动生成头文件

举例说明：

- 生成普通的JNI头文件

  ```shell
  javah -classpath path -jni -d outputdirpath com.nate.JavaNativeCode
  ```

- 在Java函数中包含Android相关的参数代码，则需要在classpath中添加android.jar包的绝对路径地址

  ```shell
  javah -classpath path:$ANDROID_HOME/path/android.jar -jni -d outputdirpath com.nate.JavaNativeCodeWithAndroid
  ```

## javap -s -p 查看函数签名

-s: 显示签名（只显示public类型的签名） -p:显示所有函数、成员变量的签名

举例说明：

```shell
javap -classpath pacakage_path_dir -s -p com.nate.JavaCode
```

# JNI 方法签名

## 为什么JNI中突然多出了一个概念叫"签名"？

因为Java是支持函数重载的，也就是说，可以定义相同方法名，但是不同参数的方法，然后Java根据其不同的参数，找到其对应的实现的方法。这样是很好，所以说JNI肯定要支持的，那JNI要怎么支持那，如果仅仅是根据函数名，没有办法找到重载的函数的，所以为了解决这个问题，JNI就衍生了一个概念——"签名"，即将参数类型和返回值类型的组合。如果拥有一个该函数的签名信息和这个函数的函数名，我们就可以顺序的找到对应的Java层中的函数了。

## 如果查看类中的方法的签名

 javap -s -p Instance.class

```java
Compiled from "Instance.java"
public class com.nate.ndkdemo.Instance {
  private static final int AGE;
    descriptor: I
  private java.lang.String name;
    descriptor: Ljava/lang/String;
  public com.nate.ndkdemo.Instance();
    descriptor: ()V

  public static native void log(java.lang.String);
    descriptor: (Ljava/lang/String;)V

  public static native int sum(int[]);
    descriptor: ([I)I

  public static native int[] array(int, int);
    descriptor: (II)[I

  public native void setName(java.lang.String);
    descriptor: (Ljava/lang/String;)V

  public static native void callJavaStaticMethod();
    descriptor: ()V

  public static native void callJavaInstanceMethod();
    descriptor: ()V

  public static native void setAge(int);
    descriptor: (I)V

  public static int getAge();
    descriptor: ()I

  public java.lang.String getName();
    descriptor: ()Ljava/lang/String;
}

```

## JNI规范定义的函数签名信息

> 当参数为引用类型的时候，参数类型的标示的根式为"L包名"，其中包名的`.`(点)要换成"/"，看我上面的例子就差不多，比如`String`就是`Ljava/lang/String`，`Menu`为`Landroid/view/Menu`。

| 类型标示 | Java类型 |
| -------- | :------: |
| Z        | boolean  |
| B        |   byte   |
| C        |   char   |
| S        |  short   |
| I        |   int    |
| J        |   long   |
| F        |  float   |
| D        |  double  |

这个 其实很好记的，除了boolean和long，其他都是首字母大写。

如果返回值是void，对应的签名是**V**。
 这里重点说1个特殊的类型，一个是数组及Array

| 类型标示           | Java类型 |
| ------------------ | :------: |
| [签名              |   数组   |
| [i                 |  int[]   |
| [Ljava/lang/Object | String[] |

# JNI头文件分析

## 命名规则：

```c
extern "C" JNIEXPORT 返回值 JNICALL Java_全路径类名_方法名__参数签名(JNIEnv* , jobject, 其它参数);
```

例子

```c
extern "C"
JNIEXPORT void JNICALL
Java_com_nate_ndkdemo_Instance_callJavaStaticMethod(JNIEnv *env, jclass clazz) {

}
```

## 说明：

**extern “C”** ：如果在使用的是C++，在函数前面加extern “C”，表示按照C的方式编译。

**JNIEXPORT**、**JNICALL**：这两个关键词是宏定义，主要是注明该函数是JNI函数，当虚拟机加载so库时，如果发现函数含有这两个宏定义时，就会链接到对应的Java层的native方法。

**Java_**：标识该函数来源于Java。

**__参数签名**：如果是重载方法，则有参数签名，否则没有。参数签名的斜杠“/”改为“_”，分号“；”改为”_2”连接。

**JNIEnv**：指向函数表指针的指针，函数表里面定义了很多JNI函数，通过这些函数可以实现Java层和JNI层的交互，就是说JNIEnv调用JNI函数可以访问Java虚拟机，操作Java对象。

**jobject**：调用该方法的Java实例对象。对于Java的native方法，static和非static方法的区别在于第二个参数，static的为jclass，非static的为jobject。

示例：

# JNI 打印log

JNI中打印Log 需要借助Android 内置的log库 #include <android/log.h>

使用方式也比较简单引入log库

```c
# include <android/log.h>

# define  LOG_TAG    "nate"

# define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

JNIEXPORT void JNICALL
Java_com_nate_ndkdemo_Instance_log(JNIEnv *env, jclass clazz, jstring data) {
    const char *log = env->GetStringUTFChars(data, 0);
    LOGE("log = %s ", log);
    env->ReleaseStringUTFChars(data, log);
}
```

 打印Log 有很多种模式，我们在这里是打印Error级别的错误

```java
Log的级别如下

- **ANDROID_LOG_VERBOSE**
- **ANDROID_LOG_DEBUG**
- **ANDROID_LOG_INFO**
- **ANDROID_LOG_WARN**
- **ANDROID_LOG_ERROR**
```

# 方法调用

## JNI方法定义

JNI回调Java方法定义

```java
package com.nate.ndkdemo;
mport android.util.Log;

public class JniInstance {
  public static void callStaticMethod(String str) {
    Log.e("nate", "callStaticMethod called -->" + str);
}

public void callJavaInstanceMethod(String str) {
    Log.e("nate", "callJavaInstanceMethod called -->" + str);
}

}
```

## 调用java静态方法

JNI方法实现

```c++
#include <jni.h>
#include <string>

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

```

## 调用java实例方法

JNI方法实现

```c++
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
```

## 成员变量访问

定义JNI方法

```java
package com.nate.ndkdemo;

public class Instance {

    private static final int AGE = 21;

    private String name = "nate";

    public static native void callJavaStaticMethod();

    public static native void callJavaInstanceMethod();

    public static native void setAge(int age);

    public native void setName(String name);

    public static int getAge() {
        return AGE;
    }

    public String getName() {
        return name;
    }
}
```

# 修改静态成员变量

```c++
extern "C"
JNIEXPORT void JNICALL
Java_com_nate_ndkdemo_Instance_setAge(JNIEnv *env, jclass clazz, jint age) {
    jfieldID ageField = env->GetStaticFieldID(clazz, "AGE", "I");
    env->SetStaticIntField(clazz, ageField, age);
}
```

# 修改实例成员变量

```c++
extern "C"
JNIEXPORT void JNICALL
Java_com_nate_ndkdemo_Instance_setName(JNIEnv *env, jobject thiz, jstring name) {
    jclass clazz = env->GetObjectClass(thiz);

    jfieldID nameField = env->GetFieldID(clazz, "name", "Ljava/lang/String;");
    env->SetObjectField(thiz, nameField, name);
}
```

# 数组操作

相关API接口

```c++
JNIEnv提供的函数          数组类型	        本地类型
GetBooleanArrayElements	jbooleanArray	jboolean
GetByteArrayElements	jbyteArray	    jbyte
GetCharArrayElements	jcharArray	    jchar
GetShortArrayElements	jshortArray	    jshort
GetIntArrayElements	    jintArray	    jint
GetLongArrayElements	jlongArray	    jlong
GetFloatArrayElements	jfloatArray	    jfloat
GetDoubleArrayElements	jdoubleArray	jdouble
```

## JNI提供了方法可以对Java层传递过来的数组进行指针操作

```shell
GetArrayLength 获取数组的长度
GetIntArrayElements 获取数据的指针地址
ReleaseIntArrayElements 释放数组指针
```

定义JNI方法

```java
package com.nate.ndkdemo;

public class Instance {

    public static native int[] array(int length,int data);

    public static native int sum(int[] data);
}
```

对用实现函数

```c++
#include <jni.h>
#include <string>

#include <android/log.h>
#define  LOG_TAG    "nate"
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
extern "C"
JNIEXPORT jint JNICALL
Java_com_nate_ndkdemo_Instance_sum(JNIEnv *env, jclass clazz, jintArray data) {
    int length = env->GetArrayLength(data);
    int *array = env->GetIntArrayElements(data, NULL);
    int index = 0;
    int result = 0;
    for (; index < length; ++index) {
        result += array[index];
        LOGE("data = %d result=%d", array[index], result);
    }
    env->ReleaseIntArrayElements(data, array, NULL);
    return result;
}
```

## 这个例子采用的是指针操作，使用起来比较简单，还有另外一种操作方式使用起来比较麻烦

```java
GetArrayLength 获取数组的长度
GetIntArrayRegion 获取java层的数据给指定的指针
```

```c++
extern "C"
JNIEXPORT jint JNICALL
Java_com_nate_ndkdemo_Instance_sum(JNIEnv *env, jclass clazz, jintArray data) {
    int length = env->GetArrayLength(data);
    int result = 0;
    int index = 0;
    int* nativeArray = (jint*)malloc(sizeof(jint) * length);

    memset(nativeArray, 0, sizeof(int));
    env->GetIntArrayRegion(data, 0, length, nativeArray);
    for (; index < length; index++) {
        result += nativeArray[index];
        LOGE("data = %d result=%d", nativeArray[index], result);
    }

    free(nativeArray);
    return result;
}
```

从JNI层返回数据给Java层

```c++
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
        c_array[index]=data;
    }
    env->ReleaseIntArrayElements(array, c_array, NULL);
    return array;
}
```

## 操作对象类型数组

在C/C++代码中，int类型的数组对应JNI中的jintArray，而类似字符串数组这种类型的，在Jni里对应的使用`jobjectArray`来声明

```
GetObjectArrayElement(JNIEnv *env, jobjectArray array, jsize index)--返回对应索引值的object.返回的是一个数组元素的值。

SetObjectArrayElement(JNIEnv* env, jobjectArray array, jsize i
```

描述：定义一个JNI方法放回一个String数组

```java
package com.nate.ndkdemo;

public class Instance {

    public static native String[] setStringArray(int length,String data);

}
```

```c++
extern "C"
JNIEXPORT jobjectArray JNICALL
Java_com_nate_ndkdemo_Instance_setStringArray(JNIEnv *env, jclass clazz, jint length,
                                              jstring context) {
    jclass stringClass = env->FindClass("java/lang/String");
    jobjectArray jobjectArray = env->NewObjectArray(length, stringClass, NULL);
    int index = 0;
    for (; index < length; index++) {
        const char *c_data = env->GetStringUTFChars(context, NULL);
        jstring data = env->NewStringUTF(c_data);
        env->SetObjectArrayElement(jobjectArray, index, data);
        env->ReleaseStringUTFChars(data, c_data);
    }
    return jobjectArray;

}
```

# NDK 崩溃信息分析

我们先看一段奔溃日志

```shell
 crash_dump64: obtaining output fd from tombstoned, type: kDebuggerdTombstone
03-03 19:38:58.773   800   800 I /system/bin/tombstoned: received crash request for pid 32064
03-03 19:38:58.775 32088 32088 I crash_dump64: performing dump of process 32064 (target tid = 32064)
03-03 19:38:58.782 32088 32088 F DEBUG   : *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** ***
03-03 19:38:58.782 32088 32088 F DEBUG   : Build fingerprint: 'HUAWEI/ALP-AL00/HWALP:9/HUAWEIALP-AL00/9.1.0.339C00:user/release-keys'
03-03 19:38:58.782 32088 32088 F DEBUG   : Revision: '0'
03-03 19:38:58.782 32088 32088 F DEBUG   : ABI: 'arm64'
03-03 19:38:58.782 32088 32088 F DEBUG   : Happend: 'Tue Mar  3 19:38:58 2020
03-03 19:38:58.782 32088 32088 F DEBUG   : '
03-03 19:38:58.782 32088 32088 F DEBUG   : SYSVMTYPE: Maple
03-03 19:38:58.782 32088 32088 F DEBUG   : APPVMTYPE: Art
03-03 19:38:58.782 32088 32088 F DEBUG   : pid: 32064, tid: 32064, name: om.nate.ndkdemo  >>> com.nate.ndkdemo <<<
03-03 19:38:58.782 32088 32088 F DEBUG   : signal 11 (SIGSEGV), code 1 (SEGV_MAPERR), fault addr 0x0
03-03 19:38:58.782 32088 32088 F DEBUG   : Cause: null pointer dereference
03-03 19:38:58.782 32088 32088 F DEBUG   :     x0  0000000000000000  x1  0000000000000000  x2  0000000000000000  x3  0000007fe41f1cb0
03-03 19:38:58.782 32088 32088 F DEBUG   :     x4  0000000000000007  x5  0000007fe41f1d30  x6  0000000000000040  x7  0000007f9639ae12
03-03 19:38:58.782 32088 32088 F DEBUG   :     x8  0000000000000001  x9  0000000000000005  x10 0000000000430000  x11 0000000000000001
03-03 19:38:58.782 32088 32088 F DEBUG   :     x12 0000000000000002  x13 69abecd1f89db687  x14 0000007293b73000  x15 ffffffffffffffff
03-03 19:38:58.782 32088 32088 F DEBUG   :     x16 00000071f0a6cd28  x17 00000071f0a36010  x18 0000000000000001  x19 000000720da15c00
03-03 19:38:58.783 32088 32088 F DEBUG   :     x20 0000000000000000  x21 000000720da15c00  x22 0000007fe41f2170  x23 00000071f129ba0d
03-03 19:38:58.783 32088 32088 F DEBUG   :     x24 0000000000000008  x25 00000072942de5e0  x26 000000720da15ca0  x27 0000000000000002
03-03 19:38:58.783 32088 32088 F DEBUG   :     x28 0000007fe41f1ea0  x29 0000007fe41f1de0
03-03 19:38:58.783 32088 32088 F DEBUG   :     sp  0000007fe41f1dc0  lr  00000071f0a36178  pc  00000071f0a36030
03-03 19:38:58.924  1145  1145 I HwNormalizedSpline: DayMode:getBrightnessLevel lux =60.0, mDefaultBrightnessFromLux =38.825485
03-03 19:38:58.938 31969 31969 E HwCryptoMessageService: ConfigInfo error:getSimIMSI(int subid) method get imsi FAILED that subid = 0
03-03 19:38:58.939 31969 31969 E HwCryptoMessageService: ConfigInfo error:getSimIMSI(int subid) method get imsi FAILED that subid = 1
03-03 19:38:58.955 32088 32088 F DEBUG   :
03-03 19:38:58.955 32088 32088 F DEBUG   : backtrace:
03-03 19:38:58.956 32088 32088 F DEBUG   :     #00 pc 0000000000011030  /data/app/com.nate.ndkdemo-RjXVlBt1L037Dwwyo5MDqA==/lib/arm64/libnative-lib.so (_JNIEnv::GetStringUTFChars(_jstring*, unsigned char*)+32)
03-03 19:38:58.956 32088 32088 F DEBUG   :     #01 pc 0000000000011174  /data/app/com.nate.ndkdemo-RjXVlBt1L037Dwwyo5MDqA==/lib/arm64/libnative-lib.so (Java_com_nate_ndkdemo_Instance_setStringArray+160)
03-03 19:38:58.956 32088 32088 F DEBUG   :     #02 pc 000000000057ade0  /system/lib64/libart.so (art_quick_generic_jni_trampoline+144)
03-03 19:38:58.956 32088 32088 F DEBUG   :     #03 pc 000000000057204c  /system/lib64/libart.so (art_quick_invoke_static_stub+604)
03-03 19:38:58.956 32088 32088 F DEBUG   :     #04 pc 00000000000d4d4c  /system/lib64/libart.so (art::ArtMethod::Invoke(art::Thread*, unsigned int*, unsigned int, art::JValue*, char const*)+232)
03-03 19:38:58.956 32088 32088 F DEBUG   :     #05 pc 000000000028810c  /system/lib64/libart.so (art::interpreter::ArtInterpreterToCompiledCodeBridge(art::Thread*, art::ArtMethod*, art::ShadowFrame*, unsigned short, art::JValue*)+344)
03-03 19:38:58.956 32088 32088 F DEBUG   :     #06 pc 0000000000282114  /system/lib64/libart.so (bool art::interpreter::DoCall<false, false>(art::ArtMethod*, art::Thread*, art::ShadowFrame&, art::Instruction const*, unsigned short, art::JValue*)+968)
03-03 19:38:58.956 32088 32088 F DEBUG   :     #07 pc 0000000000542d04  /system/lib64/libart.so (MterpInvokeStatic+204)
03-03 19:38:58.956 32088 32088 F DEBUG   :     #08 pc 0000000000564514  /system/lib64/libart.so (ExecuteMterpImpl+14612)
```

这段日志还好因为日志基本上已经能定位问题，但是很多的ndk crash是需要经过处理后才能看的懂的

### 第一种方法：ndk-stack

这个命令行工具包含在NDK工具的安装目录，和ndk-build和其他一些常用的NDK命令放在一起，比如在我的电脑上，其位置是/android-ndk-r9d/ndk-stack。根据Google官方文档，NDK从r6版本开始提供ndk-stack命令，如果你用的之前的版本，建议还是尽快升级至最新的版本。使用ndk –stack命令也有两种方式

#### 使用ndk-stack实时分析日志

在运行程序的同时，使用adb获取logcat日志，并通过管道符输出给ndk-stack，同时需要指定包含符号表的so文件位置；如果你的程序包含了多种CPU架构，在这里需求根据错误发生时的手机CPU类型，选择不同的CPU架构目录，如：

```shell
adb shell logcat | ndk-stack -sym $PROJECT_PATH/obj/local/armeabi
```

当崩溃发生时，会得到如下的信息：

```shell
********** Crash dump: **********
Build fingerprint: 'HUAWEI/ALP-AL00/HWALP:9/HUAWEIALP-AL00/9.1.0.339C00:user/release-keys'
pid: 32064, tid: 32064, name: om.nate.ndkdemo  >>> com.nate.ndkdemo <<<
signal 11 (SIGSEGV), code 1 (SEGV_MAPERR), fault addr 0x0
Stack frame #00 pc 0000000000011030  /data/app/com.nate.ndkdemo-RjXVlBt1L037Dwwyo5MDqA==/lib/arm64/libnative-lib.so (_JNIEnv::GetStringUTFChars(_jstring*, unsigned char*)+32): Routine _JNIEnv::GetStringUTFChars(_jstring*, unsigned char*) at /Users/didi/Library/Android/sdk/ndk/20.0.5594570/toolchains/llvm/prebuilt/darwin-x86_64/sysroot/usr/include/jni.h:847
Stack frame #01 pc 0000000000011174  /data/app/com.nate.ndkdemo-RjXVlBt1L037Dwwyo5MDqA==/lib/arm64/libnative-lib.so (Java_com_nate_ndkdemo_Instance_setStringArray+160): Routine Java_com_nate_ndkdemo_Instance_setStringArray at /Users/didi/workcode/native-js/hummer_19/NdkDemo/app/src/main/cpp/instance.cpp:143
Stack frame #02 pc 000000000057ade0  /system/lib64/libart.so (art_quick_generic_jni_trampoline+144)
Stack frame #03 pc 000000000057204c  /system/lib64/libart.so (art_quick_invoke_static_stub+604)
Stack frame #04 pc 00000000000d4d4c  /system/lib64/libart.so (art::ArtMethod::Invoke(art::Thread*, unsigned int*, unsigned int, art::JValue*, char const*)+232)
```

已经可以定位到具体的方法行数了

### 第二种方法：使用addr2line

这个方法适用于那些，不满足于上述ndk-stack的简单用法，而喜欢刨根问底的程序员们，这两个方法可以揭示ndk-stack命令的工作原理是什么，尽管用起来稍微麻烦一点，但是可以满足一下程序员的好奇心。

先简单说一下这两个命令，在绝大部分的linux发行版本中都能找到他们，如果你的操作系统是linux，而你测试手机使用的是Intel x86系列，那么你使用系统中自带的命令就可以了。然而，如果仅仅是这样，那么绝大多数人要绝望了，因为恰恰大部分开发者使用的是Windows，而手机很有可能是armeabi系列。

别急，在NDK中自带了适用于各个操作系统和CPU架构的工具链，其中就包含了这两个命令，只不过名字稍有变化，你可以在NDK目录的toolchains目录下找到他们。以我的Mac电脑为例，如果我要找的是适用于armeabi架构的工具，那么他们分别为arm-linux-androideabi-addr2line和arm-linux-androideabi-objdump；位置在下面目录中，后续介绍中将省略此位置：

```shell
android-ndk-r17c/toolchains/aarch64-linux-android-4.9/prebuilt/darwin-x86_64/bin
```

```shell
命令格式：
arm-linux-androideabi-addr2line -e 需要调试的so库路径 内存地址
```

好了言归正传，如何使用这两个工具，下面具体介绍：

1. 找到日志中的关键函数指针

其实很简单，就是找到backtrace信息中，属于我们自己的so文件报错的行。 首先要找到backtrace信息，有的手机会明确打印一行backtrace（比如我们这次使用的手机），那么这一行下面的一系列以“#两位数字 pc”开头的行就是backtrace信息了。有时可能有的手机并不会打印一行backtrace，那么只要找到一段以“#两位数字 pc ”开头的行，就可以了。

```shell
: backtrace:
03-03 18:23:06.617 22128 22128 F DEBUG   :     #00 pc 0000000000011030  /data/app/com.nate.ndkdemo-RjXVlBt1L037Dwwyo5MDqA==/lib/arm64/libnative-lib.so (_JNIEnv::GetStringUTFChars(_jstring*, unsigned char*)+32)
03-03 18:23:06.617 22128 22128 F DEBUG   :     #01 pc 0000000000011174  /data/app/com.nate.ndkdemo-RjXVlBt1L037Dwwyo5MDqA==/lib/arm64/libnative-lib.so (Java_com_nate_ndkdemo_Instance_setStringArray+160)
03-03 18:23:06.617 22128 22128 F DEBUG   :     #02 pc 000000000057ade0  /system/lib64/libart.so (art_quick_generic_jni_trampoline+144)
```

2. 使用addr2line查找代码位置

执行如下的命令，多个指针地址可以在一个命令中带入，以空格隔开即可

```
aarch64-linux-android-addr2line –e obj/local/armeabi/libhello-jni.so 00004de8 0000000000011030 0000000000011174
```

结果如下

```c
/NdkDemo/app/src/main/cpp/instance.cpp:143
```

补充信息：在本文完成后，有同学提供了下列方法，在使用addr2line查找代码位置时增加 -f 参数，就可以可以获取函数信息，这样可以忽略下面一小节objdump的使用

```shell
aarch64-linux-android-addr2line –e -f obj/local/armeabi/libhello-jni.so 00004de8 0000000000011030 0000000000011174
```

结果如下

```c
/NdkDemo/app/src/main/cpp/instance.cpp:143
```

# JNI 内存管理

要想能管理好JNI的内存必须要了解JNI的引用划分

- Local Reference
- Global Reference
- Weak Global Referenc

### Local Reference

局部引用是最常见的一种引用。绝大多数 JNI 函数创建的都是局部引用，比如：NewObject、FindClass、NewObjectArray 函数等等。

局部引用在 Native 函数返回后，所引用的对象会被 GC 自动回收，也可以通过 DeleteLocalRef 函数来手动回收。

**局部引用会阻止 GC 回收所引用的对象，同时，它不能在本地函数中跨函数传递，不能跨线程使用。**

### Global Reference

全局引用只能通过 `NewGlobalRef`函数来创建，然后通过 `DeleteGlobalRef` 函数来手动释放。

**全局引用和局部引用一样，也会阻止它所引用的对象被回收。但是它不会在方法返回时被自动释放，必须要通过手动释放才行，而且，全局引用可以跨方法、跨线程使用**。

### Weak Global Referenc

**弱全局引用有点类似于** Java 中的弱引用，它所引用的对象可以被 GC 回收，并且它也可以跨方法、跨线程使用。

在使用弱引用时，要先检查弱引用所指的类对象是否被 GC 给回收了。通过 `isSameObject` 方法进行检查。

`isSameObject` 方法可以用来比较两个引用类型是否相同，也可以用来比较引用是否为 NULL。同时，还可以用 `isSameObject` 来比较弱全局引用所引用的对象是否被 GC 了，返回 JNI_TRUE 则表示回收了，JNI_FALSE 则表示未被回收。

### local reference table overflow (max=512)

这个问题产生的原因Local Reference的大小有显示最多是512个如果本地创建了大量的临时变量没有删除就会出现上面问题

```c
extern "C"
JNIEXPORT void JNICALL
Java_com_nate_ndkdemo_Instance_localVariable(JNIEnv *env, jclass clazz, jint size) {
    int index = 0;
    for (; index < size; ++index) {
        jclass stringClass = env->FindClass("java/lang/String");
      //  env->DeleteLocalRef(stringClass);
    }
}
```

上面的这个方法在5.0的手机上就可以复现这个问题，解决办法就是删除本地引用，把注释的代码打开就可以了，但是上面的代码在我的9.0手机上就没有复现，这是为什么呢？

> **在 [Android 8.0](https://developer.android.com/about/versions/oreo) 之前的 Android 版本中，局部引用的数量上限取决于版本特定的限制。从 Android 8.0 开始，Android 支持无限制的局部引用。**



# 参考资料

[Android NDK开发-JNI]([https://xucanhui.com/2018/07/08/android-ndk-jni/#%E8%87%AA%E5%AE%9A%E4%B9%89%E5%AF%B9%E8%B1%A1%E5%8F%82%E6%95%B0%E7%9A%84%E4%BC%A0%E9%80%92](https://xucanhui.com/2018/07/08/android-ndk-jni/#自定义对象参数的传递))

[Android JNI在C/C++中调用Java](https://www.javatt.com/p/9356)

[如何定位Android NDK开发中遇到的错误](https://yuanfentiank789.github.io/2016/09/06/ndk_crash/)

 [你真的了解 NDK 和 jni 的区别吗

](https://juejin.im/post/5989133ff265da3e2e56ff26)

[Android JNI/NDK 使用全解

](https://juejin.im/post/5df774f7f265da33e97fceef)

 [AndroidJNI优化](https://juejin.im/post/5d2bf60a51882566d05f4672#heading-5)

 [Android NDK开发扫盲及最新CMake的编译使用](https://www.jianshu.com/p/6332418b12b1)

 [NDK开发总结](https://ejin66.github.io/2018/01/08/android-ndk.html)

[ndk-samples](https://github.com/android/ndk-samples/blob/master/webp/view/src/main/cpp/CMakeLists.txt)

 [Android NDK初步](https://rustfisher.com/2016/06/14/Android/NDK-use_sample_2/)

 [Android NDK开发系列教程5：局部引用，全局引用，弱全局引用](https://www.dazhuanlan.com/2019/10/14/5da3f9861c80e/)

 [Android develop JNI教程]](https://developer.android.com/training/articles/perf-jni)