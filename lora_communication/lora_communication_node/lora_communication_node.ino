#include <SoftwareSerial.h>
#include "SNIPE.h"

//firebase 접속시 필요한 정보들
#if defined(ESP32)//insert code start
#include <WiFi.h>
#include <FirebaseESP32.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#endif
#include <addons/TokenHelper.h>
#include <addons/RTDBHelper.h>

#define WIFI_SSID "AndroidHotspot6619"
#define WIFI_PASSWORD "1234567890"

#define API_KEY "AIzaSyAmXNIA1w7ctrHYQv1tH_yIQaHeqPikCCc"

#define DATABASE_URL "test-f8051-default-rtdb.firebaseio.com" //<databaseName>.firebaseio.com or <databaseName>.<region>.firebasedatabase.app

#define USER_EMAIL "kyj97yj@gmail.com"
#define USER_PASSWORD "dkssud454117858"

FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;

unsigned long count = 0;//insert code end


#define CENTER 0
#define FIRE 1
#define HEART 2
#define MOBILE 3  // For wifi send

#define THIS_NODE HEART    /* Please define what this node */

#define TXpin 11 //11 or D9
#define RXpin 10 //10 or D8
#define ATSerial Serial

//16byte hex key
String lora_app_key = "22 11 33 44 55 66 77 88 99 aa bb cc dd ee ff 00";  

int stat = 0;

SoftwareSerial DebugSerial(RXpin,TXpin);
SNIPE SNIPE(ATSerial);
void setup() {
   ATSerial.begin(115200);

//wifi연결하여 firebase로 접속하는 과정
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);//insert code start
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);

  config.api_key = API_KEY;

  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  config.database_url = DATABASE_URL;

  config.token_status_callback = tokenStatusCallback; // see addons/TokenHelper.h

  Firebase.begin(&config, &auth);

  Firebase.reconnectWiFi(true);

  Firebase.setDoubleDigits(5);//insert code end


  // put your setup code here, to run once:
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

#if THIS_NODE == CENTER

    if(stat == 0) // 직전에 화재 경보 받았다
    {
      if(SNIPE.lora_send("Heart send")) // 심박 노드 보내라
      {
        DebugSerial.println("Let Heart send");

//firebase로 gps 데이터를 넘길때
    if (Firebase.ready() && (millis() - sendDataPrevMillis > 15000 || sendDataPrevMillis == 0))//insert code start
  {
    sendDataPrevMillis = millis();
    
    Serial.printf("Set string... %s\n", Firebase.setString(fbdo, F("/Data/gps"), recv) ? "ok" : fbdo.errorReason().c_str());

    Serial.printf("Get string... %s\n", Firebase.getString(fbdo, F("/Data/gps")) ? fbdo.to<const char *>() : fbdo.errorReason().c_str());

    FirebaseJson json;//insert code end
  }

        
        stat = 1;
      }
    }
    else if(stat == 1) // 직전에 심박 데이터 받았다
    {
      if(SNIPE.lora_send("Fire send")) // 화재경보 노드 보내라
      {
        DebugSerial.println("Let Fire send");

//firebase로 심박수 데이터를 넘길때
    if (Firebase.ready() && (millis() - sendDataPrevMillis > 15000 || sendDataPrevMillis == 0))//insert code start
  {
    sendDataPrevMillis = millis();
    
    Serial.printf("Set string... %s\n", Firebase.setString(fbdo, F("/Data/heartbeat"), recv) ? "ok" : fbdo.errorReason().c_str());

    Serial.printf("Get string... %s\n", Firebase.getString(fbdo, F("/Data/heartbeat")) ? fbdo.to<const char *>() : fbdo.errorReason().c_str());

    FirebaseJson json;//insert code end
  }

        
        stat = 0;
      }
    }
    
    String recv = SNIPE.lora_recv();
    if (recv != "AT_RX_TIMEOUT" && recv != "AT_RX_ERROR")
    {
      DebugSerial.println(recv);
      DebugSerial.println(SNIPE.lora_getRssi());
      DebugSerial.println(SNIPE.lora_getSnr());            
    }
    else
    {
      DebugSerial.println("recv fail");
      delay(500);
    }

    
    delay(1000);

#elif THIS_NODE == FIRE  // "Fire send" 받았으면 데이터를 보낸다.
    String data = "gpsx,gpsy";
    String recv = SNIPE.lora_recv();

    if (recv != "AT_RX_TIMEOUT" && recv != "AT_RX_ERROR")
    {
      DebugSerial.println(recv);
      if(recv == "Fire send")
      {
        if(SNIPE.lora_send(data))
       {
        DebugSerial.println("send success, fire node");
         //delay(1000);
       }
        else
       {
        DebugSerial.println("send fail, fire node");
        delay(500);
       }     
      }
    }
    
   

#elif THIS_NODE == HEART // "Heart send" 받았으면 데이터를 보낸다.
    String data = "heart";
    String recv = SNIPE.lora_recv();
    
   if (recv != "AT_RX_TIMEOUT" && recv != "AT_RX_ERROR")
    {
      DebugSerial.println(recv);
      if(recv == "Heart send")
      {
        if(SNIPE.lora_send(data))
       {
        DebugSerial.println("send success, heart node");
        //delay(1000);
       }
        else
       {
        DebugSerial.println("send fail, heart node");
        delay(500);
       }     
      }
    }

    

#endif
}
