#include "device-controller.h"

int loopCount = 0;
int loopCountMax = 1000;
float deltaMicros = 0;
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
int overIPin = 18;
int overILeakPin = 17;

int qTop = 0;
int qEnable = 0;
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
float powerAvg = 0;
boolean blinky = false;
boolean led1 = false;
boolean led2 = false;
boolean led3 = false;
int overI = 0;
int overIOld = 0;
int overILeak = 0;
int overILeakOld = 0;
boolean overITrip = false;
boolean overILeakTrip = false;

void setup()
{
  setupCommunications(true, 19200);
//  Serial.begin(9600);
  delay(200);
  
  nowTime = micros();
  loopStartTime = nowTime;
  startTime1Hz = nowTime;

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
  pinMode(overIPin, INPUT);     
  pinMode(overILeakPin, INPUT);     
  digitalWrite(qEnablePin, qEnable);    
  analogWriteFrequency(qTopPin, 40000);
  analogWriteResolution(8);
  analogWrite(qTopPin,qTop);

  pinMode(2, OUTPUT);     
  digitalWrite(2, LOW);     
  pinMode(3, OUTPUT);     
  digitalWrite(2, LOW);     
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




  delay(1000);
  printMessage("qEnable", intToString(qEnable));
  printMessage("qTop", intToString(qTop));
  printMessage("overITrip", booleanToString(overITrip));
  printMessage("overILeakTrip", booleanToString(overILeakTrip));
 
}

void loop()
{
  if (dataOnSerial())
  {
/*    
    Serial.print("Topic: ");
    Serial.print(getInputTopic());
    Serial.print(" Payload: ");
    Serial.println(getInputPayload());
*/   
    
    if (getInputTopic().equals("setQEnable"))
    {
      qEnable = stringToInt(getInputPayload());
      digitalWrite(qEnablePin, qEnable);
      printMessage("qEnable", intToString(qEnable));
      if (qEnable > 0)
      {
        overITrip = false;
        printMessage("overITrip", booleanToString(overITrip));
        overIOld = 0;
        overILeakTrip = false;
        printMessage("overILeakTrip", booleanToString(overILeakTrip));
        overILeakOld = 0;
      }
    }
    if (getInputTopic().equals("getQEnable")) printMessage("qEnable", intToString(qEnable));
    
    if (getInputTopic().equals("setQTop"))
    {
      qTop = stringToInt(getInputPayload());
      analogWrite(qTopPin,qTop);
      printMessage("qTop", intToString(qTop));
    }
    if (getInputTopic().equals("getQTop")) printMessage("qTop", intToString(qTop));

    if (getInputTopic().equals("getOverITrip")) printMessage("overITrip", booleanToString(overITrip));
    if (getInputTopic().equals("getOverILeakTrip")) printMessage("overILeakTrip", booleanToString(overILeakTrip));
  }

  vAcOutMeas = (float) analogRead(vAcOutMeasPin);
  vAcOutMeasAvg = vAcOutMeasAvg + (vAcOutMeas - vAcOutMeasAvg) * deltaMicros * 0.000001;
  iAcOutMeas = (float) analogRead(iAcOutMeasPin);
  iAcOutMeasAvg = iAcOutMeasAvg + (iAcOutMeas - iAcOutMeasAvg) * deltaMicros * 0.000001;
  tempMeas1 = (float) analogRead(tempMeas1Pin);
  tempMeas1Avg = tempMeas1Avg + (tempMeas1 - tempMeas1Avg) * deltaMicros * 0.000001;
  tempMeas2 = (float) analogRead(tempMeas2Pin);
  tempMeas2Avg = tempMeas2Avg + (tempMeas2 - tempMeas2Avg) * deltaMicros * 0.000001;
  tempMeas3 = (float) analogRead(tempMeas3Pin);
  tempMeas3Avg = tempMeas3Avg + (tempMeas3 - tempMeas3Avg) * deltaMicros * 0.000001;
  powerAvg = powerAvg + ((vAcOutMeas * iAcOutMeas) - powerAvg) * deltaMicros * 0.000001;

  overI = digitalRead(overIPin);
  overILeak = digitalRead(overILeakPin);

  if ((overI == 1) && (overIOld == 1) )
  {
    qEnable = 0;
    digitalWrite(qEnablePin, qEnable);
    qTop = 0;
    analogWrite(qTopPin,qTop);
    printMessage("qEnable", intToString(qEnable));
    printMessage("qTop", intToString(qTop));
    overITrip = true;
    printMessage("overITrip", booleanToString(overITrip));
  }
  overIOld = overI;
/*
  if ((overILeak == 1) && (overILeakOld == 1) )
  {
    qEnable = 0;
    digitalWrite(qEnablePin, qEnable);
    qTop = 0;
    analogWrite(qTopPin,qTop);
    printMessage("qEnable", intToString(qEnable));
    printMessage("qTop", intToString(qTop));
    overILeakTrip = true;
    printMessage("overILeakTrip", booleanToString(overILeakTrip));
  }
  overILeakOld = overILeak;
*/  
  ++loopCount;
  nowTime = micros();
  if (loopCount > loopCountMax)
  {     
      deltaMicros = (float)(nowTime - loopStartTime) / ((float)loopCountMax);
      loopCount = 0;
      loopStartTime = nowTime;
  }

  if ((nowTime - startTime1Hz) > 1000000)
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
//    printMessage("iAcOutMeasAvg", floatToString(iAcOutMeasAvg, 2));
    printMessage("tempMeas1Avg", floatToString(tempMeas1Avg, 2));
//    printMessage("tempMeas2Avg", floatToString(tempMeas2Avg, 2));
//    printMessage("tempMeas3Avg", floatToString(tempMeas3Avg, 2));
//    printMessage("powerAvg", floatToString(powerAvg, 2));
    printMessage("loopTime", floatToString(deltaMicros,2));
    printMessage("blinky", booleanToString(blinky));
  }
}
