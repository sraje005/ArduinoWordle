#include <LiquidCrystal.h>

// Array of Output Pin variables, set to the pins being used
const int rs = 4, en = 5, d4 = 8, d5 = 9, d6 = 10, d7 = 11;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);


const int xAxis = A0; // Recall that A0 is really a number under the hood.
const int light_pattern_size = 2;
const char light_pattern[light_pattern_size] = {0x00, 0x0F};

const char sound_pattern_size = 2;
const double sound_pattern[sound_pattern_size] = {261.63, 293.66}; // C4, D4

const int buzzer = 3;

const int xAxis_median = 500; // Adjust this if needed

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool buttonheld;
const int yAxis_median = 500; //ADDED
const int yAxis = A1; //ADDED
bool shift=false;
int percounter=0;
bool resettrigger;
String functoutput;
int row=0;
int col=0;
bool cleared=false;

bool first=false;
bool full=false;
String pos1="_";
String pos2="_";
String pos3="_";
String pos4="_";
String pos5="_";

String m1=" "; //if they are matches
String m2=" ";
String m3=" ";
String m4=" ";
String m5=" ";

String pot1=" ";
String pot2=" ";
String pot3=" ";
String pot4=" ";

int p1=0;
int p2=0;
int p3=0;
int p4=0;
int guessnum=6;
int guessspot=0;
String guesses[5] {pos1,pos2,pos3,pos4,pos5};
String alpha[2][13]={{"A","B","C","D","E","F","G","H","I","J","K","L","M"},{"N","O","P","Q","R","S","T","U","V","W","X","Y","Z"}};
int selectedpos=0;
bool pickdelay=false;
bool transition=false;
int counter=0;
String compstring;

String words[10]={"LIGHT","MIGHT","SIGHT","BRIGHT","CHASE","YACHT","CRATE","SEDAN","STEAM", "CREAM"};
int pick=random(0,10);
String teststring=words[pick];
bool match=false;
bool lose=false;
double winsounds[3]={400.00,500.00,600.00};
double losesounds[3]={600.00,500.00,400.00};
int reps=0;
int wait=0;
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
typedef struct task {
  int state;
  unsigned long period;
  unsigned long elapsedTime;
  int (*TickFct)(int);
    
} task;

int delay_gcd;
const unsigned short tasksNum = 2; // the number of state machines
task tasks[tasksNum];

