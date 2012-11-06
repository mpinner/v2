  
// Example by Tom Igoe

import processing.serial.*;

int lf = 10;    // Linefeed in ASCII
String myString = null;
Serial myPort;  // The serial port

int readingCount = 4; 
int range[] = {0, 0, 0, 0};

void setup() {
    size(640, 640, P3D);
  noStroke();
  colorMode(RGB, 1);
  fill(0.4);
  
  // List all the available serial ports
  println(Serial.list());
  // Open the port you are using at the rate you want:
  myPort = new Serial(this, Serial.list()[4], 9600);
  myPort.clear();
  // Throw out the first reading, in case we started reading 
  // in the middle of a string from the sender.
  myString = myPort.readStringUntil(lf);
  myString = null;
}

void draw() {
  while (myPort.available() > 0) {
    myString = myPort.readStringUntil(lf);
    if (myString != null) {
  //    print(myString);
      String[] rangeStr = myString.split(",");
      for(int i = 0; i < rangeStr.length; i++) {
        range[i] = parseInt(rangeStr[i].trim());
      } 
    }
  }

  
  for(int rd: range) {
    print(rd + ", ");
  }
  
  println();


int widthQuarter = width / 4;
int heightQuarter = height / 4;
  
  // Set the specular color of lights that follow
  lightSpecular(1, 1, 1);
  directionalLight(0.8, 0.8, 0.8, 0, 0, -1);
  background(0);
  
  
  pushMatrix();
  translate(widthQuarter, heightQuarter);
  float s = range[0]/float(256);
  specular(s, s, s);
  sphere(120);
  popMatrix();
  
  pushMatrix();
  translate(widthQuarter*3, heightQuarter);
  s = range[1]/float(256);
  specular(s, s, s);
  sphere(120);
  popMatrix();
  
  pushMatrix();
  translate(widthQuarter, heightQuarter*3);
  s = range[2]/float(256);
  specular(s, s, s);
  sphere(120);
  popMatrix();

  pushMatrix();
  translate(widthQuarter*3, heightQuarter*3);
  s = range[3]/float(256);
  specular(s, s, s);
  sphere(120);
  popMatrix();

    
  return;
  
}

