//Use of many syncronized MaxSonar sonar modules in analog mode with arduino

const int sensorCount = 4;

int rangePin[] = {0, 1, 2, 3};

int bwPin[] = {3, 4, 7};
int bwCount = 3;

//variables needed to store values
const int arraysize = 5;  //quantity of values to find the median (sample size). Needs to be an odd number
int rangevalue[sensorCount][arraysize] = 
  {
    {0, 0, 0, 0, 0},   
    {0, 0, 0, 0, 0},   
    {0, 0, 0, 0, 0},   
    {0, 0, 0, 0, 0}   
  };


//declare an array to store the samples. not necessary to zero the array values here, it just makes the code clearer
//*********************************************************************************************
void setup() 
{
  
  for (int i = 0; i < bwCount; i++ ) {
      pinMode(bwPin[i], OUTPUT);
      digitalWrite(bwPin[i], HIGH);
  }

  //This opens up a serial connection to shoot the results back to the PC console
  Serial.begin(9600);
  printArray(rangevalue[0], arraysize);  
  delay(5000);        //wait a while to open serial monitor window

}
//********************************************************************************
void loop() 
{


  //MaxSonar Analog reads are known to be very sensitive. See the Arduino forum for more information.
  //Maxbotix does not recommend averaging readings as the occasional value can be wildly inaccurate and skew the average
  //A median sort is the preferred method. 
for ( int sensor = 0; sensor < sensorCount; sensor++) {
  
for(int i = 0; i < arraysize; i++)
   {                                                    //array pointers go from 0 to 4

    //Used to read in the analog voltage output that is being sent by the MaxSonar device.
    //The MaxSonar Scale factor is (Vcc/512) per inch. A 5V supply yields ~9.8mV/in
    //The Arduino will map input voltages between 0 and 5 volts into integer values between 0 and 1023. 
    //This yields a resolution between readings of: 5 volts / 1024 units or, .0049 volts (4.9 mV) per unit. 
    //Therefore, one unit from the arduino's ADC represents 0.5 inches

     rangevalue[0][i] = analogRead(rangePin[sensor]);
 /*
 Serial.print("i, value   ");
          Serial.print(i);
          Serial.print(" ,  ");
          Serial.print(rangevalue[i]);
          Serial.println();
*/
     delay(10);  //wait between analog samples
    }  

 //  Serial.print("unsorted ");
   printArray(rangevalue[sensor], arraysize);
//   Serial.println();
   isort(rangevalue[sensor], arraysize);
   Serial.print("sorted ");
   printArray(rangevalue[sensor], arraysize);
   Serial.println();
    
 // now show the medaian range   
    int midpoint = arraysize/2;    //midpoint of the array is the medain value in a sorted array
      //note that for an array of 5, the midpoint is element 2, as the first element is element 0
     Serial.print("median range value ");
     Serial.print(rangevalue[sensor][midpoint]);
     Serial.println();  
     Serial.println();  
  
  delay(10);    //wait a while so you can read the values on the serial monitor

}   //end of array reading loop
} // end of sensor loop
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
