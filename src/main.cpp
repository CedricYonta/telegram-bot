/*
  Titre      : communication zith TelegramBot
  Auteur     : Cedric Yonta
  Date       : 10/04/2023
  Description: Une description du programme
  Version    : 0.0.1
*/



#include <Arduino.h>
#include <WiFiNINA.h>
#include <WiFiSSLClient.h>
#include <ArduinoJson.h>
#include <ArduinoHttpClient.h>

#include <UniversalTelegramBot.h>
//#include <DHT.h>

#define WIFI_SSID "La casa"
#define WIFI_PASS "Element1"
#define BOT_TOKEN "6107678400:AAFKFlcRXA3Qml2WKe4_QZSv58YM9Ood6Bo"

// Remplacez l'ID du chat ci-dessous par l'ID de votre chat Telegram
#define CHAT_ID "5288820480"
//:AAFKFlcRXA3Qml2WKe4_QZSv58YM9Ood6Bo

// Initialisez l'objet WiFiClientSecure et l'objet UniversalTelegramBot
WiFiSSLClient client;

//HttpClient http(client, "api.telegram.org", 443);
UniversalTelegramBot bot(BOT_TOKEN, client);

int botRequestDelay = 1000;
unsigned long lastTimeBotRan;
unsigned long lastNotificationTime;
int delayNotification = 120000;
int nombreNotification ;

float temperature = 27 ; 


// Get BME280 sensor readings and return them as a String variable
String getReadings(){
  float temperature = 27 ; 
  float humidity = 80;
  String message = "Temperature: " + String(temperature) + " ºC \n";
  message += "Humidity: " + String (humidity) + " % \n";
  return message;
}


//Handle what happens when you receive new messages
void handleNewMessages(int numNewMessages) {
  Serial.println("handleNewMessages");
  Serial.println(String(numNewMessages));
   String chat_id;

  for (int i=0; i<numNewMessages; i++) {
    // Chat id of the requester
     chat_id = String(bot.messages[i].chat_id);
    if (chat_id != CHAT_ID){
      bot.sendMessage(chat_id, "Unauthorized user", "");
      continue;
    }

   // Print the received message
    String text = bot.messages[i].text;
    Serial.println(text);

    String from_name = bot.messages[i].from_name;

    if (text == "/start") {
      String welcome = "Welcome, " + from_name + ".\n";
      welcome += "Use the following command to get current readings.\n\n";
      welcome += "/readings \n";
      bot.sendMessage(chat_id, welcome, "");
    }

    if (text == "/readings") {
      String readings = getReadings();
      bot.sendMessage(chat_id, readings, "");
    }  


  }

   
}

  



void setup() {
  // Initialisez la communication série
  Serial.begin(9600);


// Se connecter au Wi-Fi
  while (WiFi.begin(WIFI_SSID, WIFI_PASS) != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connexion au Wi-Fi...");
  }
  
  // Imprimer l'adresse IP locale
  Serial.print("Adresse IP locale: ");
  Serial.println(WiFi.localIP());

    }

void loop() {

   /*if (millis() > lastTimeBotRan + botRequestDelay)  {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    while(numNewMessages) {
      Serial.println("got response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    lastTimeBotRan = millis();


        

  }*/

       if (temperature == 27)
      {

       // 

        if (nombreNotification == 0)
        {
           String message = "Temperature: " + String(temperature) + " ºC \n";
            bot.sendMessage(CHAT_ID, message, "");
            Serial.println("Motion Detected");
            lastNotificationTime = millis();
            nombreNotification += 1;
        }
        else
        {
           if ((millis()-lastNotificationTime) == 60000)
           {
               String message = "Temperature: " + String(temperature) + " ºC \n";
               bot.sendMessage(CHAT_ID, message, "");
               Serial.println("Motion Detected");
               lastNotificationTime = millis();
           }
        }
        
        
        
      }
  

  
}
