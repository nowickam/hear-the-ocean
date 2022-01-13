#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

#include <math.h>

// GUItool: begin automatically generated code
AudioSynthWaveform       oscillator;      //xy=156,367
AudioFilterStateVariable filter;        //xy=367,372
AudioMixer4              mixer;         //xy=597,369
AudioOutputI2S           i2s;           //xy=806,348
AudioConnection          patchCord1(oscillator, 0, filter, 0);
AudioConnection          patchCord2(filter, 0, mixer, 0);
AudioConnection          patchCord3(filter, 2, mixer, 1);
AudioConnection          patchCord4(mixer, 0, i2s, 1);
AudioConnection          patchCord5(mixer, 0, i2s, 0);
AudioControlSGTL5000     sgtl5000;     //xy=792,266
// GUItool: end automatically generated code


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

// input is from 0 to 8000
void setCutoff(int u)
{
  // Use an exponential curve from 50Hz to about 12kHz
  float co = 50 * exp(5.481 * u / 8000.0);
  filter.frequency(co);
  Serial.println(co);
  filter.octaveControl(log2f(12000.0 / (float)co));
}

// input is from 0 to 8000
void setResonance(int u)
{
  // Convert to an appropriate range for the Teensy Audio Library filter object
  Serial.println(u * 4.3 / 8000.0 + 0.7);
  filter.resonance(u * 4.3 / 8000.0 + 0.7);
}

int x, y, z;

void setup() {
  // reserve some memory for the audio functions
  AudioMemory(20);
  // enable the audio control chip on the Audio Shield
  sgtl5000.enable();
  sgtl5000.volume(0.5);

  // accelerometer
  if (! msa.begin()) {
    Serial.println("Failed to find MSA301 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MSA301 Found!");

//  setCutoff(2000 / 8000.0);
  setResonance(4000 / 8000.0);

  // configure and start the oscillator object
  oscillator.amplitude(1);
  oscillator.frequency(220);
  oscillator.begin(WAVEFORM_SINE);

  mixer.gain(0, 1);
  mixer.gain(1, 0);
}

void loop() {
  msa.read();

  // rescale to the range 0-8000
  x = msa.x + 4000;
  y = msa.y + 4000;
  z = msa.z + 4000;

  oscillator.frequency(inputToPitch(x));
//  Serial.println(inputToPitch(x));
//  Serial.println(x);

//  // filter
//  mixer.gain(0, (8000-y)/8000);
//  mixer.gain(1, (y)/8000);

  setCutoff(y);

  setResonance(z);
}
