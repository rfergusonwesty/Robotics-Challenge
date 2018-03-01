#include <XBee.h>
<<<<<<< HEAD

XBee xbee = XBee();

void setup(){
=======
#include <Wire.h>

XBee xbee = XBee();
int compassAddress = 0x42 >> 1;
int currentVector = 0;

void setup(){
  Wire.begin();
>>>>>>> ba8ddc54ad943c135ec4e2d7c9a9fe99a5b207b6
  Serial.begin(57600);
  Serial1.begin(57600);
  xbee.setSerial(Serial1);
}

void loop(){
<<<<<<< HEAD
  uint8_t payload[] = {100};
=======

  uint8_t payload[] = {100};

>>>>>>> ba8ddc54ad943c135ec4e2d7c9a9fe99a5b207b6
  //Address of receiving device can be anything while in broadcasting mode
  Tx16Request tx = Tx16Request(0x5678, payload, sizeof(payload));
  xbee.send(tx);
  
  //Delay must be longer than the readPacket timeout on the receiving module
<<<<<<< HEAD
  delay(30);
}


=======
  delay(10);
}



>>>>>>> ba8ddc54ad943c135ec4e2d7c9a9fe99a5b207b6
