#include <Arduino.h>

// DECLARACIÓN ANTICIPADA: Indica al compilador que la función existe.
float obtener_corriente();

// Sensibilidad del sensor en V/A
//float SENSIBILITY = 0.185;   // Modelo 5A
//float SENSIBILITY = 0.100; // Modelo 20A
float Sensibilidad = 0.066; // Modelo 30A - 66mV/A - 0.066 V/A

/*
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
    
    // Asumimos que la tensión es 220V, según el estándar del video.
    float P = Irms * 220.0; // P = I * V watts
    
    Serial.print("Ip: "); Serial.print(Ip, 3); Serial.print("A "); 
    Serial.print("Irms: "); Serial.print(Irms, 3); Serial.print("A "); 
    // ERROR CORREGIDO: Se debe incluir la variable 'P' para la impresión.
    Serial.print("Potencia: "); Serial.print(P, 3); Serial.print("W"); 
    Serial.println(); // Salto de línea para organizar la consola
    
    delay(500);
    
}

float obtener_corriente() {
    float voltajeSensor;
    float corriente = 0;
    // La lectura se realiza durante 500ms, igual que el delay en el loop.
    long tiempo = millis();
    float Imax = 0;
    float Imin = 0;
    
    while(millis() - tiempo < 500){
        // Lectura del voltaje del sensor en el pin A0
        voltajeSensor = analogRead(A0) * (5.0 / 1023.0); 
        
        // Ecuación para obtener la corriente (Filtro IIR de primer orden)
        corriente = 0.9 * corriente + 0.1 * ((voltajeSensor - 2.5) / Sensibilidad); 
        
        if(corriente > Imax){
            Imax = corriente;
        } 
        if(corriente < Imin){
            Imin = corriente;
        }
    }
    
    // Retorna la corriente pico (que es la mitad del rango Imax - Imin)
    return((Imax - Imin) / 2.0);
}