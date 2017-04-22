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
import android.widget.EditText;

import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

import io.particle.android.sdk.cloud.ParticleCloud;
import io.particle.android.sdk.cloud.ParticleCloudException;
import io.particle.android.sdk.cloud.ParticleCloudSDK;
import io.particle.android.sdk.cloud.ParticleDevice;
import io.particle.android.sdk.cloud.models.DeviceStateChange;
import io.particle.android.sdk.utils.Async;
import io.particle.android.sdk.utils.Toaster;



public class MainActivity extends AppCompatActivity {

    private Button buttonClose, buttonOpen, buttonSend, buttonWater;
    private EditText fldAir, fldSoil, currentAirFld, currentSoilFld, currentTempFld;

    private final String particleUsername = "Anders.ahp@gmail.com";
    private final String particlePw = "34283428";
    private final String deviceId = "330042000247353138383138";

    Activity thisActivity = this;
    private ParticleDevice device;

    private int currentAirHumidity = -1;
    private int currentSoilHumidity = -1;
    private int currentTemp = -1;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        buttonClose = (Button) findViewById(R.id.close);
        buttonOpen = (Button) findViewById(R.id.open);
        buttonSend = (Button) findViewById(R.id.send);
        buttonWater = (Button) findViewById(R.id.water);
        fldAir = (EditText) findViewById(R.id.humidityFld);
        fldSoil = (EditText) findViewById(R.id.waterFld);
        currentAirFld = (EditText) findViewById(R.id.currentAirFld);
        currentSoilFld = (EditText) findViewById(R.id.currentSoilFld);
        currentTempFld = (EditText) findViewById(R.id.currentTempFld);

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

        // Login to cloud when starting the app
        ParticleCloudSDK.init(getApplicationContext());
        new Thread(new Runnable() {
            @Override
            public void run() {
                try{

                    ParticleCloudSDK.getCloud().logIn(particleUsername,particlePw);
                    device = ParticleCloudSDK.getCloud().getDevice(deviceId);
                    Toaster.s(thisActivity, "Logged in!. Found device:" + device.getID());
                }catch (Exception e){
                    Toaster.s(thisActivity, "Logged in FAILED");
                    Log.e("LoginFail",e.getMessage());
                }

            }
        }).start();

    }

    void openWindow(){
        callFunctionOnDevice("openWindow",null);
    }

    void closeWindow(){
        callFunctionOnDevice("closeWindow",null);
    }

    void waterPlant(){
        getCurrentValues();
       // callFunctionOnDevice("waterSoil",null);
    }

    void sendDataToPhoton(){
        String arg1 = fldAir.getText().toString();
        String arg2 = fldSoil.getText().toString();

        callFunctionOnDevice("updateValues",arg1 + ":" + arg2);
    }

    void callFunctionOnDevice(final String functionName, String arguments){

        // Put arguments in a list for some reason?
        final List<String> args = new ArrayList<String>();
        args.add(0,arguments);

        Async.executeAsync(device, new Async.ApiWork<ParticleDevice, Integer>() {

            public Integer callApi(ParticleDevice particleDevice)
                    throws ParticleCloudException, IOException  {

                int result = -1;

                try{
                    result = particleDevice.callFunction(functionName,args);
                }catch (Exception e){
                    Log.e( functionName + " call failed:",e.getMessage());

                }
                return result;
            }

            @Override
            public void onSuccess(Integer value) {
                Toaster.s(thisActivity, functionName + " call succeded: " + value);
            }

            @Override
            public void onFailure(ParticleCloudException e) {
                Log.e( functionName + " call failed:",e.getMessage());
                Toaster.l(thisActivity, functionName + " call failed");
            }
        });

    }

    void getCurrentValues(){
        Async.executeAsync(device, new Async.ApiWork<ParticleDevice, Integer>() {

            public Integer callApi(ParticleDevice particleDevice)
                    throws ParticleCloudException, IOException  {

                try{
                    currentAirHumidity = particleDevice.getIntVariable("currAirHum");
                    currentSoilHumidity = particleDevice.getIntVariable("currSoilHum");
                    currentTemp = particleDevice.getIntVariable("currAirHum");

                }catch (Exception e){
                    Log.e( "Failed to retrieve:",e.getMessage());

                }
                return 1;
            }

            @Override
            public void onSuccess(Integer value) {
                currentAirFld.setText("" + currentAirHumidity);
                currentSoilFld.setText("" + currentSoilHumidity);
                currentTempFld.setText("" + currentTemp);
                Toaster.s(thisActivity, "Current values retrieved!");
            }

            @Override
            public void onFailure(ParticleCloudException e) {
                Log.e( "get vars failed:",e.getMessage());
                Toaster.l(thisActivity, "failed to get current values!");
            }
        });
    }
}
