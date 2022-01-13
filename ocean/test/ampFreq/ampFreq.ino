#include <math.h>

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// accelerometer
#include <Adafruit_MSA301.h>
#include <Adafruit_Sensor.h>

Adafruit_MSA301 msa;

// GUItool: begin automatically generated code
AudioSynthWaveform       oscillator;      //xy=252,335
AudioOutputI2S           i2s;           //xy=451,335
AudioConnection          patchCord1(oscillator, 0, i2s, 0);
AudioConnection          patchCord2(oscillator, 0, i2s, 1);
AudioControlSGTL5000     sgtl5000;     //xy=462,266
// GUItool: end automatically generated code

int pitch = 220;
int amp = 0.5;
int prevpitch, prevamp;
int waveform = WAVEFORM_SINE;

// pitch scales logarithmically
float inputToPitch(int input)
{
  int n = map(input, -4000, 4000, 21, 108);
  return 440 * pow(2, (n - 69) / 12.0);
}

void setup()
{
  Serial.begin(9600);

  // reserve some memory for the audio functions
  AudioMemory(20);
  // enable the audio control chip on the Audio Shield
  sgtl5000.enable();
  sgtl5000.volume(0.25);

  // configure and start the oscillator object
  oscillator.amplitude(amp);
  oscillator.frequency(pitch);
  oscillator.begin(waveform);

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
  // read the pitch pot position
  pitch = filter(msa.x, prevpitch, 0.1);
  oscillator.frequency(inputToPitch(pitch));


  // read the amp pot position
  amp = filter(msa.y, prevamp, 0.1);
  oscillator.amplitude(1.0f * amp / 1024);

  Serial.print(pitch);
  Serial.print(" ");
  Serial.println(amp);

  delay(20);
  prevpitch = pitch;
  prevamp = amp;
}

float filter(float val, float prevVal, float w)
{
  return w * val + (1 - w) * prevVal;
}
