package com.molo.app;

import java.net.InetAddress;
import java.net.NetworkInterface;
import java.net.SocketException;
import java.util.Enumeration;

import android.app.Activity;
import android.content.Intent;
import android.os.AsyncTask;
import android.os.Bundle;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

public class P2PdemoActivity extends Activity implements OnClickListener
{
    static
    {
//        System.loadLibrary("stlport_shared");
//        System.loadLibrary("gnustl_shared");
//        System.loadLibrary("jingle");
//        System.loadLibrary("xmpphelp");
//        System.loadLibrary("expat");
        System.loadLibrary("jingle_jni");
    }
    
    /** Called when the activity is first created. */
    public void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        
        
        setContentView(R.layout.main);
        Button goButton = (Button) findViewById(R.id.go);
        goButton.setOnClickListener(this);
    }

    public void onClick(View arg0)
    {
        EditText nameField = (EditText) findViewById(R.id.editText1);
        
        EditText passwordField = (EditText) findViewById(R.id.editText2);
        
        if (nameField.getText().length() != 0 && passwordField.getText().length() != 0)
        {
            final String name = nameField.getText().toString();
            final String password = passwordField.getText().toString();
            new Thread(new Runnable()
            {
                public void run()
                {
                    System.out.println("login");
                    NativeMethod native_Method = new NativeMethod(P2PdemoActivity.this);
                    NativeMethod.doLogin(name, password, native_Method);
                    // NativeMethod.doLogin(name, password);
                }
            }).start();
            Intent intent = new Intent();
            intent.putExtra("name", name + "@molohui.com"); // ex. gc1@molohui.com 
            intent.setClassName(this, "com.molo.app.MainActivity");
            startActivity(intent);
        }
        else
        {
            Toast.makeText(this, "please input your name or password", Toast.LENGTH_LONG).show();
        }
    }
    
    public String getLocalIpAddress()
    {
        try
        {
            for (Enumeration<NetworkInterface> en = NetworkInterface.getNetworkInterfaces(); en.hasMoreElements();)
            {
                NetworkInterface intf = en.nextElement();
                for (Enumeration<InetAddress> enumIpAddr = intf.getInetAddresses(); enumIpAddr.hasMoreElements();)
                {
                    InetAddress inetAddress = enumIpAddr.nextElement();
                    if (!inetAddress.isLoopbackAddress())
                    {
                        return inetAddress.getHostAddress().toString();
                    }
                }
            }
        }
        catch (SocketException ex)
        {
            Log.e("P2PdemoActivity.getLocalIpAddress", ex.toString());
        }
        return null;
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu)
    {
        super.onCreateOptionsMenu(menu);
        menu.add(0, 12345, Menu.NONE, "quit p2p");
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item)
    {
        switch (item.getItemId())
        {
            case 12345:
                new Thread(new Runnable()
                {
                    public void run()
                    {
                        System.out.println("quit");
                        NativeMethod.quit();
                    }
                }).start();
                return true;
        }
        return super.onOptionsItemSelected(item);
    }
    
    @Override
    public void onBackPressed()
    {
        new Thread(new Runnable()
        {
            public void run()
            {
                System.out.println("quit");
                NativeMethod.quit();
            }
        }).start();
        this.finish();
        super.onBackPressed();
    }
}
