#include "Playlists.h"

Playlists::Playlists(ApiCalls *apiCalls, InfraRedReciever *infraRedReciever, TFTManager *tftManager, bool *selectList)
    : apiCalls(apiCalls), infraRedReciever(infraRedReciever), tftManager(tftManager), selectList(selectList) {}

void Playlists::init()
{
    playlist = apiCalls->getPlaylists();
    ptr = playlist.begin();
    Serial.println("Created playlist");
}

void Playlists::findPlaylist(int key)
{
    // Forward
    if (playlist.size() > 0)
    {
        if (key == 16769055)
        {
            if (ptr >= playlist.end())
                ptr = playlist.begin();
            else
                ++ptr;
        }
        // backward
        if (key == 16748655)
        {
            if (ptr < playlist.begin())
                ptr = playlist.end();
            else
                --ptr;
        }
        tftManager->writeText(ptr->name, "", ST77XX_GREEN, 2);
        if (key == 16712445)
        {
            apiCalls->playOrPause("/play", "{\"context_uri\": \"" + ptr->url + "\",\"offset\":{\"position\":0},\"position_ms\": 0}");
            *selectList = false;
        }
    }
}