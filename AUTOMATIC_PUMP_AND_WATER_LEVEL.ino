#include <LiquidCrystal.h>
#define length 16.0
#include <Servo.h>

// Declaring the variables that we are going to use
double percent=100.0;
unsigned char b;
unsigned int peace;
int max_distance = 0;
int calc_distance = 0;
int Button_state = 0;
int count = 0;

Servo servo_9;                          // setting up the servo
LiquidCrystal LCD(12, 11, 5, 4, 3, 2);  // setting pins for LCD screen

// Setting up the screen
byte p1[8] = {
  0x10,
  0x10,
  0x10,
  0x10,
  0x10,
  0x10,
  0x10,
  0x10};
byte p2[8] = {
  0x18,
  0x18,
  0x18,
  0x18,
  0x18,
  0x18,
  0x18,
  0x18};
byte p3[8] = {
  0x1C,
  0x1C,
  0x1C,
  0x1C,
  0x1C,
  0x1C,
  0x1C,
  0x1C};
byte p4[8] = {
  0x1E,
  0x1E,
  0x1E,
  0x1E,
  0x1E,
  0x1E,
  0x1E,
  0x1E};
byte p5[8] = {
  0x1F,
  0x1F,
  0x1F,
  0x1F,
  0x1F,
  0x1F,
  0x1F,
  0x1F};

// ultrasonic sensor (trigger = emitting , echopin = recieve)
long readUltrasonicDistance(int triggerPin, int echoPin)
{
  pinMode(triggerPin, OUTPUT);   // Setting up the triggerpin to give output
  
  // Sets the trigger pin to LOW state for 2 microseconds
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  
  // Sets the trigger pin to HIGH state for 10 microseconds
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  
  // Sets the trigger pin back to LOW state
  digitalWrite(triggerPin, LOW);
  
  pinMode(echoPin, INPUT);       // Setting up the echopin to give input
  
  // Reads the echo pin, and returns the sound wave travel time in microseconds
  return pulseIn(echoPin, HIGH);
}

//setting up the pins
void setup()
{
  pinMode(6, INPUT);
  pinMode(8, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(13, OUTPUT);
  LCD.createChar(0, p1);
  LCD.createChar(1, p2);
  LCD.createChar(2, p3);
  LCD.createChar(3, p4);
  LCD.createChar(4, p5);
  LCD.begin(16, 2);    
  digitalWrite(10, HIGH); 
  digitalWrite(13, LOW);

}

// Loop so that we can take real time values
void loop()
{
  // v = d/t
  calc_distance = 0.01723 * readUltrasonicDistance(7, 7); // d = v*t
  Button_state = digitalRead(6);  // To check if the switch is on or not
  max_distance = 336;   // Max range of the sensor
  percent = (calc_distance/336.0)*100.0;
  
  if (Button_state == HIGH){
    digitalWrite(10, LOW); 
    digitalWrite(13, HIGH);  
    servo_9.attach(9);
    count++;
    
    // Initial message on screen
    if (count == 1){
      delay(100); 
      LCD.setCursor(0,0);
      LCD.print("                ");
      LCD.setCursor(0,1);
      LCD.print("                ");
      delay(100);
      LCD.setCursor(0, 0);
      LCD.print("POWER ON");
      LCD.setCursor(0, 1);
      LCD.print("VALVE & PUMP");
      delay(100);
    }
    
    // when capacity is low the pump is turned on
    if (calc_distance > max_distance-100) {
      if (count ==1){
        delay(1000);
      }
      servo_9.write(90);   // Rotates the servo in 90 degree
      if (count ==1){
        delay(1000);
      }
      digitalWrite(8, HIGH);  //powers the dc motor and led 
      digitalWrite(9, HIGH);  //powers the servo
    }
    
    // when capacity is full the water pump stops
    if (calc_distance < max_distance-319) {
      if (count ==1){
        delay(1000);
      }
      digitalWrite(8, LOW);  // dc motor turned off
      if (count ==1){
        delay(1000);
      }
      servo_9.write(0);  // servo is moved back to initial position
    }
    
    // Capacity displayed in screen
    if (count ==1){
      delay(1000);
    }
    LCD.setCursor(0,0);
    LCD.print("Capacity:");
    LCD.print(100 - percent);
    LCD.print(" %");
    LCD.setCursor(0,1);
    double a=length/100*(100-percent); 
    
   // drawing black rectangles on LCD
    if (a>=1) {
      for (int i=1;i<a;i++) {
        LCD.write(4);
        b=i;
      }
      a=a-b;
    }
    peace=a*5;
    
  // drawing charater's column
    switch (peace) {
    case 0:
      break;
    case 1:
      LCD.print(char(0));
      break;
    case 2:
      LCD.write(1);
      break;
    case 3:
      LCD.write(2);
      break;
    case 4:
      LCD.write(3);
      break;
    }
  //clearing line
    for (int i =0;i<(length-b);i++) {
      LCD.print(" ");
    }
    ;
  }
  
  // Switches off everything when switch is off
  if (Button_state == LOW){
    LCD.setCursor(0, 0);
    LCD.print("                ");
    LCD.setCursor(0, 1);
    LCD.print("                ");
    delay(100);
    LCD.setCursor(0, 0);
    LCD.print("POWER OFF");
    LCD.setCursor(0, 1);
    LCD.print("VALVE & PUMP");
    delay(1000);
    digitalWrite(8, LOW);
    delay(1000);
    servo_9.write(0);
    digitalWrite(1, LOW); 
    delay(1500);
    LCD.setCursor(0,0);
    LCD.print("                ");
    LCD.setCursor(0,1);
    LCD.print("                ");
    delay(100);
    digitalWrite(10, HIGH);
    digitalWrite(13, LOW); 
    count = 0;
  }
}
