#include <Adafruit_ST7789.h>
#include <Adafruit_GFX.h>    // Core graphics library
#include <SPI.h>
class TFTManager: public Adafruit_ST7789 {
  public:
    TFTManager(int DC, int CS, int RST);
    void writeText(String text1, String text2, uint16_t color, int tsize);
    void initialise();
  private:
    byte DC, CS, RST;
    String prevText1 = "";
    String prevText2 = "";

};