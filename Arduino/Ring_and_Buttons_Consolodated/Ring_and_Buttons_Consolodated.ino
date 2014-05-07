#include <Adafruit_NeoPixel.h>

//4, 6, 7, 8, 9, 10, 11 work for ring
// 5 not working
Adafruit_NeoPixel ring_a = Adafruit_NeoPixel(16, 6);
Adafruit_NeoPixel ring_b = Adafruit_NeoPixel(16, 9);
Adafruit_NeoPixel button_a = Adafruit_NeoPixel(2, 4);
Adafruit_NeoPixel button_b = Adafruit_NeoPixel(2, 10);
const int buttonPin_a = 8; 
const int buttonPin_b = 11;
int sensorPin = A4; 
int sensorValue = 0;

String person_a = "John";
String person_b = "Quincy";

//information from server
int totalToday_a = 25;
int totalToday_b = 50;
int todayGoal_a = 100;
int todayGoal_b = 100;
int buttonR_a = 0;
int buttonG_a = 0;
int buttonB_a = 255;
int buttonR_b = 120;
int buttonG_b = 160;
int buttonB_b = 180;

uint32_t color_a = ring_a.Color(255, 0, 0);
uint32_t color_b = ring_b.Color(255, 0, 0);

uint32_t buttonColor_a = button_a.Color(buttonR_a, buttonG_a, buttonB_a);
uint32_t buttonColor_b = button_b.Color(buttonR_b, buttonG_b, buttonB_b);

int buttonState_a = 0;
int buttonState_b = 0;
int previousButtonState_a = HIGH;
int previousButtonState_b = HIGH;

float fadeCounter_a = 0;
float fadeCounter_b = 0;
int confirmingCounter_a = 0;
int confirmingCounter_b = 0;
boolean confirming_a = false;
boolean confirming_b = false;

boolean recording_a = false;
boolean recording_b = false;



void setup() {
  ring_a.begin();
  ring_b.begin();
  button_a.begin();
  button_b.begin();
  ring_a.show(); // Initialize all pixels to 'off'
  ring_b.show();
  button_a.show();
  button_b.show();
  ring_a.setBrightness(64);
  ring_b.setBrightness(64);
  button_a.setBrightness(100);
  button_b.setBrightness(100);
  pinMode(buttonPin_a, INPUT);
  pinMode(buttonPin_b, INPUT);
  Serial.println("Setup Done");
}

void loop(){
//  sensorValue = analogRead(sensorPin); 
//  Serial.println(sensorValue);
//  totalToday_b = sensorValue;

//.....................................................................
  
  senseButtonA();
  senseButtonB();

  displayButtonA();
  displayButtonB();

  displayRing(totalToday_a, todayGoal_a, color_a, ring_a);
  displayRing(totalToday_b, todayGoal_b, color_b, ring_b);
  
  totalToday_b += 1;
  Serial.println(totalToday_b);
  delay(10);
}

//-----------------------------------------------------------------------

void sendAChore(){
  //start time
  //end time
  //duration?
  //person
}

//-----------------------------------------------------------------------

void senseButtonA(){
  buttonState_a = digitalRead(buttonPin_a);
  if ((buttonState_a != previousButtonState_a) && (buttonState_a == HIGH)) {
    if(recording_a == false){
      //set start time
      totalToday_a += 10;
    }
    else if(recording_a == true){
      //set end time
      //feed this start, end and person
      sendAChore();
      confirming_a = true;
    }
    recording_a = !recording_a;
    fadeCounter_a = 0;
    Serial.println(recording_a);
  }
  previousButtonState_a = buttonState_a; 
  Serial.println(buttonState_a);
}

//-----------------------------------------------------------------------

void senseButtonB(){
  buttonState_b = digitalRead(buttonPin_b);
  if ((buttonState_b != previousButtonState_b) && (buttonState_b == HIGH)) {
    if(recording_b == false){
      //set start time
      totalToday_b += 10;
    }
    else if(recording_b == true){
      //set end time
      //feed this start, end and person
      sendAChore();
      confirming_b = true;
    }
    recording_b = !recording_b;
    fadeCounter_b = 0;
  }
  previousButtonState_b = buttonState_b; 
}

//-----------------------------------------------------------------------

void displayButtonA(){
  //change button behavior when recording
  if(recording_a == true){
    button_a.setBrightness(int(cos(fadeCounter_a)*50 + 50));
    button_a.setPixelColor(0, 255, 15, 65);
    button_a.setPixelColor(1, 255, 15, 65);
    button_a.show();
    fadeCounter_a = fadeCounter_a + .03;
    if(fadeCounter_a > 6.2){
      fadeCounter_a = 0;
    }
  }
  else if(confirming_a){
    button_a.setBrightness(100);
    button_a.setPixelColor(0, 13, 255, 130);
    button_a.setPixelColor(1, 13, 255, 130);
    button_a.show();
    confirmingCounter_a ++;
    if(confirmingCounter_a > 150){
      confirming_a = false;
      confirmingCounter_a = 0;
    }
  }
  else{
    button_a.setBrightness(100);
    button_a.setPixelColor(0, buttonColor_a);
    button_a.setPixelColor(1, buttonColor_a);
    button_a.show();
  }
}

