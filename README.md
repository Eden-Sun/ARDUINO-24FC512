# ARDUINO-24FC512

This repo is used for writing EEPROM 24C512 via ARDUINO board.
### PC 
use nodejs with serialport module to generate UART packages that sended to Arduino.
Written in coffee scritp
Each package must follow this format : 
``` [ cmd,length,high_address,low_adress,byte0,byte1....byte127]```
#####cammands
are defined in code
#####length
max to 128 
#####start writing address
0~2^16 , should be devided by 128 to prevent writing errors ,in other words,7 lower bits of low_address must be 0
#####
data bits
max to 128 bytes,if less,will be set to 0xff

### AUDUINO
Native Arduino I2C Libruary has only support 32 bytes w/r buffer.    
Thus, we need to change to lib source to get support with 128.     
In ...Arduino 2.app/Contents/Java/hardware/arduino/avr/libraries/Wire.h and    
...Arduino 2.app/Contents/Java/hardware/arduino/avr/libraries/Wire/utility/twi.h    
change ```#define TWI_BUFFER_LENGTH 32``` to 130 ( 2 extra for address bytes)

