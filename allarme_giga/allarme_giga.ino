#define VERSION "allarme_giga v2.2.0 - 16/04/2024" //ArduinoJson 7.0.4 //AsyncTelegram 2.3.1

#include "secret.h" //credentials and personal information
#include <SPI.h>
#include <WiFi.h>
#include <WiFiSSLClient.h>
#include <AsyncTelegram2.h>
#include <time.h>

#define MYTZ "CET+1CEST,M3.5.0,M10.5.0/3"
#define USE_CLIENTSSL false 
#define INGRESSO 22
#define SALA_GIARDINO 24
#define CUCINA 26
#define DISIMPEGNO 28
#define CAMERA 30
#define BAGNO 32
#define GARAGE 34
#define LED 12
#define RELE_SIRENA 31
#define RELE_SENSORI 8
#define RELE_ACCENSIONE 2
#define RELE_SPEGNIMENTO 3 
#define ON "on"
#define OFF "off"
#define STATUS "status"


volatile unsigned long activation_time;
volatile unsigned long msg_time;
volatile unsigned long wifi_time;
volatile unsigned long call_time; 
volatile unsigned long last_detection;
volatile unsigned long time_first_call=0;
unsigned long x=30000; //time after activation
unsigned long y=30000;  //interval for message 30 seconds
unsigned long z=120000; //alarm time 120 seconds
unsigned long w=1800000; //check connection every 30 minutes (1800000 ms)
unsigned long wait_call=0; //this will be incremented after each call

volatile boolean isON;
String position="null";
int first_call=true;
int user_num=0;
char users[4][17]={NUMBER1,NUMBER2,NUMBER3,NUMBER4};

char ssid[] = WIFI_SSID;        
char pass[] = WIFI_PASSWORD; 
const char* token = TELEGRAM_TOKEN;

WiFiSSLClient client;
AsyncTelegram2 myBot(client);
InlineKeyboard kbd;
int status = WL_IDLE_STATUS; //status of connection

TBMessage msg;
int a=0,b=0,c=0,d=0,e=0,f=0,g=0;

void setup() {

  Serial.begin(115200);
  Serial2.begin(115200); //Serial2 is TX1 and RX1 (17-18 pin)
  
  attachInterrupt(digitalPinToInterrupt(2),interrupt_activation,RISING);   
  attachInterrupt(digitalPinToInterrupt(3),interrupt_deactivation,RISING); 

  pinMode(INGRESSO,INPUT);
  pinMode(SALA_GIARDINO,INPUT);
  pinMode(CUCINA,INPUT);
  pinMode(DISIMPEGNO,INPUT);
  pinMode(CAMERA,INPUT);
  pinMode(BAGNO,INPUT);
  pinMode(GARAGE,INPUT);
  pinMode(RELE_ACCENSIONE,INPUT);
  pinMode(RELE_SPEGNIMENTO,INPUT);
  pinMode(LED,OUTPUT);
  pinMode(RELE_SIRENA,OUTPUT);
  pinMode(RELE_SENSORI,OUTPUT);

  digitalWrite(RELE_SENSORI, HIGH);
  digitalWrite(RELE_SIRENA, HIGH);
  
  isON=false;
  activation_time=millis();
  msg_time=millis();
  wifi_time=millis();

  while (status != WL_CONNECTED) {
    status = WiFi.begin(ssid, pass);
    delay(5000);
  }

  myBot.setUpdateTime(2000);
  myBot.setTelegramToken(token);

  kbd.addButton("Attiva allarme", ON, KeyboardButtonQuery);
  kbd.addButton("Disattiva allarme", OFF, KeyboardButtonQuery);
  kbd.addRow();
  kbd.addButton("Stato allarme", STATUS, KeyboardButtonQuery);

  myBot.sendTo(ID_1, "Sono di nuovo online"); //to warn when the power goes out

  Serial.println(F(VERSION));
}

void loop(){

  if(isON==true){
    a=digitalRead(INGRESSO);
    b=digitalRead(SALA_GIARDINO);
    c=digitalRead(CUCINA);
    d=digitalRead(DISIMPEGNO);
    e=digitalRead(CAMERA);
    f=digitalRead(BAGNO);
    g=digitalRead(GARAGE);

    if(a || b || c || d || e || f || g ){
      if(a) position="INGRESSO";
      if(b) position="PORTA FINESTRA GIARDINO";
      if(c) position="CUCINA";
      if(d) position="DISIMPEGNO";
      if(e) position="CAMERA";
      if(f) position="BAGNO";
      if(g) position="GARAGE";

      if(millis()>activation_time+x){//if passed enough time from activation
        
        digitalWrite(RELE_SIRENA, LOW);

        last_detection=millis(); //time from last detection

        if(millis()>msg_time+y){ //if passed enough time from last message
          myBot.sendTo(ID_1, "ALLARME "+ position);
          myBot.sendTo(ID_2, "ALLARME "+ position);
          myBot.sendTo(ID_3, "ALLARME "+ position);
          myBot.sendTo(ID_4, "ALLARME "+ position);
          msg_time=millis();
        }

        if(first_call==true){  //time of first call
          time_first_call=millis();
          first_call=false;
        }
        
        if(millis()>time_first_call+wait_call){ //every 50 seconds calls user
          Serial2.println("AT+CHUP");
          delay(500);
          Serial2.println(users[user_num]);
          if(user_num>=USERS-1){
            user_num=0;
          }
          else{
            user_num++;
          }
          wait_call+=50000;
        } 
      }
    }
  }

  if(millis()>wifi_time+w){ //check connection every 30 minutes
    wifi_time=millis();
    status=WiFi.status();
    if(status!=WL_CONNECTED){
      //Serial.println(F("Connessione assente, provo a riconettermi..."));
      status = WiFi.begin(ssid, pass);
    }
  }

  if(millis()>last_detection+z){ //after z millisecond from last detection
    digitalWrite(RELE_SIRENA, HIGH);
    last_detection=millis();
    first_call=true;
    wait_call=0;
  }
  
  if (myBot.getNewMessage(msg)) {
    if(msg.messageType==MessageText){
      myBot.sendMessage(msg, "Ciao, questo è quello che posso fare:", kbd);
    }
    else if(msg.messageType==MessageQuery){
      if (msg.callbackQueryData.equals(ON)){
        activation();
        myBot.sendMessage(msg, "Allarme attivato");
        myBot.sendMessage(msg, "Questo è quello che posso fare:", kbd);
      }

      if (msg.callbackQueryData.equals(OFF)){
        deactivation();
        myBot.sendMessage(msg, "Allarme disattivato");
        myBot.sendMessage(msg, "Questo è quello che posso fare:", kbd);
      }

      if (msg.callbackQueryData.equals(STATUS)){
        if(isON){
          myBot.sendMessage(msg, "L'allarme è attivo"); 
        }
        else{
          myBot.sendMessage(msg, "L'allarme non è attivo"); 
        }
        myBot.sendMessage(msg, "Questo è quello che posso fare:", kbd);
      }

    }
  }
  
}

void activation(){
  isON=true;
  digitalWrite(LED, HIGH);
  digitalWrite(RELE_SENSORI, LOW);
  activation_time=millis();
}
void deactivation(){
  isON=false;
  digitalWrite(LED, LOW);
  digitalWrite(RELE_SIRENA, HIGH);
  digitalWrite(RELE_SENSORI, HIGH);
  Serial2.println("AT+CHUP");
  first_call=true;
  wait_call=0;
}

void interrupt_deactivation(){
  deactivation();
}

void interrupt_activation(){
  activation();
}



