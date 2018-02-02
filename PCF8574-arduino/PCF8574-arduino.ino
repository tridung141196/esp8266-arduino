#include <Wire.h>

#define DEVICE_1 B0100000

void setup()
{
    Wire.begin();
}

void loop()
{
   byte k;
   for(byte i = 0; i<255; i++)
   {
      IOexpanderWrite(DEVICE_1, i);      
      delay(500); 
   }
      
}

//Write a byte to the IO expander

void IOexpanderWrite(byte address, byte _data ) 
{
 Wire.beginTransmission(address);
 Wire.write(_data);
 Wire.endTransmission(); 
}

//Read a byte from the IO expander

byte IOexpanderRead(int address) 
{
 byte _data;
 Wire.requestFrom(address, 1);
 if(Wire.available()) {
   _data = Wire.read();
 }
 return _data;
}
