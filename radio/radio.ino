#include <Arduino.h>

#include <WiFi.h>
#include "AudioFileSourceHTTPStream.h"
#include "AudioFileSourceBuffer.h"
#include "AudioGeneratorMP3.h"
#include "AudioOutputI2S.h"
#include "AudioOutputMixer.h"

// To run, set your ESP8266 build to 160MHz, update the SSID info, and upload.

// Enter your WiFi setup here:
#ifndef STASSID
#define STASSID "aalto open"
#define STAPSK ""
#endif

const char *ssid = STASSID;
const char *password = STAPSK;

// Randomly picked URL
const char *URL = "http://kvbstreams.dyndns.org:8000/wkvi-am";
const char *URL2 = "http://16813.live.streamtheworld.com:80/WRVOHD3_SC";

AudioGeneratorMP3 *mp3[4];
AudioFileSourceHTTPStream *file[4];
AudioFileSourceBuffer *buff[4];
AudioOutputI2S *out;

AudioOutputMixer *mixer;
AudioOutputMixerStub *stub[4];

void setup()
{
  Serial.begin(115200);
  delay(1000);
  Serial.println("Connecting to WiFi");

  WiFi.disconnect();
  WiFi.softAPdisconnect(true);
  WiFi.mode(WIFI_STA);

  WiFi.begin(ssid, password);

  // try to connect to wifi
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("...Connecting to WiFi");
    delay(1000);
  }
  Serial.println("Connected");

  audioLogger = &Serial;

  // initialize the output
  out = new AudioOutputI2S();
  out->SetRate(44100);

  // initialize the mixer
  mixer = new AudioOutputMixer(32, out);

  // first http stream (first radio channel)
  file[0] = new AudioFileSourceHTTPStream(URL);
  file[0]->SetReconnect(3, 500);
  buff[0] = new AudioFileSourceBuffer(file[0], 4096);

  stub[0] = mixer->NewInput();
  stub[0]->SetGain(0.4);

  mp3[0] = new AudioGeneratorMP3();
  mp3[0]->begin(buff[0], stub[0]);
  Serial.println("Start 1");

  // second http stream (first radio channel, delayed)
  file[1] = new AudioFileSourceHTTPStream(URL);
  file[1]->SetReconnect(3, 500);
  buff[1] = new AudioFileSourceBuffer(file[1], 4096);

  stub[1] = mixer->NewInput();
  stub[1]->SetGain(0.3);

  mp3[1] = new AudioGeneratorMP3();
  delay(1000);
  mp3[1]->begin(buff[1], stub[1]);
  Serial.println("Start 2");

  // third http stream (second radio channel)
  file[2] = new AudioFileSourceHTTPStream(URL2);
  file[2]->SetReconnect(3, 500);
  buff[2] = new AudioFileSourceBuffer(file[2], 4096);

  stub[2] = mixer->NewInput();
  stub[2]->SetGain(0.2);

  mp3[2] = new AudioGeneratorMP3();
  delay(1000);
  mp3[2]->begin(buff[2], stub[2]);
  Serial.println("Start 3");

  // fourth http stream (second radio channel, delayed)
  file[3] = new AudioFileSourceHTTPStream(URL2);
  file[3]->SetReconnect(3, 500);
  buff[3] = new AudioFileSourceBuffer(file[3], 4096);

  stub[3] = mixer->NewInput();
  stub[3]->SetGain(0.1);

  mp3[3] = new AudioGeneratorMP3();
  delay(1000);
  mp3[3]->begin(buff[3], stub[3]);
  Serial.println("Start 4");
}

void loop()
{
  // run the mp3s

  if (mp3[0]->isRunning())
  {
    if (!mp3[0]->loop())
    {
      mp3[0]->stop();
      stub[0]->stop();
      Serial.println("Stop 1");
    }
  }

  if (mp3[1]->isRunning())
  {
    if (!mp3[1]->loop())
    {
      mp3[1]->stop();
      stub[1]->stop();
      Serial.println("Stop 2");
    }
  }

  if (mp3[2]->isRunning())
  {
    if (!mp3[2]->loop())
    {
      mp3[2]->stop();
      stub[2]->stop();
      Serial.println("Stop 3");
    }
  }

  if (mp3[3]->isRunning())
  {
    if (!mp3[3]->loop())
    {
      mp3[3]->stop();
      stub[3]->stop();
      Serial.println("Stop 4");
    }
  }
}
