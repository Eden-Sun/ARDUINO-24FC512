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
  if(number == 133 && mode==WRITE){
    
    unsigned int address = (((unsigned int)bufferArray[1])<<8) + (unsigned int)bufferArray[2];
    unsigned char length = bufferArray[3];
    byte check = 0;
    for(unsigned char i = 0;i<133-1;i++){
      check+=bufferArray[i];
    }
    if (check!=bufferArray[132]){ // fail
      Serial.write(FAIL);
      return;
    }else {
      unsigned char cut = 30;
      for(int i=0 ; i*cut < length ; i++ ){
        Wire.beginTransmission(theDeviceAddress);
        Wire.write(bufferArray[1]);
        Wire.write(bufferArray[2]+i*cut);
        byte least = length - i*cut;
        if(least < cut) Wire.write(bufferArray+4+i*cut,least);
        else Wire.write(bufferArray+4+i*cut,cut);
        Wire.endTransmission(1);
        delay(4);
      }
      Serial.write(SUCCESS);
      return;
    }   
  }else
  if(number==133 && mode== READ ){
    byte readbuf[128]={0};
    Wire.beginTransmission(theDeviceAddress);
    Wire.write(bufferArray[1]);
    Wire.write(bufferArray[2]);
    Wire.endTransmission();
    delay(4);
    for(int i=0;i<4;i++){
      Wire.requestFrom(theDeviceAddress, 32);
      for(int j=0;j<32;j++){
        readbuf[i*32+j]=Wire.read();
      }
    }
    Serial.write(bufferArray[1]);
    Serial.write(bufferArray[2]);    
    Serial.write(readbuf,128);
  }
//  else Serial.write(TIMEUP);
}


void WireEepromRead(int theDeviceAddress, unsigned int theMemoryAddress, int theByteCount, byte* theByteArray) {
  for (int theByteIndex = 0; theByteIndex < theByteCount; theByteIndex++) {
    Wire.beginTransmission(theDeviceAddress);
    Wire.write((byte)((theMemoryAddress + theByteIndex) >> 8));
    Wire.write((byte)((theMemoryAddress + theByteIndex) >> 0));
    Wire.endTransmission();
    delay(5);
    Wire.requestFrom(theDeviceAddress, sizeof(byte));
    theByteArray[theByteIndex] = Wire.read();
  }
}

byte WireEepromReadByte(int theDeviceAddress, unsigned int theMemoryAddress) {
  byte theByteArray[sizeof(byte)];
  WireEepromRead(theDeviceAddress, theMemoryAddress, sizeof(byte), theByteArray);
  return (byte)(((theByteArray[0] << 0)));
}


