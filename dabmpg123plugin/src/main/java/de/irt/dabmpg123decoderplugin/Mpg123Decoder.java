package de.irt.dabmpg123decoderplugin;

import android.app.Service;
import android.content.Intent;
import android.os.IBinder;
import android.os.RemoteException;
import android.util.Log;

import de.irt.dabaudiodecoderplugininterface.IDabPluginCallback;
import de.irt.dabaudiodecoderplugininterface.IDabPluginInterface;

public class Mpg123Decoder extends Service {

    private final String TAG = "Mpg123Decoder";

    private IDabPluginCallback mCallback;

    private native int decode(byte[] audioData, int length);
    private native int init();

    static {
        System.loadLibrary("mpg123plug");
    }

    private void decodedDataCallback(byte[] pcmData) {
        try {
            mCallback.decodedPcmData(pcmData);
        } catch(RemoteException remExc) {
            if(BuildConfig.DEBUG)Log.e(TAG, "RemoteException!");
        }
    }

    @Override
    public void onCreate() {
        super.onCreate();

	    if(BuildConfig.DEBUG)Log.d(TAG, "onCreate");
        init();
    }

    @Override
    public IBinder onBind(Intent intent) {
        if(BuildConfig.DEBUG)Log.d(TAG, "onBind");
        return mDabPlugBinder;
    }

    @Override
    public boolean onUnbind(Intent intent) {
	    if(BuildConfig.DEBUG)Log.d(TAG, "onUnBind");
        return super.onUnbind(intent);
    }

    IDabPluginInterface.Stub mDabPlugBinder = new IDabPluginInterface.Stub() {

        @Override
        public void setCallback(IDabPluginCallback callback) {
	        if(BuildConfig.DEBUG)Log.d(TAG, "setCallback");
            mCallback = callback;
        }

        @Override
        public void configure(int codec, int samplingRate, int channelCount, boolean sbr) throws RemoteException {
	        if(BuildConfig.DEBUG)Log.d(TAG, "configure");
        }

        @Override
        public void enqueueEncodedData(byte[] audioData) throws RemoteException {
            decode(audioData, audioData.length);
        }
    };


}
