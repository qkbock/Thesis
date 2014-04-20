#include <Adafruit_NeoPixel.h>

Adafruit_NeoPixel ring_a = Adafruit_NeoPixel(16, 6);
Adafruit_NeoPixel ring_b = Adafruit_NeoPixel(16, 7);
Adafruit_NeoPixel button_a = Adafruit_NeoPixel(16, 8);
Adafruit_NeoPixel button_b = Adafruit_NeoPixel(16, 9);

String person_a = "John";
String person_b = "Quincy";

float totalToday_a = 0;
float totalToday_b = 0;
float todayGoal_a = 670;
float todayGoal_b = 670;

float light_a;
float light_b;
uint32_t color_a = ring_a.Color(255, 0, 0);
uint32_t color_b = ring_b.Color(255, 0, 0);

uint32_t buttonColor_a = ring_a.Color(0, 0, 255);
uint32_t buttonColor_b = ring_b.Color(0, 0, 255);

const int buttonPin_a = 10; 
const int buttonPin_b = 11;
int buttonState_a = 0;
int buttonState_b = 0;
int previousButtonState_a = HIGH;
int previousButtonState_b = HIGH;

boolean recording_a = false;
boolean recording_b = false;

int sensorPin = A0; 
int sensorValue = 0;

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
//    Serial.println(sensorValue);
    
    buttonState_a = digitalRead(buttonPin_a);
    buttonState_b = digitalRead(buttonPin_b);
    
    //button sensing for A only
    if ((buttonState_a != previousButtonState_a) && (buttonState_a == HIGH)) {
      if(recording_a == false){
        //set start time
      }
      else if(recording_a == true){
        //set end time
        
        //feed this start, end and person
        sendAChore();
      }
      recording_a = !recording_a;
    }

    
    previousButtonState_a = buttonState_a; 
    previousButtonState_b = buttonState_b;
    
    totalToday_b = sensorValue;

    //caculate number of lights
    light_a = (totalToday_a*16)/todayGoal_a;
    light_b = (totalToday_b*16)/todayGoal_b;

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
    
    //change button behavior when recording
    if(recording_a == true){
    
    }
    else{
      button_a.setPixelColor(0, buttonColor_a);
      button_a.setPixelColor(1, buttonColor_a);
    }
    
    button_b.setPixelColor(0, buttonColor_b);
    button_b.setPixelColor(1, buttonColor_b);
      
    ring_a.show();
    ring_b.show();
    button_a.show();
    button_b.show();
}



void sendAChore(){
  //start time
  //end time
  //duration?
  //person
}


