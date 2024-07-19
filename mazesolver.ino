// To use PWM replace the digitalWrite with analogWrite
// But dont know if PWM pins will support 4 motors
#include <Arduino.h>


#define BUTTON 7
#define FrontIR 1
#define RightIR 2
#define LeftIR 3

#define FORWARD 83
#define BACKWARD 66
#define STOP 23


char mazeArray[200];
char shortA[200];
static int arrayit =0;
bool LearnToggle =0;
int m=0;
int mazeLength=0;

struct MotorPins{
int pin1;
int pin2;
};


int motorpins[][2] = {
  {4,5}, //front right : 0
  {6,7} //front left  : 1
  //{8,9}, // back right : 2
  //{10,11} // back left : 3
  // for four motor bot, can change to 2 wheel drive if required by commenting out some lines
};

void rotatemotor(int motorNumber, int motorDirection)
{ //// Make chnages here for PWM motor speed control. Or add delay loops with small intervals to mimic the pwm effect
  if (motorDirection == FORWARD)
  {
    analogWrite(motorpins[motorNumber][0], 150);
    analogWrite(motorpins[motorNumber][1], 0);    
  }
  else if (motorDirection == BACKWARD)
  {
    analogWrite(motorpins[motorNumber][0], 0);
    analogWrite(motorpins[motorNumber][1], 150);     
  }
  else
  {
    analogWrite(motorpins[motorNumber][0], 0);
    analogWrite(motorpins[motorNumber][1], 0);       
  }
}
void stop(){
  rotatemotor(0,23);
  rotatemotor(1,23);
}
void moveForward();

void turnLeft(){
rotatemotor(0,FORWARD); //front right
//rotatemotor(2,FORWARD); // back right
rotatemotor(1,BACKWARD); //front left
//rotatemotor(3,BACKWARD); //back left
delay(100); // add suitable delay after calibration
rotatemotor(0,STOP);
//rotatemotor(2,STOP);
rotatemotor(1,STOP);
//rotatemotor(3,STOP);
}

void turnRight(){
rotatemotor(0,BACKWARD); //front right
//rotatemotor(2,BACKWARD); // back right
rotatemotor(1,FORWARD); // front left
//rotatemotor(3,FORWARD); //back left
delay(100); // add suitable delay after calibration
rotatemotor(0,STOP);
//rotatemotor(2,STOP);
rotatemotor(1,STOP);
//rotatemotor(3,STOP);
}

void turnBack(){
rotatemotor(0,BACKWARD); //front right
//rotatemotor(2,BACKWARD); // back right
rotatemotor(1,FORWARD); // front left
//rotatemotor(3,FORWARD); //back left
delay(200); // add suitable delay after calibration
rotatemotor(0,STOP);
//rotatemotor(2,STOP);
rotatemotor(1,STOP);
//rotatemotor(3,STOP);
}

void moveForward(){
rotatemotor(0,FORWARD);
//rotatemotor(2,FORWARD);
rotatemotor(1,FORWARD);
//rotatemotor(3,FORWARD);
}


void executeShort(int front){
  while(front){
  moveForward();
  }
  for (int i=0; i<=m;i++){
  if(shortA[i]=='S'){while(front){moveForward();}}
  else if(shortA[i]=='R'){stop();turnRight();}
  else if(shortA[i]=='R'){stop();turnLeft();}
  else if(shortA[i]=='R'){stop();turnBack();}
  else if(shortA[i]=='F'){moveForward();delay(100);stop();}
  }
}

void pinInitialise(){
  for (int i=0; i< 2;i++){
    pinMode(motorpins[i][0],OUTPUT);
    pinMode(motorpins[i][1],OUTPUT);
  }
}


