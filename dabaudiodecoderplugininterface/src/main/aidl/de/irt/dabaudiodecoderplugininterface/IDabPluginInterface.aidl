package de.irt.dabaudiodecoderplugininterface;

import de.irt.dabaudiodecoderplugininterface.IDabPluginCallback;

interface IDabPluginInterface {

    void setCallback(in IDabPluginCallback callback);

    void configure(in int codec, in int samplingRate, in int channelCount, in boolean sbr);

    void enqueueEncodedData(in byte[] audioData);
}
