#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioSynthWaveform       waveform1;      //xy=219,349
AudioSynthWaveform       waveform2;      //xy=223,396
AudioMixer4              mixer1;         //xy=554,369
AudioOutputI2S           i2s1;           //xy=806,348
AudioConnection          patchCord1(waveform1, 0, mixer1, 0);
AudioConnection          patchCord2(waveform2, 0, mixer1, 1);
AudioConnection          patchCord3(mixer1, 0, i2s1, 1);
AudioConnection          patchCord4(mixer1, 0, i2s1, 0);
AudioControlSGTL5000     sgtl5000;     //xy=792,266
// GUItool: end automatically generated code

// accelerometer
#include <Adafruit_MSA301.h>
#include <Adafruit_Sensor.h>

Adafruit_MSA301 msa;

int pitch = 220;
int amp = 0.5;
int prevpitch, prevdetune, detune;
int waveform;
int detunemsa = 0;

// pitch scales logarithmically
float inputToPitch(int input, int detune)
{
  int n = map(input, -4000, 4000, 21, 108);
  float f = 440 * pow(2, (n - 69) / 12.0);
  // calculate the detuned frequency using the fact that there are 1200 cents in an octave
  return f * pow(2, detune / 1200.0);
}

float filter(float val, float prevVal, float w)
{
  return w * val + (1 - w) * prevVal;
}

int filterSmooth(int newvalue, int curvalue, int n)
{
  return floor(((n - 1) * curvalue + newvalue) / n);
}


void setup()
{
  Serial.begin(9600);

  // reserve some memory for the audio functions
  AudioMemory(20);
  // enable the audio control chip on the Audio Shield
  sgtl5000.enable();
  sgtl5000.volume(0.5);

  // configure and start the oscillator object
  waveform1.amplitude(0.5);
  waveform1.frequency(220);
  waveform1.begin(WAVEFORM_TRIANGLE);

  waveform2.amplitude(0.5);
  waveform2.frequency(220);
  waveform2.begin(WAVEFORM_TRIANGLE);

  // accelerometer
  if (! msa.begin()) {
    Serial.println("Failed to find MSA301 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MSA301 Found!");
}

void loop()
{
  msa.read();

  Serial.print(8000);
  Serial.print(" ");
  Serial.print(0);
  Serial.print(" ");
  int msax = msa.x+4000;
  int msay = msa.y+4000;
  int msaz = msa.z+4000;
  Serial.print(msax);
  Serial.print(" ");
  Serial.print(msay);
  Serial.print(" ");
  Serial.print(msaz);
  Serial.println();
  
  pitch = pitch;
  detune = msa.y;

  detunemsa = filterSmooth(detune, detunemsa, 16);
  detune = detunemsa / 5 - 102; // range is -102 to 102 cents
  // make 0 a bit sticky since it is an important value and there is
  // a lot of noise
  if (detune > 0)
    detune = max(0, detune - 2);
  else if (detune < 0)
    detune = min(0, detune + 2);

  waveform1.frequency(inputToPitch(pitch, 0));
  waveform2.frequency(inputToPitch(pitch, detune));

//  Serial.print(inputToPitch(pitch, 0));
//  Serial.print(" ");
//  Serial.println(inputToPitch(pitch, detune));
}
