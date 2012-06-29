import processing.serial.*;

Serial myPort;

void setup(){
  myPort = new Serial(this, Serial.list()[0], 9600);
  size(1024,40);
}
void draw() {
  background(0);
  while (myPort.available() > 0) {
    String myString = myPort.readStringUntil('\n');
    if (myString != null) {
      int x = int(myString);
      ellipse(x, 20, 20, 20);
    }
  }
}
