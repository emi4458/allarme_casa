
#include <SoftwareSerial.h>

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
#define RELE_SPEGNIMENTO 3  //DA COLLEGARE AL PIN INT0 DELL INTERRUPT
#define CHECK_SIM 53
#define ACCENSIONE_SIM 51
SoftwareSerial SIM900(47, 49);

char s;
volatile unsigned long  tempo;
volatile unsigned long  tempo_msg;
unsigned long tempo_sirena;
unsigned long x=30000; //attesa dopo attivazione
unsigned long y=5000;  //attesa per non inviare messaggi ripetitivi
unsigned long z=40000; //durata del suono della sirena
volatile boolean isON;


void accensione(){
  Serial.println("interrupt accensione");
  digitalWrite(LED, HIGH);
  digitalWrite(RELE_SENSORI, HIGH);
  isON=true;
  tempo=millis();
}
void spegnimento(){
  Serial.println("interrupt spegnimento");
  isON=false;
  digitalWrite(LED, LOW);
  digitalWrite(RELE_SIRENA, LOW);
  digitalWrite(RELE_SENSORI, LOW);
}


void interruzione_spegnimento(){
  spegnimento();
}

void interruzione_accensione(){
  //digitalWrite(RELE_SIRENA,LOW);
  accensione();
  tempo=millis(); //ancora più new
  //digitalWrite(RELE_SIRENA,LOW);
}


void setup() {
  
  attachInterrupt(0,interruzione_accensione,RISING);                                                                  //PAG 275 LIBRO, INT 0 è IL PIN INT0 DELLA SCHEDA
  attachInterrupt(1,interruzione_spegnimento,RISING); 
  //pinMode(49,OUTPUT);
  //pinMode(47,OUTPUT);
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
  pinMode(CHECK_SIM,INPUT);
  pinMode(ACCENSIONE_SIM,OUTPUT);
  
  SIM900.begin(19200);
  Serial.begin(9600);
  Serial3.begin(9600);
  isON=false;
  delay(2000);
  tempo=millis();
  tempo_msg=millis();
  if(digitalRead(CHECK_SIM)==0){
    accendiSim();
  }
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

    if(a==1 || b==1 || c==1 || d==1 || e==1 || f==1 || g==1){
      String rilevatore="vuota";
      if(a==1) rilevatore="Ingresso";
      if(b==1) rilevatore="Porta-finestra giardino";
      if(c==1) rilevatore="Cucina";
      if(d==1) rilevatore="Disimpegno";
      if(e==1) rilevatore="Camera";
      if(f==1) rilevatore="Bagno";
      if(g==1) rilevatore="Garage";
      
//        Serial.print("millis: ");
//        Serial.println(millis());
//        Serial.print("\ntempo: ");
//        Serial.println(tempo);
        
      if(millis()>tempo+x){                         //se è passato il tempo dall'attivazione dell allarme
        if(millis()>tempo_msg+y){                   //se è passato il tempo dall ultimo messaggio
          Serial3.write("ALLARME ");
          Serial3.println(rilevatore);
          Serial3.flush();
          tempo_msg=millis();
        }
        
        digitalWrite(RELE_SIRENA, HIGH);
        tempo_sirena=millis();
        //Serial.println("movimento rilevato inizio le chiamate");
        //call_emiliano();
        //delay(20000);
        //call_simonetta();
        //call_gianpaolo();
        //Serial.println("fine delle chiamate");
      }
    }
  }

  
  if (Serial3.available()>0){  //comunicazione con esp
    String s=Serial3.readString();
    
    if(s.equalsIgnoreCase("attiva")){
      accensione();
    }
    else if(s.equalsIgnoreCase("disattiva")){
      spegnimento();
    }
    else if(s.equalsIgnoreCase("eattivo")){
      delay(50);
      if(isON) Serial3.write("eattivo");
      if(!isON) Serial3.write("noneattivo");
      
    }
    else {
      Serial3.write("Non ho capito");
    }
    Serial3.flush();
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
  if(digitalRead(CHECK_SIM)==0){
    accendiSim();
  }
  if(millis()>tempo_sirena+z){
    digitalWrite(RELE_SIRENA, LOW);
  }
}

/********************FUNZIONI CHIAMATA*****************************/
void call_emiliano(){
  Serial.println("chiamo emi");
  SIM900.println("ATD + +0000000000;");
  delay(20000);
  SIM900.println("ATH");
  }
  
void call_gianpaolo(){
  Serial.println("chiamo gian");
  SIM900.println("ATD + +0000000000;");
  delay(20000);
  SIM900.println("ATH");
  }
  
void call_simonetta(){
  Serial.println("chiamo simo");
  SIM900.println("ATD + +0000000000;");
  delay(20000);
  SIM900.println("ATH");
  }
void accendiSim(){
  digitalWrite (ACCENSIONE_SIM, HIGH);
  delay (2500);
  digitalWrite (ACCENSIONE_SIM, LOW);
  delay (3000);
}
/******************************************************************/
  
