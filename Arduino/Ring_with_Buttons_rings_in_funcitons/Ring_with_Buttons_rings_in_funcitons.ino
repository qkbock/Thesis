#include <Adafruit_NeoPixel.h>

//6, 8, 9, 10, 11 work for ring
// 7, 14, 15 not working
Adafruit_NeoPixel ring_a = Adafruit_NeoPixel(16, 7);
Adafruit_NeoPixel ring_b = Adafruit_NeoPixel(16, 6);
Adafruit_NeoPixel button_a = Adafruit_NeoPixel(2, 8);
Adafruit_NeoPixel button_b = Adafruit_NeoPixel(2, 9);
const int buttonPin_a = 10; 
const int buttonPin_b = 11;
int sensorPin = A4; 
int sensorValue = 0;

String person_a = "John";
String person_b = "Quincy";

//information from server
int totalToday_a = 0;
int totalToday_b = 0;
int todayGoal_a = 670;
int todayGoal_b = 670;
int buttonR_a = 0;
int buttonG_a = 0;
int buttonB_a = 255;
int buttonR_b = 120;
int buttonG_b = 160;
int buttonB_b = 180;

float light_a;
float light_b;
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
}

void loop(){
  sensorValue = analogRead(sensorPin); 
//  Serial.println(sensorValue);
  totalToday_b = sensorValue;

//.....................................................................
  
  senseButtonA();
  senseButtonB();
  displayButtonA();
  displayButtonB();

  displayRingA();
  displayRingB();
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
    }
    else if(recording_a == true){
      //set end time
      //feed this start, end and person
      sendAChore();
      confirming_a = true;
    }
    recording_a = !recording_a;
    fadeCounter_a = 0;
//    Serial.println(recording_a);
  }
  previousButtonState_a = buttonState_a; 
//  Serial.println(buttonState_a);
}

//-----------------------------------------------------------------------

