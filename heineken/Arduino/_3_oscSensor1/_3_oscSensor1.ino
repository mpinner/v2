#include <SPI.h>
#include <Ethernet.h>

#include <ArdOSC.h>

byte myMac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
byte myIp[]  = { 192, 168, 132, 177 }; 


int destPort=12000;
byte destIp[]  = { 192, 168, 132, 145 };

OSCClient client;

//create new osc message
OSCMessage global_mes;
  
int v3=10;
float v4=10.0;
char str1[]="simple send 1!";

int v1=0;
float v2=0.0;



//Analog pin 1 for reading in the analog voltage from the MaxSonar device.
int anPin = 0;

//variables needed to store values
int arraysize = 5;  //quantity of values to find the median (sample size). Needs to be an odd number
int rangevalue[] = {0, 0, 0, 0, 0};    //declare an array to store the samples. not necessary to zero the array values here, it just makes the code clearer
int midpoint = arraysize/2;    //midpoint of the array is the medain value in a sorted array



void setup(){ 
  
 
 Ethernet.begin(myMac ,myIp); 
 

  //This opens up a serial connection to shoot the results back to the PC console
  Serial.begin(9600);
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
  
  delay(1 00);    //wait a while so you can read the values on the serial monitor


  global_mes.setAddress(destIp,destPort);
  
  global_mes.beginMessage("/ard/send1");
  global_mes.addArgInt32(rangevalue[midpoint]);
  global_mes.addArgFloat(v2);
  global_mes.addArgString(str1);
  
  client.send(&global_mes);
  
  global_mes.flush(); //object data clear
  

  
}



void send2(){
  //loacal_mes,str is release by out of scope
  OSCMessage loacal_mes;
  
  loacal_mes.setAddress(destIp,destPort);
  
  loacal_mes.beginMessage("/ard/send2");
  loacal_mes.addArgInt32(v3);
  loacal_mes.addArgFloat(v4);
  
  char str[]="simple send2 !!";
  loacal_mes.addArgString(str);
  
  client.send(&loacal_mes);
  
  v3++;
  v4 += 0.1;
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
}
