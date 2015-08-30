#include <Wire.h>
#define READ 0x55
#define WRITE 0x66
#define SUCCESS 0x77
#define FAIL 0x88
#define TIMEOUT 0x99

char mode = 0;
const int theDeviceAddress = 80;
byte bufferArray[200];

void setup(void) {
  Serial.begin(115200);
  Wire.begin(); 
}

void loop(void) {
  byte number = Serial.readBytes(bufferArray, 133);
  unsigned char mode = bufferArray[0];
  int length = bufferArray[1];
  if(number == 133 && mode==WRITE){
    byte check = 0;
    for(unsigned char i = 0;i<133-1;i++){
      check+=bufferArray[i];
    }
    if (check!=bufferArray[132]){ // fail
      Serial.write(FAIL);
      return;
    }else {
      Wire.beginTransmission(theDeviceAddress);
      Wire.write(bufferArray+2,length+2);
      Wire.endTransmission();
      Serial.write(SUCCESS);
      return;
    }   
  }else
  if(number==133 && mode== READ ){
    byte readbuf[130]={0};
    Wire.beginTransmission(theDeviceAddress);
    Wire.write(bufferArray[1]);
    Wire.write(bufferArray[2]);
    Wire.endTransmission();
    Wire.requestFrom(theDeviceAddress, 128);
    for(int j=0;j<128;j++){
      readbuf[j+2]=Wire.read();
    }
    readbuf[0]=bufferArray[1];
    readbuf[1]=bufferArray[2];    
    Serial.write(readbuf,130);
  }
//  else Serial.write(TIMEUP);
}


