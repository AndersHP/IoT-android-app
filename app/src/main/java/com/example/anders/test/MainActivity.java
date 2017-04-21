package com.example.anders.test;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;

import java.io.BufferedInputStream;
import java.io.BufferedReader;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.net.HttpURLConnection;
import java.net.URL;


public class MainActivity extends AppCompatActivity {

    final private String PHOTON_URL = "https://api.particle.io/v1/devices/330042000247353138383138/lightValue?access_token=fa56b9f2d87bb78b0641e3d2c651dd80a7371df5";
    Button buttonClose;
    Button buttonOpen;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        buttonClose = (Button) findViewById(R.id.close);
        buttonOpen = (Button) findViewById(R.id.open);


        buttonOpen.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                SendSomething();
            }
        });


        buttonClose.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                SendSomething();
            }
        });

    }

    void SendSomething(){
        try{
            URL url = new URL("http://www.android.com/");
            HttpURLConnection urlConnection = (HttpURLConnection)url.openConnection();

            urlConnection.setRequestMethod("GET");

            int statusCode = urlConnection.getResponseCode();

            if (statusCode ==  200) {

                InputStream it = new BufferedInputStream(urlConnection.getInputStream());

                InputStreamReader read = new InputStreamReader(it);
                BufferedReader buff = new BufferedReader(read);
                StringBuilder dta = new StringBuilder();
                String chunks ;
                while((chunks = buff.readLine()) != null)
                {
                    dta.append(chunks);
                }
                Log.d("yo","ASd");
            }
            else
            {
                Log.d("s","asdss");
            }
        }catch (Exception E){
            Log.d("EXEP","");
        }

    }
}
