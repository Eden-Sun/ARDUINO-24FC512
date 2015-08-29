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
      byte *bpt = bufferArray+4;
      WireEepromWriteEx(theDeviceAddress,address,length,bpt);
      Serial.write(SUCCESS);
      return;
    }   
  }else
  if(number==133 && mode== READ ){
    unsigned int address = ((unsigned int)bufferArray[1])*256;
    address += (unsigned int)bufferArray[2];
    byte dataRead = WireEepromReadByte(theDeviceAddress, address);
    Serial.write(dataRead);
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


void WireEepromWrite(int theDeviceAddress, unsigned int theMemoryAddress, int theByteCount, byte* theByteArray) {
 
  for (int theByteIndex = 0; theByteIndex < theByteCount; theByteIndex++) {
    Wire.beginTransmission(theDeviceAddress);
    Wire.write((byte)((theMemoryAddress + theByteIndex) >> 8));
    Wire.write((byte)((theMemoryAddress + theByteIndex) >> 0));
    Wire.write(theByteArray[theByteIndex]);
    Wire.endTransmission();
    delay(5);
  }
}

unsigned char WireEepromWriteEx(
  int theDeviceAddress, unsigned int theMemoryAddress,
  unsigned char count, byte* theByteArray
){
    unsigned char cut = 30;
    for(int i=0;i<5;i++){ 
       byte bpt[cut];
       for(byte j=0;j<cut;j++){
          bpt[j]=theByteArray[j+i*cut];
       }
       Wire.beginTransmission(theDeviceAddress);
       Wire.write((byte)((theMemoryAddress+i*cut) >> 8));
       Wire.write((byte)((theMemoryAddress+i*cut) >> 0));
       if(i==4) Wire.write(bpt,8);
       else Wire.write(bpt,cut);
       Wire.endTransmission(1);
       delay(4);
    }
   return 1;
}

