/* 
이 로직은 단방향 동작기기(START OR STOP)를 제어하기 위함이다.
최종 수정일자 : 2018.09.05
*/ 

const int input1 = 2; // START 스위치 입력
const int input2 = 3; // STOP 스위치 입력
const int input3 = 4; // 42A 입력
const int input4 = 5; // NOT ASSIGN
const int input5 = 6; // AUX Start 입력
const int input6 = 7; // Aux Stop 입력
const int input7 = 8; // 예비접점
const int output1 = 8; // 제어신호 출력단자
const int output2 = 9; // START feedback 신호
const int output3 = 10; // STOP feedback 신호
const int output4 = 11; // 제어신호 feedback beep음 출력단자 
boolean var1 = LOW; // 스위치 start 변수
boolean var2 = LOW; // 스위치 stop 변수
boolean var3 = LOW; // 스위치 ACK 변수
int comm = 0; // 통신으로 수신하는 제어명령! 1 START/2 STOP/3 ACK
int feed = 0; // 통신으로 올려보낼 기기상태 정보! 1 STARTED/2 STOPPED /3 TRBL/4 DSBL
boolean var4 = LOW; // 시리얼 start 변수 
boolean var5 = LOW; // 시리얼 stop 변수
boolean var6 = LOW; // 시리얼 ack 변수
boolean comm_start = LOW; // START 최종신호
boolean comm_stop = LOW; // STOP 최종신호
boolean comm_ack = LOW; // ack 최종신호
boolean aux_start = LOW; // Aux Start
boolean aux_stop = LOW; // Aux Stop 
 
boolean sr_q = LOW; // SR래치에 사용하는 변수
boolean beep_feedback = LOW;

void setup() {
  pinMode(input1, INPUT); //START 스위치를 변수 input1로 설정
  pinMode(input2, INPUT); //STOP 스위치를 변수 input1로 설정
  pinMode(input3, INPUT); //42A 접점을 변수 input3으로 설정
  pinMode(input4, INPUT); //예비접점
  pinMode(input5, INPUT); //aux start 명령 수신을input5로 설정
  pinMode(input6, INPUT); //aux stop 명령 수신을input6으로 설정
  pinMode(input7, INPUT); //예비접점
  pinMode(output1, OUTPUT); //출력을 변수 output1로 설정
  pinMode(output4, OUTPUT); //비프음 출력변수 output4로 설정
  Serial.begin(9600); // 시리얼통신 baudrate 설정
  Serial.println("Enter Command : 1(start),2(stop),3(ackowledge)");
}

void loop() {
  //Hardwired handsiwtch 읽기
  var1 = digitalRead(input1); // START 스위치 입력감시
  var2 = digitalRead(input2); // STOP 스위치 입력감시
  if( var1 || var2 == HIGH ){
    beep_feedback = HIGH;
  }
  
  //Serial 통신으로 data 읽기
  if(Serial.available() > 0){ // serial 통신이 가능하다면 통신시작
    beep_feedback = HIGH; // command가 들어오면 beep 한번 삑 
    comm = Serial.read(); // serial로 읽어온 값을 comm에 저장
    if( comm == '1'){
      var4 = HIGH;
    }
    else if( comm == '2'){
      var5 = HIGH;
    }
    else if( comm == '3'){
      var6 = HIGH;
    }
    Serial.write(comm); // putty창 표시용
    delay(100);
  }

  // 스위치입력과 통신입력 oring
  comm_start = var1 | var4;
  comm_stop = var2 | var5;
  comm_ack = var3 | var6;

  // 다시 통신으로 받은 DATA를 0으로 초기화
  var4 = LOW;
  var5 = LOW;
  var6 = LOW; 

  
  //---SR LATCH---
  if(comm_start == HIGH & comm_stop == LOW){
    sr_q = HIGH;
  }
  else if(comm_start == LOW & comm_stop == LOW){
    sr_q = sr_q;
  }
  else if(comm_stop == HIGH) {
    sr_q = LOW;
  }
  //---SR LATCH.끝---

  digitalWrite(output1, sr_q);
  digitalWrite(output4, beep_feedback);
}
