#include <Arduino.h>
#include <WiFiClient.h>
#include <HTTPClient.h>
#include <string>
#include <Base64.h>
#include <WebServer.h>
#include <ArduinoJson.h>



#include <SpotifyArduino.h>
// Library for connecting to the Spotify API

// Install from Github
// https://github.com/witnessmenow/spotify-api-arduino

// including a "spotify_server_cert" variable
// header is included as part of the SpotifyArduino libary
//#include <SpotifyArduinoCert.h>
class WebServerCallback : WebServer, SpotifyArduino
{
public:
  WebServerCallback(int port, Client &client, const char *clientId, const char *clientSecret, const char *refreshToken, bool *needRefresh, String *authToken);

  String ParseJson(String key, String json);
  String getAuthToken();

  void handleRoot();

  void handleCallback();

  void handleNotFound();

private:
  const char *webpageTemplate =
      R"(
<!DOCTYPE html>
<html>
  <head>
    <meta charset="utf-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no" />
  </head>
  <body>
    <div>
     <a href="https://accounts.spotify.com/authorize?client_id=%s&response_type=code&redirect_uri=%s&scope=%s">spotify Auth</a>
    </div>
  </body>
</html>
)";
  WiFiClientSecure client;

  String *authToken;
  String pscope = "user-read-playback-state%20user-modify-playback-state%20playlist-read-private";
  const char *callbackURI;
  bool *needRefresh;
  String pclientId = "03fbc0234d1b48de8f32d01ba7c6402d";     // Your client ID of your spotify APP
  String pclientSecret = "3e58c52ae52e4b6c8310bcd7e20c390f"; // Your client Secret of your spotify APP (Do Not share this!)
};