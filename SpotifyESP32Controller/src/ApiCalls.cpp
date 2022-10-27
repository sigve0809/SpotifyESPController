#include "ApiCalls.h"
ApiCalls::ApiCalls(bool *needRefresh, String *authToken) : needRefresh(needRefresh), authToken(authToken)
{
    jsonParser = new JsonParser(pFoundSong, pPrevSong, pArtist, pPlaying);
}

ApiCalls::response ApiCalls::apiConnection(String req, String endpoint, String body)
{
    String serverPath = "https://api.spotify.com/v1" + endpoint;
    if (WiFi.status() == WL_CONNECTED)
    {
        HTTPClient http;
        int httpResponseCode = 0;
        http.begin(serverPath);

        String authorization = "Bearer " + *authToken;

        http.addHeader("Accept", "application/json");
        http.addHeader(F("Content-Type"), "application/json");
        http.addHeader(F("Authorization"), authorization);
        http.addHeader(F("Content-Length"), "0");
        http.addHeader(F("Host"), "api.spotify.com");
        if (req == "GET")
        {
            response getResponse;
            // endpoint=  /player/currently-playing?
            // Send HTTP GET request
            httpResponseCode = http.GET();
            // Building up callback URL using IP address.
            String payload;
            if (httpResponseCode == 200)
            {
                payload = http.getString();
                getResponse.httpCode = 200;
                getResponse.responseBody = payload;
                http.end();
                return getResponse;
            }
            // Free resources
            http.end();
            getResponse.httpCode = httpResponseCode;
            getResponse.responseBody = "Failed";
            return getResponse;
        }
        if (req == "POST")
        {
            response postResponse;
            int httpResponseCode = http.POST(body);
            Serial.println("HTTP Response code: " + httpResponseCode);
            //Serial.println(httpResponseCode);
            http.end();
            postResponse.httpCode = httpResponseCode;
            postResponse.responseBody = "";
            return postResponse;
        }
        if (req == "PUT")
        {
            response putResponse;

            // endpoint=  /player/next
            String url = "https://api.spotify.com/v1" + endpoint;
            Serial.print(url);
            Serial.print(" returned: ");
            httpResponseCode = http.PUT(body);
            putResponse.httpCode = httpResponseCode;
            putResponse.responseBody = "";
            http.end();
            return putResponse;
        }
    }
    response finalResponse;
    finalResponse.httpCode = -1;
    finalResponse.responseBody = "";
    return finalResponse;
}

String ApiCalls::getCurrent(String s)
{
    response getResponse = apiConnection("GET", "/me/player/currently-playing?market=NO", "");
    if (getResponse.httpCode == 204)
    {
        Serial.println("Nothing Playing");
        playing = false;
        return "";
    }
    if (getResponse.httpCode != 200)
    {
        *needRefresh = true;
        return "Getting data";
    }

    if (getResponse.httpCode > 0)
    {
        Serial.print("HTTP Response code: ");
        Serial.println(getResponse.httpCode);
        if (getResponse.httpCode == 200)
        {
            jsonParser->parseCurrent(getResponse.responseBody.c_str());
        }
        return prevSong;
    }
    return prevSong;
}

void ApiCalls::changeSong(String s)
{
    response postResponse = apiConnection("POST", "/me/player" + s, "");
    getCurrent("");

    Serial.print("HTTP Response code: ");
    Serial.println(postResponse.httpCode);
}
void ApiCalls::playOrPause(String endpoint, String body)
{
    // String body = "{\"context_uri\": \""+playingURL+"\",\"offset\":{\"position\":56},\"position_ms\":"+positionMS+"}";
    if (body.length() > 2)
    {
        apiConnection("PUT", "/me/player/play", body);
        //Serial.println("Failed");
    }
    else if (playing)
        apiConnection("PUT", "/me/player/pause", body);
    else
        apiConnection("PUT", "/me/player/play", body);
    playing = playing ? false : true;
}
// player/shuffle
void ApiCalls::shuffle(String endpoint)
{
    String body = "{}";
    if (shuffleState == false)
        apiConnection("PUT", "/me/player/shuffle?state=true", body);
    else
        apiConnection("PUT", "/me/player/shuffle?state=false", body);
    shuffleState = shuffleState ? false : true;
}

std::vector<JsonParser::playlistObject> ApiCalls::getPlaylists()
{
    // in getPlaylist
    std::vector<JsonParser::playlistObject> playlist;
    response getResponse = apiConnection("GET", "/me/playlists?limit=20&offset=0", "");
    if (getResponse.httpCode != 200)
    {
        *needRefresh = true;
        Serial.print("HTTP Response code: ");
        Serial.println(getResponse.httpCode);
    }

    if (getResponse.httpCode > 0)
    {
        Serial.print("HTTP Response code: ");
        Serial.println(getResponse.httpCode);
        if (getResponse.httpCode == 200)
        {
            // Serial.println(getResponse.responseBody);
            playlist = jsonParser->parseplaylists(getResponse.responseBody.c_str());
        }
    }
    return playlist;
}
void ApiCalls::getRecommended()
{
    ApiCalls::response resp = apiConnection("GET", "/recommendations?limit=1&market=ES&seed_artists=4NHQUGzhtTLFvgF5SZesLK&seed_genres=classical%2Ccountry&seed_tracks=0c6xIDDpzE81m2q797ordA", "");
    JsonParser::recommended test = jsonParser->parseRecommended(resp.responseBody);
    playOrPause("" ,"{\"context_uri\": \""+test.url+"\",\"offset\":{\"position\":"+test.offset+"},\"position_ms\": 0}");

    //Serial.println(resp.responseBody);

}