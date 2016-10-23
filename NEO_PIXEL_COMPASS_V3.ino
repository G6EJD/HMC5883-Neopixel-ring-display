/*
"NeoPixel Digital Compass with Heading"
This uses Adafruits' Flora 16 Pixel NeoPixel Ring
running with an Arduino
*/
 
#include <Adafruit_NeoPixel.h>
#include <Wire.h> //I2C Arduino Library
 
// NeoPixel Arduino Pin
#define PIN 0
 
// Initiate NeoPixel Wheel
Adafruit_NeoPixel strip = Adafruit_NeoPixel(16, PIN, NEO_GRB + NEO_KHZ800);

// define Compass adress and offsets
#define address 0x1E //0011110b, I2C 7bit address of HMC5883
const int x_offset = 0;
const int y_offset = 0;
const int z_offset = 0;

// Assign Pixels
const int NNW = 15;
const int NW = 14;
const int WNW = 13;
const int W = 12;
const int WSW = 11;
const int SW = 10;
const int SSW = 9;
const int S = 8;
const int SSE = 7;
const int sE = 6;
const int ESE = 5;
const int E = 4;
const int ENE = 3;
const int NE = 2;
const int NNE = 1;
const int N = 0;
 
//Marker colour
int green[] = {0, 255, 0}; //green
int blue[]  = {0, 0, 255};  //blue
int red[]   = {255, 0, 0};  //red
int white[] = {255, 255, 255}; //white

// Initial Value = North
int indicator, previous_indicator = 0;
 
// Brightness Level 0-255
const byte level = 20;
 
// Declare main variable used 
float heading, previous_heading;
double angle;

void setup(void)
{
  Wire.begin();
  Serial.begin(9600);
  //Put the HMC5883 IC into the correct operating mode
  Wire.beginTransmission(address); //open communication with HMC5883
  Wire.write(0x02); //select mode register
  Wire.write(0x00); //continuous measurement mode
  Wire.endTransmission();
  strip.begin();
  strip.show();
  heading = 360;
  strip.setBrightness(level);
  colorWipe(strip.Color(0,0,255), 50);
  colorWipe(strip.Color(0, 0, 0), 0);
  delay(200);
  angle = 0;
}
 
void loop(void) 
{ 
  read_compass();
  heading = angle;
  display_heading(heading);
  delay(500);
  pixel_off(previous_indicator);
  Serial.println(angle);
}
 
void display_heading(int heading) {  // broken up into section to save processor time
  if ((heading >= 11.25)&&(heading < 101.25));  // NNE to E 
  {
    if ((heading >= 11.25) && (heading < 33.75))
    {
      indicator = NNE;
    }  //end if NNE
 
      if ((heading >= 33.75) && (heading < 56.25))
    {
      indicator = NE;
    }  //end if NE
 
      if ((heading >= 56.25) && (heading < 78.75))
    {
      indicator = ENE;
    }  //end if ENE
 
      if ((heading >= 78.75) && (heading < 101.25))
    {
      indicator = E;
    }  //end if E
  }    //end if NNE to E
 
    if ((heading >= 101.25) && (heading < 191.25))    // ESE to S
  {
    if ((heading >= 101.25) && (heading < 123.75))
    {
      indicator = ESE;
    }  //end if ESE
 
      if ((heading >= 123.75) && (heading < 146.25))
    {
      indicator = sE;    // Do not change to SE -  "SE" is apparrently a library-used keyword
    }  //end if sE
 
      if ((heading >= 146.25) && (heading < 168.75))
    {
      indicator = SSE;
    }  //end if SSE
 
      if ((heading >= 168.75) && (heading < 191.25))
    {
      indicator = S;
    }   //end if S
  }    //end if ESE to S
  if ((heading < 281.25) && (heading > 191.25))    // SSW to W
  {
 
    if ((heading >= 191.25) && (heading < 213.75))
    {
      indicator = SSW;
    }  //end if SSW
 
      if ((heading >= 213.75) && (heading < 236.25))
    {
      indicator = SW;
    }   //end if SW
 
      if ((heading >= 236.25) && (heading < 258.75))
    {
      indicator = WSW;
    }  //end if WSW
 
      if ((heading >= 258.75) && (heading < 281.25))
    {
      indicator = W;
    }    //end if W
  }    //end if SSW to W
 
    if ((heading >= 281.25) || (heading < 11.25))    // WNW to N
  {
    if ((heading >= 281.25) && (heading < 303.75))
    {
      indicator = WNW;
    }    //end if WNW
 
      if ((heading >= 303.75) && (heading < 326.25))
    {
      indicator = NW;
    }  //end if NW
 
      if ((heading >= 326.25) && (heading < 348.75))
    {
      indicator = NNW;
    }  //end if NNW
 
      if ((heading >= 348.75) || (heading < 11.25))
    {
      indicator = N;
 
    }   //end if N
 
  }  // end if WNW to N
  pixel_on(indicator);
}  // end void setIndicator()
 
void pixel_on(byte LED) {
  strip.setPixelColor(LED, 000, 000, 255);
  strip.setBrightness(level);  
  strip.show();              
}

void pixel_off(byte pixel) {
  strip.setPixelColor(pixel, 000, 000, 000);
  strip.setBrightness(level);  
  strip.show();              
}

// Fill the pixels one after the other with a color    
// * Borrowed from ADAFRUIT Example Code
void colorWipe(int c, int wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(15-i, c);
    strip.show();
    delay(wait);
  }
}


/*
An Arduino code example for interfacing with the HMC5883
Uses:
Analog input 4 I2C SDA
Analog input 5 I2C SCL
*/

void read_compass() {
  int x,y,z; //triple axis data
  //Tell the HMC5883 where to begin reading data
  Wire.beginTransmission(address);
  Wire.write(0x03); //select register 3, X MSB register
  Wire.endTransmission();
 
 //Read data from each axis, 2 registers per axis
  Wire.requestFrom(address, 6);
  if(6<=Wire.available()){
    x  = Wire.read()<<8; //X msb
    x |= Wire.read();    //X lsb
    z  = Wire.read()<<8; //Z msb
    z |= Wire.read();    //Z lsb
    y  = Wire.read()<<8; //Y msb
    y |= Wire.read();    //Y lsb
  }
  angle = atan2((double)y - y_offset,(double)x - x_offset)* (180 / 3.14159265) + 180; // Direction (angle) in degrees
}

