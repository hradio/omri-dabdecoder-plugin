#include <jni.h>
#include <android/log.h>

#include <mpgdecoder.h>

//Globals
JavaVM * g_vm = NULL;
jobject g_obj = NULL;
jclass g_decoderClass = NULL;
jmethodID m_decoderCallbackMId = NULL;

MpgDecoder* m_mpgDecoder = NULL;
mpg123_handle* m_mpg123Decoder = NULL;
unsigned char m_mpgOut[192000];

MpgDecoder::MpgDecoder() {
	__android_log_print(ANDROID_LOG_INFO, "MpgDecoder", "Constructing!");

	int errCode;
    errCode = mpg123_init();
	m_mpg123Decoder = mpg123_new(NULL, &errCode);
	if (m_mpg123Decoder != NULL) {
		errCode = mpg123_param(m_mpg123Decoder, MPG123_VERBOSE, 2, 0);
		__android_log_print(ANDROID_LOG_INFO, "MpgDecoder", "Setting verbose: %d", errCode);
		//errCode = mpg123_param(m_mpg123Decoder, MPG123_ADD_FLAGS, MPG123_FORCE_STEREO, 0);
		//__android_log_print(ANDROID_LOG_INFO, "MpgDecoder", "Setting force Stereo: %d", errCode);
		
		errCode = mpg123_open_feed(m_mpg123Decoder);
        if ( errCode == 0 ) {
			__android_log_print(ANDROID_LOG_INFO, "MpgDecoder", "Opening decoder okay!");

        } else {
			__android_log_print(ANDROID_LOG_INFO, "MpgDecoder", "Error Opening decoder!");
        }
	}
}

MpgDecoder::~MpgDecoder() {
	__android_log_print(ANDROID_LOG_INFO, "MpgDecoder", "Destructing!");

	if(m_mpg123Decoder != NULL) {
        mpg123_close(m_mpg123Decoder);
        mpg123_delete(m_mpg123Decoder);
        mpg123_exit();
        m_mpgDecoder = NULL;
    }
}

size_t MpgDecoder::decode(u_int8_t* audioData, int length) {
	//__android_log_print(ANDROID_LOG_INFO, "MpgDecoder", "Decoding: %d", length);

	size_t done;
    if(m_mpg123Decoder != NULL) {
		int mpg123_rc = mpg123_decode(m_mpg123Decoder, audioData, length, m_mpgOut, 192000u, &done);
		switch (mpg123_rc) {
                case MPG123_ERR : {
		    __android_log_print(ANDROID_LOG_INFO, "MpgDecoder", "Error");
                    return -1;
                }
				
                case MPG123_BUFFERFILL : {
                    return 0;
                }
				
                case MPG123_NEED_MORE : {	
					JNIEnv* env;
					g_vm->GetEnv ((void **) &env, JNI_VERSION_1_6);
					jbyteArray decData = env->NewByteArray(done);
    				env->SetByteArrayRegion (decData, 0, (int)done, (jbyte*)m_mpgOut);
					env->CallVoidMethod(g_obj, m_decoderCallbackMId, decData);
					env->DeleteLocalRef(decData);

                    return done;
                }
                case MPG123_OK : {
					__android_log_print(ANDROID_LOG_INFO, "MpgDecoder", "MPG123_OK: 0x%x 0x%x 0x%x", m_mpgOut[0], m_mpgOut[1], m_mpgOut[2]);
					JNIEnv* env;
					g_vm->GetEnv ((void **) &env, JNI_VERSION_1_6);
					jbyteArray decData = env->NewByteArray(done);
					env->SetByteArrayRegion (decData, 0, (int)done, (jbyte*)m_mpgOut);
					env->CallVoidMethod(g_obj, m_decoderCallbackMId, decData);
					env->DeleteLocalRef(decData);

                    return done;
                }
                case MPG123_NEW_FORMAT : {
                    __android_log_print(ANDROID_LOG_INFO, "MpgDecoder", "NewFormat");

                    long rate;
                    int channels;
                    int encoding;

                    mpg123_rc = mpg123_getformat(m_mpg123Decoder, &rate, &channels, &encoding);

					__android_log_print(ANDROID_LOG_INFO, "MpgDecoder", "NewFormat SamplingRate: %ld", rate);
					__android_log_print(ANDROID_LOG_INFO, "MpgDecoder", "NewFormat Channels: %d", channels);
					__android_log_print(ANDROID_LOG_INFO, "MpgDecoder", "NewFormat Encoding: %d", encoding);

					return 0;
                    break;
                }
                default: {
					__android_log_print(ANDROID_LOG_INFO, "MpgDecoder", "Default Error: %s", mpg123_strerror(m_mpg123Decoder));
					return -1;
                }
            }
	} else {
		__android_log_print(ANDROID_LOG_INFO, "MpgDecoder", "MPG Decoder is NULL!");
	}

	delete[] audioData;
	return -1;
}

extern "C" {

	JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved) {
		JNIEnv* env;
		vm->GetEnv ((void **) &env, JNI_VERSION_1_6);

		g_decoderClass = (jclass)env->NewGlobalRef(env->FindClass("de/irt/dabmpg123decoderplugin/Mpg123Decoder"));
        if(g_decoderClass != NULL) {
			m_decoderCallbackMId = env->GetMethodID(g_decoderClass, "decodedDataCallback", "([B)V");
		} else {
	    	__android_log_print(ANDROID_LOG_INFO, "MpgDecoder", "######### Decoder Class NOT found!!! ########");
		}

		m_mpgDecoder = new MpgDecoder();

		return JNI_VERSION_1_6;
	}

	JNIEXPORT jint JNICALL Java_de_irt_dabmpg123decoderplugin_Mpg123Decoder_init(JNIEnv* env, jobject thiz) {
		env->GetJavaVM(&g_vm);
		g_obj = env->NewGlobalRef(thiz);

		return 0;
	}
	
	JNIEXPORT jint JNICALL Java_de_irt_dabmpg123decoderplugin_Mpg123Decoder_decode(JNIEnv* env, jobject thiz, jbyteArray audioData, int dataLength) {
		u_int8_t* buf = new u_int8_t[dataLength];
		env->GetByteArrayRegion(audioData, 0, dataLength, reinterpret_cast<jbyte*>(buf));
		size_t doneDecoded = m_mpgDecoder->decode(buf, dataLength);

		delete[] buf;

		return doneDecoded;
	}
}
