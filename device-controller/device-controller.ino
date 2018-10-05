#include "device-controller.h"

int loopCount = 0;
int loopCountMax = 1000;
int deltaMicros = 0;
unsigned long nowTime;
unsigned long loopStartTime;
unsigned long startTime5kHz;
float fcorner = 10.0;

int qTopPin = 10;
int qEnablePin = 23;
int vAcOutMeasPin = 3;

int qTop = 0;
int qEnable = 0;
float vAcOutMeas;
float vAcOutMeasAvg;

void setup()
{
  setupCommunications(true, 9600);
  delay(200);
  
  nowTime = micros();
  loopStartTime = nowTime;
  startTime5kHz = nowTime;

  pinMode(qTopPin, OUTPUT);     
  pinMode(qEnablePin, OUTPUT);     
  pinMode(vAcOutMeasPin, INPUT);     
  digitalWrite(qEnablePin, qEnable);    
  analogWriteFrequency(qTopPin, 40000);
  analogWriteResolution(8);
  analogWrite(qTopPin,qTop);
}

void loop()
{
  if (dataOnSerial())
  {
   
    if (getInputTopic().equals("getLoopTime"))
    {
      printMessage("loopTime", floatToString(deltaMicros,2));
    }
    
    if (getInputTopic().equals("setQEnable"))
    {
      qEnable = stringToInt(getInputPayload());
      digitalWrite(qEnablePin, qEnable);
      printMessage("qEnable", intToString(qEnable));
    }
    if (getInputTopic().equals("getQEnable")) printMessage("qEnable", intToString(qEnable));
    
    if (getInputTopic().equals("setQTop"))
    {
      qTop = stringToInt(getInputPayload());
      analogWrite(qTopPin,qTop);
      printMessage("qTopValue", intToString(qTop));
    }
    if (getInputTopic().equals("getQTop")) printMessage("qTop", intToString(qTop));
    //if (getInputTopic().equals("getVAcOutMeasAvg")) printMessage("vAcOutMeasAvg", "5");
  }

  vAcOutMeas = (float) analogRead(vAcOutMeasPin);
  vAcOutMeasAvg = vAcOutMeasAvg + (vAcOutMeas - vAcOutMeas) * deltaMicros * 0.00001;

  
  ++loopCount;
  if (loopCount > loopCountMax)
  {
      nowTime = micros();
      deltaMicros = (nowTime - loopStartTime) / loopCountMax;
      loopCount = 0;
      loopStartTime = nowTime;
  }
}

