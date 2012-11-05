#include <SPI.h>
#include <Ethernet.h>

#include <ArdOSC.h>

byte myMac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
byte myIp[]  = { 192, 168, 132, 177 };
int  serverPort  = 10000;

int destPort=12000;

char oscadr[]="/ard/aaa";

OSCServer server;
OSCClient client;



int anPin = 0;

//variables needed to store values
int arraysize = 5;  //quantity of values to find the median (sample size). Needs to be an odd number
int rangevalue[] = {0, 0, 0, 0, 0};    //declare an array to store the samples. not necessary to zero the array values here, it just makes the code clearer



void setup(){ 
  
 Serial.begin(9600);
 
 Ethernet.begin(myMac ,myIp); 
 server.begin(serverPort);
 
 //set callback function
 server.addCallback(oscadr,&func1);
 
  printArray(rangevalue, arraysize);  
  delay(5000);        //wait a while to open serial monitor window
 
}
  
void loop(){
  
   pinMode(anPin, INPUT);

  //MaxSonar Analog reads are known to be very sensitive. See the Arduino forum for more information.
  //Maxbotix does not recommend averaging readings as the occasional value can be wildly inaccurate and skew the average
  //A median sort is the preferred method. 

   for(int i = 0; i < arraysize; i++)
   {                                                    //array pointers go from 0 to 4

    //Used to read in the analog voltage output that is being sent by the MaxSonar device.
    //The MaxSonar Scale factor is (Vcc/512) per inch. A 5V supply yields ~9.8mV/in
    //The Arduino will map input voltages between 0 and 5 volts into integer values between 0 and 1023. 
    //This yields a resolution between readings of: 5 volts / 1024 units or, .0049 volts (4.9 mV) per unit. 
    //Therefore, one unit from the arduino's ADC represents 0.5 inches

     rangevalue[i] = analogRead(anPin);
          Serial.print("i, value   ");
          Serial.print(i);
          Serial.print(" ,  ");
          Serial.print(rangevalue[i]);
          Serial.println();

     delay(10);  //wait between analog samples
    }  

   Serial.print("unsorted ");
   printArray(rangevalue, arraysize);
   Serial.println();
   isort(rangevalue, arraysize);
   Serial.print("sorted ");
   printArray(rangevalue, arraysize);
   Serial.println();
    
 // now show the medaian range   
    int midpoint = arraysize/2;    //midpoint of the array is the medain value in a sorted array
      //note that for an array of 5, the midpoint is element 2, as the first element is element 0
     Serial.print("median range value ");
     Serial.print(rangevalue[midpoint]);
     Serial.println();  
     Serial.println();  
  

  
  if(server.aviableCheck()>0){
     Serial.println("alive! "); 
  }
  
}


void func1(OSCMessage *_mes){
  
  logIp(_mes);
  logOscAddress(_mes);
  
  //get source ip address
  byte *sourceIp = _mes->getIpAddress();

  //get 1st argument(int32)
  int tmpI=_mes->getArgInt32(0);
  
  //get 2nd argument(float)
  float tmpF=_mes->getArgFloat(1);
  
  //get 3rd argument(string)
  int strSize=_mes->getArgStringSize(2);
  char tmpStr[strSize]; //string memory allocation
  _mes->getArgString(2,tmpStr); 



  //create new osc message
  OSCMessage newMes;
  
  //set destination ip address & port no
  newMes.setAddress(sourceIp,destPort);
  
  //set argument
  newMes.beginMessage(oscadr);
  newMes.addArgInt32(tmpI+1);
  newMes.addArgFloat(tmpF+0.1);
  newMes.addArgString(tmpStr);
 
  //send osc message
  client.send(&newMes);

}




void logIp(OSCMessage *_mes){
  byte *ip = _mes->getIpAddress();
  Serial.print("IP:");
  Serial.print(ip[0],DEC);
  Serial.print(".");
  Serial.print(ip[1],DEC);
  Serial.print(".");
  Serial.print(ip[2],DEC);
  Serial.print(".");
  Serial.print(ip[3],DEC);
  Serial.print(" ");
}

void logOscAddress(OSCMessage *_mes){
  Serial.println(_mes->getOSCAddress());
} 

