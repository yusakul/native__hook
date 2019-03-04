package com.yusakul.nativehook;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;

public class MainActivity extends AppCompatActivity {

    private Button bind;
    private Button unbind;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        //
        bind = (Button) findViewById(R.id.bind);
        unbind = (Button) findViewById(R.id.unbind);

        bind.setOnClickListener(new View.OnClickListener() {

            @Override
            public void onClick(View v) {
//				HookRead.init();
                MultiThreadsRead t = new MultiThreadsRead();
                t.start();
            }
        });

        unbind.setOnClickListener(new View.OnClickListener() {

            @Override
            public void onClick(View v) {
				HookRead.uninit();
            }
        });

    }

}
