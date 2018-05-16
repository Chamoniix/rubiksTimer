#include <EEPROM.h>
#include <LiquidCrystal.h> 

LiquidCrystal screen(12, 11, 5, 4, 3, 2); 

const String states[8] = {"menu",               //0
                          "wait_for_start",     //1
                          "running",            //2
                          "stoped",             //3
                          "saving",             //4
                          "leaderboard",        //5
                          "melanger",           //6
                          "fun"};               //7
String state = states[0];

unsigned long a, b, time_millis;
unsigned long time_int, mm, ss, m;

const int butn_start_1 = 13;
const int butn_start_2 = 10;

bool just_ended = false, saved = false;
int choice = 0;

void setup() {
  pinMode(butn_start_1, INPUT);
  pinMode(butn_start_2, INPUT);
  
  //initialisation de l'écran LCD
  screen.begin(16, 2); 
  screen.clear(); 
}


void loop() {
  isButnPress();

  if (state == states[2]){ 
    count();  
  }  

  affichage();
}

void count() {
  if (state == states[2]) {
    b = millis();
    time_millis = (b - a);
    time_int = round(time_millis);
    m = time_int % 1000;
    ss = ((time_int - m) / 1000) % 60;
    mm = (time_int - ss*1000 - m) / 60000;
  }
}


void affichage() {
  screen.setCursor(0, 0);
  if(state == states[0]){
    String str = "";
    if (choice == 0)
      str.concat(">");
    else
      str.concat(" ");
    str.concat(" Jouer             ");
    
    if (choice == 2)
      str.concat(">");
    else
      str.concat(" ");
    str.concat(" Leaderboard       ");
    
    if (choice == 1)
      str.concat(">");
    else
      str.concat(" ");
    str.concat(" Melanger          ");
    
    if (choice == 3)
      str.concat(">");
    else
      str.concat(" ");
    str.concat(" Fun               ");
    
    screen.setCursor(0, 0);
    screen.print(str);  
  }
  if(state == states[1]){
    screen.print("Both hands on");  
    screen.setCursor(0,3);
    screen.print("buttons !");
    
  }
  else if (state == states[2]){
    screen.setCursor(6,1);
    screen.print(String(mm) + ":" + String(ss) + ":" + String(m));  
  }
  else if (state == states[3]){
     if (just_ended) {
      for(int i=0; i<3; i++) {
        screen.setCursor(6,1);
        screen.print(String(mm) + ":" + String(ss) + ":" + String(m)); 
        delay(1000);
        screen.clear();
        delay(500);
      }
      screen.clear();
      just_ended = false;
    } 
    screen.setCursor(6,0);
    screen.print(String(mm) + ":" + String(ss) + ":" + String(m));  
    screen.setCursor(0,2);
    screen.print("Valider cet essai ? YES               NO");
  }
  else if (state == states[4]){
    if (saved == false) {
      int address = 0;
      EEPROM.put(address,mm);
      address += sizeof(mm);
      EEPROM.put(address,ss);
      address += sizeof(ss);
      EEPROM.put(address,m);
      saved = true;
    }
    screen.setCursor(3,1);
    screen.print("Comming Soon");  
    delay(3000);
    state = states[0];
  }
  else if (state == states[5]){
    screen.setCursor(3,1);
    screen.print("1. Hugo");  
    delay(3000);
    state = states[0];
  }
  else if (state == states[6]){
    screen.setCursor(0,1);
    screen.print("FRUU'FR2BLLF");  
    delay(3000);
    state = states[0];
  }
  else if (state == states[7]){
    screen.setCursor(3,1);
    screen.print("FUUUUUUUUUN");  
    delay(3000);
    state = states[0];
  }
}


void isButnPress() {
  if (state == states[0]) {
    if(digitalRead(butn_start_1) == HIGH) {
      if (choice == 3)
        choice = 0;
       else
        choice++;
       delay(500);
    }
    else if (digitalRead(butn_start_2) == HIGH) { 
      screen.clear();
      if (choice == 0)
        state = states[1];
      else if (choice == 1)
        state = states[6];
      else if (choice == 2)
        state = states[5];
      else if (choice == 3)
        state = states[7]; 
      delay(500);
    }  
  }
  if (state == states[1]) {
    if(digitalRead(butn_start_1) == HIGH && digitalRead(butn_start_2) == HIGH) { 
      screen.clear();
      screen.print("Go when you are ready !");
      while(!(digitalRead(butn_start_1) == LOW && digitalRead(butn_start_2) == LOW)) {
      }
      screen.clear();
      a = millis();
      state = states[2];
      delay(500);
    }  
  }
  else if(state == states[2]) {
    if(digitalRead(butn_start_1) == HIGH && digitalRead(butn_start_2) == HIGH) { 
      screen.clear();
      state = states[3];
      just_ended = true;
      delay(500);
    }
  }
  else if (state == states[3]) {
    if(digitalRead(butn_start_2) == HIGH) {
      screen.clear();
      state = states[0];
      delay(500);
    }
    else if (digitalRead(butn_start_1) == HIGH) {
      screen.clear();
      state = states[4];
      delay(500);
    }
  }
}

