#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioSynthNoisePink      pink1;          //xy=162,584
AudioSynthNoisePink      pink2; //xy=163.3333282470703,700
AudioSynthWaveform       waveform1;      //xy=200,756.6666666666666
AudioFilterStateVariable filter1;        //xy=331,583
AudioFilterStateVariable filter2; //xy=336,700
AudioEffectFreeverb      freeverb3; //xy=490,538
AudioEffectFreeverb      freeverb4; //xy=508.88885498046875,664.4443969726562
AudioMixer4              mixer3; //xy=711,568
AudioOutputI2S           i2s1;           //xy=880.000036239624,468.8888969421387
AudioConnection          patchCord1(pink1, 0, filter1, 0);
AudioConnection          patchCord2(pink1, 0, filter1, 1);
AudioConnection          patchCord3(pink2, 0, filter2, 0);
AudioConnection          patchCord4(waveform1, 0, filter2, 1);
AudioConnection          patchCord5(filter1, 0, freeverb3, 0);
AudioConnection          patchCord6(filter2, 0, freeverb4, 0);
AudioConnection          patchCord7(freeverb3, 0, mixer3, 0);
AudioConnection          patchCord8(freeverb4, 0, mixer3, 1);
AudioConnection          patchCord9(mixer3, 0, i2s1, 0);
AudioConnection          patchCord10(mixer3, 0, i2s1, 1);
AudioControlSGTL5000     sgtl5000_1;     //xy=846.6666666666666,367.77777777777777
// GUItool: end automatically generated code



// Use these with the Teensy Audio Shield
#define SDCARD_CS_PIN    10
#define SDCARD_MOSI_PIN  7
#define SDCARD_SCK_PIN   14

// accelerometer
#include <Adafruit_MSA301.h>
#include <Adafruit_Sensor.h>

Adafruit_MSA301 msa;

// pitch scales logarithmically
float inputToPitch(int input)
{
  int n = map(input, 0, 8000, 21, 108);
  return 440 * pow(2, (n - 69) / 12.0);
}

int x, y, z;
int msaX, msaY, prevMsaX, prevMsaY;

float filter(float val, float prevVal, float w)
{
  return w * val + (1 - w) * prevVal;
}

void setup() {
  // reserve some memory for the audio functions
  AudioMemory(20);
  // enable the audio control chip on the Audio Shield
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.5);

  // accelerometer
  if (! msa.begin()) {
    Serial.println("Failed to find MSA301 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MSA301 Found!");

    // get the sd card
  SPI.setMOSI(SDCARD_MOSI_PIN);
  SPI.setSCK(SDCARD_SCK_PIN);
  if (!(SD.begin(SDCARD_CS_PIN))) {
    // stop here, but print a message repetitively
    while (1) {
      Serial.println("Unable to access the SD card");
      delay(500);
    }
  }

  // ocean

  freeverb3.roomsize(1);
  freeverb3.damping(1);

  filter1.frequency(300);
  filter1.resonance(1);

  pink1.amplitude(1);

  // waves
  
  freeverb4.roomsize(0.8);
  freeverb4.damping(1);

  filter2.frequency(600);
  filter2.resonance(0.5);

  waveform1.frequency(0.25);
  waveform1.amplitude(1);
  waveform1.begin(WAVEFORM_SAWTOOTH);

  pink2.amplitude(1);

  // mixing
  mixer3.gain(0, 1);
  mixer3.gain(1, 0.75);

}

void loop()
{
  msa.read();
  x = msa.x + 4000;
  y = msa.y + 4000;
  z = msa.z + 4000;

  freeverb3.roomsize(constrain(map(msa.x, -4000, 4000, 0.5, 1), 0.5, 1));
  
  waveform1.frequency(constrain(map(msa.y, -4000, 4000, 0.2, 0.6), 0.2, 0.6));
  filter2.frequency(constrain(map(msa.y, -4000, 4000, 500, 700), 500, 700));
  filter2.resonance(constrain(map(msa.y, -4000, 4000, 0, 1), 0, 1));
}
