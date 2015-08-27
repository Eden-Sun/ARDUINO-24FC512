#include <Wire.h>
#define READ 0x55
#define WRITE 0x66
#define SUCCESS 0x77
#define FAIL 0x88

char mode = 0;
const int theDeviceAddress = 80;
//unsigned int length = 1535;
//byte data[] = {0x34,0xff,0x88};
byte buffer[200];

void setup(void) {
  Serial.begin(115200);
  Wire.begin();
}

void loop(void) {
  byte number = Serial.readBytes(buffer, 105);
  unsigned char mode = buffer[0];
  if(number == 105 && mode==WRITE){
    
    unsigned int address = (((unsigned int)buffer[1])<<8) + (unsigned int)buffer[2];
    unsigned char length = buffer[3];
    byte check = 0;
    for(unsigned char i = 0;i<104;i++){
      check+=buffer[i];
    }
    if (check!=buffer[104]){ // fail
      Serial.write(FAIL);
      return;
    }else {
      WireEepromWriteEx(theDeviceAddress,address,length,buffer);
      Serial.write(SUCCESS);
      return;
    }   
  }else
  if(number==4 && buffer[0]== READ ){
    unsigned int address = ((unsigned int)buffer[1])*256;
    address += (unsigned int)buffer[2];
    byte dataRead = WireEepromReadByte(theDeviceAddress, address);
    Serial.write(dataRead);
  }else Serial.write(FAIL);
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

void WireEepromWriteEx(int theDeviceAddress, unsigned int theMemoryAddress, unsigned char theByteCount, byte* theByteArray) {
   Wire.beginTransmission(theDeviceAddress); 
   for (int theByteIndex = 0; theByteIndex < theByteCount; theByteIndex++) {
      Wire.write(theByteArray[theByteIndex]);
      Wire.write((byte)((theMemoryAddress + theByteIndex) >> 8));
      Wire.write((byte)((theMemoryAddress + theByteIndex) >> 0));
   }
   Wire.endTransmission();
   delay(5);
}

