# 화재 경보 및 응급 대처 시스템

## Gateway
[lora_communication_gateway.ino](https://github.com/kyj0619/IoT_Communication_System/blob/main/lora_communication/lora_communication_gateway/lora_communication_gateway.ino)

### 사용 하드웨어
* Wemos D1 Board
* MechaSolution 로라 쉴드

### 기능
양쪽 아두이노의 로라 신호를 수신하고, 데이터를 총합해 와이파이로 FireBase 서버에 올리는 기능


## 화재 경보 노드
[fire_node_final.ino](https://github.com/kyj0619/IoT_Communication_System/tree/main/fire_node_final/fire_node_final.ino)

### 사용 하드웨어
* Arduino Uno Board
* MechaSolution 로라 쉴드
* 불꽃 감지 센서
* 부저

### 기능
불꽃 감지 센서를 이용해 화재를 감시하고, 화재 발생 시 부저가 울리는 기능.

로라 쉴드를 통해서 화재 미발생 시에는 "No Fire"을 전송하다가, 화재 발생 시 설치해둔 곳의 좌표값을 전송한다.


## 응급 대처 노드
[Heart_node_final.ino](https://github.com/kyj0619/IoT_Communication_System/blob/main/heart_node_final/heart_node_final.ino)

### 사용 하드웨어
* Arduino Uno Board
* MechaSolution 로라 쉴드
* 심장 박동 센서
* 부저
* 압력 센서
* LCD 모니터

### 기능
심박수 센서를 이용해 심박수를 체크하고, 박동이 감지되면 부저가 울리는 기능.

CPR 응급처치 시 압력 센서를 이용해 압력을 체크하고, 권고 사항을 LCD 모니터에 출력하는 기능.

로라 쉴드를 통해서 감지된 심박수 값을 전송한다.


## 안드로이드 앱
[emptyactivity](https://github.com/kyj0619/IoT_Communication_System/tree/main/emptyactivity)

### 기능
FireBase 상에 업로드된 내용을 가져와 안드로이드 앱에 출력한다.
