//XBEE 2.4GHZ Transmitter System For Delivering Location Relative Bearing 
//in Degrees. For HMC compass system.
//Finalized by Jack Maydan based off Adam St. Amard's earlier versions.
//Edited by Robert Belter 10/30/2015
//Edited by Thomas Horning 02/07/2017
//
//This program works based on the Spark Fun Arduino FIO v3.3 with an XBEE transmitter hooked to an extended antennae.
//The board also is hooked to a 3 axis magnetometer. 
//
//The entire module rotates, calculates the bearing based off magnetomer, 
//and transmits it through the patch antennae.
//
//This code is open source and free to use, its derivatives should follow the same guidelines.

#include <XBee.h>
#include <Wire.h>
#include <HMC5883L.h>
#define address 0x1E 

//--------------------CALIBRATION FOR MAGNETOMETER---------------------
//In order to ensure that your transmitter will read the correct heading,
//we have provided a calibration mode that will print the values over the
//Xbees. Make sure to use with XB_RX_Calibration

//Uncomment the below line to activate print out over serial for magnetometer x,y,z
//#define calibration_mode

//Uncomment the below line to activate output above ^ over XBee 
//#define output_calibration

//Axis offsets for magnetometer
int xoff = -7;
int yoff = 54;
int zoff = 0;

//Axis scales for magnetometer
float xscale = 1.070;
float yscale = 1.117;
float zscale = 1;

#ifdef output_calibration
union{
  int i[3];
  uint8_t b[6];
}calibration_converter;
#endif

//Current readings from magnetometer axis
int xout, yout, zout; 

//-----------------------END CALIBRATION FOR MAGNETOMETER------------------

XBee xbee = XBee();
int compassAddress = 0x42 >> 1;

uint8_t payload[12];
int payload_size = 4;


union{
  float f;
  uint8_t b[4];
}heading_converter;

union{
  float f;
  uint8_t b[4];
}heading2_converter;


union{
  float f;
  uint8_t b[4];
}heading3_converter;


union{
  float f;
  uint8_t b[4];
}heading4_converter;

float avg = 0;

void setup(){
  Wire.begin();
  Serial.begin(57600);
  Serial1.begin(57600);
  xbee.setSerial(Serial1);
  Wire.beginTransmission(address); //open communication with HMC5883
  Wire.write(0x02); //select mode register
  Wire.write(0x00); //continuous measurement mode
  Wire.endTransmission();
}

void loop(){
  getVectorHMC(avg);
  Serial.print("Theta Original: ");
  Serial.println(heading_converter.f, 2); // print the heading/bearing
  float pre_prev = heading_converter.f;
// ONE

  //Create payload
 memcpy(payload, heading_converter.b, 4);
  
  //Address of receiving device can be anything while in broadcasting mode
  
 Tx16Request tx = Tx16Request(0x5678, payload, payload_size);
  
  xbee.send(tx);
  
delay(10);

//TWO
  Serial.print("Theta 2: ");
  Serial.println(heading2_converter.f, 2); // print the heading/bearing

  //Create payload
  memcpy(payload, heading2_converter.b, 4);
  
  //Address of receiving device can be anything while in broadcasting mode
  
 Tx16Request tx1 = Tx16Request(0x5678, payload, payload_size);
  
  xbee.send(tx1);

  //Delay must be longer than the readPacket timeout on the receiving module
  delay(10);

//THREE
  Serial.print("Theta 3: ");
  Serial.println(heading3_converter.f, 2); // print the heading/bearing

 //Create payload
  memcpy(payload, heading3_converter.b, 4);
  
  //Address of receiving device can be anything while in broadcasting mode
  
 Tx16Request tx3 = Tx16Request(0x5678, payload, payload_size);
  
  xbee.send(tx3);

  //Delay must be longer than the readPacket timeout on the receiving module
  delay(10);
 // FOUR
  Serial.print("Theta 4: ");
  Serial.println(heading4_converter.f, 2); // print the heading/bearing

  //Create payload
  memcpy(payload, heading4_converter.b, 4);
  
  //Address of receiving device can be anything while in broadcasting mode
  
 Tx16Request tx4 = Tx16Request(0x5678, payload, payload_size);
  
  xbee.send(tx4);

  //Delay must be longer than the readPacket timeout on the receiving module
  delay(10);
  
  getVectorHMC(avg);
  float prev = heading_converter.f;
  avg = getAverage(prev, pre_prev);
  
}

float getAverage(float prev, float pre_prev){
  float avg;
  if(prev >= pre_prev){
    avg = (prev - pre_prev)/4;
    if(avg + prev >= 360){
      avg = 0;
    }
  }
  else{
    avg = (pre_prev - prev)/4;
  }
  
  return avg;
}

/*--------------------------------------------------------------
This the the fucntion which gathers the heading from the compass.
----------------------------------------------------------------*/
void getVectorHMC(float avg){
  float reading = -1;
  int x,y,z;

  // step 1: instruct sensor to read echoes 
  Wire.beginTransmission(address);  // transmit to device
  // the address specified in the datasheet is 66 (0x42) 
  // but i2c adressing uses the high 7 bits so it's 33 
  Wire.write(0x03);          // command sensor to measure angle  
  Wire.endTransmission();  // stop transmitting 

  // step 3: request reading from sensor 
  Wire.requestFrom(address, 6);

  // step 4: receive reading from sensor 
  if(6 <= Wire.available())     // if two bytes were received 
  { 
    x = Wire.read()<<8; //X msb
    x |= Wire.read(); //X lsb
    z = Wire.read()<<8; //Z msb
    z |= Wire.read(); //Z lsb
    y = Wire.read()<<8; //Y msb/
    y |= Wire.read(); //Y lsb
   
  } 
  //Adjust values by offsets
  x += xoff;
  y += yoff;
  z += zoff;
  //Scale axes
  x *= xscale;
  y *= yscale;
  z *= zscale;
  
#ifdef calibration_mode
  xout = x;
  yout = y;
  zout = z;
  char output[100];
  sprintf(output, "x: %d, y: %d, z: %d", xout, yout, zout);
  Serial.println(output);
#endif

  float heading = atan2(y,x);  
  heading += PI/2;
  if(heading < 0)
    heading += 2*PI;
  if(heading > 2*PI)
    heading -= 2*PI;
  reading = heading * 180/PI;
  heading_converter.f = reading;  

   float heading2 = atan2(y,x);  
    heading2 += PI/2;
  if(heading2 < 0)
    heading2 += 2*PI;
  if(heading2 > 2*PI)
    heading2 -= 2*PI;
  reading = heading2 * 180/PI;
    heading2_converter.f = reading + avg;

     float heading3 = atan2(y,x);   
      heading3 += PI/2;
  if(heading3 < 0)
    heading3 += 2*PI;
  if(heading3 > 2*PI)
    heading3 -= 2*PI;
  reading = heading3 * 180/PI;
  heading3_converter.f = reading + 2*avg;

     float heading4 = atan2(y,x);  
      heading4 += PI/2;
  if(heading4 < 0)
    heading4 += 2*PI;
  if(heading4 > 2*PI)
    heading4 -= 2*PI;
  reading = heading4 * 180/PI;
  heading4_converter.f = reading + 3*avg;
  
  // return the heading or bearing
}
