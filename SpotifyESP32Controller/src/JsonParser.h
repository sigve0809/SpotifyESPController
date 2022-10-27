#include <Arduino.h>
#include <WiFiClient.h>
#include <HTTPClient.h>
#include <string>
#include <Base64.h>
#include <WebServer.h>
#include <ArduinoJson.h>
#include <SpotifyArduino.h>
#include <vector>
#include "TFTManager.h"

class JsonParser
{
public:
    JsonParser(bool *foundSong, String *prevSong, String *artist, bool *playing);

    void parseCurrent(String input);

    struct recommended
    {
        String url;
        int offset;
    };
    recommended parseRecommended(String input);
    struct playlistObject
    {
        String url;
        String name;
    };
    std::vector<playlistObject> parseplaylists(String input);
    


private:
    TFTManager *tftManager;
    bool *foundSong;
    String *prevSong;
    String *artist;
    bool *playing;
};