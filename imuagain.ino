
#include "MPU9250.h"

int pin=11;
MPU9250 myIMU;
boolean conn=false;
float avx=0,avy=0,avz=0;
void setup() {
  Serial.begin(250000);
  initIMU();
  
}
long t1=0,t2=0,dt=0;
float a1,a2,a3,del;
void loop() {

  if (myIMU.readByte(MPU9250_ADDRESS, INT_STATUS) & 0x01)
  {
    myIMU.readGyroData(myIMU.gyroCount);  
    myIMU.getGres();
    myIMU.gx = (float)myIMU.gyroCount[0]*myIMU.gRes-avx;
    myIMU.gy = (float)myIMU.gyroCount[1]*myIMU.gRes-avy;
    myIMU.gz = (float)myIMU.gyroCount[2]*myIMU.gRes-avz;
    t2=micros();
    dt=(t2-t1);
    t1=t2;
    del=(float)dt/1000000.0f;
    a1=a1+del*myIMU.gx;
    a2=a2+del*myIMU.gy;
    a3=a3+del*myIMU.gz;
    Serial.print(a1);
    Serial.print("\t");
    Serial.print(a2);
    Serial.print("\t");
    Serial.println(a3);
    //Serial.println(dt);
    //Serial.print("\t");
//    Serial.print(myIMU.gx);
//    Serial.print("\t");
//    Serial.print(myIMU.gy);
//    Serial.print("\t");
//    Serial.println(myIMU.gz);
  }
}

void initIMU()
{
  Wire.begin();
  byte c = myIMU.readByte(MPU9250_ADDRESS, WHO_AM_I_MPU9250);
  pinMode(pin,OUTPUT);
  digitalWrite(pin,HIGH);
  do
  {
    c = myIMU.readByte(MPU9250_ADDRESS, WHO_AM_I_MPU9250);
    if (c == 0x73)
  {
    Serial.println("MPU is online");
    myIMU.initMPU9250();
    conn=true;
  }
  else
  {
    Serial.println("not connected");
    digitalWrite(pin,LOW);
    delay(500);
    digitalWrite(pin,HIGH);
    delay(500);
  }
  }
  while(!conn);
  myIMU.getGres();
  calBias();
  Serial.print(avx);
  Serial.print(avy);
  Serial.println(avz);
  delay(2000);
}

void calBias()
{
  int i=0;
  while(i<1000)
  {
    if (myIMU.readByte(MPU9250_ADDRESS, INT_STATUS) & 0x01)
  {
    myIMU.readGyroData(myIMU.gyroCount);  
    avx+= (float)myIMU.gyroCount[0]*myIMU.gRes;
    avy+= (float)myIMU.gyroCount[1]*myIMU.gRes;
    avz+= (float)myIMU.gyroCount[2]*myIMU.gRes;
    i++;
  }
  }
  avx=avx/1000;
  avy=avy/1000;
  avz=avz/1000;
}

