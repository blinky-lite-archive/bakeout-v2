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
boolean led1 = false;
boolean led2 = false;
boolean led3 = false;

void setup()
{  
  digitalWrite(led1Pin, led1);
  digitalWrite(led2Pin, led2);
  digitalWrite(led3Pin, led3);
  delay(3000);
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

  pinMode(0, OUTPUT);     
  pinMode(1, OUTPUT);     
  pinMode(2, OUTPUT);     
  pinMode(3, OUTPUT);     
  pinMode(4, OUTPUT);     
  pinMode(5, OUTPUT);     
  pinMode(6, OUTPUT);     
  pinMode(7, OUTPUT);     
  pinMode(8, OUTPUT);     
  pinMode(9, OUTPUT);     
  pinMode(11, OUTPUT);     
  pinMode(12, OUTPUT);     
  pinMode(26, OUTPUT);     

  digitalWrite(qEnablePin, qEnable);    
  analogWriteFrequency(qTopPin, 40000);
  analogWriteResolution(8);
  analogWrite(qTopPin,qTop);
  digitalWrite(0, LOW);     
  digitalWrite(1, LOW);     
  digitalWrite(2, LOW);     
  digitalWrite(4, LOW);     
  digitalWrite(5, LOW);     
  digitalWrite(6, LOW);     
  digitalWrite(7, LOW);     
  digitalWrite(8, LOW);     
  digitalWrite(9, LOW);     
  digitalWrite(11, LOW);     
  digitalWrite(12, LOW);     
  digitalWrite(26, LOW);  

  for (int ii = 0; ii < 15; ++ii)
  {
    led1 = !led1;
    led2 = !led2;
    led3 = !led3;
    digitalWrite(led1Pin, led1);
    digitalWrite(led2Pin, led2);
    digitalWrite(led3Pin, led3);
    delay(50);
  }
  delay(5000);
  
  for (int ii = 0; ii < 26; ++ii)
  {
    led1 = !led1;
    led2 = !led2;
    led3 = !led3;
    digitalWrite(led1Pin, led1);
    digitalWrite(led2Pin, led2);
    digitalWrite(led3Pin, led3);
    delay(50);
  }

  qEnable = 1;
  digitalWrite(qEnablePin, qEnable);    
  delay(5000);

  for (int ii = 0; ii < 201; ++ii)
  {
    led1 = !led1;
    led2 = !led2;
    led3 = !led3;
    digitalWrite(led1Pin, led1);
    digitalWrite(led2Pin, led2);
    digitalWrite(led3Pin, led3);
    qTop = ii;
    analogWrite(qTopPin,qTop);

    delay(200);
  }

  for (int ii = 0; ii < 25; ++ii)
  {
    led1 = !led1;
    led2 = !led2;
    led3 = !led3;
    digitalWrite(led1Pin, led1);
    digitalWrite(led2Pin, led2);
    digitalWrite(led3Pin, led3);
    delay(50);
  }

 
}

void loop()
{
  ++loopCount;
  nowTime = micros();

  if ((nowTime - startTime1Hz) > 1000000)
  {
    startTime1Hz = nowTime;
    led1 = !led1;
    led2 = !led2;
    led3 = !led3;
    digitalWrite(led1Pin, led1);
    digitalWrite(led2Pin, led2);
    digitalWrite(led3Pin, led3);
  }
}
