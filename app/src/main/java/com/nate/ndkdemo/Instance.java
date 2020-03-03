package com.nate.ndkdemo;

public class Instance {

    private static final int AGE = 21;

    private String name = "nate";

    public static native void localVariable(int size);
    public static native void log(String data);
    public static native int sum(int[] data);
    public static native String[] setStringArray(int length,String data);
    public static native int[] array(int length,int data);
    public native void setName(String name);
    public static native void callJavaStaticMethod();
    public static native void callJavaInstanceMethod();
    public static native void setAge(int age);

    public static int getAge() {
        return AGE;
    }

    public String getName() {
        return name;
    }
}
