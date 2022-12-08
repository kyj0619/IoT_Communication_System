#include <SoftwareSerial.h>
#include "SNIPE.h"


//This node is Fire node

#define TXpin 9 //11 or D9
#define RXpin 8 //10 or D8
#define ATSerial Serial

//16byte hex key
String lora_app_key = "22 11 33 44 55 66 77 88 99 aa bb cc dd ee ff 00";  

SNIPE SNIPE(ATSerial);

int flame = A1;     //센서 연결
int buzzer = 2;     //buzzer 연결
SoftwareSerial gps(11, 12); 
SoftwareSerial DebugSerial(RXpin,TXpin);
char c = "";
String str = "";
String targetStr = "GPGGA";
                                                                                                 
void setup() {

  // put your setup code here, to run once:

 // Serial.begin(9600);
  gps.begin(9600);
  pinMode(flame, INPUT);
  pinMode(buzzer, OUTPUT);

  //Lora Setup -------------------------
  ATSerial.begin(115200);

  while(ATSerial.read()>= 0) {}
  while(!ATSerial);

  DebugSerial.begin(115200);

  /* SNIPE LoRa Initialization */
  if (!SNIPE.lora_init()) {
    DebugSerial.println("SNIPE LoRa Initialization Fail!");
    while (1);
  }

  /* SNIPE LoRa Set Appkey */
  if (!SNIPE.lora_setAppKey(lora_app_key)) {
    DebugSerial.println("SNIPE LoRa app key value has not been changed");
  }
  
  /* SNIPE LoRa Set Frequency */
  if (!SNIPE.lora_setFreq(LORA_CH_1)) {
    DebugSerial.println("SNIPE LoRa Frequency value has not been changed");
  }

  /* SNIPE LoRa Set Spreading Factor */
  if (!SNIPE.lora_setSf(LORA_SF_7)) {
    DebugSerial.println("SNIPE LoRa Sf value has not been changed");
  }

  /* SNIPE LoRa Set Rx Timeout 
   * If you select LORA_SF_12, 
   * RX Timout use a value greater than 5000  
  */
  if (!SNIPE.lora_setRxtout(5000)) {
    DebugSerial.println("SNIPE LoRa Rx Timout value has not been changed");
  }  
    
  DebugSerial.println("SNIPE LoRa Message Test");
}



void loop() {
  bool newData = false;
  unsigned long chars;
  unsigned short sentences, failed;
  

  // For one second we parse GPS data and report some key values
  for (unsigned long start = millis(); millis() - start < 1000;)
  {
    while (gps.available())
    {
      char c = gps.read();
      // Serial.write(c); // uncomment this line if you want to see the GPS data flowing
      if (c == '\n' && targetStr.equals(str.substring(1, 6))) // Did a new valid sentence come in?
        newData = true;
    }
  }
  
  int val = analogRead(flame);
  noTone(buzzer);

  String recv = SNIPE.lora_recv();
  DebugSerial.println(recv);
    if(newData){ // new data 있으면
      char gps_buf[32];

       // if(c == '\n'){ // \n 값인지 구분.

          // \n 일시. 지금까지 저장된 str 값이 targetStr과 맞는지 구분

       //   if(targetStr.equals(str.substring(1, 6))){

            // NMEA 의 GPGGA 값일시

            //Serial.println(str);

            // , 를 토큰으로서 파싱.

            int first = str.indexOf(",");

            int two = str.indexOf(",", first+1);

            int three = str.indexOf(",", two+1);

            int four = str.indexOf(",", three+1);

            int five = str.indexOf(",", four+1);

            // Lat과 Long 위치에 있는 값들을 index로 추출

            String Lat = str.substring(two+1, three);

            String Long = str.substring(four+1, five);

            // Lat의 앞값과 뒷값을 구분

            String Lat1 = Lat.substring(0, 2);

            String Lat2 = Lat.substring(2);

            // Long의 앞값과 뒷값을 구분

            String Long1 = Long.substring(0, 3);

            String Long2 = Long.substring(3);

            // 좌표 계산.

            double LatF = Lat1.toDouble() + Lat2.toDouble()/60;

            float LongF = Long1.toFloat() + Long2.toFloat()/60;

            // 좌표 출력.
            if(recv == "Fire send") {
              if(val < 1022){
                //Serial.print("Lat : ");

                //Serial.println(LatF, 15);

                //Serial.print("Long : ");

                //Serial.println(LongF, 15);

                sprintf(gps_buf, "%s:%s", LatF, LongF);
                String data = (String)gps_buf;

                //6String data = String(LatF) + ', ' + String(LongF); 
                if(SNIPE.lora_send(data))
                {    //DebugSerial.listen();
                     DebugSerial.println("send success, fire node");
                     //delay(1000);
                }

                tone(buzzer, 500, 100);
             }
             else{
                Serial.print("No Fire");
                noTone(buzzer);
                if(SNIPE.lora_send("No Fire"))
                {
                      // DebugSerial.listen();
                       DebugSerial.println("send success, No Fire");
                     //delay(1000);
                }
              }
            }

            Serial.print("\n");

            
          

          // str 값 초기화 

          str = "";
        
      }
}
