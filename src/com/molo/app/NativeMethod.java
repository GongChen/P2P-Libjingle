/**
 *
 * Copyright 2012 Molo, Inc. All rights reserved.
 * NativeMethod.java
 *
 */
package com.molo.app;


import android.app.Activity;
import android.app.ActivityManager;
import android.content.ComponentName;
import android.content.Intent;
import android.util.Log;

/**
 *@author kane (kanegong@molo.cn)
 *@date 2012-6-4
 */
public class NativeMethod
{
    
    private Activity activity;
    // login
    public static native boolean doLogin(String myName, String myPassword, NativeMethod callback);

    // create p2p channel
    public static native boolean createChannel(String myName, String remoteName);

    // sendDataPackage
    public static native boolean sendData(String remoteName, String data);
    
    // quit
    public static native boolean quit(); 
    
    public NativeMethod(Activity activity)
    {
        this.activity = activity;
    }
    
    public void output(String out)
    {
        System.out.println("Android from JNI-Native::callback : " + out);
        
        Intent intent = new Intent();//activity.getApplicationContext(), MainActivity.class);
        intent.setAction("p2p");
        intent.putExtra("msg", out);
        activity.sendBroadcast(intent);
        
//        ActivityManager am = (ActivityManager) activity.getSystemService(activity.ACTIVITY_SERVICE);  
//        ComponentName cn = am.getRunningTasks(1).get(0).topActivity; 
//        Log.d("", "pkg:"+cn.getPackageName());  
//        Log.d("", "cls:"+cn.getClassName()); 
//        
       
    }
}
