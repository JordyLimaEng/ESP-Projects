#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

//ESP Web Server Library to host a web page
#include <ESP8266WebServer.h>

#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"

// For the Adafruit shield, these are the default.
#define TFT_DC 2
#define TFT_CS 5


// Use hardware SPI (on Uno, #13, #12, #11) and the above for CS/DC
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);
// If using the breakout, change pins as desired
//Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST, TFT_MISO);

//Static IP address configuration
IPAddress staticIP(192, 168, 0, 108); //ESP static ip
IPAddress gateway(192, 168, 0, 1);   //IP Address of your WiFi Router (Gateway)
IPAddress subnet(255, 255, 255, 0);  //Subnet mask
IPAddress dns(8, 8, 8, 8);  //DNS

String IP_ESP01 = "http://192.168.0.101";
const char* deviceName = "NODE TFT";

#define Rele 3  
bool  flag = false;

//SSID and Password of your WiFi router
//const char* ssid = "Telecom";
//const char* password = "senha";
const char* ssid = "Jordy_Mast Telecom";
const char* password = "jordylima";

//Declare a global object variable from the ESP8266WebServer class.
ESP8266WebServer server(80); //Server on port 80

const long utcOffsetInSeconds = -10800;
char daysOfTheWeek[7][12] = {"Domingo", "Segunda", "Terça", "Quarta", "Quinta", "Sexta", "Sabado"};

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "south-america.pool.ntp.org", utcOffsetInSeconds);

void Padrao() {
  tft.fillScreen(ILI9341_BLACK);
  tft.setCursor(0, 0);
  tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);  tft.setTextSize(1);
  tft.setTextSize(1);
  tft.print("Conectado a ");
  tft.println(ssid);
  tft.setTextSize(2);
  tft.print("CONTROLE DE LAMPADAS");
  //tft.println(tft.width());
  //tft.println(tft.height());
  tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(1);
  tft.drawCircle(10, 40, 10, ILI9341_YELLOW);
  tft.setCursor(25, 35);
  tft.print("Lampada Cozinha Externa");
}

void Recebe(){
  String uri = server.uri(), recado;
  Serial.println(uri);
   
   if(uri == "/lg_ext"){
      tft.fillCircle(10, 40, 7, ILI9341_YELLOW);
      
      HTTPClient http;
      
      http.begin(IP_ESP01 + "/lg_ext");
      //http.addHeader("Content-Type", "text/plain");
      http.POST("lg_ext");
      String payload = http.getString();
      //Serial.println(httpCode);
      Serial.println(payload);
      Serial.println(payload.substring(34,40));
      
      http.end();
   }else if(uri == "/dlg_ext"){
      //tft.fillCircle(10, 40, 10, ILI9341_YELLOW);
      tft.fillCircle(10, 40, 10, ILI9341_BLACK);
      tft.drawCircle(10, 40, 10, ILI9341_YELLOW);
      
      HTTPClient http;
      
      http.begin(IP_ESP01 + "/dlg_ext");
      //http.addHeader("Content-Type", "text/plain");
      http.POST("dlg_ext");
      String payload = http.getString();
      //Serial.println(httpCode);

      Serial.println(payload);
      Serial.println(payload.substring(34,43));
      
      http.end();
   }else{
      tft.setCursor(0, 80);
      tft.setTextSize(2);
      tft.println("Recado:");
      tft.setTextSize(1.8);
      tft.setCursor(0, 105);
      tft.drawRoundRect(0, 100, 240, 220, 10, ILI9341_WHITE);
        for(int x=0; x<=uri.substring(1).length();x++){
            if(uri.substring(1)[x]=='%'){
              recado += " ";
            }else if(uri.substring(1)[x]=='2'){
              recado += "";
            }else if(uri.substring(1)[x]=='0'){
              recado += "";
            }
            else{
              recado += uri.substring(1)[x];
            }
        }
      Serial.println(recado);
      tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
      tft.println(recado);
   }   
}

void Status(){
    HTTPClient http;
      
    http.begin(IP_ESP01 + "/status");
    //http.addHeader("Content-Type", "text/plain");
    http.POST("status");
    String payload = http.getString();
    Serial.println(payload.substring(34,40));
    
    if(payload.substring(34,40) == "Ligada"){
      tft.fillCircle(10, 40, 7, ILI9341_YELLOW);
    }else{
      tft.fillCircle(10, 40, 10, ILI9341_BLACK);
      tft.drawCircle(10, 40, 10, ILI9341_YELLOW);
    }
      
    http.end();

}

void Auto(String cmd){
  if(cmd == "liga"){
    tft.fillCircle(10, 40, 7, ILI9341_YELLOW);      
    HTTPClient http;      
    http.begin(IP_ESP01 + "/lg_ext");
    http.POST("lg_ext");      
    http.end();
  }else{
    tft.fillCircle(10, 40, 10, ILI9341_BLACK);
    tft.drawCircle(10, 40, 10, ILI9341_YELLOW);      
    HTTPClient http;
    http.begin(IP_ESP01 + "/dlg_ext");
    http.POST("dlg_ext");      
    http.end();
  }  
}


//==============================================================
//                  SETUP
//==============================================================
void setup(void){
  Serial.begin(115200);
  tft.begin();
  
  WiFi.begin(ssid, password);     
  Serial.println("");

  pinMode(Rele,OUTPUT);   

  WiFi.disconnect();  
  
  WiFi.hostname(deviceName);      
  WiFi.config(staticIP, subnet, gateway, dns);
  WiFi.begin(ssid, password);

  WiFi.mode(WIFI_STA); 
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Conectado em - ");
  Serial.println(ssid);
  Serial.print("Endereço de IP - ");
  Serial.println(WiFi.localIP());
  
  //Tratamento do que for recebido via HTTP::Request
  server.on("/"       ,Padrao); 
  server.on("/Status"       ,Status); 
  server.onNotFound(Recebe);     
  //server.on("/uri"   ,uri);
  //server.on("/recado"   ,recado); 
  //server.on("/liga"  ,liga );
  //server.on("/desliga"  ,desliga ); 

  server.begin();                  //Start server
  Serial.println("Server on - Esperando comandos.");
  tft.setRotation(2);
  Padrao();
  timeClient.begin();
}
//==============================================================
//                     LOOP
//==============================================================
void loop(void){
  server.handleClient();          //Handle client requests
  timeClient.update();
  Serial.print(daysOfTheWeek[timeClient.getDay()]);
  Serial.print(", ");
  Serial.print(timeClient.getHours());
  Serial.print(":");
  Serial.print(timeClient.getMinutes());
  Serial.print(":");
  Serial.println(timeClient.getSeconds());
  if(timeClient.getHours() == 18){
    Auto("liga");
  }else if(timeClient.getHours() == 5){
    Auto("desl");
  }
}
