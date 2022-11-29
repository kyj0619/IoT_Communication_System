#include <SoftwareSerial.h>
#include "SNIPE.h"

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
        stat = 1;
      }
    }
    else if(stat == 1) // 직전에 심박 데이터 받았다
    {
      if(SNIPE.lora_send("Fire send")) // 화재경보 노드 보내라
      {
        DebugSerial.println("Let Fire send");
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