enum SM1_States {GUESS,PICK, ENTER,WIN,LOSE,RESET};
int SM1_Tick(int state){
    //Read thing
    switch(state){ // State transitions
    
      case GUESS:
          if (digitalRead(2)==LOW&&transition){
            state = PICK;
            //col=0;
            //row=0;
            lcd.clear();
            lcd.setCursor(0,0);
          }
          else{
            state = GUESS;
          }

          
          if(!cleared) //THIS ONLY HAPPENS ONCE, IMMEDIATELY AFTER A TRIGGER, MAKE SURE TO RESET IT
          {
            lcd.clear();
           lcdsetup();
          }

          if(col==5&&row==0&& digitalRead(2)==LOW)
          {
            state=ENTER;
          }
          
          
         //State Transition
        break;
      case PICK:
               // if(transition==true) state=GUESS;
                //else state=PICK;
           //Serial.println("made it to pick");
             //State Transition
        break;



        
    case ENTER:
      if(match)state=WIN;
      if(!match) state=GUESS;
      if(lose) state=LOSE;
     break;






        
      case WIN:
      if(digitalRead(2)==LOW&&wait>2) state=RESET;
      wait++;
      
        break;  
      case LOSE:
      if(digitalRead(2)==LOW&&wait>2) state=RESET;
      wait++;
        break;
      case RESET:
      state=GUESS;
      break;  
    }
    


//ACTIONS-----------------------------------------------------------------
    switch(state){ // State Action
     
      case GUESS:
            //State Action
            lcd.blink();
            lcd.clear();
             lcdsetup(); //these two were added

             
            jsoutput();
            lcd.setCursor(col,row);
            guessspot=col;
            transition=true;
            Serial.print(teststring);
         //State Action
        break;


        
      case PICK:
            //State Action
            //transition=false;
            if(!first)                  //THIS JUST DISPLAYS WHAT WAS SELECTED
            {
              Serial.println(guessspot);
              first=true;
              Serial.print("row: ");
              Serial.println(row);
              Serial.print("col: ");
              Serial.println(col);
              //col=0;
              //row=0;
            }


            
          lcd.clear();
          guesssetup();
          jsoutput();       
          lcd.setCursor(col,row);
          if(digitalRead(2)==LOW&&counter>3)
          {
            guesses[guessspot]=alpha[row][col];
            Serial.print("selected letter: ");
            Serial.println(alpha[row][col]);
            Serial.println("the outputs: ");
            for(int rep=0;rep<5;rep++)
            {
              Serial.println(guesses[rep]);
            }
            transition=true;
            first=false;
            lcd.clear();
            state=GUESS;
            counter=0;
          }
          pickdelay=true;
          //Serial.println("in the pick actions");
         //State Action
        // transition=true;
        counter++;
        break;




        
     case ENTER:
     guessnum--;
     for(int rep=0;rep<5;rep++)
     {
      compstring+= guesses[rep];
     }
     Serial.print("the string that is going to be compared");
     Serial.println(compstring);
     Serial.print("the test string (answer)");
     Serial.println(teststring);
     if(compstring==teststring)
     {
      match=true;
      
     }
     else // IF THE STRINGS DONT MATCH
     {
      Serial.println("THE STRINGS DONT MATCH");
      if(compstring.substring(0,1)==teststring.substring(0,1))
      {
        m1=compstring.substring(0,1);
        Serial.println("m1 updated");
      }
      if(compstring.substring(1,2)==teststring.substring(1,2))
      {
        m2=compstring.substring(1,2);
         Serial.println("m2 updated");
      }
      if(compstring.substring(2,3)==teststring.substring(2,3))
      {
         Serial.println("m3 updated");
        m3=compstring.substring(2,3);
      }
      if(compstring.substring(3,4)==teststring.substring(3,4))
      {
         Serial.println("m4 updated");
        m4=compstring.substring(3,4);
      }
      if(compstring.substring(4,5)==teststring.substring(4,5))
      {
        Serial.println("m5 updated");
        m5=compstring.substring(4,5);
      }
        
      if(teststring.indexOf(compstring.substring(0,1))!=-1)
     {
      pot1=compstring.substring(0,1);
      p1++;
     }
     if(teststring.indexOf(compstring.substring(1,2))!=-1)
     {
      pot2=compstring.substring(1,2);
      p2++;
     }
     if(teststring.indexOf(compstring.substring(2,3))!=-1)
     {
      pot3=compstring.substring(2,3);
      p3++;
     }
     if(teststring.indexOf(compstring.substring(3,4))!=-1)
     {
      pot4=compstring.substring(3,4);
      p4++;
     }
      match=false;
     }
     if(guessnum==0)lose=true;
     compstring=""; //THIS MAY BREAK SOMETHING
     break; 

     
     case WIN:
     
     lcd.clear();
     lcd.print("CORRECT: ");
     lcd.print(teststring);
     break;
     
     case LOSE:
     lcd.clear();
     lcd.print("Correct: ");
     lcd.print(teststring);
     lcd.setCursor(0,1);
     lcd.write("Fail");
     break;
  case RESET:
    match=false;
    lose=false;
    pos1="_";
    pos2="_";
    pos3="_";
    pos4="_";
    pos5="_";
     break;
    }
    
    return state;
}
//-------------------------------------------------------------------------------------------------SM2----------------------------------------------------------------------------------------------
enum SM2_States {WAIT, WON, LOST};
int SM2_Tick(int state){
    //Read thing
    switch(state){ // State transitions
     case WAIT:
     if(match) state=WON;
     if(lose) state=LOST;
     break;
      case WON:
      if(digitalRead(2)==LOW&&wait>1) state=WAIT;
            //state = OFF;
        //State Transition
        break;
      case LOST:
      if(digitalRead(2)==LOW&&wait>1) state=WAIT;    
        break;
    }
    switch(state){ // State Action
      case WAIT:
      noTone(buzzer);
      break;
      case WON:
            //State Action
           tone(buzzer,winsounds[reps]);
           reps++;
            if(reps>2) reps=0;
         //State Action
        break;
      case LOST:
            //State Action
            tone(buzzer,losesounds[reps]);
           reps++;
             if(reps>2) reps=0;
           
         //State Action
        break;
    }

    return state;
}
//--------------------------------------------------------------------------------------------------------------fucntion to generate the joystick outputs-------------------------------------------------------------------------------------------------
void jsoutput()
{

  if (analogRead(xAxis) < xAxis_median-200&&buttonheld==false){ //RIGHT STICK
                
                buttonheld=true;
               
               // return "a";
               col++;
            }
  if (analogRead(xAxis) > xAxis_median+200&&buttonheld==false ){ //LEFT STICK
               
                buttonheld=true;
              
               // return "b";
               col--;
            }
   if (analogRead(yAxis) > yAxis_median +200&&buttonheld==false ){ //UP STICK
              
               buttonheld=true;
              row--;
               // return "c";
            }
 if (analogRead(yAxis) < yAxis_median-200&&buttonheld==false ){ //DOWN STICK
               
               buttonheld=true;
              row++;
               //return "d";
              // Serial.print("moved down a row");
            }
if(col>15)col=0;
if(col<0)col=15;
if(row>1) row=0;
if(row<0) row=1;
  else if((analogRead(yAxis) < yAxis_median +200) && (analogRead(yAxis) > yAxis_median-200) && (analogRead(xAxis) < xAxis_median+200) && (analogRead(xAxis) > xAxis_median-200))
  {
    
    buttonheld=false;
    
  }

}

