#include <ESP8266WiFi.h>

#define iot_1 5  // D1 NodeMcu
#define iot_2 4  // D2 NodeMcu
#define iot_3 14 // D5 NodeMcu
#define iot_4 12 // D6 NodeMcu

const char* ssid = "IOT";
const char* password = "PROJECT";

WiFiServer server(80);

void gpio_init(void);
void wifi_init(void);
void update_web(void);
void gpio_read(void);
void Read_Voltage(void);

int v = 0;
float Solar_V = 0.00;

void setup() {
    Serial.begin(115200);
    gpio_init();
    wifi_init();
    server.begin();
}

void loop() {
    WiFiClient client = server.available();
    if (!client) {
        return;
    }

    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println("Connection: close");
    client.println();
    client.println("<!DOCTYPE HTML>");
    client.println("<HTML>");
    client.println("<HEAD><TITLE>Patient Monitor</TITLE></HEAD>");
    client.println("<BODY>");

    client.println("<H1 style='color:Blue;font-size:35px;text-align:center;'>");
    client.println("Patient ID : GMC_NGP :- 1168");
    client.println("</H1>");

    if (!digitalRead(iot_1)) {
        client.println("<H1 style='color:Red;font-size:45px;text-align:center;'>I Want Water</H1>");
    }

    if (!digitalRead(iot_2)) {
        client.println("<H1 style='color:Red;font-size:45px;text-align:center;'>I Want Medicine</H1>");
    }

    if (!digitalRead(iot_3)) {
        client.println("<H1 style='color:Red;font-size:45px;text-align:center;'>I Want To Go To Washroom</H1>");
    }

    if (!digitalRead(iot_4)) {
        client.println("<H1 style='color:Red;font-size:45px;text-align:center;'>Panic Switch Pressed</H1>");
        client.println("<H1 style='color:Red;font-size:45px;text-align:center;'>Emergency Need Help</H1>");
    }

    if (digitalRead(iot_1) && digitalRead(iot_2) && digitalRead(iot_3) && digitalRead(iot_4)) {
        client.println("<H1 style='color:Green;font-size:45px;text-align:center;'>No Input</H1>");
    }

    if (digitalRead(iot_4)) {
        client.println("<H1 style='color:Green;font-size:45px;text-align:center;'>Body Temperature OK</H1>");
        client.println("<H1 style='color:Green;font-size:45px;text-align:center;'>Pulse Rate OK</H1>");
    }

    client.println("</BODY>");
    client.println("</HTML>");
    client.stop();
}

void gpio_init() {
    pinMode(iot_1, INPUT);
    pinMode(iot_2, INPUT);
    pinMode(iot_3, INPUT);
    pinMode(iot_4, INPUT);
}

void wifi_init() {
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nWiFi Connected!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
}
