#include "WebServerCallback.h"

WebServerCallback::WebServerCallback(int port, Client &client, const char *clientId, const char *clientSecret, const char *refreshToken, bool *needRefresh, String *authToken)
    : WebServer(port), SpotifyArduino(client, clientId, clientSecret, refreshToken), needRefresh(needRefresh), authToken(authToken)
{
    IPAddress ipAddress = WiFi.localIP();
    String test = "%s%s%shttp%3A%2F%2F" + ipAddress.toString() + "%2Fcallback%2F";
    callbackURI = test.c_str();
    //sprintf(callbackURI, "%s%s%s", "http%3A%2F%2F", ipAddress.toString().c_str(), "%2Fcallback%2F");
}
String WebServerCallback::ParseJson(String key, String json)
  {
    String retVal = "";
    int index = json.indexOf(key);

    // Serial.print( "index: " );
    // Serial.println(index);
    if (index > 0)
    {
      bool copy = false;
      for (int i = index; i < json.length(); i++)
      {
        // Serial.print( "charAt: " );
        // Serial.println( json.charAt(i) );
        if (copy)
        {
          if (json.charAt(i) == '"' || json.charAt(i) == ',')
          {
            break;
          }
          else
          {
            retVal += json.charAt(i);
          }
        }
        else if (json.charAt(i) == ':')
        {
          copy = true;
          if (json.charAt(i + 1) == '"')
          {
            i++;
          }
        }
      }
    }
    return retVal;
  }
  String WebServerCallback::getAuthToken()
  {
    if (WiFi.status() == WL_CONNECTED)
    {
      WiFiClient client;
      HTTPClient http;

      String clientID = "03fbc0234d1b48de8f32d01ba7c6402d";
      String clientSECRET = "3e58c52ae52e4b6c8310bcd7e20c390f";
      String body = "grant_type=refresh_token&refresh_token=AQAelUd8ductxXhvp-o5rrX_sQuVxsBeMp1UpMmUioctnJUjGarnJfcl3v3Abi7EM0Fn8V7nGEWohN9Zkp0HtnvZEcSC43kv1mUd6tIiNFxcc5ns-2DURBwRi8Ylew2hxYs";
      String authorizationRaw = clientID + ":" + clientSECRET;
      String authorization = base64::encode(authorizationRaw);
      // Your Domain name with URL path or IP address with path
      http.begin("https://accounts.spotify.com/api/token");
      http.addHeader("Content-Type", "application/x-www-form-urlencoded");
      http.addHeader("Authorization", "Basic " + authorization);
      // Send HTTP POST request
      int httpCode = http.POST(body);
      String payload = http.getString();

      if (httpCode == 200)
      {
        String accessToken = ParseJson("access_token", payload);
        Serial.println(accessToken);
        *authToken = accessToken;
        Serial.println("Got Token");
        http.end();
        *needRefresh = false;
        return accessToken;
      }
      else
      {
        Serial.print("BadRequest: ");
        Serial.println(httpCode);
        Serial.println(payload);
      }

      Serial.print("HTTP Response code: ");
      Serial.println(httpCode);

      // Free resources
      http.end();
    }
    else
    {
      Serial.println("WiFi Disconnected");
    }
    return "ERROR";
  }

  void WebServerCallback::handleRoot()
  {
    char webpage[800];
    sprintf(webpage, webpageTemplate, pclientId, callbackURI, pscope);
    send(200, "text/html", webpage);
  }

  void WebServerCallback::handleCallback()
  {
    String code = "";
    const char *refreshToken = NULL;
    for (uint8_t i = 0; i < args(); i++)
    {
      if (argName(i) == "code")
      {
        code = arg(i);
        refreshToken = requestAccessTokens(code.c_str(), callbackURI);
      }
    }

    if (refreshToken != NULL)
    {
      send(200, "text/plain", refreshToken);
    }
    else
    {
      send(404, "text/plain", "Failed to load token, check serial monitor");
    }
  }

  void WebServerCallback::handleNotFound()
  {
    String message = "File Not Found\n\n";
    message += "URI: ";
    message += uri();
    message += "\nMethod: ";
    message += (method() == HTTP_GET) ? "GET" : "POST";
    message += "\nArguments: ";
    message += args();
    message += "\n";

    for (uint8_t i = 0; i < args(); i++)
    {
      message += " " + argName(i) + ": " + arg(i) + "\n";
    }

    Serial.print(message);
    send(404, "text/plain", message);
  }