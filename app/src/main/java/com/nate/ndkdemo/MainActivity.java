package com.nate.ndkdemo;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.util.Log;
import android.widget.TextView;

import java.util.Arrays;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // Example of a call to a native method
        TextView tv = findViewById(R.id.sample_text);
        tv.setText(stringFromJNI());

        Instance.callJavaStaticMethod();
        Instance.callJavaInstanceMethod();

        Instance.setAge(11);
        System.out.println(Instance.getAge());
        Instance instance = new Instance();
        instance.setName("tom");
        System.out.println(instance.getName());

        int[] data = {1, 2, 3, 4, 5};
        Log.e("nate", "sum = " + Instance.sum(data));
        Log.e("nate", "array = " + Arrays.toString(Instance.array(5,1)));
        Log.e("nate", "array = " + Arrays.toString(Instance.setStringArray(5,"I")));

//        Instance.log("from Java!");
        Instance.localVariable(2000);


    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();
}
