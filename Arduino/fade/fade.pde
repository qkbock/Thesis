float counter = 0.0;
int brightness; 

void setup(){
  size(200, 200);
  noStroke();
  background(0);
  colorMode(HSB);
  frameRate(10);

}

void draw(){
      background(0);

    brightness = int(cos(counter)*50 + 50);
    
    fill(255, 100, brightness);
    rect(50, 50, 100, 100);
    counter = counter + .1;
    if(counter > 6.2){
      counter = 0;
    }
  println(brightness);
}
