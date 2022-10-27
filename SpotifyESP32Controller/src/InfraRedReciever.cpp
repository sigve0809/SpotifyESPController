#include "InfraRedReciever.h"

InfraRedReciever::InfraRedReciever(int pin) : IRrecv(pin){enableIRIn();}
int InfraRedReciever::getKey(){
    if (decode(&results))
    {
        Serial.println(results.value);
        resume();
        return results.value;
    }
    return 0;
}