#include <Arduino.h>
#include <IRremote.h>

class InfraRedReciever : public IRrecv
{

public:
    InfraRedReciever(int pin);
    int getKey();
private:
    decode_results results;

};