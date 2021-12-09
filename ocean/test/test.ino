#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioSynthWaveform       waveform2;      //xy=63.75,1065
AudioSynthNoisePink      pink1;          //xy=136.66666666666666,555.5555555555555
AudioSynthNoisePink      pink2; //xy=163.3333282470703,700
AudioSynthWaveformDc     dc1;            //xy=166.66666412353516,861.1110858917236
AudioSynthWaveformModulated waveformMod2;   //xy=179.99999237060547,949.9999828338623
AudioSynthWaveform       waveform1;      //xy=200,756.6666666666666
AudioFilterBiquad        biquad1;        //xy=255.55555555555554,434.44444444444446
AudioFilterFIR           fir1;           //xy=270,351.1111111111111
AudioFilterStateVariable filter1;        //xy=331,583
AudioFilterStateVariable filter2; //xy=336,700
AudioEffectMultiply      multiply1;      //xy=361.1111145019531,878.888918876648
AudioEffectFreeverb      freeverb3; //xy=490,538
AudioEffectFreeverb      freeverb4; //xy=508.88885498046875,664.4443969726562
AudioSynthWaveformModulated waveformMod1; //xy=571.2500076293945,846.2500133514404
AudioFilterStateVariable filter3;        //xy=727.5000114440918,760.0000114440918
AudioMixer4              mixer3; //xy=898.7777824401855,548.000020980835
AudioOutputI2S           i2s1;           //xy=992.2222633361816,469.9999942779541
AudioConnection          patchCord1(waveform2, 0, waveformMod2, 0);
AudioConnection          patchCord2(pink1, 0, filter1, 0);
AudioConnection          patchCord3(pink1, 0, filter1, 1);
AudioConnection          patchCord4(pink2, 0, filter2, 0);
AudioConnection          patchCord5(dc1, 0, multiply1, 0);
AudioConnection          patchCord6(waveformMod2, 0, multiply1, 1);
AudioConnection          patchCord7(waveform1, 0, filter2, 1);
AudioConnection          patchCord8(filter1, 0, freeverb3, 0);
AudioConnection          patchCord9(filter2, 0, freeverb4, 0);
AudioConnection          patchCord10(multiply1, 0, waveformMod1, 0);
AudioConnection          patchCord11(freeverb3, 0, mixer3, 0);
AudioConnection          patchCord12(freeverb4, 0, mixer3, 1);
AudioConnection          patchCord13(waveformMod1, 0, filter3, 0);
AudioConnection          patchCord14(filter3, 1, mixer3, 2);
AudioConnection          patchCord15(mixer3, 0, i2s1, 0);
AudioConnection          patchCord16(mixer3, 0, i2s1, 1);
AudioControlSGTL5000     sgtl5000_1;     //xy=973.3333435058594,367.7777729034424
// GUItool: end automatically generated code


// Use these with the Teensy Audio Shield
#define SDCARD_CS_PIN    10
#define SDCARD_MOSI_PIN  7
#define SDCARD_SCK_PIN   14

// accelerometer
#include <Adafruit_MSA301.h>
#include <Adafruit_Sensor.h>

Adafruit_MSA301 msa;

float prevFreq;

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

   accelerometer
  if (! msa.begin()) {
    Serial.println("Failed to find MSA301 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MSA301 Found!");

  // get the sd card
//  SPI.setMOSI(SDCARD_MOSI_PIN);
//  SPI.setSCK(SDCARD_SCK_PIN);
//  if (!(SD.begin(SDCARD_CS_PIN))) {
//    // stop here, but print a message repetitively
//    while (1) {
//      Serial.println("Unable to access the SD card");
//      delay(500);
//    }
//  }

  // ocean
  freeverb3.roomsize(1);
  freeverb3.damping(1);

  filter1.frequency(200);
  filter1.resonance(2);

  pink1.amplitude(1);

  // waves
  freeverb4.roomsize(0.8);
  freeverb4.damping(1);

  filter2.frequency(600);
  filter2.resonance(0.5);

  waveform1.frequency(0.25);
  waveform1.amplitude(0.5);
  waveform1.begin(WAVEFORM_SAWTOOTH);

  pink2.amplitude(1);

  //bubbles?
  
  dc1.amplitude(0.5);

  waveformMod1.amplitude(1);
  waveformMod1.frequency(100);
  waveformMod1.begin(WAVEFORM_SINE);
  waveformMod1.frequencyModulation(1);

  waveformMod2.amplitude(1);
  waveformMod2.frequency(10);
  waveformMod2.begin(WAVEFORM_SAWTOOTH);
  waveformMod2.frequencyModulation(1);

  waveform2.frequency(1);
  waveform2.amplitude(1);
  waveform2.begin(WAVEFORM_SAMPLE_HOLD);

  filter3.frequency(250);
  filter3.resonance(1);
  
  mixer3.gain(0, 1);
  mixer3.gain(1, 0.75);
  mixer3.gain(2, 0.75);

  prevFreq = 0;

}

void loop()
{
  msa.read();
  x = msa.x;
  y = msa.y;
  z = msa.z;

//  Serial.println(msa.x);

  freeverb3.roomsize(constrain(map(x, -4000, 4000, 0.5, 1), 0.5, 1));
  filter1.frequency(constrain(map(x, -4000, 4000, 150, 350), 150, 350));

  waveform1.frequency(constrain(map(y, -4000, 4000, 0.1, 0.6), 0.1, 0.6));
  filter2.frequency(constrain(map(y, -4000, 4000, 500, 700), 500, 700));
  filter2.resonance(constrain(map(y, -4000, 4000, 0, 1), 0, 1));

  waveform2.frequency(constrain(map(z, -4000, 4000, 100, 300), 100, 300));
  waveform3.frequency(constrain(map(z, -4000, 4000, 2, 4), 2, 4));
  waveform3.phase(constrain(map(z, -4000, 4000, 0, 60), 0, 60));
}
