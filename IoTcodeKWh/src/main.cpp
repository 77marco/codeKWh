#include <Arduino.h>

float Sensibilidad = 0.066; //sensibilidad en V/A para nustro sensor
/*
30A - 66mV/A - 0.066 v/A

v = Im + 2.5 //m : sensibilidad
Despejando la ecuación
I = (v - 2.5)/m
*/
void setup() {
    Serial.begin(9600);
}

void loop() {
    float Ip = obtener_corriente(); //obtenemos la corriente pico
    float Irms = Ip * 0.707; // Intensidad RMS = Ipico/(2 elevado a 1/2)
    float P = Irms * 220.0; // P = I * V watts
    
    Serial.print("Ip: "); Serial.print(Ip,3); Serial.print("A "); 
    Serial.print("Irms: "); Serial.print(Irms,3); Serial.print("A "); 
    Serial.print("Potencia: "); Serial.print(); Serial.print("W"); 
    delay(500);
    
}

float obtener_corriente() {
    float voltajeSensor;
    float corriente = 0;
    long tiempo = millis();
    float Imax=0;
    float Imin=0;
    while(millis() - tiempo<500){
        voltajeSensor = analogRead(A0) * (5.0 / 1023.0); // lectura
        corriente = 0.9*corriente+0.1*((voltajeSensor-2.5)/Sensibilidad); // Ecuación para obtener la corriente
        if(corriente>Imax){
        Imax = corriente;
    } 
    if(corriente<Imin){
    Imin = corriente;
}
}
return((Imax-Imin)/2);
}