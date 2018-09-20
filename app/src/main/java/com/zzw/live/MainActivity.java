package com.zzw.live;

import android.Manifest;
import android.content.Intent;
import android.os.Bundle;
import android.os.Environment;
import android.support.v4.app.ActivityCompat;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.View;

import java.io.File;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        ActivityCompat.requestPermissions(this, new String[]{
                Manifest.permission.WRITE_EXTERNAL_STORAGE
                , Manifest.permission.READ_EXTERNAL_STORAGE,Manifest.permission.RECORD_AUDIO,
                Manifest.permission.CAMERA,
        }, 5);

        findViewById(R.id.push).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                startActivity(new Intent(MainActivity.this, LivePushActivity.class));
            }
        });
    }


    public void camera(View view) {
        startActivity(new Intent(this, CameraActivity.class));
    }

    public void recode(View view) {
        startActivity(new Intent(this, RecodeActivity.class));
    }


}
