package com.windlazio.mp3encoder;

import android.os.Environment;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.widget.TextView;
import android.widget.Button;
import android.widget.Toast;
import android.view.View;

import java.io.File;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
        System.loadLibrary("native-lib1");
        System.loadLibrary("lame");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // Example of a call to a native method
        TextView tv = (TextView) findViewById(R.id.sample_text);
        //tv.setText(stringFromJNI());
        tv.setText(stringFromJNI1());

        final Button button = findViewById(R.id.encode_button);
        button.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                // Code here executes on main thread after user presses button
                Toast.makeText(getApplicationContext(), "Lame Version..." + getLameVersion(),
                        Toast.LENGTH_SHORT).show();

                String storageDir = Environment.getExternalStorageDirectory().toString();
                String pcmFileSrc = storageDir + "/003.pcm";
                String mp3FileDst = storageDir + "/testcase.mp3";

                initEncoder(1,48000,48,1,5);
                encodeFile(pcmFileSrc,mp3FileDst);
                destroyEncoder();
                Toast.makeText(getApplicationContext(), "Encode Complete...",
                        Toast.LENGTH_SHORT).show();
            }
        });
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();
    public native String stringFromJNI1();

    private native void initEncoder(int numChannels, int sampleRate, int bitRate, int mode, int quality);
    private native void destroyEncoder();
    private native int encodeFile(String sourcePath, String targetPath);
    public native String getLameVersion();
}
