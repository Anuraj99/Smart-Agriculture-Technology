#include <SoftwareSerial.h> 
SoftwareSerial espSerial =  SoftwareSerial(2,3);       
// arduino RX pin=0  arduino TX pin=1    
 
#include <DHT11.h>      
int pin=4; // digital pin number 4 
DHT11 dht11(pin); 
 
#define airquality_sensor_pin 0 
#define gas_sensor_pin 1 
#define VAL_PROBE 2 
// Analog pin 3 
String apiKey = "1157DDJB012C2VY6";    
 // replace with your channel's thingspeak WRITE API key  
String ssid="123";    
// Wifi network SSID 
String password ="12345678";  // Wifi network password 
 
boolean DEBUG=true; 
 
//================ // showResponce || //================ 
 
void showResponse(int waitTime)
 { 
    long t=millis();     
char c;     
while (t+waitTime>millis())     
{       if (espSerial.available())       
{         c=espSerial.read();         
if (DEBUG)              
Serial.print(c);     
  } 
    } 
             } 
 
boolean thingSpeakWrite(float value1, float value2, int value3, int value4, int value5) 
{   String cmd = "AT+CIPSTART=\"TCP\",\"";     // TCP connection   
cmd += "api.thingspeak.com";   
cmd += "\",80"; 
  espSerial.println(cmd);   
if (DEBUG)        
Serial.println(cmd);   
if(espSerial.find("Error"))   
{     
if (DEBUG)        
Serial.println("AT+CIPSTART error");     
return false;   }      
String getStr = "GET /update?api_key=";   // prepare GET string   
getStr += apiKey;      
getStr +="&field1=";   
getStr += String(value1);   
getStr +="&field2=";   
getStr += String(value2);    
getStr +="&field3=";    
getStr += String(value3);    
getStr +="&field4=";    
getStr += String(value4);       
getStr +="&field5=";    
getStr += String(value5);   // ...   
getStr += "\r\n\r\n"; 
 
  // send data length   
cmd = "AT+CIPSEND=";   
cmd += String(getStr.length());   
espSerial.println(cmd);   
if (DEBUG)  
Serial.println(cmd);      
delay(100);   
if(espSerial.find(">"))   
{     espSerial.print(getStr);     
if (DEBUG)        
 Serial.print(getStr);  
 }   
else   
{     
espSerial.println("AT+CIPCLOSE");     // alert user     
if (DEBUG)          
Serial.println("AT+CIPCLOSE");    
 return false; 
  } 
  return true; } 
 
 
//========= // setup || //========= 
 
void setup()  
{                   
DEBUG=true;           // enable debug serial   
Serial.begin(9600);      
espSerial.begin(115200);       
espSerial.println("AT+RST");   
showResponse(1000); 
 
  espSerial.println("AT+UART_CUR=9600,8,1,0,0");   
showResponse(1000);       
 
  espSerial.println("AT+CWMODE=1");   // set esp8266 as client   
showResponse(1000); 
 espSerial.println("AT+CWJAP=\""+ssid+"\",\""+password+"\"");  // set your home router SSID and password   
showResponse(5000); 
 
if (DEBUG)       
Serial.println("Setup completed"); } 
 
// ======= // loop || // ======= 
 
void loop()  { 
 
  // Read sensor values     
float temp=0.0, humi=0.0;     
int err;      
int airquality_value = analogRead(airquality_sensor_pin);     
int gas_value = analogRead(gas_sensor_pin); 
 int moisture = analogRead(VAL_PROBE);     
if((err=dht11.read(humi, temp))!=0)     
{             Serial.println();             
Serial.print("Error No :");             
Serial.print(err);             
Serial.println();     
  }     
Serial.println();     
Serial.print("temperature:");     
Serial.print(temp);     
Serial.println();     
Serial.print(" humidity:");     
Serial.print(humi);     
Serial.println();     
Serial.print("Gas Value:> ");     
Serial.print(gas_value);     
Serial.println();     
Serial.print("Air Quality Value:> ");     
Serial.print(airquality_value);         
Serial.println();     
Serial.print("Soil Moisture Value:> ");     
Serial.println(1050-moisture);     
Serial.println("% send to Thingspeak");       
Serial.println();    
 Serial.print("______________________________________\n");     
delay(1000);     
thingSpeakWrite(temp,humi,gas_value,airquality_value,moisture);       // Write values to thingspeak        // thingspeak needs 15 sec delay between updates,          
delay(20000);   }