//-------------------------------------------------------------------------------------------------------------fnction to setup the lcd-------------------------------------------------------------------------------------------
void lcdsetup()
{
  for(int rep=0;rep<5;rep++) //this shoud print the guesses
            {
             lcd.print(guesses[rep]);
            }
            lcd.print("*");
            //lcd.print(" :   :   "); //THIS WILL NEED TO BE MODIFIED TO BE ABLE TO UPDATE
            
            lcd.print(pot1);
            lcd.print(":");
            lcd.print(p1);
            lcd.print(" ");
            lcd.print(pot2);
            lcd.print(":");
            lcd.print(p2);
            lcd.print(" ");
            
            lcd.print(guessnum);
            lcd.setCursor(0,1);
            lcd.print(m1);
            lcd.print(m2);
            lcd.print(m3);
            lcd.print(m4);
            lcd.print(m5);
            lcd.setCursor(6,1);
            //lcd.print(" :   :  ");
            
            lcd.print(pot3);
            lcd.print(":");
            lcd.print(p3);
            lcd.print(" ");
            lcd.print(pot4);
            lcd.print(":");
            lcd.print(p4);
            lcd.print(" ");
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void guesssetup()
{
  lcd.print("ABCDEFGHIJKLM");
  lcd.setCursor(0,1);
  lcd.print("NOPQRSTUVWXYZ");
  lcd.setCursor(0,0);
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void setup() {
    //some set up (Default Arduino Function)

    // LEDs
   randomSeed(analogRead(A5));
    Serial.begin(9600);

  pinMode(2, INPUT_PULLUP);
  unsigned char i = 0;
  tasks[i].state = GUESS;
  tasks[i].period = 100; //this has been changed
  tasks[i].elapsedTime = 0;
  tasks[i].TickFct = &SM1_Tick;
  i++;
  tasks[i].state = WAIT;
  tasks[i].period = 500;
  tasks[i].elapsedTime = 0;
  tasks[i].TickFct = &SM2_Tick;

  delay_gcd = 500; // GCD
  lcd.begin(16,2);

  lcd.print("WE LOVE");
}


void loop() {
  // put your main code here, to run repeatedly:
  unsigned char i;
  for (i = 0; i < tasksNum; ++i) {
     if ( (millis() - tasks[i].elapsedTime) >= tasks[i].period) {
        tasks[i].state = tasks[i].TickFct(tasks[i].state);
        tasks[i].elapsedTime = millis(); // Last time this task was ran
      
     }
   }
}
