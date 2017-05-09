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
import java.math.BigDecimal;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;

import io.particle.android.sdk.cloud.ParticleCloud;
import io.particle.android.sdk.cloud.ParticleCloudException;
import io.particle.android.sdk.cloud.ParticleCloudSDK;
import io.particle.android.sdk.cloud.ParticleDevice;
import io.particle.android.sdk.cloud.models.DeviceStateChange;
import io.particle.android.sdk.utils.Async;
import io.particle.android.sdk.utils.Toaster;
import io.particle.android.sdk.cloud.ParticleDevice.VariableType;



public class MainActivity extends AppCompatActivity {

    private Button buttonClose, buttonOpen, buttonSend, buttonWater, buttonUpdate;
    private EditText fldAir, fldSoil, fldTemp, fldAirNight, fldSoilNight, fldTempNight, currentAirFld, currentSoilFld, currentTempFld, currentServoFld, currentLightFld;

    //private final String particleUsername = "Anders.ahp@gmail.com";
    //private final String particlePw = "34283428";
    //private final String deviceId = "330042000247353138383138";
    private final String particleUsername = "peev.alexander@gmail.com";
    private final String particlePw = "ParticleAlexander";
    private final String deviceId = "340028000347343337373738";



    Activity thisActivity = this;
    private ParticleDevice device;

    private double currentServo = -1.0;
    private double currentAirHumidity = -1.0;
    private double currentSoilHumidity = -1.0;
    private double currentTemp = -1.0;
    private int currentLightValue = -1;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        buttonClose = (Button) findViewById(R.id.close);
        buttonOpen = (Button) findViewById(R.id.open);
        buttonSend = (Button) findViewById(R.id.send);
        buttonWater = (Button) findViewById(R.id.water);
        buttonUpdate = (Button) findViewById(R.id.update);

        fldAir = (EditText) findViewById(R.id.humidityFld);
        fldSoil = (EditText) findViewById(R.id.waterFld);
        fldTemp = (EditText) findViewById(R.id.tempFld);
        fldAirNight = (EditText) findViewById(R.id.humidityFldNight);
        fldSoilNight = (EditText) findViewById(R.id.waterFldNight);
        fldTempNight = (EditText) findViewById(R.id.tempFldNight);

        currentAirFld = (EditText) findViewById(R.id.currentAirFld);
        currentSoilFld = (EditText) findViewById(R.id.currentSoilFld);
        currentTempFld = (EditText) findViewById(R.id.currentTempFld);
        currentServoFld = (EditText) findViewById(R.id.currentServoFld);
        currentLightFld = (EditText) findViewById(R.id.currentLightFld);

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
                configurePhoton();
            }
        });
        buttonWater.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                waterPlant();
            }
        });
        buttonUpdate.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) { getCurrentValues();
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
        callFunctionOnDevice("SetOpening","100");
    }

    void closeWindow(){
        callFunctionOnDevice("SetOpening","0");
    }

    void waterPlant(){
       // callFunctionOnDevice("waterSoil",null);
    }

    void configurePhoton(){
        String desiredAirHum = fldAir.getText().toString();
        String desiredSoilHum = fldSoil.getText().toString();
        String desiredTemp = fldTemp.getText().toString();

        String desiredAirHumNight = fldAirNight.getText().toString();
        String desiredSoilHumNight = fldSoilNight.getText().toString();
        String desiredTempNight = fldTempNight.getText().toString();

        callFunctionOnDevice("Configure",desiredAirHum + ":" + desiredSoilHum + ":" + desiredTemp + ":" + desiredAirHumNight + ":" + desiredSoilHumNight + ":" + desiredTempNight);
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

                    currentServo = particleDevice.getDoubleVariable("GetOpening");
                    currentAirHumidity = particleDevice.getDoubleVariable("GetHumidity");
                    currentSoilHumidity = particleDevice.getDoubleVariable("GetSoil");
                    currentTemp = particleDevice.getDoubleVariable("GetTemp");
                    currentLightValue = particleDevice.getIntVariable("GetLight");

                }catch (Exception e){
                    Log.e( "Failed to retrieve:",e.getMessage());

                }
                return 1;
            }

            @Override
            public void onSuccess(Integer value) {

                currentAirFld.setText(String.format("%.2f", currentAirHumidity));
                currentSoilFld.setText(String.format("%.2f", currentSoilHumidity));
                currentTempFld.setText(String.format("%.2f", currentTemp) + "\u2103"); //"\u00b0" er grader tegn
                currentLightFld.setText("" + currentLightValue);
                currentServoFld.setText(String.format("%.2f",currentServo));

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
