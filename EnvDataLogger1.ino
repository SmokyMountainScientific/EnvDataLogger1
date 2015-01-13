/*
  EvnDataLogger1
 
 Program prompts user to enter military time (0900 is 9 am, 2100 is 9 pm).
 On receiving time stamp, program reads data at intervals.
 Interval is set as readDelay.
 Analog inputs are read into memory.
 The maximum number of readings is limited by available RAM.
 the Tiva C launchpad will store > 3000 sets of two ints.
 Experiment is stopped and data written to the serial monitor when user enters "%" in command line.
 After data is written, user is prompted for another start time.
 
 
 Written begining Sept 18, 2014 
 by JSummers
 starting from T. Igoe's "AnalogInOutSerial"
 
 This example code is in the public domain.
 
 */

int vcc = 3300;    // mV full scale
//const int tempPin1 = A11;  // temperature 1
const int tempPin2 = A9;  // temperature 2
const int pHpin = A8;  // pH meter
int acqs = 3000;   // number of acquisitions
//int sensorValue1 = 0;        // temperature 1
int sensorValue2[3000];        // temperature 2
int sensorValue3[3000];        // pH meter reading
//int outputValue = 0;        // value output to the PWM (analog out)
int readDelay = 200;
int maxJ;
int m;
   boolean rsStatus;  // reset status
   boolean runStatus;
   int sTime;  // start time
   int readPlace[4] = {1000,100,10,1};
   int readByte[4] = {0,0,0,0};

void setup() {
  Serial.begin(9600); 
}

void loop() {
  int n=0;
  sTime = 0;
  Serial.println("Enter start time (hhmm)");
  while(Serial.available() <= 0) {}
  while(Serial.available() > 0) {
    readByte[n] = Serial.read()-48;
    sTime =(readByte[n]*readPlace[n])+sTime;
    ++n;
 delay(2);   
  }
  Serial.println(sTime);
 rsStatus=false;
//////////////////////////////  // read the analog in value:
for (int j=0; j<acqs; j++) {
  sensorValue2[j] =0;
  sensorValue3[j] =0; 
  for (int m=0; m<16; m++) {            // average 16 measured values 
  sensorValue2[j] = analogRead(tempPin2)+sensorValue2[j];
  sensorValue3[j] = analogRead(pHpin)+sensorValue3[j];  
  }
  sensorValue2[j] = sensorValue2[j]/16;
  sensorValue3[j] = sensorValue3[j]/16; 
   maxJ = j;    // maxJ used to sync output with the number of data aquired
 
    if(Serial.available()>0) {   // look for message
     rsStatus = Serial.find("%");  // find % symbol in comm
   }  
   if (rsStatus == true) {
     j=acqs;          //  if % symbol is transmitted, terminate data acquisition
   }                  //  and print out data
  delay(readDelay);   
}
Serial.print("Start time = ");
Serial.println(sTime);
Serial.println("exp,   temp 2,    pH ");
for (int k=0; k<=maxJ; k++){
  // print the results to the serial monitor:
  Serial.print(k );  
  Serial.print("\t");   
  Serial.print(sensorValue2[k]);      
  Serial.print("\t");         // pH value = "); 
int pH = (-sensorValue3[k]/0.2818+14305) ;   // gives 1000*pH
int firstDig = pH/1000;         // mickey mouse way of getting 3 digit precision
int nextThree = pH%1000;
  Serial.print(firstDig); 
  Serial.print(".");
  Serial.println(nextThree);  
}

 delay(1000);                     
}


