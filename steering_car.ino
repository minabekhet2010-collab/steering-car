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

int direction_button = 2;
int print_button = A2;

float distance_front;
float distance_back;
volatile bool reverse = false;
String state_car;
float velocity;
int rpm;

float ultrasonic(int trig,int echo) {
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
  }
}
void stop(int mapped_speed){
  for(int i=mapped_speed; i>0; i--){
  	analogWrite(ENA1, i);
    analogWrite(ENA2, i);
  }
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}
void printbutton(){

  lcd.setCursor(4, 0);
  lcd.print("velocity");
  lcd.setCursor(5, 1);
  lcd.print(velocity);
  delay(1500);
  lcd.clear();
  lcd.setCursor(4, 0);
  lcd.print("car state");
  lcd.setCursor(5, 1);
  lcd.print(state_car);
  delay(1500);
  lcd.clear();
  lcd.setCursor(7, 0);
  
 lcd.print("RPM");
 lcd.setCursor(7, 1);
 lcd.print(rpm);
 delay(1500);
  lcd.clear();
}
void change_direction(){
 unsigned long now = millis();
 static long lastint = 0;
  if(now - lastint > 200){
    reverse = !reverse;
 }
 lastint = now;
}
void setup() {
  // put your setup code here, to run once:
 Serial.begin(9600);
 lcd.begin(16,2);
 lcd.init();
 lcd.backlight();
 lcd.clear();
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
  attachInterrupt(digitalPinToInterrupt(direction_button), change_direction, FALLING);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(digitalRead(print_button)==LOW){
    printbutton();
  }
int pot_direction_reading= analogRead(pot_direction);
int servo_angle = map(pot_direction_reading,0,1023 ,0,180);
ourservo.write(servo_angle);
int pot_speed_reading = analogRead(pot_speed);
int mapped_speed = map(pot_speed_reading ,0,1023 ,0,255);
rpm=map(pot_speed_reading,0,1023 ,0,600);
  
  velocity=2*3.14*3.25*(rpm/60.0);
  distance_front= ultrasonic(trigfront,echofront);
  distance_back= ultrasonic(trigback,echoback);

Serial.println(digitalRead(direction_button));

if (distance_front<=30){
      stop(mapped_speed);
  state_car = "stop";
    delay(500);
  backward(mapped_speed);
    state_car = "backward";
      delay(1000);
 forward(mapped_speed);
    state_car = "forward";
}
  else if(distance_back <= 30){
  stop(mapped_speed);
  state_car = "stop";
    delay(500);
     forward(mapped_speed);
    state_car = "forward";
      delay(1000);
    backward(mapped_speed);
    state_car = "backward";

}
  
else{
  if(reverse == false){
    forward(mapped_speed);
    state_car = "forward";
  }
  else {
   backward(mapped_speed);
   state_car = "backward";
  }
 }
}