#include <stdio.h>
#include <stdlib.h>
#include <jni.h>
//#include <string.h>
#include <android/log.h>
#include "lame.h"

#define LOG_TAG "LAME ENCODER"
#define LOGD(format, args...)  __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, format, ##args);
//#define LOGD(format, args...)
#define BUFFER_SIZE 8192
#define be_short(s) ((short) ((unsigned short) (s) << 8) | ((unsigned short) (s) >> 8))

lame_t lame;

JNIEXPORT void JNICALL
Java_com_windlazio_mp3encoder_MainActivity_initEncoder(JNIEnv *env,
		jobject jobj, jint in_num_channels, jint in_samplerate, jint in_brate,
		jint in_mode, jint in_quality) {
	lame = lame_init();

	LOGD("Init parameters:");
	lame_set_num_channels(lame, in_num_channels);
	LOGD("Number of channels: %d", in_num_channels);
	lame_set_in_samplerate(lame, in_samplerate);
    lame_set_out_samplerate(lame, in_samplerate);
	LOGD("Sample rate: %d", in_samplerate);
	lame_set_brate(lame, in_brate);
	LOGD("Bitrate: %d", in_brate);
	lame_set_mode(lame, MONO);
	LOGD("Mode: %d", in_mode);
	lame_set_quality(lame, in_quality);
	LOGD("Quality: %d", in_quality);
    lame_set_VBR(lame, vbr_default);

    int res = lame_init_params(lame);
	LOGD("Init returned: %d", res);
}

JNIEXPORT void JNICALL
Java_com_windlazio_mp3encoder_MainActivity_destroyEncoder(
		JNIEnv *env, jobject jobj) {
	int res = lame_close(lame);
	LOGD("Deinit returned: %d", res);
}

JNIEXPORT void JNICALL
Java_com_windlazio_mp3encoder_MainActivity_encodeFile(JNIEnv *env,
		jobject jobj, jstring in_source_path, jstring in_target_path) {
	const char *source_path, *target_path;
	source_path = (*env)->GetStringUTFChars(env, in_source_path, NULL);
	target_path = (*env)->GetStringUTFChars(env, in_target_path, NULL);

	FILE *input_file, *output_file;
	input_file = fopen(source_path, "rb");
	output_file = fopen(target_path, "wb");

	short int input[BUFFER_SIZE];
	char output[BUFFER_SIZE];
	int nb_read = 0;
	int nb_write = 0;
	int nb_total = 0;

	LOGD("Encoding started...");

    do{
        nb_read = fread(input,sizeof(short int), BUFFER_SIZE,input_file);
        nb_total +=  nb_read* sizeof(short int);
        LOGD("Encode Processing ....%d", nb_total);

        if(nb_read!=0){
            nb_write = lame_encode_buffer(lame, input, NULL, nb_read, output, 8192);//单声道
            //write = lame_encode_buffer_interleaved(lame,wav_buffer,read,mp3_buffer,8192);
            //把转化后的mp3数据写到文件里
            fwrite(output,sizeof(unsigned char),nb_write,output_file);
        }
        if(nb_read==0){
            lame_encode_flush(lame,output,8192);
        }
    }while(nb_read!=0);
    LOGD("Encoding finished!");

    fclose(input_file);
	fclose(output_file);
}

JNIEXPORT jstring JNICALL
Java_com_windlazio_mp3encoder_MainActivity_getLameVersion(
        JNIEnv *env,
        jobject jobj/* this */) {
    char* str = get_lame_version();//"Hello Java! 我是一只来自C世界的Dog，汪!!!";
    return (*env)->NewStringUTF(env, str);
}