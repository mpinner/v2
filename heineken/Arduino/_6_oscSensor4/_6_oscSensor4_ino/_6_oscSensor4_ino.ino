// matt pinner
// and
// vsquaredlabs all rights reserved (c) 2012
//
//////
//
// intent: detect people and population of a space
// impl: this arduino interfaces with 4 sonar sensors 
//       and transfer the data over a network using OSC
//

// OSC libraries part of arduino base
#include <SPI.h>
#include <Ethernet.h>

// OSC lib from https://github.com/recotana/ArdOSC
#include <ArdOSC.h>

// network setup
byte myMac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
byte myIp[]  = { 192, 168, 132, 177 }; 

// not sure these are strictly required ???
int destPort=12000;
byte destIp[]  = { 192, 168, 132, 145 };

OSCClient client;


//Use of many Simultaneous MaxSonar sonar modules in analog mode with arduino
const int sensorCount = 4;

int rangePin[] = {0, 1, 2, 3};

const int rxPin = 6;
const int preReadDelayUs = 50;
const int toggleDelayUs = 30;

boolean bwPinState = HIGH;
int bwPin[] = {3, 4, 7};
int bwCount = 3;

//variables needed to store values
const int arraysize = 10;  //quantity of values to find the median (sample size). Needs to be an odd number
const int midpoint = arraysize/2;    //midpoint of the array is the medain value in a sorted array

int rangevalue[sensorCount][arraysize];




void setup(){ 
  
  
  Ethernet.begin(myMac ,myIp); 
  
  for (int i = 0; i < bwCount; i++ ) {
    pinMode(bwPin[i], OUTPUT);
    digitalWrite(bwPin[i], bwPinState);
  }
  
  
  //This opens up a serial connection to shoot the results back to the PC console
  Serial.begin(9600);
  
  // init readings buffer 
  for ( int sensor = 0; sensor < sensorCount; sensor++) {
    printArray(rangevalue[sensor], arraysize);  
    for (int reading = 0; reading < arraysize; reading++ ) {
      rangevalue[sensor][reading] = 0;
    }
  }
  
  delay(3000);        //wait a while to open serial monitor window
}

void loop(){
  
  //MaxSonar Analog reads are known to be very sensitive. See the Arduino forum for more information.
  //Maxbotix does not recommend averaging readings as the occasional value can be wildly inaccurate and skew the average
  //A median sort is the preferred method. 
  for ( int sensor = 0; sensor < sensorCount; sensor++) {
    
    for(int i = 0; i < arraysize; i++)
      {     
	
	toggleRx();
	//   delay(50);  //wait between analog samples
	
	rangevalue[sensor][i] = analogRead(rangePin[sensor]);
	/*
	  Serial.print("i, value   ");
          Serial.print(i);
          Serial.print(" ,  ");
          Serial.print(rangevalue[i]);
          Serial.println();
	*/
      }  
    
    
    
    //  Serial.print("unsorted ");
    //   printArray(rangevalue[sensor], arraysize);
    //   Serial.println();
    isort(rangevalue[sensor], arraysize);
    //   Serial.print("sorted ");
    //   printArray(rangevalue[sensor], arraysize);
    //   Serial.println();
    
    
    /*
      Serial.print("median range value ");
      Serial.print(rangevalue[sensor][midpoint]);
      Serial.println();  
      Serial.println();  
    */  
    
  } // end of sensor loop
  
  
  send(0,rangevalue[0][midpoint]);
  
  return;
}


//*********************************************************************************
// crafts and sends the OSC payload
void send(int sensor, int reading){
  //loacal_mes,str is release by out of scope
  OSCMessage loacal_mes;
  
  loacal_mes.setAddress(destIp,destPort);
  
  loacal_mes.beginMessage("/sonar/send" + sensor);
  loacal_mes.addArgInt32(reading);
  
  client.send(&loacal_mes);
  
}



//*********************************************************************************
// sort function
void isort(int *a, int n)
//  *a is an array pointer function
{
  for (int i = 1; i < n; ++i)
    {
      int j = a[i];
      int k;
      for (k = i - 1; (k >= 0) && (j < a[k]); k--)
	{
	  a[k + 1] = a[k];
	}
      a[k + 1] = j;
    }
  return;
}
//***********************************************************************************
//function to print array values
void printArray(int *a, int n)
{
  
  for (int i = 0; i < n; i++)
    {
      Serial.print(a[i], DEC);
      Serial.print(' ');
    }
  
  Serial.println();
  return;
}

//***********************************************************************************
//toggles sonar rx pins trigger next reading
void toggleRx() {
  digitalWrite(rxPin, HIGH);
  delayMicroseconds(toggleDelayUs);
  digitalWrite(rxPin, LOW);
  //  delayMicroseconds(preReadDelayUs);
  return;
}
