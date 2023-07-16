#include "CTBot.h"
#define DISATTIVA "disattiva"
#define ATTIVA "attiva"

String s="vuota";
CTBot myBot;
int incomingByte=0;
String ssid = "Casa Pedica";  
String pass = "password"; 
String token = "token";

CTBotInlineKeyboard pulsantiera;
TBMessage msg;
unsigned long tempo=0;

void setup() {
  Serial.begin(9600);
  //Serial.println("Avvio il setup...");
  
CONNESSIONE:
  //Serial.println("Provo a connettere");
  myBot.wifiConnect(ssid, pass);
  myBot.setTelegramToken(token);

  // check if all things are ok
  if (myBot.testConnection())
    millis();
    //Serial.println("\ntestConnection OK");
  else{
    //Serial.println("\ntestConnection NOK");
    goto CONNESSIONE;
  }
  
  pulsantiera.addButton("Attiva allarme", "attiva", CTBotKeyboardButtonQuery);
  pulsantiera.addButton("Disattiva allarme", "disattiva", CTBotKeyboardButtonQuery);
  pulsantiera.addRow();
  pulsantiera.addButton("È attivo?", "eattivo", CTBotKeyboardButtonQuery);
  tempo=millis();
}

void loop() {
   TBMessage msg;
   
   if (myBot.getNewMessage(msg)){
    if (msg.text.equalsIgnoreCase("attiva")){
        Serial.print("attiva");
    }
    else if (msg.text.equalsIgnoreCase("disattiva")){
        Serial.print("disattiva");
    }
    
    else if (msg.callbackQueryData.equals("attiva")){
        Serial.print("attiva");
    }
    
    else if (msg.callbackQueryData.equals("disattiva")){
        Serial.print("disattiva");
    }
    
    else if (msg.callbackQueryData.equals("eattivo")){
        Serial.print("eattivo");
    }
    
    else{
      //Serial.print(msg.text);
      myBot.sendMessage(msg.sender.id,"Ciao, questo è quello che posso fare: " ,pulsantiera);
    }
    Serial.flush();
  }


  if(Serial.available()>0){
    String s=Serial.readString();
    if(s.equalsIgnoreCase("allarme")){
      myBot.sendMessage(176562109, "ALLARME");
    }
    
    else if(s.equalsIgnoreCase("eattivo")){
      myBot.sendMessage(msg.sender.id,"Si, è attivo");
    }
    else if(s.equalsIgnoreCase("noneattivo")){
      myBot.sendMessage(msg.sender.id,"No, non è attivo");
    }
    
    else{
    myBot.sendMessage(msg.sender.id, s);
    myBot.sendMessage(176562109, s);
    }

  }
  if(millis()>tempo+120000){
    if (myBot.testConnection()==false);{
      myBot.wifiConnect(ssid, pass);
      }
  }
  delay(50);
}
