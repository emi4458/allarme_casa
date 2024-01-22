#define VERSION "allarme_giga v2.0 - 22/01/2024" //ArduinoJson 6.21.5 //AsyncTelegram 2.2.3 (or 2.2.0 is ok)


#include "secret.h" //credentials for wifi and telegram
#include <SPI.h>
#include <WiFi.h>
#include <WiFiSSLClient.h>
#include <AsyncTelegram2.h>

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
#define RELE_SPEGNIMENTO 3  //connect to pin INT0 INTERRUPT
#define USE_CLIENTSSL true 
#define ON "on"
#define OFF "off"
#define STATUS "status"


volatile unsigned long  activation_time;
volatile unsigned long  msg_time;
volatile unsigned long  wifi_time;
unsigned long alarm_time;
unsigned long x=30000; //time after activation
unsigned long y=5000;  //interval for message
unsigned long z=60000; //alarm time 60 seconds
unsigned long w=1800000; //check connection every 30 minutes
volatile boolean isON;

char ssid[] = wifi_ssid;        
char pass[] = wifi_password; 
const char* token = telegram_token;

WiFiSSLClient client;
AsyncTelegram2 myBot(client);
InlineKeyboard kbd;
int status = WL_IDLE_STATUS; //status of connection


void activation(){
  isON=true;
  digitalWrite(LED, HIGH);
  digitalWrite(RELE_SENSORI, HIGH);
  activation_time=millis();
}
void deactivation(){
  isON=false;
  digitalWrite(LED, LOW);
  digitalWrite(RELE_SIRENA, LOW);
  digitalWrite(RELE_SENSORI, LOW);
}


void interrupt_deactivation(){
  deactivation();
}

void interrupt_activation(){
  activation();
  activation_time=millis(); //ancora più new
}


void setup() {
  
  attachInterrupt(0,interrupt_activation,RISING);             //page 275 of the book, INT 0 è IL PIN INT0 DELLA SCHEDA
  attachInterrupt(1,interrupt_deactivation,RISING); 
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
  
  Serial.begin(9600);
  isON=false;
  delay(2000);
  activation_time=millis();
  msg_time=millis();
  wifi_time=millis();

  while (status != WL_CONNECTED) {
    status = WiFi.begin(ssid, pass);
    delay(5000);
  }
  myBot.setUpdateTime(1000);
  myBot.setTelegramToken(token);

  kbd.addButton("Attiva allarme", ON, KeyboardButtonQuery);
  kbd.addButton("Disattiva allarme", OFF, KeyboardButtonQuery);
  kbd.addRow();
  kbd.addButton("Stato allarme", STATUS, KeyboardButtonQuery);

  myBot.sendTo(id_1, "Sono online");
  Serial.println(VERSION);
}

void loop() {
  
  int a=0,b=0,c=0,d=0,e=0,f=0,g=0;

  if(isON==true){
    a=digitalRead(INGRESSO);
    b=digitalRead(SALA_GIARDINO);
    c=digitalRead(CUCINA);
    d=digitalRead(DISIMPEGNO);
    e=digitalRead(CAMERA);
    f=digitalRead(BAGNO);
    g=digitalRead(GARAGE);

    if(a || b || c || d || e || f || g ){
      String position="null";
      if(a) position="Ingresso";
      if(b) position="Porta-finestra giardino";
      if(c) position="Cucina";
      if(d) position="Disimpegno";
      if(e) position="Camera";
      if(f) position="Bagno";
      if(g) position="Garage";
      
      if(millis()>activation_time+x){                         //if passed enough time from activation
        if(millis()>msg_time+y){                   //if passed enough time from last message
          myBot.sendTo(id_1, "ALLARME "+ position);
          msg_time=millis();
        }
        digitalWrite(RELE_SIRENA, HIGH);
        alarm_time=millis();
      }
    }
  }

  if(millis()>wifi_time+w){ //check connection every 30 minutes
    status=WiFi.status();
    if(status!=WL_CONNECTED){
      status = WiFi.begin(ssid, pass);
      delay(5000);
    }
  }

  
  if(isON==false){
    digitalWrite(RELE_SIRENA, LOW);
    digitalWrite(RELE_SENSORI, LOW);
    digitalWrite(LED,LOW);
  }
  if(isON==true){
    digitalWrite(RELE_SENSORI,HIGH);
    digitalWrite(LED,HIGH);
  }
  if(millis()>alarm_time+z){
    digitalWrite(RELE_SIRENA, LOW);
  }

  TBMessage msg;
  if (myBot.getNewMessage(msg)) {

    if(msg.messageType==MessageText){
      myBot.sendMessage(msg, "Questo è quello che posso fare:", kbd);
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
