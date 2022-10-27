#include <Arduino.h>
#include "InfraRedReciever.h"
#include "ApiCalls.h"
class Playlists
{

public:

    Playlists(ApiCalls *apiCalls, InfraRedReciever *infraRedReciever, TFTManager *tftManager, bool *selectList);
    void findPlaylist(int key);
    void init();
private:
    bool *selectList;
    ApiCalls *apiCalls;
    InfraRedReciever *infraRedReciever;
    TFTManager *tftManager;
    std::vector<JsonParser::playlistObject> playlist;
    std::vector<JsonParser::playlistObject>::iterator ptr;
};