void senseButtonB(){
  buttonState_b = digitalRead(buttonPin_b);
  if ((buttonState_b != previousButtonState_b) && (buttonState_b == HIGH)) {
    if(recording_b == false){
      //set start time
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

void displayRingA(){
  //calculate number of lights
  light_a = (totalToday_a*16)/todayGoal_a;
  //color ring A
  if(light_a > 16){
    color_a = ring_a.Color(0, 255, 0);
  }
  else if(light_a > 15){
    color_a = ring_a.Color(40, 255, 0);
  }
  else if(light_a > 14){
    color_a = ring_a.Color(80, 255, 0);
  }
  else if(light_a > 13){
    color_a = ring_a.Color(100, 255, 0);
  }
  else if(light_a > 12){
    color_a = ring_a.Color(125, 255, 0);
  }
  else if(light_a > 11){
    color_a = ring_a.Color(150, 255, 0);
  }
  else if(light_a > 10){
    color_a = ring_a.Color(190, 255, 0);
  }
  else if(light_a > 9){
    color_a = ring_a.Color(220, 255, 0);
  }
  else if(light_a > 8){
    color_a = ring_a.Color(255, 240, 0);
  }
  else if(light_a > 7){
    color_a = ring_a.Color(255, 220, 0);
  }
  else if(light_a > 6){
    color_a = ring_a.Color(255, 190, 0);
  }
  else if(light_a > 5){
    color_a = ring_a.Color(255, 150, 0);
  }
  else if(light_a > 4){
    color_a = ring_a.Color(255, 100, 0);
  }
  else if(light_a > 3){
    color_a = ring_a.Color(255, 60, 0);
  }
  else if(light_a > 2){
    color_a = ring_a.Color(255, 30, 0);
  }
  else{
    color_a = ring_a.Color(255, 0, 0);
  }


  //light ring A
  if(light_a > 0){
    ring_a.setPixelColor(15, color_a);
  }
  else{
    ring_a.setPixelColor(15, 0, 0, 0);
  }
  if(light_a > 2){
    ring_a.setPixelColor(14, color_a);
  }
  else{
    ring_a.setPixelColor(14, 0, 0, 0);
  }
  if(light_a > 3){
    ring_a.setPixelColor(13, color_a);
  }
  else{
    ring_a.setPixelColor(13, 0, 0, 0);
  }
  if(light_a > 4){
    ring_a.setPixelColor(12, color_a);
  }
  else{
    ring_a.setPixelColor(12, 0, 0, 0);
  }
  if(light_a > 5){
    ring_a.setPixelColor(11, color_a);
  }
  else{
    ring_a.setPixelColor(11, 0, 0, 0);
  }
  if(light_a > 6){
    ring_a.setPixelColor(10, color_a);
  }
  else{
    ring_a.setPixelColor(10, 0, 0, 0);
  }
  if(light_a > 7){
    ring_a.setPixelColor(9, color_a);
  }
  else{
    ring_a.setPixelColor(9, 0, 0, 0);
  }
  if(light_a > 8){
    ring_a.setPixelColor(8, color_a);
  }
  else{
    ring_a.setPixelColor(8, 0, 0, 0);
  }
  if(light_a > 9){
    ring_a.setPixelColor(7, color_a);
  }
  else{
    ring_a.setPixelColor(7, 0, 0, 0);
  }
  if(light_a > 10){
    ring_a.setPixelColor(6, color_a);
  }
  else{
    ring_a.setPixelColor(6, 0, 0, 0);
  }
  if(light_a > 11){
    ring_a.setPixelColor(5, color_a);
  }
  else{
    ring_a.setPixelColor(5, 0, 0, 0);
  }
  if(light_a > 12){
    ring_a.setPixelColor(4, color_a);
  }
  else{
    ring_a.setPixelColor(4, 0, 0, 0);
  }
  if(light_a > 13){
    ring_a.setPixelColor(3, color_a);
  }
  else{
    ring_a.setPixelColor(3, 0, 0, 0);
  }
  if(light_a > 14){
    ring_a.setPixelColor(2, color_a);
  }
  else{
    ring_a.setPixelColor(2, 0, 0, 0);
  }
  if(light_a > 15){
    ring_a.setPixelColor(1, color_a);
  }
  else{
    ring_a.setPixelColor(1, 0, 0, 0);
  }
  if(light_a > 16){
    ring_a.setPixelColor(0, color_a);
  }
  else{
    ring_a.setPixelColor(0, 0, 0, 0);
  }


  ring_a.show();
}

//-----------------------------------------------------------------------

void displayRingB(){
  //caculate number of lights
  light_b = (totalToday_b*16)/todayGoal_b;

  //color ring B
  if(light_b > 16){
    color_b = ring_b.Color(0, 255, 0);
  }
  else if(light_b > 15){
    color_b = ring_b.Color(40, 255, 0);
  }
  else if(light_b > 14){
    color_b = ring_b.Color(80, 255, 0);
  }
  else if(light_b > 13){
    color_b = ring_b.Color(100, 255, 0);
  }
  else if(light_b > 12){
    color_b = ring_b.Color(125, 255, 0);
  }
  else if(light_b > 11){
    color_b = ring_b.Color(150, 255, 0);
  }
  else if(light_b > 10){
    color_b = ring_b.Color(190, 255, 0);
  }
  else if(light_b > 9){
    color_b = ring_b.Color(220, 255, 0);
  }
  else if(light_b > 8){
    color_b = ring_b.Color(255, 240, 0);
  }
  else if(light_b > 7){
    color_b = ring_b.Color(255, 220, 0);
  }
  else if(light_b > 6){
    color_b = ring_b.Color(255, 190, 0);
  }
  else if(light_b > 5){
    color_b = ring_b.Color(255, 150, 0);
  }
  else if(light_b > 4){
    color_b = ring_b.Color(255, 100, 0);
  }
  else if(light_b > 3){
    color_b = ring_b.Color(255, 60, 0);
  }
  else if(light_b > 2){
    color_b = ring_b.Color(255, 30, 0);
  }
  else{
    color_b = ring_b.Color(255, 0, 0);
  }


  //light ring B
  if(light_b > 0){
    ring_b.setPixelColor(15, color_b);
  }
  else{
    ring_b.setPixelColor(15, 0, 0, 0);
  }
  if(light_b > 2){
    ring_b.setPixelColor(14, color_b);
  }
  else{
    ring_b.setPixelColor(14, 0, 0, 0);
  }
  if(light_b > 3){
    ring_b.setPixelColor(13, color_b);
  }
  else{
    ring_b.setPixelColor(13, 0, 0, 0);
  }
  if(light_b > 4){
    ring_b.setPixelColor(12, color_b);
  }
  else{
    ring_b.setPixelColor(12, 0, 0, 0);
  }
  if(light_b > 5){
    ring_b.setPixelColor(11, color_b);
  }
  else{
    ring_b.setPixelColor(11, 0, 0, 0);
  }
  if(light_b > 6){
    ring_b.setPixelColor(10, color_b);
  }
  else{
    ring_b.setPixelColor(10, 0, 0, 0);
  }
  if(light_b > 7){
    ring_b.setPixelColor(9, color_b);
  }
  else{
    ring_b.setPixelColor(9, 0, 0, 0);
  }
  if(light_b > 8){
    ring_b.setPixelColor(8, color_b);
  }
  else{
    ring_b.setPixelColor(8, 0, 0, 0);
  }
  if(light_b > 9){
    ring_b.setPixelColor(7, color_b);
  }
  else{
    ring_b.setPixelColor(7, 0, 0, 0);
  }
  if(light_b > 10){
    ring_b.setPixelColor(6, color_b);
  }
  else{
    ring_b.setPixelColor(6, 0, 0, 0);
  }
  if(light_b > 11){
    ring_b.setPixelColor(5, color_b);
  }
  else{
    ring_b.setPixelColor(5, 0, 0, 0);
  }
  if(light_b > 12){
    ring_b.setPixelColor(4, color_b);
  }
  else{
    ring_b.setPixelColor(4, 0, 0, 0);
  }
  if(light_b > 13){
    ring_b.setPixelColor(3, color_b);
  }
  else{
    ring_b.setPixelColor(3, 0, 0, 0);
  }
  if(light_b > 14){
    ring_b.setPixelColor(2, color_b);
  }
  else{
    ring_b.setPixelColor(2, 0, 0, 0);
  }
  if(light_b > 15){
    ring_b.setPixelColor(1, color_b);
  }
  else{
    ring_b.setPixelColor(1, 0, 0, 0);
  }
  if(light_b > 16){
    ring_b.setPixelColor(0, color_b);
  }
  else{
    ring_b.setPixelColor(0, 0, 0, 0);
  }



  ring_b.show();
}



