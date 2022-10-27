#include <Arduino.h>
#include <WiFiClient.h>
#include <HTTPClient.h>
#include <string>
#include <Base64.h>
#include <WebServer.h>
#include <ArduinoJson.h>
#include <SpotifyArduino.h>
#include "JsonParser.h"

class ApiCalls
{

public:
    ApiCalls(bool *needRefresh, String *authToken);
    String getCurrent(String s);
    void changeSong(String s);
    void playOrPause(String endpoint, String body);
    void shuffle(String endpoint);
    std::vector<JsonParser::playlistObject> getPlaylists();
    void getRecommended();

private:
    JsonParser *jsonParser;
    struct response
    {
        int httpCode;
        String responseBody;
    };
    
    response apiConnection(String req, String endpoint, String body);
    bool *needRefresh;
    String *authToken;
    bool playing = true;
    bool *pPlaying = &playing;
    bool foundSong = true;
    bool *pFoundSong = &foundSong;
    String prevSong = "";
    String *pPrevSong = &prevSong;
    String currentSong = "";
    String *pCurrentSong = &currentSong;
    String artist = "";
    String *pArtist = &artist;
    bool shuffleState = false;
};