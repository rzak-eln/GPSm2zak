//Rx D1  TX D2 
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <WiFi.h>
#include <SPI.h>
#include <Wire.h>
#include <WebServer.h>


TinyGPSPlus gps;
SoftwareSerial SerialGPS(4, 5); 

char* ssid = "Y-OLO";
char* password = "LibebcnofneM3S71911";


float Latitude , Longitude;
int year , month , date, hour , minute , second;
String DateString , TimeString , LatitudeString , LongitudeString;

WiFiServer server(80);

void setup()
{
  Serial.begin(9600);
  SerialGPS.begin(9600);
  Serial.println();
  Serial.print("Connecting");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("IP");
  Serial.println("WiFi connected");
 
  server.begin();
  Serial.println("Server started");
  Serial.println(WiFi.localIP());
}

void loop()
{
  while (SerialGPS.available() > 0)
    if (gps.encode(SerialGPS.read()))
    {
      if (gps.location.isValid())
      {
        Latitude = gps.location.lat();
        LatitudeString = String(Latitude , 6);
        Longitude = gps.location.lng();
        LongitudeString = String(Longitude , 6);
      }

      if (gps.date.isValid())
      {
        DateString = "";
        date = gps.date.day();
        month = gps.date.month();
        year = gps.date.year();

        if (date < 10)
        DateString = '0';
        DateString += String(date);

        DateString += " / ";

        if (month < 10)
        DateString += '0';
        DateString += String(month);
        DateString += " / ";

        if (year < 10)
        DateString += '0';
        DateString += String(year);
      }

      if (gps.time.isValid())
      {
        TimeString = "";
        hour = gps.time.hour()+ 1; //adjust UTC
        minute = gps.time.minute();
        second = gps.time.second();
    
        if (hour < 10)
        TimeString = '0';
        TimeString += String(hour);
        TimeString += " : ";

        if (minute < 10)
        TimeString += '0';
        TimeString += String(minute);
        TimeString += " : ";

        if (second < 10)
        TimeString += '0';
        TimeString += String(second);
      }

    }
  WiFiClient client = server.available();
  if (!client)
  {
    return;
  }

  //Response

String s= "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n <!DOCTYPE html>\ <html> <head> <title>Location GPS</title> <style>";
s += "* {box-sizing: border-box; margin: 0; padding: 0; }";
s += "body {font-family: Georgia, \"Times New Roman\", Times, serif; color: #424242; background-image:linear-gradient(rgba(0, 0, 0, 0), rgba(0, 0, 0, 0.7)),url(https://c4.wallpaperflare.com/wallpaper/381/165/306/simple-background-black-background-minimalism-digital-art-map-hd-wallpaper-preview.jpg)}";
s += "h1 {font-size: 32px; color: white ; position: absolute; left: 70%; top: 20%; transform: translate(-50%, -50%); width: 50%; align-items: center;}";
s += "table {position: absolute; z-index: 2; left: 50%; top: 50%; transform: translate(-50%, -50%); width: 60%; border-collapse: collapse; border-spacing: 0; box-shadow: 0 2px 15px rgba(64, 64, 64, 0.7); border-radius: 12px 12px 0 0; overflow: hidden;linear-gradient(rgba(0, 0, 0, 0), rgba(0, 0, 0, 0.7));}";
s += "td, th {padding: 15px 20px; text-align: center; font-family:Cambria, Cochin, Georgia, Times, 'Times New Roman', serif; }";
s += "th {background-color:  linear-gradient(rgba(0, 0, 0, 0), rgba(0, 0, 0, 0.7)), #a0250691; color: #131315; font-family: Cambria, Cochin, Georgia, Times, 'Times New Roman', serif; font-weight: 600; text-transform: uppercase; font-family: Georgia, \"Times New Roman\", Times, serif;}";
s += "tr {width: 100%; background-color: #fafafa; font-family: Cambria, Cochin, Georgia, Times, 'Times New Roman', serif;}";
s += "tr:nth-child(even) {background-color: #bc9374;}";
s += "td {color: rgb(14, 14, 133); font-family: Cambria, Cochin, Georgia, Times, \"Times New Roman\", serif; font-weight: bold;}";

s += "</style> </head> <body>";
s += "<h1>GPS Reading</h1>";
s += "<table>";
s += "<tr> <th>Latitude</th> <td>" + LatitudeString + "</td> </tr>";
s += "<tr> <th>Longitude</th> <td>" + LongitudeString + "</td> </tr>";
s += "<tr> <th>Date</th> <td>" + DateString + "</td></tr>";
s += "<tr> <th>Time</th> <td>" + TimeString + "</td> </tr>";
s += "</table>";
s += "<a style=\"--clr: green\" class='button' href='http://maps.google.com/maps?&z=15&mrt=yp&t=k&q=" + LatitudeString + "+" + LongitudeString + "' target='_top'><button> Get Location </button></a>";
s += "<style>";
s += "button { outline: none; cursor: pointer; border: none; margin: 0; font-family: Cambria, Cochin, Georgia, Times, 'Times New Roman', serif; font-size: inherit; position: absolute; display: inline-block; letter-spacing: 0.05rem; font-weight: 200; font-size: 25px; border-radius: 10px; overflow: hidden; background: #66ff66; color:#a0250691; left: 70%; top: 70%; transform: translate(-50%, -50%);}";
s += "button span { position: relative; z-index: 10; transition: color 0.4s; color:#a0250691}";
s += "button:hover span {color:#a0250691 ;}";
s += "button::before, button::after {position: absolute; top: 0; left: 0; width: 100%; height: 100%; z-index: 0;}";
s += "button::before {content: \"\"; background: #000; width: 120%; left: -10%; transform: skew(30deg); transition: transform 0.4s cubic-bezier(0.3, 1, 0.8, 1);color:#a0250691 }";
s += "button:hover::before {transform: translate3d(100%, 0, 0);  color:#a0250691 }";

s += "</style> </body> </html> \n";

  client.print(s);
  delay(100);

}