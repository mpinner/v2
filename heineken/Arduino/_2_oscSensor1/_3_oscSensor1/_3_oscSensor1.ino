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

void setup(){ 
  
 
 Ethernet.begin(myMac ,myIp); 
 

}
  
void loop(){
  
  global_mes.setAddress(destIp,destPort);
  
  global_mes.beginMessage("/ard/send1");
  global_mes.addArgInt32(v1);
  global_mes.addArgFloat(v2);
  global_mes.addArgString(str1);
  
  client.send(&global_mes);
  
  global_mes.flush(); //object data clear
  
  delay(500);
  
  
  
  send2();
  
  delay(500);
  
  v1++;
  v2 += 0.1;

  
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
