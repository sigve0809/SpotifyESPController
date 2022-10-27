#include "TFTManager.h"

TFTManager::TFTManager(int DC, int CS, int RST): Adafruit_ST7789(CS, DC, RST) {initialise();}

void TFTManager::writeText(String text1,String text2, uint16_t color, int tsize)
{
  //Updates where its needed
  setRotation(1);
  if(prevText1 != text1){
    prevText1 = text1;
    fillRect(0, 20, 240, 50, ST77XX_BLACK);
    setCursor(5, 20);
    setTextSize(tsize);
    setTextColor(color);
    print(text1);
  }
  if(prevText2 != text2){
    fillRect(5, 60, 240, 100, ST77XX_BLACK);
    prevText2 = text2;
    setCursor(5, 60);
    setTextSize(tsize);
    setTextColor(ST77XX_WHITE);
    print(text2);
  }

}
//Initializes at beginning so tft has black background
void TFTManager::initialise() {
  init(135, 240); // Init ST7789 240x135
  fillScreen(ST77XX_BLACK);
  Serial.print("Done");

}