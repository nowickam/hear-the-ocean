#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioSynthWaveformDc     dc2; //xy=68.57142639160156,1077.142822265625
AudioSynthWaveformModulated waveformMod2; //xy=227.14289093017578,1035.7142944335938
AudioSynthWaveformDc     dc1;            //xy=347.9166603088379,854.8610963821411
AudioEffectWaveshaper    waveshape1;     //xy=414.2857131958008,989.9999713897705
AudioSynthNoisePink      pink1;          //xy=531.6666679382324,509.30554962158203
AudioSynthNoisePink      pink2; //xy=534.5833320617676,641.2500104904175
AudioSynthWaveform       waveform1;      //xy=537.5000038146973,730.4166984558105
AudioEffectMultiply      multiply1;      //xy=553.6111221313477,898.8889293670654
AudioFilterStateVariable filter1;        //xy=756.0000038146973,508.00000953674316
AudioFilterStateVariable filter2; //xy=758.5000038146973,690.0000095367432
AudioSynthWaveformModulated waveformMod1; //xy=761.2500152587891,898.7500123977661
AudioFilterStateVariable filter3; //xy=930,898.5714721679688
AudioEffectFreeverb      freeverb4; //xy=950.1388626098633,679.4444065093994
AudioEffectFreeverb      freeverb3; //xy=955.0000076293945,509.25000762939453
AudioEffectDelay         delay1;         //xy=961.4286003112793,1198.5715141296387
AudioMixer4              mixer1;         //xy=962.8571434020996,1027.1427516937256
AudioMixer4              mixer3; //xy=1150.0277862548828,691.7500076293945
AudioOutputI2S           i2s;           //xy=1350.9723052978516,691.2500085830688
AudioConnection          patchCord1(dc2, 0, waveformMod2, 0);
AudioConnection          patchCord2(waveformMod2, waveshape1);
AudioConnection          patchCord3(dc1, 0, multiply1, 0);
AudioConnection          patchCord4(waveshape1, 0, multiply1, 1);
AudioConnection          patchCord5(pink1, 0, filter1, 0);
AudioConnection          patchCord6(pink1, 0, filter1, 1);
AudioConnection          patchCord7(pink2, 0, filter2, 0);
AudioConnection          patchCord8(waveform1, 0, filter2, 1);
AudioConnection          patchCord9(multiply1, 0, waveformMod1, 0);
AudioConnection          patchCord10(filter1, 0, freeverb3, 0);
AudioConnection          patchCord11(filter2, 0, freeverb4, 0);
AudioConnection          patchCord12(waveformMod1, 0, filter3, 0);
AudioConnection          patchCord13(filter3, 1, mixer1, 0);
AudioConnection          patchCord14(freeverb4, 0, mixer3, 1);
AudioConnection          patchCord15(freeverb3, 0, mixer3, 0);
AudioConnection          patchCord16(delay1, 0, mixer1, 1);
AudioConnection          patchCord17(mixer1, delay1);
AudioConnection          patchCord18(mixer1, 0, mixer3, 2);
AudioConnection          patchCord19(mixer3, 0, i2s, 0);
AudioConnection          patchCord20(mixer3, 0, i2s, 1);
AudioControlSGTL5000     sgtl5000_1;     //xy=1348.3333930969238,605.2777767181396
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

float WAVESHAPE_EXAMPLE[9] = {
  -1,
  -0.6,
  -0.45,
  -0.376,
  -0.228,
  -0.122,
  0.3,
  0.7,
  0.1
};

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

//   accelerometer
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
  
  dc1.amplitude(1);

  waveformMod1.amplitude(1);
  waveformMod1.frequency(150);
  waveformMod1.begin(WAVEFORM_SINE);
  waveformMod1.frequencyModulation(1);

  dc2.amplitude(-1);


  waveformMod2.amplitude(1);
  waveformMod2.frequency(4);
  waveformMod2.begin(WAVEFORM_SAWTOOTH);
  waveformMod2.frequencyModulation(1);

  waveshape1.shape(WAVESHAPE_EXAMPLE, 9);

//  waveform2.frequency(1);
//  waveform2.amplitude(1);
//  waveform2.begin(WAVEFORM_SAMPLE_HOLD);

  mixer1.gain(0, 0.85);
  mixer1.gain(1, 0.45);

  delay1.delay(0, 20);

  filter3.frequency(350);
  filter3.resonance(1);
  
  mixer3.gain(0, 0.75);
  mixer3.gain(1, 0.75);
  mixer3.gain(2, 0.5);

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

  dc2.amplitude(2*constrain(map(float(z), -4000, 4000, -1, 1), -1, 1));
  waveformMod1.frequency(constrain(map(float(z), -4000, 4000, 100, 200), 100, 200));
  waveformMod2.frequency(constrain(map(float(z), -4000, 4000, 2, 6), 2, 6));
}
