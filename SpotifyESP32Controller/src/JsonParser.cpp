#include "JsonParser.h"

JsonParser::JsonParser(bool *foundSong, String *prevSong, String *artist, bool *playing) : foundSong(foundSong), prevSong(prevSong), artist(artist), playing(playing)
{
    tftManager = new TFTManager(2, 15, -1);
}

void JsonParser::parseCurrent(String input)
{
    DynamicJsonDocument doc(4096);

    DeserializationError error = deserializeJson(doc, input);

    if (error)
    {
        Serial.print("deserializeJson() failed: ");
        Serial.println(error.c_str());
        return;
    }

    long long timestamp = doc["timestamp"]; // 1660050113081

    JsonObject context = doc["context"];

    const char *context_external_urls_spotify = context["external_urls"]["spotify"];

    const char *context_type = context["type"]; // "playlist"
    const char *context_uri = context["uri"];   // "spotify:playlist:6qfALrUPbtYdigF6xlzCc2"

    long progress_ms = doc["progress_ms"]; // 123320

    JsonObject item = doc["item"];

    JsonObject album = item["album"];

    JsonObject album_artists_0 = album["artists"][0];

    const char *album_artists_0_id = album_artists_0["id"];     // "3fMbdgg4jU18AjLCKBhRSm"
    const char *album_artists_0_name = album_artists_0["name"]; // "Michael Jackson"
    const char *album_artists_0_uri = album_artists_0["uri"];   // "spotify:artist:3fMbdgg4jU18AjLCKBhRSm"

    const char *album_external_urls_spotify = album["external_urls"]["spotify"];

    const char *album_href = album["href"]; // "https://api.spotify.com/v1/albums/24TAupSNVWSAHL0R7n71vm"
    const char *album_id = album["id"];     // "24TAupSNVWSAHL0R7n71vm"

    for (JsonObject album_image : album["images"].as<JsonArray>())
    {

        int album_image_height = album_image["height"]; // 640, 300, 64
        const char *album_image_url = album_image["url"];
        int album_image_width = album_image["width"]; // 640, 300, 64
    }

    const char *album_name = album["name"];                 // "Bad 25th Anniversary"
    const char *album_release_date = album["release_date"]; // "1987-08-31"
    int album_total_tracks = album["total_tracks"];         // 24
    const char *album_uri = album["uri"];                   // "spotify:album:24TAupSNVWSAHL0R7n71vm"

    JsonObject artists_0 = item["artists"][0];

    const char *artists_0_name = artists_0["name"]; // "Michael Jackson"

    int disc_number = item["disc_number"];  // 1
    long duration_ms = item["duration_ms"]; // 318680

    const char *name = item["name"];     // "Man in the Mirror - 2012 Remaster"
    bool is_playing = doc["is_playing"]; // false
    *foundSong = true;
    *prevSong = name;
    *artist = album_artists_0_name;
    *playing = is_playing;
    tftManager->writeText(*prevSong, *artist, ST77XX_GREEN, 2);
}

std::vector<JsonParser::playlistObject> JsonParser::parseplaylists(String input)
{

    std::vector<playlistObject> playlistVector;
    DynamicJsonDocument doc(8192);

    DeserializationError error = deserializeJson(doc, input);

    if (error)
    {
        Serial.print("deserializeJson() failed: ");
        Serial.println(error.c_str());
        return playlistVector;
    }

    const char *href = doc["href"];

    for (JsonObject item : doc["items"].as<JsonArray>())
    {
        playlistObject myObject;
        const char *external_urls_spotify = item["external_urls"]["spotify"];

        const char *href = item["href"]; // "https://api.spotify.com/v1/playlists/59N4ba6Elc8DamBWxkrN5M", ...
        const char *id = item["id"];     // "59N4ba6Elc8DamBWxkrN5M", "5GUMVSMSyVII0n3JGB4Jyz", ...

        const char *name = item["name"]; // "ChillRock", "Summer", "ReLaX", "Old Mix"
        const char *uri = item["uri"];   // "spotify:playlist:59N4ba6Elc8DamBWxkrN5M", ...
        myObject.name = name;
        myObject.url = uri;
        playlistVector.emplace_back(myObject);
    }
    for (auto &&i : playlistVector)
    {
        Serial.println(i.name);
    }
    return playlistVector;
}
JsonParser::recommended JsonParser::parseRecommended(String input)
{
    StaticJsonDocument<96> filter;

    JsonObject filter_tracks_0 = filter["tracks"].createNestedObject();
    filter_tracks_0["album"]["external_urls"]["spotify"] = true;
    filter_tracks_0["track_number"] = true;

    StaticJsonDocument<256> doc;

    DeserializationError error = deserializeJson(doc, input, DeserializationOption::Filter(filter));
    recommended r;
    if (error)
    {
        Serial.print("deserializeJson() failed: ");
        Serial.println(error.c_str());
        r.offset = 0;
        r.url = "fail";
        return r;
    }

    const char *tracks_0_album_external_urls_spotify = doc["tracks"][0]["album"]["external_urls"]["spotify"];

    int tracks_0_track_number = doc["tracks"][0]["track_number"]; // 9

    Serial.println("tracks_0_name");
    String uri = tracks_0_album_external_urls_spotify;

    r.offset = tracks_0_track_number;
    r.url = tracks_0_album_external_urls_spotify;
    
    return r;
}