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

const String shuffles[12] = {'F', 'R', 'L', 'U', 'D', 'B', 'f', 'r', 'l', 'u', 'd', 'b'};

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

void affichage() {
  screen.setCursor(0, 0);
  
  /********************************/
  /*          menu                */
  /********************************/
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
  
  /********************************/
  /*       wait_for_start         */
  /********************************/
  if(state == states[1]){
    screen.print("Place tes mains sur");  
    screen.setCursor(0,3);
    screen.print("les bouttons !");
  }
  
  /********************************/
  /*            running           */
  /********************************/
  else if (state == states[2]){
    screen.setCursor(6,1);
    screen.print(String(mm) + ":" + String(ss) + ":" + String(m));  
  }
  
  /********************************/
  /*          stoped              */
  /********************************/
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
  
  /********************************/
  /*           saving             */
  /********************************/
  else if (state == states[4]){
	  // TODO
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
  
  /********************************/
  /*           leaderboard        */ 
  /********************************/
  else if (state == states[5]){
	  // TODO
    screen.setCursor(3,1);
    screen.print("1. Hugo");  
  }
  
  /********************************/
  /*          melanger            */
  /********************************/
  else if (state == states[6]){
	  // TODO : Is it good ?
	string str = getShuffleString(15);
    screen.setCursor(0,1);
    screen.print(str);
  }
  
  /********************************/
  /*           fun                */
  /********************************/
  else if (state == states[7]){
    screen.setCursor(3,1);
    screen.print("FUUUUUUUUUN");
  }
}

void isButnPress() {
  /********************************/
  /*          menu                */
  /********************************/
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
  
  /********************************/
  /*       wait_for_start         */
  /********************************/
  if (state == states[1]) {
    if(digitalRead(butn_start_1) == HIGH && digitalRead(butn_start_2) == HIGH) { 
      screen.clear();
      screen.print("Commence quand tu es pret !");
      while(!(digitalRead(butn_start_1) == LOW && digitalRead(butn_start_2) == LOW)) {
      }
      screen.clear();
      a = millis();
      state = states[2];
      delay(500);
    }  
  }
  
  /********************************/
  /*            running           */
  /********************************/
  else if(state == states[2]) {
    if(digitalRead(butn_start_1) == HIGH && digitalRead(butn_start_2) == HIGH) { 
      screen.clear();
      state = states[3];
      just_ended = true;
      delay(500);
    }
  }
  
  /********************************/
  /*          stoped              */
  /********************************/
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
  
  /********************************/
  /*           saving             */
  /********************************/
  else if (state == states[4]){
	// TODO
  }
  
  /********************************/
  /*           leaderboard        */ 
  /********************************/
  else if (state == states[5]){
	if(digitalRead(butn_start_1) == HIGH || digitalRead(butn_start_2) == HIGH) { 
      screen.clear();
      state = states[0];
      delay(500);
    }
  }
  
  /********************************/
  /*          melanger            */
  /********************************/
  else if (state == states[6]) {
	if(digitalRead(butn_start_1) == HIGH || digitalRead(butn_start_2) == HIGH) { 
      screen.clear();
      state = states[0];
      delay(500);
    }  
  }
  
  /********************************/
  /*           fun                */
  /********************************/
  else if (state == states[7]){
    if(digitalRead(butn_start_1) == HIGH || digitalRead(butn_start_2) == HIGH) { 
      screen.clear();
      state = states[0];
      delay(500);
    }
  }
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


string getShuffleString(int len) {
  
  string strShuffle = "";
  for (int i = 0; i < len; i++) {
    int randNumber = random(12);
    strShuffle += shuffles[randNumber];
  }
  return strShuffle;
}