//-----------------------------------------------------------------------

void displayButtonB(){
  //change button behavior when recording
  if(recording_b == true){
    button_b.setBrightness(int(cos(fadeCounter_b)*50 + 50));
    button_b.setPixelColor(0, 255, 15, 65);
    button_b.setPixelColor(1, 255, 15, 65);
    button_b.show();
    fadeCounter_b = fadeCounter_b + .03;
    if(fadeCounter_b > 6.2){
      fadeCounter_b = 0;
    }
  }
  else if(confirming_b){
    button_b.setBrightness(100);
    button_b.setPixelColor(0, 13, 255, 130);
    button_b.setPixelColor(1, 13, 255, 130);
    button_b.show();
    confirmingCounter_b ++;
    if(confirmingCounter_b > 150){
      confirming_b = false;
      confirmingCounter_b = 0;
    }
  }
  else{
    button_b.setBrightness(100);
    button_b.setPixelColor(0, buttonColor_b);
    button_b.setPixelColor(1, buttonColor_b);
    button_b.show();
  }
}

//-----------------------------------------------------------------------

void displayRing(int totalToday, int todayGoal, uint32_t color, Adafruit_NeoPixel ring){
  //calculate number of lights
  float light = (totalToday*16)/todayGoal;
  //color ring A
  if(light > 16){
    color = ring.Color(0, 255, 0);
  }
  else if(light > 15){
    color = ring.Color(40, 255, 0);
  }
  else if(light > 14){
    color = ring.Color(80, 255, 0);
  }
  else if(light > 13){
    color = ring.Color(100, 255, 0);
  }
  else if(light > 12){
    color = ring.Color(125, 255, 0);
  }
  else if(light > 11){
    color = ring.Color(150, 255, 0);
  }
  else if(light > 10){
    color = ring.Color(190, 255, 0);
  }
  else if(light > 9){
    color = ring.Color(220, 255, 0);
  }
  else if(light > 8){
    color = ring.Color(255, 240, 0);
  }
  else if(light > 7){
    color = ring.Color(255, 220, 0);
  }
  else if(light > 6){
    color = ring.Color(255, 190, 0);
  }
  else if(light > 5){
    color = ring.Color(255, 150, 0);
  }
  else if(light > 4){
    color = ring.Color(255, 100, 0);
  }
  else if(light > 3){
    color = ring.Color(255, 60, 0);
  }
  else if(light > 2){
    color = ring.Color(255, 30, 0);
  }
  else{
    color = ring.Color(255, 0, 0);
  }


  //light ring A
  if(light > 0){
    ring.setPixelColor(15, color);
  }
  else{
    ring.setPixelColor(15, 0, 0, 0);
  }
  if(light > 2){
    ring.setPixelColor(14, color);
  }
  else{
    ring.setPixelColor(14, 0, 0, 0);
  }
  if(light > 3){
    ring.setPixelColor(13, color);
  }
  else{
    ring.setPixelColor(13, 0, 0, 0);
  }
  if(light > 4){
    ring.setPixelColor(12, color);
  }
  else{
    ring.setPixelColor(12, 0, 0, 0);
  }
  if(light > 5){
    ring.setPixelColor(11, color);
  }
  else{
    ring.setPixelColor(11, 0, 0, 0);
  }
  if(light > 6){
    ring.setPixelColor(10, color);
  }
  else{
    ring.setPixelColor(10, 0, 0, 0);
  }
  if(light > 7){
    ring.setPixelColor(9, color);
  }
  else{
    ring.setPixelColor(9, 0, 0, 0);
  }
  if(light > 8){
    ring.setPixelColor(8, color);
  }
  else{
    ring.setPixelColor(8, 0, 0, 0);
  }
  if(light > 9){
    ring.setPixelColor(7, color);
  }
  else{
    ring.setPixelColor(7, 0, 0, 0);
  }
  if(light > 10){
    ring.setPixelColor(6, color);
  }
  else{
    ring.setPixelColor(6, 0, 0, 0);
  }
  if(light > 11){
    ring.setPixelColor(5, color);
  }
  else{
    ring.setPixelColor(5, 0, 0, 0);
  }
  if(light > 12){
    ring.setPixelColor(4, color);
  }
  else{
    ring.setPixelColor(4, 0, 0, 0);
  }
  if(light > 13){
    ring.setPixelColor(3, color);
  }
  else{
    ring.setPixelColor(3, 0, 0, 0);
  }
  if(light > 14){
    ring.setPixelColor(2, color);
  }
  else{
    ring.setPixelColor(2, 0, 0, 0);
  }
  if(light > 15){
    ring.setPixelColor(1, color);
  }
  else{
    ring.setPixelColor(1, 0, 0, 0);
  }
  if(light > 16){
    ring.setPixelColor(0, color);
  }
  else{
    ring.setPixelColor(0, 0, 0, 0);
  }


  ring.show();
}

