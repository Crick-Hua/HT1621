# HT1621
Arduino(STM32duino) HT1621 LCD  
display_Vcc(float):display two digital voltages with a decimal point。  
display_Char(char):display Char,like icon ...   
undisplay_Char(char):no display Char,like icon ...   
display_Battery(int):display Battery icon.  

修改：
不同接口的LCD需要修改数组，如下：
char _digitalnum[10] = {0xBE,0x6,0x7C,0x5E,0xC6,0xDA,0xFA,0xE,0xFE,0xDE};//显示字符数组 0-9，
