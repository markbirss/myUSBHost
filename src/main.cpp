#include "EspUsbHost.h"
#include <Wire.h>

#if defined(myATOM_S3)

//Meshtastic Logo
#include "mt.c"

#include <M5AtomS3.h>
 //https://docs.m5stack.com/en/core/AtomS3
 #define SDA_PIN 2 
 #define SCL_PIN 1  
#elif defined(mySTAMP_S3)
 //https://docs.m5stack.com/en/core/stamps3
 #define SDA_PIN  13
 #define SCL_PIN  15  

#endif

uint8_t keyPressed=0;
uint8_t KEY=0;

class MyEspUsbHost : public EspUsbHost 
{
  void onKeyboardKey(uint8_t ascii, uint8_t keycode, uint8_t modifier) {
    KEY=ascii;keyPressed=1;
    if (' ' <= ascii && ascii <= '~') {

      #if defined(myATOM_S3)
      M5.Lcd.clear();
      M5.Lcd.setCursor(48,48);
      M5.Lcd.setTextSize(6);
      char c;
      c=("%c", ascii);
      M5.Lcd.println(c);
      #endif

    } else if (ascii == '\r') {

      #if defined(myATOM_S3)
      M5.Lcd.clear();
      M5.Lcd.setCursor(5,48);
      M5.Lcd.setTextSize(4);
      M5.Lcd.println("Enter");
      #endif
    }
  };
};

MyEspUsbHost usbHost;
void requestEvent() 
{
  if (keyPressed == 1) {
    Wire.write(KEY);
    keyPressed = 0;
  } else {
    Wire.write(0);
  }
}

void setup() 
{
  usbHost.begin();
  //usbHost.setHIDLocal(HID_LOCAL_Japan_Katakana);

  Wire.onRequest(requestEvent);
  Wire.setPins(SDA_PIN, SCL_PIN);
  Wire.begin(0x5F);

#if defined(myATOM_S3)

  M5.begin(true, false, false, false);  // Init M5AtomS3
  M5.Lcd.setRotation(2);
  M5.Lcd.pushImage(0, 0, mtWidth, mtHeight, mt);
  delay(3500);
  M5.Lcd.clear();
  M5.Lcd.setCursor(0,0);
  M5.Lcd.setTextSize(4);

#endif

}

void loop() 
{
usbHost.task();

#if defined(myATOM_S3)
  M5.update();
  if (M5.Btn.wasReleased() || M5.Btn.pressedFor(1000)) {
    M5.Lcd.clear();
    M5.Lcd.setCursor(0,0);
  }
#endif

}