

#include <HT1621.h> // include our library


HT1621 lcd; // create an "lcd" object

const int analogPin = PB0;

void setup(){
    pinMode(analogPin, INPUT_ANALOG); 

  lcd.begin(PA0, PA1, PA2); 
  lcd.clear(); // clear the screen
}

void loop(){
  lcd.displayVcc(analogRead(analogPin)*3.3/4095); // 12ADC采集，显示电压值
  delay(1); // wait 1 millisecond
}
