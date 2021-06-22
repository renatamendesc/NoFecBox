#include <LiquidCrystal.h>
#include <Stepper.h>

#define STEPS_BY_TURN 65

#define ECHO_ALCOHOL 3
#define TRIG_ALCOHOL 2

#define ECHO_MASKS 53
#define TRIG_MASKS 51

#define BUTTON 6

#define IN1 11
#define IN2 10 

Stepper myStepper(STEPS_BY_TURN, 28, 24, 26, 22); // Initialize masks motor
LiquidCrystal LCD (45,43,41,39,37,35,33);

int ballsCounter = 0;
unsigned long int timeAlcohol = 0; // Keeps the time between starting arduino and last activity from alcohol motor
unsigned long int timeMasks = 0; //  Keeps the time between starting arduino and last activity from masks motor

int distance(int trig, int echo){ // Verifies distance between sensor and hand
    
    digitalWrite(trig, LOW);
    delay(10);

    digitalWrite(trig, HIGH);
    delayMicroseconds(10);
    digitalWrite(trig, LOW);

    int timing = pulseIn(echo, HIGH);
    int distance = timing/58;

    delay(500);
    
    return distancia;
}

void setup(){
  
    Serial.begin(9600);
    myStepper.setSpeed(300); // Masks motor speed
    LCD.begin(16,2);
    
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(ECHO_ALCOHOL, INPUT);
    pinMode(TRIG_ALCOHOL, OUTPUT);
    pinMode(ECHO_MASKS, INPUT);
    pinMode(TRIG_MASKS, OUTPUT);
}

void loop(){
   refill();
   
   if(ballsCounter == 0){
    
      LCD.clear();
      LCD.setCursor(1,0);
      LCD.print("Sem máscaras!");
      
    }else{
      
      LCD.clear();
      LCD.setCursor(4,0);
      LCD.print("NoFecBox");
      LCD.setCursor(0,1);
      LCD.print("Seguranca sempre");
      masksDispenser();
      
    }
    
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, HIGH); 
    alcoholDispenser(); 
    
}

void alcoholDispenser(){
   
    if(distance(TRIG_ALCOHOL, ECHO_ALCOHOL) <= 12 && millis() - timeAlcohol >= 3000){ // Turns on alcohol motor
      LCD.clear();
      LCD.setCursor(2,0);
      LCD.print("Protejam-se!");
      
      // Serial.println(distance(TRIG_ALCOHOL, ECHO_ALCOHOL));
      
      digitalWrite(IN2, LOW);
      delay(750);
      
      // Turns off motor
      digitalWrite(IN2, HIGH);
      delay(100);
      
      timeAlcohol = millis(); // Registra o tempo da última atividade do motor
    }
    
    delay(10);
}

void masksDispenser(){
  
    if(distance(TRIG_MASKS, echoM) <= 12 && millis() - timeMasks >= 3000){
      
      ballsCounter--;
      
      /*
      if(ballsCounter <= 0){
        ballsCounter = 0;
      }
      */
      
      Serial.println("Mascara ok!!");
      
      LCD.clear();
      LCD.setCursor(2,0);
      LCD.print("Use Mascara!");
      
      Serial.print("Bolas: ");
      Serial.println(ballsCounter);
      
      for(int i = 0; i < 5; i++){
          myStepper.step(-STEPS_BY_TURN);
      }
    
      delay(100);
      timeMasks = millis();
      
    }
    
    delay(10);
}

void refill(){
  
  if(digitalRead(BUTTON)){
    ballsCounter++;
    Serial.print("Bolas: ");
    Serial.println(ballsCounter);
    delay(1);
  }
  
  if(ballsCounter > 7){
    ballsCounter = 7;
  }
  
  Serial.print("Bolas: ");
  Serial.println(ballsCounter);
}
