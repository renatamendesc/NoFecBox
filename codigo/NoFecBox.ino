#include <LiquidCrystal.h>
#include <Stepper.h> //Biblioteca do motor da máscara

#define PassosPorVolta 65 //Número de passos por volta

#define but 6 // Botão contador

#define echoA 3 //Echo do alcool
#define trigA 2 //Trig do alcool

#define echoM 53//Echo da mascara50
#define trigM 51 //Trig da mascara52

#define IN1 11 //Entradas do motor do alcool 46
#define IN2 10 

Stepper myStepper(PassosPorVolta, 28, 24, 26, 22); //Inicializa o motor da máscara
LiquidCrystal LCD (45,43,41,39,37,35,33);

int contador_bolas = 0;
unsigned long int tempoPassadoA = 0; //Armazena o período entre o inicio do Arduino até a última atividade do motor do alcool
unsigned long int tempoPassadoM = 0; //Armazena o período entre o inicio do Arduino até a última atividade do motor do alcool

int distanciaCm(int trig, int echo){ //Função que verifica a distância entre o sensor e a mão
    digitalWrite(trig, LOW);
    delay(10);

    digitalWrite(trig, HIGH);
    delayMicroseconds(10);
    digitalWrite(trig, LOW);

    int tempo = pulseIn(echo, HIGH);
    int distancia = tempo/58;

    delay(500);
    return distancia;
}

void setup(){
    Serial.begin(9600);

    myStepper.setSpeed(300); //Velocidade do motor da máscara
    LCD.begin(16,2);
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(echoA, INPUT);
    pinMode(trigA, OUTPUT);
    pinMode(echoM, INPUT);
    pinMode(trigM, OUTPUT);
}

void loop(){
    refil();
   if(contador_bolas == 0){
      LCD.clear();
      LCD.setCursor(1,0);
      LCD.print("Sem Mascaras!");
    }
    else{
      LCD.clear();
      LCD.setCursor(4,0);
      LCD.print("Nofecbox");
      LCD.setCursor(0,1);
      LCD.print("Seguranca sempre");
      maskdispenser();
    }
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, HIGH); 
    alcooldispenser(); 
    
}

void alcooldispenser(){ //Função que verifica se teve alguma atividade nos últimos 3 segundos e algum objeto a 12 cm de distância 
    if(distanciaCm(trigA, echoA) <= 12 && millis() - tempoPassadoA >= 3000){ //Quando as condições acima descritas são aceitas, liga o motor do álcool
      LCD.clear();
      LCD.setCursor(2,0);
      LCD.print("Protejam-se!");
      
      //Serial.println(distanciaCm(trigA, echoA));
      //Gira o Motor no sentido horario
      digitalWrite(IN2, LOW);
      delay(750);
      //Desliga o motor
      digitalWrite(IN2, HIGH);
      delay(100);
      tempoPassadoA = millis();// Registra o tempo da última atividade do motor
    }
     delay(10);
}

void maskdispenser(){ //Função que verifica se teve alguma atividade nos últimos 3 segundos e algum objeto a 12 cm de distância 
    if(distanciaCm(trigM, echoM) <= 12 && millis() - tempoPassadoM >= 3000){ //Quando as condições acima descritas são aceitas
      contador_bolas--;
      /*if(contador_bolas <= 0){
        contador_bolas = 0;
      }*/
      Serial.println("Mascara ok!!");
      LCD.clear();
      LCD.setCursor(2,0);
      LCD.print("Use Mascara!");
      Serial.print("Bolas: ");
      Serial.println(contador_bolas);
      
      for(int i = 0; i < 5; i++){ //Laço for controla o tempo em que o motor irá girar, ainda nao decidido
          myStepper.step(-PassosPorVolta); //Gira o Motor no sentido horario
      }
    
      delay(100);
      tempoPassadoM = millis(); //Registra o tempo da última atividade do motor
    }
     delay(10);
}

void refil(){
  if(digitalRead(but)){
    contador_bolas++;
    Serial.print("Bolas: ");
    Serial.println(contador_bolas);
    delay(1);
  }
  if(contador_bolas > 7){
    contador_bolas = 7;
  }
  Serial.print("Bolas: ");
  Serial.println(contador_bolas);
}
