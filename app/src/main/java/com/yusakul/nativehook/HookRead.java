package com.yusakul.nativehook;


public class HookRead {
    static {
        System.loadLibrary("hookread");
    }

    public static native void init();

    public static native void uninit();

    public static native void purArray(byte[] bytes);

    public static native void deleteArray();

}
