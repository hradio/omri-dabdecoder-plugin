# DabMpg123DecoderPlugin

This is a Plugin-App to decode MPEG-1 Layer 2 DAB Audio.

## Why

Most Android devices do not provide a decoder for MPEG-1 Layer-2 Audio. Even the MP3 decoders do not decode Layer-2 audio.
The plugin will spawn a service with the defined interface in the "dabaudiodecoderplugininterface".

The used decoder is MPG123 and therefore this plugin uses the same license. See https://www.mpg123.de/

## How

To compile the project to an Android Application (.apk) just import it into Android Studio or issue the 'gradlew' command on Linux/Mac or use the 'gradlew.bat' on Windows.
You need to have the NDK installed.
The compiled APK will be located in the build output folder.


