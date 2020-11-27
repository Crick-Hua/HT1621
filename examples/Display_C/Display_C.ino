
#include <HT1621.h> // include our library
HT1621 lcd; // create an "lcd" object


void setup(){
  lcd.begin(PA0, PA1, PA2);  
  lcd.clear(); // clear the screen
}

void loop(){
    lcd.display_Char('H');   //显示‘H’
    delay(1000); 
    lcd.undisplay_Char('H'); //不显示‘H’
    delay(1000);  
    lcd.display_Char('P');
    delay(1000); 
    lcd.undisplay_Char('P'); 
    delay(1000);  
    lcd.display_Char('A');
    delay(1000); 
    lcd.undisplay_Char('A'); 
    delay(1000);  
    lcd.display_Char('V');
    delay(1000); 
    lcd.undisplay_Char('V'); 
    delay(1000);  
    lcd.display_Char('O');
    delay(1000); 
    lcd.undisplay_Char('O'); 
    delay(1000);  
    lcd.display_Char('L');
    delay(1000); 
    lcd.undisplay_Char('L'); 
    delay(1000);  
    lcd.display_Char('R');
    delay(1000); 
    lcd.undisplay_Char('R');
    delay(1000); 
    lcd.display_Char('D');
    delay(1000); 
    lcd.undisplay_Char('D'); 
    delay(1000);  
    lcd.display_Char('%');
    delay(1000); 
    lcd.undisplay_Char('%'); 
    delay(1000);   
}