void calculatePath(char MAZE_ARRAY[], int SIZE_OF_ARRAY)
{
  /*ONCE THE ROBOT COMPLETES THE MAZE THE FINAL SHORTEST PATH CALCULATED
  IS STORED IN THE ROBOT MEMORY.THIS SHORTEST PATH IS USED TO COMPLETE
  THE SAME MAZE IN SHORTEST AMOUNT OF TIME.(L :LEFT, R:RIGHT, B:BACK,S:STRAIGHT)
  BELOW ARE THE FEW SUBSTITUTIONS TO CONVERT FULL MAZE PATH TO ITS 
  SHORTEST PATH:
  LBL = S
  LBR = B
  LBS = R
  RBL = B
  SBL = R
  SBS = B
  LBL = S */

  char ACTION;
 
  
  for(int i = 0; i <= SIZE_OF_ARRAY-2; i++)
    {
      ACTION = MAZE_ARRAY[i];
           
      if(ACTION == 'B')
        {
          if(MAZE_ARRAY[i-1]== 'L' && MAZE_ARRAY[i+1] == 'R')
            {
              MAZE_ARRAY[i] = 'B';
              MAZE_ARRAY[i-1] = 0;
              MAZE_ARRAY[i+1] = 0;
             // REARRANGE(MAZE_ARRAY,SIZE_OF_ARRAY,i-1,i,i+1); 
            }

           if(MAZE_ARRAY[i-1]== 'L' && MAZE_ARRAY[i+1] == 'S')
            {
              MAZE_ARRAY[i] = 'R';
              MAZE_ARRAY[i-1] = 0;
              MAZE_ARRAY[i+1] = 0;
              //REARRANGE(MAZE_ARRAY,SIZE_OF_ARRAY,i-1,i,i+1);
            }

            if(MAZE_ARRAY[i-1]== 'R' && MAZE_ARRAY[i+1] == 'L')
            {
              MAZE_ARRAY[i] = 'B';
              MAZE_ARRAY[i-1] = 0;
              MAZE_ARRAY[i+1] = 0;
              //REARRANGE(MAZE_ARRAY,SIZE_OF_ARRAY,i-1,i,i+1);         
            }

            if(MAZE_ARRAY[i-1]== 'S' && MAZE_ARRAY[i+1] == 'L')
            {
              MAZE_ARRAY[i] = 'R';
              MAZE_ARRAY[i-1] = 0;
              MAZE_ARRAY[i+1] = 0;
              //REARRANGE(MAZE_ARRAY,SIZE_OF_ARRAY,i-1,i,i+1);             
            }

            if(MAZE_ARRAY[i-1]== 'S' && MAZE_ARRAY[i+1] == 'S')
            {
              MAZE_ARRAY[i] = 'B';
              MAZE_ARRAY[i-1] = 0;
              MAZE_ARRAY[i+1] = 0;
             // REARRANGE(MAZE_ARRAY,SIZE_OF_ARRAY,i-1,i,i+1);             
            }

            if(MAZE_ARRAY[i-1]== 'L' && MAZE_ARRAY[i+1] == 'L')
            {
              MAZE_ARRAY[i] = 'S';
              MAZE_ARRAY[i-1] = 0;
              MAZE_ARRAY[i+1] = 0;
              //REARRANGE(MAZE_ARRAY,SIZE_OF_ARRAY,i-1,i,i+1);
            }
            
          i = -1;
        }
       
       delay(100);   
    }
  
  for(int i=0; i < SIZE_OF_ARRAY; i++){
    if(MAZE_ARRAY[i] != 0 && MAZE_ARRAY[i] != 'F'){shortA[m++]=MAZE_ARRAY[i];}
    if(MAZE_ARRAY[i] != 'F'){shortA[m]='F';break;}
  }
}

void setup() {
  // put your setup code here, to run once:
pinInitialise();
pinMode(FrontIR,INPUT);
pinMode(RightIR,INPUT);
pinMode(LeftIR, INPUT);
pinMode(BUTTON, INPUT);



int mazeLength =0;
while(digitalRead(FrontIR)){
  moveForward();
}

}
static char lastop = 'S';
void loop() {
  // put your main code here, to run repeatedly:
  int right = digitalRead(RightIR);
  int left = digitalRead(LeftIR); //not gonna be used
  int front = digitalRead(FrontIR);


  int toggle = digitalRead(BUTTON);
  if(toggle == 1){ LearnToggle = !LearnToggle;}
  if(LearnToggle == 1){
    executeShort(front);
  }
  else{


  if (right==0 && front ==1){ //need to see when ir sensor gives postive output
    moveForward();
    char currentop = 'S';
    if(lastop != currentop){
    mazeArray[arrayit++]=currentop; lastop=currentop;}

  }
  else if(right ==0 && front ==0&& left == 1){
    turnLeft();
    char currentop = 'L';
    if(lastop != currentop){
    mazeArray[arrayit++]=currentop; lastop=currentop;}
  }
  else if (front==0 && right ==1){
    turnRight();
    char currentop = 'R';
    if(lastop != currentop){
    mazeArray[arrayit++]=currentop; lastop=currentop;}
  }
  else if (right ==0 && front ==0 && left == 0){
    turnBack();
    char currentop = 'B';
    if(lastop != currentop){
    mazeArray[arrayit++]=currentop; lastop=currentop;}
  }
  else if(right ==1 && front ==1 && left == 1){
    mazeArray[arrayit++]='F';
    for(int t=0; t<200; t++){
      if (mazeArray[t]=='F'){mazeLength = t+1; break;}
    }
    calculatePath(mazeArray,mazeLength);

  }

}
}
