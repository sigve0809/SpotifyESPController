#include <Arduino.h>
#include <WiFi.h>
#include "WebServerCallback.h"
#include "Playlists.h"

char ssid[] = "********";                // your network SSID (name)
char password[] = "********";; // your network password
bool needRefresh = true;
bool *pNeedRefresh = &needRefresh;

bool selectList = false;
bool *pSelectList = &selectList;

WebServerCallback *wsCallback;
ApiCalls *apiCalls;
InfraRedReciever *infraRedReciever;
Playlists *playlists;
TFTManager *tftManager;

String authToken = "";
String *pAuthToken = &authToken;

const int buttonPin1 = 33; // the number of the pushbutton pin
const int buttonPin2 = 35;
const int buttonPin3 = 32;

// variables will change:
int buttonState1 = 0; // variable for reading the pushbutton status
int buttonState2 = 0;
int buttonState3 = 0;

String clientId = "*************";
String clientSecret = "****************";

unsigned long lastTime = 0;
// Timer set to 10 minutes (600000)
// unsigned long timerDelay = 600000;
// Set timer to 5 seconds (5000)
unsigned long timerDelay = 10000;
void setup()
{

  Serial.begin(9600);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  IPAddress ipAddress = WiFi.localIP();
  Serial.println(ipAddress);

  WiFiClientSecure client;
  wsCallback = new WebServerCallback(80, client, clientId, clientSecret, "", pNeedRefresh, pAuthToken);
  apiCalls = new ApiCalls(pNeedRefresh, pAuthToken);
  infraRedReciever = new InfraRedReciever(26);
  tftManager = new TFTManager(2, 15, -1);
  playlists = new Playlists(apiCalls, infraRedReciever, tftManager, pSelectList);

  wsCallback->getAuthToken();
  pinMode(buttonPin1, INPUT);
  pinMode(buttonPin2, INPUT);
  pinMode(buttonPin3, INPUT);
}

void loop()
{
  buttonState1 = digitalRead(buttonPin1);
  buttonState2 = digitalRead(buttonPin2);
  buttonState3 = digitalRead(buttonPin3);
  delay(100);

  int remote = infraRedReciever->getKey();

  if (selectList == true)
    playlists->findPlaylist(remote);

  else
  {
    if ((millis() - lastTime) > timerDelay)
    {
      if (!needRefresh)
        apiCalls->getCurrent("");

      else
        wsCallback->getAuthToken();

      lastTime = millis();
    }
    if (buttonState1 == HIGH || remote == 16720605)
      apiCalls->changeSong("/previous");

    else if (buttonState2 == HIGH || remote == 16761405)
      apiCalls->changeSong("/next");

    else if (buttonState3 == HIGH || remote == 16712445)
      apiCalls->playOrPause("pause", "{}");
    else if (remote == 16756815)
      apiCalls->shuffle("/shuffle");
    else if (remote == 16738455)
      apiCalls->getRecommended();
  }

  if (remote == 16769565)
  {
    selectList = selectList ? false : true;
    if (selectList == true)
    {
      playlists->init();
    }
  }
}
