#include <wiringPi.h>

int main(){
    int led_pin = 18;
    int switch_pin = 24;
    int i;
    
    wiringPiSetupGpio();
    
    pinMode(led_pin, OUTPUT);
    pinMode(switch_pin, INPUT);

    for (i = 0; i < 100000000; i++){
        if(digitalRead(switch_pin) == LOW){
            digitalWrite(switch_pin, HIGH);
            delay(100);
            digitalWrite(led_pin, LOW);
        } 
    }
    return 0;
}