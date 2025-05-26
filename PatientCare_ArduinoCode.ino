#include <LiquidCrystal.h>

#define LCD_RS 13 
#define LCD_EN 12
#define LCD_DB4 11
#define LCD_DB5 10
#define LCD_DB6 9
#define LCD_DB7 8
#define iot_1 7
#define iot_2 6
#define iot_3 5
#define iot_4 3
#define panic_sw 4
#define flex_Sensor_1 A0
#define flex_Sensor_2 A1
#define flex_Sensor_3 A2
#define Temp_Sensor A3
#define Pulse_Sensor A4

LiquidCrystal lcd(LCD_RS, LCD_EN, LCD_DB4, LCD_DB5, LCD_DB6, LCD_DB7);

void gpio_init();
void update_lcd();
void read_adc();
void read_temp();
void Beep();
void Read_PV();

const int thermistor_output = A3;
int thermistor_adc_val = 0;
double temperature;
float Solar_V = 0.00;
int Battery = 0;

void setup() {
    gpio_init();
    lcd.begin(16, 2);
    lcd.setCursor(4, 0);
    lcd.print(" Welcome ");
    delay(2000);
    lcd.clear();
}

void loop() { 
    update_lcd();
    read_adc();
    read_temp();
    delay(250);
}

void gpio_init() { 
    Serial.begin(9600);
    delay(100);
    Serial.println("\n ~ I WANT WATER ~");
    
    pinMode(panic_sw, INPUT_PULLUP);
    pinMode(iot_1, OUTPUT);
    pinMode(iot_2, OUTPUT);
    pinMode(iot_3, OUTPUT);
    pinMode(iot_4, OUTPUT); 
    
    digitalWrite(iot_1, HIGH);
    digitalWrite(iot_2, HIGH);
    digitalWrite(iot_3, HIGH);
    digitalWrite(iot_4, HIGH);
}

void update_lcd() {
    // You can add display updates here
}

void read_adc() {
    int a = analogRead(flex_Sensor_1);
    int b = analogRead(flex_Sensor_2);
    int c = analogRead(flex_Sensor_3);
    int pulse = analogRead(Pulse_Sensor);
    
    Serial.print("\n Flex Sensor 1 := "); Serial.print(a);
    Serial.print("\n Flex Sensor 2 := "); Serial.print(b);
    Serial.print("\n Flex Sensor 3 := "); Serial.print(c);
    Serial.print("\n Pulse: = "); Serial.print(pulse); 
    
    if (a <= 630) { 
        lcd.setCursor(0, 0); lcd.print("I Want Water ...");
        lcd.setCursor(0, 1); lcd.print("Flex S Input 1.");
        digitalWrite(iot_1, LOW); digitalWrite(iot_2, HIGH); digitalWrite(iot_3, HIGH); digitalWrite(iot_4, HIGH);
        delay(3500);
    }
    
    if (b <= 630) {
        lcd.setCursor(0, 0); lcd.print("I Want Medicine.");
        lcd.setCursor(0, 1); lcd.print("Flex S Input 2.");
        digitalWrite(iot_1, HIGH); digitalWrite(iot_2, LOW); digitalWrite(iot_3, HIGH); digitalWrite(iot_4, HIGH);
        delay(3500);
    }
    
    if (c <= 630) {
        lcd.setCursor(0, 0); lcd.print("I Want to Go");
        lcd.setCursor(0, 1); lcd.print("Washroom ...");
        digitalWrite(iot_1, HIGH); digitalWrite(iot_2, HIGH); digitalWrite(iot_3, LOW); digitalWrite(iot_4, HIGH);
        delay(3500);
    }

    if (a >= 630 && b >= 630 && c >= 630) {
        digitalWrite(iot_1, HIGH);
        digitalWrite(iot_2, HIGH);
        digitalWrite(iot_3, HIGH);
    }
    
    if (!digitalRead(panic_sw)) {
        while (1) {
            lcd.setCursor(0, 0); lcd.print("Panic SW Pressed");
            lcd.setCursor(0, 1); lcd.print("Help Help Help ...");
            digitalWrite(iot_1, HIGH);
            digitalWrite(iot_2, HIGH);
            digitalWrite(iot_3, HIGH);
            digitalWrite(iot_4, LOW);
        }
    }
    
    lcd.setCursor(0, 0); lcd.print("Temp : ^C ");
    lcd.setCursor(6, 0); lcd.print(temperature);
    
    int bpm = 0;
    if (pulse <= 505) { bpm = 75 + (pulse / 97); } 
    else { bpm = 0; }
    
    lcd.setCursor(0, 1); lcd.print("PULSE: BPM ");
    lcd.setCursor(6, 1); lcd.print(bpm);
}

void read_temp() { 
    double output_voltage, thermistor_resistance, therm_res_ln;  
    thermistor_adc_val = analogRead(thermistor_output);
    thermistor_adc_val = 1023 - thermistor_adc_val;
    
    output_voltage = (thermistor_adc_val * 5) / 1023.0;
    thermistor_resistance = ((5 * (10.0 / output_voltage)) - 10) * 1000;
    therm_res_ln = log(thermistor_resistance);
    
    Serial.print("Temperature in Celsius = "); Serial.print(temperature);
    Serial.print("\t\t Resistance in Ohms = "); Serial.print(thermistor_resistance);
    Serial.print("\n\n");
    
    delay(1000);  
}

void Read_PV() {
    float Vout = 0.00;
    float Vin = 0.00;
    float R1 = 10000.00; 
    float R2 = 10000.00; 
    int val = 0;

    delay(50);
    val = analogRead(A0);
    Serial.print("\n ADC_VAL := "); Serial.print(val);
    
    Vout = (val * 5.00) / 1024.00;
    Vin = Vout / (R2 / (R1 + R2));

    if (Vin < 0.09) { Vin = 0.00; }

    Solar_V = Vin * 2.51;
    Serial.print("\n Solar Voltage := "); Serial.print(Solar_V);
}
