#include <iostream>
#include <Windows.h>

int main(void) {
    // Play the audio file using Windows Multimedia API


    // this libarary appears to be the simplest libarary for playing audio 
    // we may change it up but for now we stick this one
    // all files MUST be .wav for them to play on it so keep that in mind


    printf("Playing right now\n");
    if (PlaySound(TEXT("C:\\Users\\muham\\OneDrive\\Desktop\\DS Final Project\\DS Final Project\\randomsound.wav"), NULL, SND_FILENAME) == 0) {
        std::cerr << "Failed to play audio file" << std::endl;
        return 1;
    }

    // Wait for the audio to finish playing
    Sleep(3000); // Adjust the duration as needed

    return 0;
}


