#include "device-controller.h"
unsigned long nowTime;
unsigned long loopStartTime;
unsigned long startTime1Hz;

int qTopPin = 10;
int qEnablePin = 23;
int vAcOutMeasPin = 21;
int iAcOutMeasPin = 20;
int tempMeas1Pin = 19;
int tempMeas2Pin = 16;
int tempMeas3Pin = 15;
int led1Pin = 14;
int led2Pin = 22;
int led3Pin = 13;
int overIPin = 2;
int overILeakPin = 17;

volatile int qTop = 0;
int qEnable = 0;
int qTopSign = 1;
int qTopRampDelay = 100;
int heartBeatRate = 1000;
float vAcOutMeas;
float vAcOutMeasAvg = 0;
float iAcOutMeas;
float iAcOutMeasAvg = 0;
float tempMeas1;
float tempMeas1Avg = 0;
float tempMeas2;
float tempMeas2Avg = 0;
float tempMeas3;
float tempMeas3Avg = 0;
boolean blinky = false;
boolean led1 = false;
boolean led2 = false;
boolean led3 = false;
volatile boolean  overIInterrupt = false;
volatile boolean  overI = false;
volatile int interruptCounter = 0;
volatile int maxInterruptCounts = 3;
volatile int qTopDropPerInterrupt = 1;

float nsamples = 6100.0;
float nsampleRate;

void setup()
{
  setupCommunications(true, 9600);
//  Serial.begin(9600);
  delay(200);
  
  nowTime = millis();
  loopStartTime = nowTime;
  startTime1Hz = nowTime;
  nsampleRate = 1.0 / nsamples;

  pinMode(qTopPin, OUTPUT);     
  pinMode(qEnablePin, OUTPUT);     
  pinMode(vAcOutMeasPin, INPUT);     
  pinMode(iAcOutMeasPin, INPUT);     
  pinMode(tempMeas1Pin, INPUT);     
  pinMode(tempMeas2Pin, INPUT);     
  pinMode(tempMeas3Pin, INPUT);     
  pinMode(led1Pin, OUTPUT);     
  pinMode(led2Pin, OUTPUT);     
  pinMode(led3Pin, OUTPUT);     
  pinMode(13, OUTPUT);     
//  pinMode(overIPin, INPUT_PULLUP);     
  pinMode(overIPin, INPUT);     
  pinMode(overILeakPin, INPUT);     
  digitalWrite(qEnablePin, qEnable);    
  analogWriteFrequency(qTopPin, 40000);
  analogWriteResolution(8);
  analogWrite(qTopPin,qTop);

//  pinMode(18, INPUT_PULLUP);     
  pinMode(18, INPUT);     
  pinMode(3, OUTPUT);     
  digitalWrite(3, LOW);     
  pinMode(4, OUTPUT);     
  digitalWrite(4, LOW);     
  pinMode(5, OUTPUT);     
  digitalWrite(5, LOW);     
  pinMode(6, OUTPUT);     
  digitalWrite(6, LOW);     
  pinMode(7, OUTPUT);     
  digitalWrite(7, LOW);     
  pinMode(8, OUTPUT);     
  digitalWrite(8, LOW);     
  pinMode(9, OUTPUT);     
  digitalWrite(9, LOW);     
  pinMode(11, OUTPUT);     
  digitalWrite(11, LOW);     
  pinMode(12, OUTPUT);     
  digitalWrite(12, LOW);     
  pinMode(26, OUTPUT);     
  digitalWrite(26, LOW);     

  attachInterrupt(overIPin, overIInterruptHandler, HIGH);
  getParameters();
  delay(200);
 
}

