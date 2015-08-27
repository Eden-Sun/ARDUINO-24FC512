#include <Wire.h>

char mode = 0;
const int theDeviceAddress = 80;
//unsigned int length = 1535;
//byte data[] = {0x34,0xff,0x88};
byte buffer[20];

void setup(void) {
  Serial.begin(115200);
  Wire.begin();
}

void loop(void) {
    byte number = Serial.readBytes(buffer, 4);
    if(number==4){
      if(buffer[0]==0x55){  // write
          unsigned int address = ((unsigned int)buffer[1])*256;
          address += (unsigned int)buffer[2];
          WireEepromWriteByte(theDeviceAddress, address, buffer[3]);
          Serial.write(buffer[1]^buffer[2]^buffer[3]);  
        }else if(buffer[0]==0x88){  //read
          unsigned int address = ((unsigned int)buffer[1])*256;
          address += (unsigned int)buffer[2];
          byte dataRead = WireEepromReadByte(theDeviceAddress, address);
//          Serial.write(buffer[1]);
//          Serial.write(buffer[2]);
          Serial.write(dataRead);
        }
       
    }
}
//        Serial.print("START ");
//        for (unsigned int theMemoryAddress = 0; theMemoryAddress < length; theMemoryAddress++) {
//              WireEepromWriteByte(theDeviceAddress, theMemoryAddress, *eeprom+theMemoryAddress);
//              Serial.print(theMemoryAddress);
//              Serial.print(" is");
//              Serial.print((byte)(*ptr));
//              Serial.println(" done");
//        }
//     else 
//     {
//        for (unsigned int theMemoryAddress = 0; theMemoryAddress < length; theMemoryAddress++) {
//              Serial.print("Byte: ");
//              Serial.print(theMemoryAddress);
//              Serial.print(", ");
//              Serial.print((int)WireEepromReadByte(theDeviceAddress, theMemoryAddress));
//              Serial.println(".");
//        }
//        mode++;
//        Serial.println("done");
//     }

//    for (unsigned int theMemoryAddress = 0; theMemoryAddress < 1024; theMemoryAddress++) {
//        WireEepromWriteInt(theDeviceAddress, theMemoryAddress * 2, (int)0xffff);
//    }
//    for (unsigned int theMemoryAddress = 0; theMemoryAddress < 1024; theMemoryAddress++) {
//        Serial.print("Int: ");
//        Serial.print(theMemoryAddress);
//        Serial.print(", ");
//        Serial.print(WireEepromReadInt(theDeviceAddress, theMemoryAddress * 2));
//        Serial.println(".");
//    }
//}

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

int WireEepromReadInt(int theDeviceAddress, unsigned int theMemoryAddress) {
  byte theByteArray[sizeof(int)];
  WireEepromRead(theDeviceAddress, theMemoryAddress, sizeof(int), theByteArray);
  return (int)(((theByteArray[0] << 8)) | (int)((theByteArray[1] << 0)));
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

//void WireEepromWriteEx(int theDeviceAddress, unsigned int theMemoryAddress, int theByteCount, byte* theByteArray) {
//   Wire.beginTransmission(theDeviceAddress);
//   Wire.write((byte)((theMemoryAddress + theByteIndex) >> 8));
//   Wire.write((byte)((theMemoryAddress + theByteIndex) >> 0));
//   for (int theByteIndex = 0; theByteIndex < theByteCount; theByteIndex++) {
//      Wire.write(theByteArray[theByteIndex]);
//   }
//   Wire.endTransmission();
//   delay(5);
//}

void WireEepromWriteByte(int theDeviceAddress, unsigned int theMemoryAddress, byte theByte) {
  byte theByteArray[sizeof(byte)] = {(byte)(theByte >> 0)};
  WireEepromWrite(theDeviceAddress, theMemoryAddress, sizeof(byte), theByteArray);
}

void WireEepromWriteInt(int theDeviceAddress, unsigned int theMemoryAddress, int theInt) {
  byte theByteArray[sizeof(int)] = {(byte)(theInt >> 8), (byte)(theInt >> 0)};
  WireEepromWrite(theDeviceAddress, theMemoryAddress, sizeof(int), theByteArray);
}
