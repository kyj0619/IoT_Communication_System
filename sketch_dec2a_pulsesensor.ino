#define USE_ARDUINO_INTERRUPTS true
#include <PulseSensorPlayground.h>       


const int PulseWire = A0;    
const int buzzer = 9; 
int Threshold = 30;         
                               
PulseSensorPlayground pulseSensor;

void setup() {   

  Serial.begin(9600);
  pinMode(buzzer,HIGH);       


  pulseSensor.analogInput(PulseWire);          
  pulseSensor.setThreshold(Threshold);   


   if (pulseSensor.begin()) {
    Serial.println("We created a pulseSensor Object !"); 
  }
}



void loop() {

 int myBPM = pulseSensor.getBeatsPerMinute(); 


if (myBPM > Threshold) {            
 Serial.println("A HeartBeat Happened ! ");
 Serial.print("BPM: ");               
 Serial.println(myBPM);  
 digitalWrite(buzzer, HIGH);                 
}

else {
  digitalWrite(buzzer,LOW);
}

  delay(20);                 

}