void loop()
{
  if (dataOnSerial())
  {    
    if (getInputTopic().equals("setQEnable"))
    {
      qEnable = stringToInt(getInputPayload());
      if (qEnable == 0)
      {
        rampQTop(0);
      }
      else
      {
        qTop = 0;
        analogWrite(qTopPin,qTop);
        printMessage("qTop", intToString(qTop));
      }
      digitalWrite(qEnablePin, qEnable);
      printMessage("qEnable", intToString(qEnable));
      if (qEnable > 0)
      {
        overI = false;
        printMessage("overI", booleanToString(overI));
      }
    }
    if (getInputTopic().equals("setQTop"))
    {
      qTop = stringToInt(getInputPayload());
      analogWrite(qTopPin,qTop);
      printMessage("qTop", intToString(qTop));
    }
    if (getInputTopic().equals("setQTopRampDelay")) 
    {
      qTopRampDelay = stringToInt(getInputPayload());
      printMessage("qTopRampDelay", intToString(qTopRampDelay));
    }
    if (getInputTopic().equals("setNsamples"))
    {
      nsamples = stringToFloat(getInputPayload());
      nsampleRate = 1.0 / nsamples;
      printMessage("nsamples", floatToString(nsamples, 2));
    }
    if (getInputTopic().equals("setHeartBeatRate"))
    {
      heartBeatRate = stringToInt(getInputPayload());
      printMessage("heartBeatRate", intToString(heartBeatRate));
    }
    if (getInputTopic().equals("setMaxInterruptCounts"))
    {
      maxInterruptCounts = stringToInt(getInputPayload());
      printMessage("maxInterruptCounts", intToString(maxInterruptCounts));
    }
    if (getInputTopic().equals("setQTopDropPerInterrupt"))
    {
      qTopDropPerInterrupt = stringToInt(getInputPayload());
      printMessage("qTopDropPerInterrupt", intToString(qTopDropPerInterrupt));
    }

    
    if (getInputTopic().equals("getParameters"))    getParameters();

  }

  vAcOutMeas = (float) analogRead(vAcOutMeasPin);
  vAcOutMeasAvg = vAcOutMeasAvg + (vAcOutMeas - vAcOutMeasAvg) * nsampleRate;
  iAcOutMeas = (float) analogRead(iAcOutMeasPin);
  iAcOutMeasAvg = iAcOutMeasAvg + (iAcOutMeas - iAcOutMeasAvg) * nsampleRate;
  tempMeas1 = (float) analogRead(tempMeas1Pin);
  tempMeas1Avg = tempMeas1Avg + (tempMeas1 - tempMeas1Avg) * nsampleRate;
  tempMeas2 = (float) analogRead(tempMeas2Pin);
  tempMeas2Avg = tempMeas2Avg + (tempMeas2 - tempMeas2Avg) * nsampleRate;
  tempMeas3 = (float) analogRead(tempMeas3Pin);
  tempMeas3Avg = tempMeas3Avg + (tempMeas3 - tempMeas3Avg) * nsampleRate;

  if (overIInterrupt)
  {
    overI = true;
    printMessage("overI", booleanToString(overI));
    qEnable = 0;
    printMessage("qEnable", intToString(qEnable));
    rampQTop(0);
    digitalWrite(qEnablePin, qEnable);
    overIInterrupt = false;  
  }

  nowTime = millis();

  if ((nowTime - startTime1Hz) > heartBeatRate)
  {
    startTime1Hz = nowTime;
    blinky = !blinky;
    led1 = !led1;
    led2 = !led2;
    led3 = !led3;
    digitalWrite(led1Pin, led1);
    digitalWrite(led2Pin, led2);
    digitalWrite(led3Pin, led3);
    printMessage("vAcOutMeasAvg", floatToString(vAcOutMeasAvg, 2));
    printMessage("iAcOutMeasAvg", floatToString(iAcOutMeasAvg, 2));
    printMessage("tempMeas1Avg", floatToString(tempMeas1Avg, 2));
    printMessage("tempMeas2Avg", floatToString(tempMeas2Avg, 2));
    printMessage("tempMeas3Avg", floatToString(tempMeas3Avg, 2));
    printMessage("blinky", booleanToString(blinky));
  }
}
void getParameters()
{
  printMessage("qEnable", intToString(qEnable));
  printMessage("qTop", intToString(qTop));
  printMessage("qTopRampDelay", intToString(qTopRampDelay));
  printMessage("nsamples", floatToString(nsamples, 2));
  printMessage("overI", booleanToString(overI));
  printMessage("heartBeatRate", intToString(heartBeatRate));
  printMessage("qTopRampDelay", intToString(qTopRampDelay));
  printMessage("maxInterruptCounts", intToString(maxInterruptCounts));
  printMessage("qTopDropPerInterrupt", intToString(qTopDropPerInterrupt));
}
void rampQTop(int qTopSet)
{
    qTopSign = 1;
    if (qTopSet < qTop) qTopSign = -1;
    while (qTopSet != qTop)
    {
      qTop = qTop + qTopSign;
      delay(qTopRampDelay);
      analogWrite(qTopPin,qTop);
      printMessage("qTop", intToString(qTop));
    }
}
void overIInterruptHandler() 
{
  if (interruptCounter < maxInterruptCounts)
  {
    ++interruptCounter;
    return;
  }
  qTop = qTop - qTopDropPerInterrupt;
  if (qTop < 0) qTop = 0;
  analogWrite(qTopPin,qTop);
  overIInterrupt = true;
  interruptCounter = 0;
}
