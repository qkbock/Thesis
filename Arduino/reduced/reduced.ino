void displayRingA(float light, int totalToday, int todayGoal, uint32_t color, Adafruit_NeoPixel ring){
  //calculate number of lights
  light = (totalToday*16)/todayGoal;
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

