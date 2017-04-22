package com.example.anders.test;

import android.app.Activity;
import android.content.Intent;
import android.support.annotation.NonNull;
import android.support.annotation.WorkerThread;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;

import android.util.Log;
import android.view.View;
import android.widget.Button;

import java.io.IOException;

import io.particle.android.sdk.cloud.ParticleCloud;
import io.particle.android.sdk.cloud.ParticleCloudException;
import io.particle.android.sdk.cloud.ParticleCloudSDK;
import io.particle.android.sdk.cloud.ParticleDevice;
import io.particle.android.sdk.cloud.models.DeviceStateChange;
import io.particle.android.sdk.utils.Async;
import io.particle.android.sdk.utils.Toaster;



public class MainActivity extends AppCompatActivity {

    final private String PHOTON_URL = "https://api.particle.io/v1/devices/330042000247353138383138/lightValue?access_token=fa56b9f2d87bb78b0641e3d2c651dd80a7371df5";

    private Button buttonClose, buttonOpen, buttonSend, buttonWater;

    private final String particleUsername = "Anders.ahp@gmail.com";
    private final String particlePw = "34283428";
    private final String deviceId = "330042000247353138383138";

    Activity thisActivity = this;
    private ParticleDevice device;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        buttonClose = (Button) findViewById(R.id.close);
        buttonOpen = (Button) findViewById(R.id.open);
        buttonSend = (Button) findViewById(R.id.send);
        buttonWater = (Button) findViewById(R.id.water);

        buttonOpen.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
               openWindow();
            }
        });
        buttonClose.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                closeWindow();
            }
        });
        buttonSend.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                sendDataToPhoton();
            }
        });
        buttonWater.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                waterPlant();
            }
        });

        ParticleCloudSDK.init(getApplicationContext());
        new Thread(new Runnable() {
            @Override
            public void run() {
                try{

                    ParticleCloudSDK.getCloud().logIn(particleUsername,particlePw);
                    device = ParticleCloudSDK.getCloud().getDevice(deviceId);
                    Toaster.l(thisActivity, "Logged in!, found device:" + device.getID());
                }catch (Exception e){
                    Toaster.s(thisActivity, "Logged in FAILED");
                    Log.e("LoginFail",e.getMessage());
                }

            }
        }).start();

    }

    void openWindow(){


        Async.executeAsync(device, new Async.ApiWork<ParticleDevice, Integer>() {

            public Integer callApi(ParticleDevice particleDevice)
                    throws ParticleCloudException, IOException  {

                int result = -1;
                try{
                    result = particleDevice.getIntVariable("lightValue");
                }catch (Exception E){
                    Toaster.l(thisActivity, "FAIL");
                    Log.e("Yasd",E.getMessage());

                }
                return result;
            }

            @Override
            public void onSuccess(Integer value) {
                Toaster.s(thisActivity, "Light value is " + value);
            }

            @Override
            public void onFailure(ParticleCloudException e) {
                Log.e("some tag", "Something went wrong making an SDK call: ", e);
                Toaster.l(thisActivity, "Uh oh, something went wrong.");
            }
        });
    }

    void closeWindow(){

    }

    void sendDataToPhoton(){

    }

    void waterPlant(){

    }

    void sendHttpRequest(String requestType, String airHumidity, String soilHumidity){

        // DO!:
    
    }
}
