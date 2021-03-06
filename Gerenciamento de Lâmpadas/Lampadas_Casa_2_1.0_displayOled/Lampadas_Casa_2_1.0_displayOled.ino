#include <ESP8266WiFi.h>
#include <WiFiClient.h>

//ESP Web Server Library to host a web page
#include <ESP8266WebServer.h>
#include <string.h>

#include <Wire.h>
#include "SSD1306Wire.h"

SSD1306Wire  display(0x3c, D1, D2);

//---------------------------------------------------------------
//página HTML que será exibida
const char MAIN_page[] PROGMEM = R"=====(
  <!DOCTYPE html>
  <html lang="pt-BR">
  <title>LAMPADAS - CASA 2</title>
  
  <script language="javascript">
    
  function id( el ){
  return document.getElementById( el );
  }

  function getXmlHttpRequest() {
    if (window.XMLHttpRequest) {
      return new XMLHttpRequest();
    } else if (window.ActiveXObject) {
      return new ActiveXObject("Microsoft.XMLHTTP");
    }
  }

  function Atualiza(){
        location.reload();
    }
    
</script>

<style>

.dropdown {
    position: relative;
    display: inline-block;
  color: black;
}

.dropdown-content {
    display: none;
    position: absolute;
    background-color: #f9f9f9;
    min-width: 160px;
    box-shadow: 0px 8px 16px 0px rgba(0,0,0,0.2);
    padding: 12px 16px;
    z-index: 1;
}

.dropdown:hover .dropdown-content {
    display: block;
}

.button{
     position: relative;
    background-color: #FF0000;
    border: none;
    font-size: 12.8px;
    color: #FFFFFF;
    padding: 3.0px;
    width: 75px;
    text-align: center;
    -webkit-transition-duration: 0.4s; /* Safari */
    transition-duration: 0.4s;
    text-decoration: none;
    overflow: hidden;
    cursor: pointer;
}
.button:after {
    content: "";
    background: #f1f000;
    display: block;
    position: absolute;
    padding-top: 300%;
    padding-left: 350%;
    margin-left: -20px !important;
    margin-top: -120%;
    opacity: 0;
    transition: all 0.8s
}

.button:active:after {
    padding: 0;
    margin: 0;
    opacity: 1;
    transition: 0s
}

.button2{
      position: relative;
    background-color: black;
    border: none;
    font-size: 12.8px;
    color: #FFFFFF;
    padding: 3.0px;
    width: 75px;
    text-align: center;
    -webkit-transition-duration: 0.4s; /* Safari */
    transition-duration: 0.4s;
    text-decoration: none;
    overflow: hidden;
    cursor: pointer;
}
.button2:after {
    content: "";
    background: #f1f000;
    display: block;
    position: absolute;
    padding-top: 300%;
    padding-left: 350%;
    margin-left: -20px !important;
    margin-top: -120%;
    opacity: 0;
    transition: all 0.8s
}

.button2:active:after {
    padding: 0;
    margin: 0;
    opacity: 1;
    transition: 0s
}

.switch {
  position: relative;
  display: inline-block;
  width: 60px;
  height: 34px;
}

.switch input {display:none;}

.slider {
  position: absolute;
  cursor: pointer;
  top: 0;
  left: 0;
  right: 0;
  bottom: 0;
  background-color: #cc0000;
  -webkit-transition: .4s;
  transition: .4s;
}

.slider:before {
  position: absolute;
  content: "";
  height: 26px;
  width: 26px;
  left: 4px;
  bottom: 4px;
  background-color: white;
  -webkit-transition: .4s;
  transition: .4s;
}

input:checked + .slider {
  background-color: #009933;
}

input:focus + .slider {
  box-shadow: 0 0 1px #009933;
}

input:checked + .slider:before {
  -webkit-transform: translateX(26px);
  -ms-transform: translateX(26px);
  transform: translateX(26px);
}

/* Rounded sliders */
.slider.round {
  border-radius: 34px;
}

.slider.round:before {
  border-radius: 50%;
}
</style>
  
  <body>
  <center>
  <h1>Controle de Lampadas da Casa 2 - externo e corredor</h1><br>
  
  <td><button  class="button" onclick="Atualiza();">  Atualizar </button></td>
  <hr>
  </center>
  
  </body>
  </html>
)=====";
//---------------------------------------------------------------


//Static IP address configuration
IPAddress staticIP(192, 168, 0, 20); //ESP static ip
IPAddress gateway(138, 185, 242, 129);   //IP Address of your WiFi Router (Gateway)138.185.242.129
IPAddress subnet(255, 255, 255, 0);  //Subnet mask
IPAddress dns(8, 8, 8, 8);  //DNS


const char* deviceName = "Lampadas - Casa 2";

#define Rele 5  
bool  flag = false;

//SSID and Password of your WiFi router
//const char* ssid = "Telecom";
//const char* password = "senha";
const char* ssid = "Jordy_Mast Telecom";
const char* password = "jordylima";

