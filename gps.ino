#include <SoftwareSerial.h>

int flame = A1;     //센서 연결
int buzzer = 2;     //buzzer 연결
SoftwareSerial gps(11, 12); 
char c = "";
String str = "";
String targetStr = "GPGGA";
                                                                                                 
void setup() {

  // put your setup code here, to run once:

  Serial.begin(9600);
  gps.begin(9600);
  pinMode(flame, INPUT);
  pinMode(buzzer, OUTPUT);

}



void loop() {
  int val = analogRead(flame);
  noTone(buzzer);

    if(gps.available()){ // gps 센서 통신 가능?
        c=gps.read(); // 센서의 값 읽기

        if(c == '\n'){ // \n 값인지 구분.

          // \n 일시. 지금까지 저장된 str 값이 targetStr과 맞는지 구분

          if(targetStr.equals(str.substring(1, 6))){

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
            if(val < 1022){
              Serial.print("Lat : ");

              Serial.println(LatF, 15);

              Serial.print("Long : ");

              Serial.println(LongF, 15);

              tone(buzzer, 500, 100);
            }
            else{
              Serial.print("No Fire");
              noTone(buzzer);
            }

            Serial.print("\n");

            
          }

          // str 값 초기화 

          str = "";

        }else{ // \n 아닐시, str에 문자를 계속 더하기

          str += c;

        }

      }
}


