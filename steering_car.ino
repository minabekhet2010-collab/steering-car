#include <Servo.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);
int servo = 11;
Servo ourservo;
int pot_speed = A0;
int pot_direction = A1;
int echofront = 13;
int trigfront = 4;

int echoback = 6;
int trigback = 7;
int ENA1 = 3;
int in1 = 8;
int in2 = 9;

int ENA2 = 5;
int in3 = 10;
int in4 = 12;

int direction_button = 3;
int print_button = 2;

float distance_front;
float distance_back;
bool reverse=false;
volatile bool print_reqouest=false;

float ultra(int trig,int echo) {
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(2);
  digitalWrite(trig, LOW);
  long time = pulseIn(echo, HIGH);
  float distance = (time * 0.0343) / 2;
  return distance;
}
void forward(int mapped_speed) {
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  for(int i=0; i<mapped_speed; i++){
  	analogWrite(ENA1, i);
    analogWrite(ENA2, i);
    delay(20);
  }
}
void backward(int mapped_speed) {
  digitalWrite(in2, HIGH);
  digitalWrite(in1, LOW);
  digitalWrite(in4, HIGH);
  digitalWrite(in3, LOW);
  for(int i=0; i<mapped_speed; i++){
  	analogWrite(ENA1, i);
    analogWrite(ENA2, i);
    delay(20);
  }
}
void stop(int mapped_speed){
  for(int i=mapped_speed; i>0; i--){
  	analogWrite(ENA1, i);
    analogWrite(ENA2, i);
    delay(20);
  }
  digitalWrite(in1, HIGH);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, HIGH);
}
void setup() {
  // put your setup code here, to run once:
 Serial.begin(9600);
 lcd.begin(16,2);
 lcd.init();
 lcd.backlight();
  ourservo.attach(servo);
  pinMode(echofront, INPUT);
  pinMode(echoback, INPUT);
  pinMode(trigfront, OUTPUT);
  pinMode(trigback, OUTPUT);
  pinMode(ENA1, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(ENA2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(direction_button, INPUT_PULLUP);
  pinMode(print_button, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(direction_button), onInterrupt, FALLING);
}

void loop() {
  // put your main code here, to run repeatedly:


int pot_speed_reading = analogRead(pot_speed);
int mapped_speed = map(pot_speed_reading ,0,1023 ,0,255);
analogWrite(ENA1,mapped_speed);
analogWrite(ENA2,mapped_speed);
int rpm=map(pot_speed_reading,0,1023 ,0,600);
  
float velocity=2*3.14*3.25*(rpm/60.0);
lcd.clear();
  distance_front= ultra(trigfront,echofront);
  distance_back= ultra(trigback,echoback);

int pot_direction_reading= analogRead(pot_direction);
int servo_angle = map(pot_direction_reading,0,1023 ,0,180);
ourservo.write(servo_angle);
if(digitalRead(direction_button)==LOW){
  
 reverse =!reverse;
  delay(50);
 }
if(distance_front<=30){
  stop(mapped_speed);
  delay(500);
   String stop_moving = "stop";
backward(mapped_speed);
  delay(1000);
   String move_backward = "backward";
   forward(mapped_speed);
      String move_forward = "forward";
}
  else if(distance_back <= 30){
   stop(mapped_speed);
      String stop_moving;
  forward(mapped_speed);
  delay(500);
      String move_forward = "forward";
  backward(mapped_speed);
     String move_backward ;
  }
if(reverse==false){
 forward(mapped_speed) ;
 String move_forward;
}

else{
 backward(mapped_speed);
   String move_backward;
}





     if ( print_reqouest== true)
{
   print_reqouest= false;
printbutton;
}

}



void printbutton(float velocity,String move_forward,String move_backward,String stop_moving,int rpm){

 if (digitalRead(print_button) == LOW){
    lcd.setCursor(4, 0);
  lcd.print("velocity");
      lcd.setCursor(4, 1);
  lcd.print(velocity);
  delay(2500);
  lcd.clear();
    lcd.setCursor(4, 0);
 lcd.print("car state");
       lcd.setCursor(4, 1);
 lcd.print(stop_moving);
 lcd.print(move_backward);
 lcd.print(move_forward);
delay(2500);
  lcd.clear();
      lcd.setCursor(7, 0);
    lcd.setCursor(7, 1);
 lcd.print("rpm");
 lcd.print(rpm);
 }
}
void dirbutton(){
           if(digitalRead(direction_button)==LOW){
 reverse !=reverse;
 }
           }
           


           
void printInterrupt(volatile bool print_reqouest)
{
  print_reqouest= true;
} 