//Declare a global object variable from the ESP8266WebServer class.
ESP8266WebServer server(80); //Server on port 80

//===============================================================
// This routine is executed when you open its IP in browser
//===============================================================
void Tela_Inicial() {
 //header padrão para habilitar trânsito de dados entre as páginas.
 server.sendHeader("Access-Control-Allow-Origin", "*");
 server.sendHeader("Access-Control-Max-Age", "10000");
 server.sendHeader("Access-Control-Allow-Methods", "POST,GET,OPTIONS");
 server.sendHeader("Access-Control-Allow-Headers", "Origin, X-Requested-With, Content-Type, Accept");
 
 //Serial.println("Tela Inicial - ON");
 String s = MAIN_page; //Read HTML contents
 
 if(flag == true){server.send(200, "text/html",s+"<br>Lampada 1 - Ligada</br>" );}
 else{server.send(200, "text/html"    ,s+"<br>Lampada 1 - Desligada</br>" );}
}

void Status() {
 //header padrão para habilitar trânsito de dados entre as páginas.
 server.sendHeader("Access-Control-Allow-Origin", "*");
 server.sendHeader("Access-Control-Max-Age", "10000");
 server.sendHeader("Access-Control-Allow-Methods", "POST,GET,OPTIONS");
 server.sendHeader("Access-Control-Allow-Headers", "Origin, X-Requested-With, Content-Type, Accept");
 
 if(flag == true){server.send(200, "text/html","<html><h1>Lampada Ligada</html></h1>" );}
 else{server.send(200, "text/html"    ,"<html><h1>Lampada Desligada</html></h1>" );}
}

void Tela_Ip_Rele_On() {  
 //header padrão para habilitar trânsito de dados entre as páginas.
 server.sendHeader("Access-Control-Allow-Origin", "*");
 server.sendHeader("Access-Control-Max-Age", "10000");
 server.sendHeader("Access-Control-Allow-Methods", "POST,GET,OPTIONS");
 server.sendHeader("Access-Control-Allow-Headers", "Origin, X-Requested-With, Content-Type, Accept");
 
 //Serial.println("Requisição via IP -" + WiFi.localIP());
 //Serial.println("Lâmpada Ligada.");
 digitalWrite(Rele,HIGH);  
 flag = true;
 
 server.send(200, "text/html", "<html><h1>Lampada Ligada</html></h1>");

}

void Tela_Ip_Rele_Off() { 
 //header padrão para habilitar trânsito de dados entre as páginas. 
 server.sendHeader("Access-Control-Allow-Origin", "*");
 server.sendHeader("Access-Control-Max-Age", "10000");
 server.sendHeader("Access-Control-Allow-Methods", "POST,GET,OPTIONS");
 server.sendHeader("Access-Control-Allow-Headers", "Origin, X-Requested-With, Content-Type, Accept");
 
 //Serial.println("Requisição via IP -" + WiFi.localIP());
 //Serial.println("Lâmpada Desligada.");
 digitalWrite(Rele,LOW);
 flag = false;
 server.send(200, "text/html", "<html><h1>Lampada Desligada</html></h1>");
 
}


//==============================================================
//                  SETUP
//==============================================================
void setup(void){
  Serial.begin(115200);
  
  display.init();
  display.flipScreenVertically();
  
  WiFi.begin(ssid, password);     //Connect to your WiFi router
  Serial.println("");

  pinMode(Rele,OUTPUT);   

  WiFi.disconnect();  //Prevent connecting to wifi based on previous configuration
  
  WiFi.hostname(deviceName);      
  WiFi.config(staticIP, subnet, gateway, dns);
  WiFi.begin(ssid, password);

  WiFi.mode(WIFI_STA); //WiFi mode station (connect to wifi router only
  
  // Wait for connection
  int x=0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(200);
    Serial.print(".");    
    display.setFont(ArialMT_Plain_10);    
    display.drawString(x, 0, "- ");
    display.display();
    x++;
  }
  display.clear();

  String IP = WiFi.localIP().toString();
  String ssid2 = ssid;
  
  //If connection successful show IP address in serial monitor
  Serial.println("");
  Serial.print("Conectado em - " + ssid2);
  display.drawString(0, 5, "SSID:"+ ssid2);
  Serial.println("");
  display.drawString(0, 30, "IP - " + IP);
  Serial.print("Endereço de IP - ");
  display.display();
  Serial.println(WiFi.localIP());  //IP address assigned to your ESP

  //Tratamento do que for recebido via HTTP::Request
  server.on("/"         ,Tela_Inicial     );      
  server.on("/lg_ext"   ,Tela_Ip_Rele_On  ); 
  server.on("/dlg_ext"  ,Tela_Ip_Rele_Off ); 
  server.on("/status"   ,Status           );

  server.begin();                  //Start server
  Serial.println("Server on - Esperando comandos.");
}
//==============================================================
//                     LOOP
//==============================================================
void loop(void){
  server.handleClient();          //Handle client requests
}
