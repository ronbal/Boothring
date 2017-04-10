
// to use SerialUI, you need to include the main header:
#include <SerialUI.h>
#include <SPI.h>
#include <SD.h>
//#include <EEPROM.h>
#include <Adafruit_NeoPixel.h>
//#include <SoftwareSerial.h>
#include <avr/dtostrf.h>




File myFile;
int pixel = 60;
long sekunden = 4;
int helligkeit = 50;
int ring_r_f = 255;  //Ring RED Value foreground
int ring_g_f = 0;    //Ring green value background
int ring_b_f = 0;  //Ring blue value foreground
int ring_r_b = 0;  //Ring red value background
int ring_g_b = 0;  //Ring green value background
int ring_b_b = 255;  //Ring blue value background
int druckend = 50;
int sersch = 1;
int boothr = 0;
int boothg = 0;
int boothb = 255;
int boothli = 29;

#define serielle &Serial
#define serielle1 &Serial1

Adafruit_NeoPixel ring = Adafruit_NeoPixel(255, 5, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel printerl = Adafruit_NeoPixel(255, 4, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel boothl = Adafruit_NeoPixel(255, 3, NEO_GRB + NEO_KHZ800);

#define serial_baud_rate           57600
#define serial_input_terminator   '\r'

#define ledpin  5 // Set Pin for NEopixel
#define PIN 10 //Pin der angeschlossenen LEDS
#define rxPin 9
#define txPin 10




SUI_DeclareString(device_greeting,
 "+++ Welcome to the BoothRing Menu +++\r\nEnter ? for help.");

SUI_DeclareString(top_menu_title, "BoothRing Main Menu");


SUI_DeclareString(enable_key, "animations");
SUI_DeclareString(enable_help, "View programmed Animations");

SUI_DeclareString(settings_title, "SuperBlinker Settings");
SUI_DeclareString(settings_key, "settings");
SUI_DeclareString(settings_help, "Perform setup and config");

SUI_DeclareString(info_key, "info");
SUI_DeclareString(info_help, "view BoothRing Settings");




//********************************RING WERDE SETZEN***********************************************
SUI_DeclareString(enable_r_r_f_key, "r_r_f"); 
SUI_DeclareString(r_r_f_help, "RED value foreground");
SUI_DeclareString(enable_r_g_f_key, "r_g_f"); 
SUI_DeclareString(r_g_f_help, "GREEN value foreground");
SUI_DeclareString(enable_r_b_f_key, "r_b_f"); 
SUI_DeclareString(r_b_f_help, "BLUE value foreground");

SUI_DeclareString(enable_r_r_b_key, "r_r_b"); 
SUI_DeclareString(r_r_b_help, "RED Value background");
SUI_DeclareString(enable_r_g_b_key, "r_g_b"); 
SUI_DeclareString(r_g_b_help, "GREEN value background");
SUI_DeclareString(enable_r_b_b_key, "r_b_b"); 
SUI_DeclareString(r_b_b_help, "BLUE value background");
//********************************RING WERDE SETZEN***********************************************


//********************************Boothlight setzen***********************************************
SUI_DeclareString(enable_boothr_key, "boothr"); 
SUI_DeclareString(boothr_help, "Boothlight RED value");
SUI_DeclareString(enable_boothg_key, "boothg"); 
SUI_DeclareString(boothg_help, "Boothlight GREEN value");
SUI_DeclareString(enable_boothb_key, "boothb"); 
SUI_DeclareString(boothb_help, "Boothlight BLUE value");
SUI_DeclareString(enable_boothli_key, "boothli"); 
SUI_DeclareString(boothli_help, "Boothlight PIXEL value");
//********************************Boothlight setzen***********************************************

SUI_DeclareString(enable_count1_key, "count"); //RED Countdown
SUI_DeclareString(count1_help, "this is the standart countdown in your Colors");
SUI_DeclareString(enable_doublecount_key, "doublecount"); //RED Countdown
SUI_DeclareString(doublecount_help, "this is the double countdown in your Colors");
SUI_DeclareString(enable_count2_key, "theatre"); //GREEN Countdown
SUI_DeclareString(count2_help, "theatre Animation in fore- & background colors");
SUI_DeclareString(enable_theatyell_key, "yeltheat"); //GREEN Countdown
SUI_DeclareString(theatyell_help, "yellow theatre Animation");
SUI_DeclareString(enable_theatgreen_key, "gretheat"); //GREEN Countdown
SUI_DeclareString(theatgreen_help, "green theatre Animation");
SUI_DeclareString(enable_theatblue_key, "blutheat"); //GREEN Countdown
SUI_DeclareString(theatblue_help, "blue theatre Animation");
SUI_DeclareString(enable_theatred_key, "redtheat"); //GREEN Countdown
SUI_DeclareString(theatred_help, "red theatre Animation");

SUI_DeclareString(enable_rainbow_key, "rainbow"); //RED Countdown
SUI_DeclareString(rainbow_help, "Rainbow animation");
SUI_DeclareString(enable_on_key, "on");
SUI_DeclareString(enable_off_key, "off");
SUI_DeclareString(enable_printer_key, "printer");
SUI_DeclareString(settings_serial_key, "Serial");
SUI_DeclareString(settings_serial_help, "select 1 for bluetooth / 0 for USB connection");
SUI_DeclareString(settings_pixel_key, "Pixel");
SUI_DeclareString(settings_seconds_key, "seconds");
SUI_DeclareString(settings_seconds_help, "Set here the Countdown time for the BoothRing");
SUI_DeclareString(settings_pixel_help, "Set number of pixels on the ring Value 0 - 255");
SUI_DeclareString(settings_brightness_key, "Brightness");
SUI_DeclareString(settings_brightness_help, "Set the brightness of the lights. Value 0 - 255");
SUI_DeclareString(settings_printend_key, "PrinterEnd");
SUI_DeclareString(settings_printend_help, "Set the end Pixel of the Printer light");
SUI_DeclareString(settings_devid_key, "deviceid");
SUI_DeclareString(settings_devid_help, "Set dev ID [string]");
SUI_DeclareString(settings_show_key, "show");

SUI_DeclareString(exit_key, "exit");
SUI_DeclareString(exit_help, "exit (and terminate Druid)");



// a few labels for our variable auto-tracking
SUI_DeclareString(enabled_label, "enabled");

SUI::SerialUI mySUI(device_greeting,1,&Serial1);

//SUI::SerialUI mySUI(device_greeting);
#define dev_id_maxlen  30

// deviceInfo struct, a place to store our settings
typedef struct deviceInfo {

  unsigned long red;
  unsigned long green;
  unsigned long blue;
  char dev_id[dev_id_maxlen + 1];
  bool state;
}
deviceInfo;

// Just declare a global deviceInfo structure for

// use below, initialized to all-zeros:
deviceInfo myDevice = {0};
// NOTE: In real life, I think the device resets as we
// close the serial connection, meaning that if you want
// your config to be permanent, you probably have to save
// it to eeprom or somesuch.




void setup()
{
  
Serial1.begin(serial_baud_rate);
Serial.begin(serial_baud_rate);



mySUI.print("Initializing SD card...");
 // On the Ethernet Shield, CS is pin 4. It's set as an output by default.
 // Note that even if it's not used as the CS pin, the hardware SS pin
 // (10 on most Arduino boards, 53 on the Mega) must be left as an output
 // or the SD library functions will not work.
 pinMode(10, OUTPUT);
 if (!SD.begin(10)) {

//writeSDSettings();   

   mySUI.println("SD CARD initialization failed! check if the Card is inserted!!!");
 return;
 }
 mySUI.println("SD CARD initialization done.");

   readdata();

   delay(1000);


if (sersch < 1){
SUI::SerialUI mySUI(device_greeting,0,serielle);
}
else{
  SUI::SerialUI mySUI(device_greeting,0,serielle1);
  }
  
mySUI.print("Serielle schnittstelle: ");
mySUI.println(sersch);
mySUI.begin(serial_baud_rate);

show_info();
  
  mySUI.setTimeout(20000);      // timeout for reads (in ms), same as for Serial.
  mySUI.setMaxIdleMs(30000);    // timeout for user (in ms)
  // how we are marking the "end-of-line" (\n, by default):
  mySUI.setReadTerminator(serial_input_terminator);


  // Setup variable state tracking -- will report changes to
  // Druid4Arduino (v >= 1.3.0) so it can automatically display
  // these in GUI.  See the VariableTracking example for more on this...
  mySUI.trackState(enabled_label, &(myDevice.state));
  mySUI.trackState(settings_pixel_key, &(myDevice.red));
  mySUI.trackState(settings_brightness_key, &(myDevice.green));
  mySUI.trackState(settings_printend_key, &(myDevice.blue));

  

  // The SerialUI menu setup is a bit involved, and it
  // needs to know about the functions we'll be using as
  // callbacks. Instead of having a bunch of function
  // declarations, all the work is contained in a function
  // of its own at the bottom of the program.
  // Yes: *DO* check it out!
  setupMenus();

  // set our blinker pin as an output.
  pinMode(ledpin, OUTPUT);
  
// *********************************************initiale Farbe setzen *******************
  //readdata();
  ring.begin();
  printerl.begin();
  boothl.begin();
boothlight();
  ring.setBrightness(helligkeit);
  for( uint16_t i=0; i< pixel ;  i++) {
      ring.setPixelColor(i, ring_r_b,ring_g_b,ring_b_b);
     
          
  }
   ring.show();
   delay(10);

}


boolean cur_blink_state = true;
void loop()
{
ring.setBrightness(helligkeit);



  /* We checkForUser() periodically, to see
  ** if anyone is attempting to send us some
  ** data through the serial port.
  **
  ** This code checks all the time, for 150 ms,
  ** upon entering the loop.  Should you want to
  ** check for user access only once (say have a
  ** 10 second wait on startup, and then forgo
  ** allowing SerialUI access), then increase the
  ** delay parameter and use checkForUserOnce(), e.g.
  **
  **    mySUI.checkForUserOnce(15000);
  **
  ** to allow 15 seconds to connect on startup only.
  */
if (mySUI.checkForUser())
{
    // we have a user initiating contact, show the
    // greeting message and prompt
   mySUI.enter();


    /* Now we keep handling the serial user's
    ** requests until they exit or timeout.
    */
    while (mySUI.userPresent())
    {
      // actually respond to requests, using
      mySUI.handleRequests();

      if (myDevice.state)
      {
        mySUI.print(F("ON at "));
        mySUI.println(millis());
      }
    }

  } /* end if we had a user on the serial line */


  // we toggle the LED pin just to show we're alive
  // and not currently processing serial interaction
  cur_blink_state = !cur_blink_state;
  digitalWrite(ledpin, cur_blink_state);


}


/*
** ********************* Callbacks ***********************
*/

// callback: turn_on
// set device (and the ledpin) on

void printer()
{
  readdata();
for (int x=0;x<2;x++){
uint16_t i, j;
for (int x=0; x<1;x++) {
  for(j=0; j<256; j++) {
    for(i=0; i<druckend; i++) {
      printerl.setPixelColor(i, Wheel((i+j) & 255));
    }
    printerl.show();
    
    //delay(1);
  }
}
}
  // do our business, normally you'd probably
  // use the RGB settings to do something more
  // interesting, but for this example we'll
  // just turn on the ledpin
//  myDevice.state = true;
//  digitalWrite(ledpin, HIGH);
for (int x= 0 ; x<druckend ;x++){
  printerl.setPixelColor(x,0,0,0);
  }
  printerl.show();


 turn_on(); 
}

void boothlight()
  {
     uint16_t i, j;

  for(j=0; j<256; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< pixel; i++) {
      ring.setPixelColor(i, Wheel(((i * 256 / pixel) + j) & 255));
    }
    ring.show();
    
  }
    
  
  readdata();



  for(j=0; j<256; j++) {
    for(i=0; i<boothli; i++) {
      boothl.setPixelColor(i, Wheel((i+j) & 255));
    }
    boothl.show();
    
    //delay(1);
  }
for(j=0; j<256; j++) {
    for(i=0; i<druckend; i++) {
      printerl.setPixelColor(i, Wheel((i+j) & 255));
    }
    printerl.show();
    }
for (int x= 0 ; x<druckend ;x++){
  printerl.setPixelColor(x,0,0,0);
  }
  printerl.show();

  // do our business, normally you'd probably
  // use the RGB settings to do something more
  // interesting, but for this example we'll
  // just turn on the ledpin
//  myDevice.state = true;
//  digitalWrite(ledpin, HIGH);
for (int x= 0 ; x<boothli ;x++){
  boothl.setPixelColor(x,boothr,boothg,boothb);
  }
  boothl.show();


 turn_on(); 
}


//****************Coountdown counterclockwise
void count1()
{
  readdata();
mySUI.println("OFF");
int countdown = sekunden*1000 / pixel;
    for( uint16_t i=pixel-1; i>0; i--) {
      ring.setPixelColor(i, ring_r_f,ring_g_f,ring_b_f);
      ring.show();
      delay(countdown);
    
      
  }
 ring.setPixelColor(0, ring_r_f,ring_g_f,ring_b_f);
      ring.show();
      delay(countdown);
turn_on();       
}


//****************Coountdown double
void doublecount()
{
  readdata();
int mitte = 0;
mitte = pixel / 2;
int x= 0;
int countdown = sekunden*1000 / pixel*2;
    for( uint16_t i=pixel-1; i>mitte-1; i--) {
     
     ring.setPixelColor(i, ring_r_f,ring_g_f,ring_b_f);
      ring.setPixelColor(x, ring_r_f,ring_g_f,ring_b_f);
      x=x+1;
      ring.show();
      delay(countdown);
    
      
  }
  
 ring.setPixelColor(0, ring_r_f,ring_g_f,ring_b_f);
      ring.show();
      delay(countdown);
turn_on();       
}



//****************theatre effect
void count2()
{
 
  readdata();
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (int i=0; i < pixel; i=i+3) {
        ring.setPixelColor(i+q, ring_r_f,ring_g_f,ring_b_f);    //turn every third pixel on
      }
      ring.show();
     
      delay(100);
     
      for (int i=0; i < pixel; i=i+3) {
        ring.setPixelColor(i+q, ring_r_b,ring_g_b,ring_b_b);        //turn every third pixel off
      }
    }
  }
  turn_on();
}
//****************theatre effect yellow
void theatyell()
{
 
  readdata();
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (int i=0; i < pixel; i=i+3) {
        ring.setPixelColor(i+q, 255,255,0);    //turn every third pixel on
      }
      ring.show();
     
      delay(100);
     
      for (int i=0; i < pixel; i=i+3) {
        ring.setPixelColor(i+q, ring_r_b,ring_g_b,ring_b_b);        //turn every third pixel off
      }
    }
  }
  turn_on();
}

//****************theatre effect green
void theatgreen()
{
 
  readdata();
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (int i=0; i < pixel; i=i+3) {
        ring.setPixelColor(i+q, 0,255,0);    //turn every third pixel on
      }
      ring.show();
     
      delay(100);
     
      for (int i=0; i < pixel; i=i+3) {
        ring.setPixelColor(i+q, ring_r_b,ring_g_b,ring_b_b);        //turn every third pixel off
      }
    }
  }
  turn_on();
}


//****************theatre effect blue
void theatblue()
{
 
  readdata();
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (int i=0; i < pixel; i=i+3) {
        ring.setPixelColor(i+q, 0,0,255);    //turn every third pixel on
      }
      ring.show();
     
      delay(100);
     
      for (int i=0; i < pixel; i=i+3) {
        ring.setPixelColor(i+q, ring_r_b,ring_g_b,ring_b_b);        //turn every third pixel off
      }
    }
  }
  turn_on();
}

//****************theatre effect red
void theatred()
{
 
  readdata();
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (int i=0; i < pixel; i=i+3) {
        ring.setPixelColor(i+q, 255,0,0);    //turn every third pixel on
      }
      ring.show();
     
      delay(100);
     
      for (int i=0; i < pixel; i=i+3) {
        ring.setPixelColor(i+q, ring_r_b,ring_g_b,ring_b_b);        //turn every third pixel off
      }
    }
  }
  turn_on();
}

void rainbow(){
  uint16_t i, j;

  for(j=0; j<256*4; j++) { // 5 cycles of all colors on wheel
    j=j+3;
    for(i=0; i< pixel; i++) {
      ring.setPixelColor(i, Wheel(((i * 256 / pixel) + j) & 255));
    }
    ring.show();
    
  }
  turn_on();
}



void turn_on()
{

  readdata();
    ring.setBrightness(helligkeit);
  ring.show();
  for( uint16_t i=0; i< pixel;  i++) {
      ring.setPixelColor(i, ring_r_b,ring_g_b,ring_b_b);
               
  }
   ring.show();
   delay(20);

  // do our business, normally you'd probably
  // use the RGB settings to do something more
  // interesting, but for this example we'll
  // just turn on the ledpin
 // myDevice.state = true;
//  digitalWrite(ledpin, HIGH);

 
}



// callback: turn_off
// set device (and the ledpin) off
void turn_off()
{
  readdata();
mySUI.println("OFF");
int countdown = sekunden/ pixel*1000;
    for( uint16_t i=pixel+1; i>0; i--) {
      ring.setPixelColor(i-1, 0,0,0);
  
  
  }
    ring.show();
  delay(20);  
  /* here we'd turn the device "on" 
      (whatever that means)
      for now, we just:
   */
   mySUI.println("ON");
 
  myDevice.state = false;
  digitalWrite(ledpin, LOW);

  // provide some feedback
  mySUI.println(F("OFF"));
  mySUI.returnOK();
}



// callback: set_devid
// Get a string from the user to set as the
// device id
void set_devid()
{
  // Here, we want some additional input from
  // the user, so we show the "enter data prompt"
  // using... showEnterDataPrompt
  mySUI.showEnterDataPrompt();


  // Now, we actually get the input
  // We want a string (of up to dev_id_maxlen
  // characters), so you can use readBytesToEOL().
  // to ensure we get the whole string
  // no matter what the serial line terminator settings are
  // (newline, carriage return or both) we can use readBytesUntil().
  //
  // readBytesToEOL: works in all line termination setups and won't
  //  mess up with a slow serial line (like 9600baud)
  //
  // readBytesUntil: needs the correct line termination char
  byte numRead = mySUI.readBytesToEOL(myDevice.dev_id, dev_id_maxlen);

  // make sure the string is null-terminated
  myDevice.dev_id[numRead] = '\0';

  // provide some feedback
  mySUI.println(myDevice.dev_id);
  mySUI.returnOK();
}



// callback: set_pixel
// set the device value for "red"

void set_pixel()
{
  // Here, we want some additional (numerical) input from
  // the user, so we show the "enter data prompt"
  // using... showEnterNumericDataPrompt
  mySUI.showEnterNumericDataPrompt();

  // Now, we actually get the input
  // we can use any Serial method on our SerialUI
  // object, so:
  int new_red = mySUI.parseInt();

  // do some validation of the input
  if (new_red < 0 || new_red > 255)
  {
    // returnError() is the only SerialUI method that takes
    // a regular char* argument, so we don't have to
    // pre-program all the error messages as program memory strings:
    return mySUI.returnError("invalid red val");
  }

  // input looks good, set it
  myDevice.red = new_red;
pixel = new_red;
writeSDSettings();

  // provide some feedback
  mySUI.println(new_red, DEC);
  mySUI.returnOK();
}


void set_r_r_f()
{
   mySUI.showEnterNumericDataPrompt();

  int new_red = mySUI.parseInt();

  if (new_red < 0 || new_red > 255)
  {
    return mySUI.returnError("invalid red val");
  }
  myDevice.red = new_red;
  mySUI.println(new_red, DEC);
  mySUI.returnOK();
  ring_r_f = new_red;
  mySUI.println(ring_r_f);
  writeSDSettings();
  readdata();
}

void set_r_g_f()
{
   mySUI.showEnterNumericDataPrompt();

  int new_red = mySUI.parseInt();

  if (new_red < 0 || new_red > 255)
  {
    return mySUI.returnError("invalid red val");
  }
  myDevice.red = new_red;
  mySUI.println(new_red, DEC);
  mySUI.returnOK();
  ring_g_f = new_red;
  mySUI.println(ring_g_f);
  writeSDSettings();
  readdata();
}

void set_r_b_f()
{
     mySUI.showEnterNumericDataPrompt();

  int new_red = mySUI.parseInt();

  if (new_red < 0 || new_red > 255)
  {
    return mySUI.returnError("invalid red val");
  }
  myDevice.red = new_red;
  mySUI.println(new_red, DEC);
  mySUI.returnOK();
  ring_b_f = new_red;
    mySUI.println(ring_b_f);
  writeSDSettings();
  readdata();
}
  
void set_r_r_b()
{
     mySUI.showEnterNumericDataPrompt();

  int new_red = mySUI.parseInt();

  if (new_red < 0 || new_red > 255)
  {
    return mySUI.returnError("invalid red val");
  }
  myDevice.red = new_red;
  mySUI.println(new_red, DEC);
  mySUI.returnOK();
  ring_r_b = new_red;
    mySUI.println(ring_r_b);
  writeSDSettings();
  readdata();
}

void set_r_g_b()
{
     mySUI.showEnterNumericDataPrompt();

  int new_red = mySUI.parseInt();

  if (new_red < 0 || new_red > 255)
  {
    return mySUI.returnError("invalid red val");
  }
  myDevice.red = new_red;
  mySUI.println(new_red, DEC);
  mySUI.returnOK();
  ring_g_b = new_red;
    mySUI.println(ring_g_b);
  writeSDSettings();
  readdata();
}

void set_r_b_b()
{
     mySUI.showEnterNumericDataPrompt();

  int new_red = mySUI.parseInt();

  if (new_red < 0 || new_red > 255)
  {
    return mySUI.returnError("invalid red val");
  }
  myDevice.red = new_red;
  mySUI.println(new_red, DEC);
  mySUI.returnOK();
  ring_b_b = new_red;
    mySUI.println(ring_b_b);
  writeSDSettings();
  readdata();
}

void set_boothr()
{
   mySUI.showEnterNumericDataPrompt();

  int new_red = mySUI.parseInt();

  if (new_red < 0 || new_red > 255)
  {
    return mySUI.returnError("invalid red val");
  }
  myDevice.red = new_red;
  mySUI.println(new_red, DEC);
  mySUI.returnOK();
  boothr = new_red;
  mySUI.println(ring_r_f);
  writeSDSettings();
  readdata();
}

void set_boothg()
{
   mySUI.showEnterNumericDataPrompt();

  int new_red = mySUI.parseInt();

  if (new_red < 0 || new_red > 255)
  {
    return mySUI.returnError("invalid green val");
  }
  myDevice.red = new_red;
  mySUI.println(new_red, DEC);
  mySUI.returnOK();
  boothg = new_red;
  mySUI.println(ring_r_f);
  writeSDSettings();
  readdata();
}
void set_boothb()
{
   mySUI.showEnterNumericDataPrompt();

  int new_red = mySUI.parseInt();

  if (new_red < 0 || new_red > 255)
  {
    return mySUI.returnError("invalid blue val");
  }
  myDevice.red = new_red;
  mySUI.println(new_red, DEC);
  mySUI.returnOK();
  boothb = new_red;
  mySUI.println(ring_r_f);
  writeSDSettings();
  readdata();
}

void set_boothli()
{
   mySUI.showEnterNumericDataPrompt();

  int new_red = mySUI.parseInt();

  if (new_red < 0 || new_red > 255)
  {
    return mySUI.returnError("invalid length val");
  }
  myDevice.red = new_red;
  mySUI.println(new_red, DEC);
  mySUI.returnOK();
  boothli = new_red;
  mySUI.println(ring_r_f);
  writeSDSettings();
  readdata();
}

void set_serial()
{
     mySUI.showEnterNumericDataPrompt();

  int new_red = mySUI.parseInt();

  if (new_red < 0 || new_red > 255)
  {
    return mySUI.returnError("invalid red val");
  }
  myDevice.red = new_red;
  mySUI.println(new_red, DEC);
  mySUI.returnOK();
  sersch = new_red;
  writeSDSettings();
  readdata();
}



// callback: set_brightness
// same as set_pixel, but set the device value for "green"
void set_brightness()
{
   mySUI.showEnterNumericDataPrompt();

  int new_red = mySUI.parseInt();

  if (new_red < 0 || new_red > 100)
  {
    return mySUI.returnError("invalid red val");
  }
  myDevice.red = new_red;
  mySUI.println(new_red, DEC);
  mySUI.returnOK();
  helligkeit = new_red;
  writeSDSettings();
  readdata();
ring.setBrightness(helligkeit);
printerl.setBrightness(helligkeit);
boothl.setBrightness(helligkeit);
ring.show();
printerl.show();
boothl.show();
 
}

// callback: set_printend
// same as set_pixel, but set the device value for "blue"
void set_printend()
{
  // Here, we want some additional (numerical) input from
  // the user, so we show the "enter data prompt"
  // using... showEnterNumericDataPrompt
  mySUI.showEnterNumericDataPrompt();

  // Now, we actually get the input
  // we can use any Serial method on our SerialUI
  // object, so:
  int new_blue = mySUI.parseInt();

  // do some validation of the input
  if (new_blue < 0 || new_blue > 255)
  {
    // returnError() is the only SerialUI method that takes
    // a regular char* argument, so we don't have to
    // pre-program all the error messages as program memory strings:
    return mySUI.returnError("invalid blue val");
  }

  // input looks good, set it
  druckend = new_blue;
  writeSDSettings();
//EEPROM.write(99, new_blue);
  // provide some feedback
  mySUI.println(new_blue, DEC);
  mySUI.returnOK();
}


void set_seconds()
{
  // Here, we want some additional (numerical) input from
  // the user, so we show the "enter data prompt"
  // using... showEnterNumericDataPrompt
  mySUI.showEnterNumericDataPrompt();

  // Now, we actually get the input
  // we can use any Serial method on our SerialUI
  // object, so:
  long new_green = mySUI.parseInt();

  // do some validation of the input
  if (new_green < 0 || new_green > 5000)
  {
    // returnError() is the only SerialUI method that takes
    // a regular char* argument, so we don't have to
    // pre-program all the error messages as program memory strings:
    return mySUI.returnError("invalid green val");
  }

  // input looks good, set it
  myDevice.green = new_green;

  // provide some feedback
  sekunden = new_green;
  writeSDSettings();
//  EEPROM.write(2, new_green);
  mySUI.println(new_green, DEC);
  mySUI.returnOK();
}



// callback: show_info
// Output some details about the device
void show_info()
{
readdata();
mySUI.println("#####################################################");
mySUI.println("SD CARD initialization done.");
mySUI.print("pixel: ");
mySUI.println(pixel);
mySUI.print("seconds: ");
mySUI.println(sekunden);
mySUI.print("serial: ");
mySUI.println(sersch);
mySUI.print("brightnes max. 100!!!: ");
mySUI.println(helligkeit);
mySUI.print("Ring red Foreground: ");
mySUI.println(ring_r_f);
mySUI.print("Ring green Foreground: ");
mySUI.println(ring_g_f);
mySUI.print("Ring blue Foreground: ");
mySUI.println(ring_b_f);
mySUI.print("Ring red background: ");
mySUI.println(ring_r_b);
mySUI.print("Ring green background: ");
mySUI.println(ring_g_b);
mySUI.print("Ring blue background: ");
mySUI.println(ring_b_b);
 mySUI.print("boothlight red=");
 mySUI.println(boothr);
 mySUI.print("boothlight green=");
 mySUI.println(boothg);
 mySUI.print("boothlight blue=");
 mySUI.println(boothb);
 mySUI.print("boothlight pixel=");
 mySUI.println(boothli);
 mySUI.print("printerend=");
 mySUI.println(druckend);

mySUI.println("#####################################################");

}

void do_exit() {
  // though you can always just use the "quit" command from
  // the top level menu, this demonstrates using exit(), which
  // will automatically close the Druid4Arduino GUI, if
  // being used.
  mySUI.print(F("Exit requested, terminating GUI if present"));
  mySUI.exit();
}

/*
** ********************* setupMenus() ***********************
**
** setupMenus creates the top level menu and 2 sub-menus, and
** demonstrates a few additional functions of SerialUI setup.
**
**
** Menu item order is set by the order with which they are added, using
** addCommand()/subMenu() (see below).

*/


void setupMenus()
{
  /*
  ** Now it's time to start creating menus.  SerialUI always has
  ** one "top level" menu available which you can use to create items for
  ** commands and access to sub menus.
  */

  // Get a handle to the top level menu
  // Note that menus are returned as pointers.
  SUI::Menu * mainMenu = mySUI.topLevelMenu();
  if (! mainMenu)
  {
    // what? Could not create :(
    return mySUI.returnError("Something is very wrong, could not get top level menu?");

  }


  // we can set the name (title) of any menu using
  // setName().  This shows up in help output and when
  // moving up and down the hierarchy.  If it isn't set,
  // the menu key will be used, for sub-menus, and the default
  // SUI_SERIALUI_TOP_MENU_NAME ("TOP") for top level menus.
  mainMenu->setName(top_menu_title);


  /*
  ** addCommand(KEY, CALLBACK [, HELP])
  **
  ** Use addCommand() to add a command menu item to a menu.
  ** The parameters are:
  **
  **  KEY: the (SUI_DeclareString-created) string to use as the command
  **
  **  CALLBACK: the name of the void(void) callback function (as described
  **            in "Callbacks", above).
  **
  **  HELP: optional (SUI_DeclareString-created) string to display for
  **        this item when menu help (?) is invoked.
  **
  ** Return:  returns boolean true on success, false if command could not
  **          be added.
  */
  if (! mainMenu->addCommand(info_key, show_info, info_help) )
  {
    // should check that addCommand succeeded -- we'll skip this below for brevity.
    // You might want to #ifdef these checks to enable during dev and disable when
    // you know everything's working, if space is tight.

    // what? Could not create :(
    return mySUI.returnError("Could not addCommand to mainMenu?");

  }



  /*
  ** subMenu(KEY [, HELP])
  **
  ** Use subMenu to create a sub-menu accessible by KEY.  The params are:
  **
  **  KEY: The (SUI_DeclareString-created) string to use to enter the sub-menu
  **       from the current menu.
  **
  **  HELP: optional (SUI_DeclareString-created) string to display for
  **        this item when menu help (?) is invoked.
  **
  **  Return: returns a SUI::Menu pointer, which will be NULL if the submenu
  **          could not be created.
  **
  */
  SUI::Menu * enableMenu = mainMenu->subMenu(enable_key, enable_help);
  if (! enableMenu)
  {
    // ah, could not create :(
    return mySUI.returnError("Couldn't create enable menu!?");


  }

  // enable menu created, add our commands
  mainMenu->addCommand(enable_on_key, turn_on);
  enableMenu->addCommand(enable_count1_key, count1,count1_help);
  enableMenu->addCommand(enable_doublecount_key, doublecount,doublecount_help);
  enableMenu->addCommand(enable_rainbow_key, rainbow,rainbow_help);
  enableMenu->addCommand(enable_count2_key, count2,count2_help);
  enableMenu->addCommand(enable_theatyell_key, theatyell,theatyell_help);
  enableMenu->addCommand(enable_theatgreen_key, theatgreen,theatgreen_help);
  enableMenu->addCommand(enable_theatblue_key, theatblue,theatblue_help);
  enableMenu->addCommand(enable_theatred_key, theatred,theatred_help);
  mainMenu->addCommand(enable_off_key, turn_off);
  enableMenu->addCommand(enable_printer_key, printer);


  SUI::Menu * settingsMenu = mainMenu->subMenu(settings_key, settings_help);
  if (! settingsMenu)
  {
    // ah, could not create :(
    return mySUI.returnError("Couldn't create settings menu!?");

  }
  // settings menu created.
  // set its name and add the commands
  settingsMenu->setName(settings_title);
  settingsMenu->addCommand(enable_r_r_f_key, set_r_r_f,r_r_f_help);
  settingsMenu->addCommand(enable_r_g_f_key, set_r_g_f,r_g_f_help);
  settingsMenu->addCommand(enable_r_b_f_key, set_r_b_f,r_b_f_help);
  settingsMenu->addCommand(enable_r_r_b_key, set_r_r_b,r_r_b_help);
  settingsMenu->addCommand(enable_r_g_b_key, set_r_g_b,r_g_b_help);
  settingsMenu->addCommand(enable_r_b_b_key, set_r_b_b,r_b_b_help);
  settingsMenu->addCommand(enable_boothr_key, set_boothr,boothr_help);
  settingsMenu->addCommand(enable_boothg_key, set_boothg,boothg_help);
  settingsMenu->addCommand(enable_boothb_key, set_boothb,boothb_help);
  settingsMenu->addCommand(enable_boothli_key, set_boothli,boothli_help);

  settingsMenu->addCommand(settings_serial_key, set_serial,settings_serial_help);
  settingsMenu->addCommand(settings_pixel_key, set_pixel, settings_pixel_help);
  settingsMenu->addCommand(settings_seconds_key, set_seconds,settings_seconds_help);
  settingsMenu->addCommand(settings_brightness_key, set_brightness,settings_brightness_help);
  settingsMenu->addCommand(settings_printend_key, set_printend,settings_printend_help);
  settingsMenu->addCommand(settings_devid_key, set_devid, settings_devid_help);
  settingsMenu->addCommand(settings_show_key, show_info);


  if (! mainMenu->addCommand(exit_key, do_exit, exit_help) )
  {
    return mySUI.returnError("Could not addCommand to mainMenu?");
  }

  // Done setting up the menus!

}


void writeSDSettings() {


  
 // Delete the old One
 SD.remove("settings.txt");
 // Create new one
 myFile = SD.open("settings.txt", FILE_WRITE);
 // writing in the file works just like regular print()/println() function
 myFile.print("[");
 myFile.print("serial="); //Anzahl Pixel Ring
 myFile.print(sersch);
 myFile.println("]");
 myFile.print("[");
 myFile.print("pixel="); //Anzahl Pixel Ring
 myFile.print(pixel);
 myFile.println("]");
 myFile.print("[");
 myFile.print("seconds="); // Sekunden für Countdown
 myFile.print(sekunden);
 myFile.println("]");  
 myFile.print("[");
 myFile.print("brightnes max 100 !!!=");
 myFile.print(helligkeit);
 myFile.println("]");
 myFile.print("[");
 myFile.print("ring_red_foreground=");
 myFile.print(ring_r_f);
 myFile.println("]");
 myFile.print("[");
 myFile.print("ring_green_foreground=");
 myFile.print(ring_g_f);
 myFile.println("]");
 myFile.print("[");
 myFile.print("ring_blue_foreground=");
 myFile.print(ring_b_f);
 myFile.println("]");
 myFile.print("[");
 myFile.print("ring_red_background=");
 myFile.print(ring_r_b);
 myFile.println("]");
 myFile.print("[");
 myFile.print("ring_green_background=");
 myFile.print(ring_g_b);
 myFile.println("]");
 myFile.print("[");
 myFile.print("ring_blue_background=");
 myFile.print(ring_b_b);
 myFile.println("]");
  myFile.print("[");
 myFile.print("printerend=");
 myFile.print(druckend);
 myFile.println("]");
  myFile.print("[");
 myFile.print("boothlight red=");
 myFile.print(boothr);
 myFile.println("]");
  myFile.print("[");
 myFile.print("boothlight green=");
 myFile.print(boothg);
 myFile.println("]");
  myFile.print("[");
 myFile.print("boothlight blue=");
 myFile.print(boothb);
 myFile.println("]");
  myFile.print("[");
 myFile.print("boothlight pixel=");
 myFile.print(boothli);
 myFile.println("]");
 // close the file:
 myFile.close();
 //Serial.println("Writing done.");
mySUI.println("geschrieben");

 }


void readdata(){
char character;
 String settingName;
 String settingValue;
 myFile = SD.open("settings.txt");
 if (myFile) {
 while (myFile.available()) {
 character = myFile.read();
 while((myFile.available()) && (character != '[')){
 character = myFile.read();
 }
 character = myFile.read();
 while((myFile.available()) && (character != '=')){
 settingName = settingName + character;
 character = myFile.read();
 }
 character = myFile.read();
 while((myFile.available()) && (character != ']')){
 settingValue = settingValue + character;
 character = myFile.read();
 }
 if(character == ']'){
 
 
 /*Debuuging Printing
 Serial1.println("###########################################");
 Serial1.print("Name:");
 Serial1.println(settingName);
 Serial1.print("Value :");
 Serial1.println(settingValue);
 Serial1.println("###########################################"); 
 */
 // Apply the value to the parameter
 applySetting(settingName,settingValue);
 // Reset Strings
 settingName = "";
 settingValue = "";
 }
 }
 // close the file:
 myFile.close();
 } else {
 // if the file didn't open, print an error:
 Serial.println("error opening settings.txt");
 }
 }
 void applySetting(String settingName, String settingValue) {
  /* Apply the value to the parameter by searching for the parameter name
 Using String.toInt(); for Integers
 toFloat(string); for Float
 toBoolean(string); for Boolean
 toLong(string); for Long
 */
   if(settingName == "serial") {
 sersch=settingValue.toInt();
 }
   if(settingName == "pixel") {
 pixel=settingValue.toInt();
 }
 if(settingName == "seconds") {
 sekunden=settingValue.toInt();
 }
 if(settingName == "brightnes max 100 !!!") {
 helligkeit=settingValue.toInt();
 }
 if(settingName == "ring_red_foreground") {
 ring_r_f=settingValue.toInt();
 }
   if(settingName == "ring_green_foreground") {
 ring_g_f=settingValue.toInt();
 }
   if(settingName == "ring_blue_foreground") {
 ring_b_f=settingValue.toInt();
 }
   if(settingName == "ring_red_background") {
 ring_r_b=settingValue.toInt();
 }
   if(settingName == "ring_green_background") {
 ring_g_b=settingValue.toInt();
 }
   if(settingName == "ring_blue_background") {
 ring_b_b=settingValue.toInt();
 } 
    if(settingName == "printerend") {
 druckend=settingValue.toInt();
 }
    if(settingName == "boothlight red") {
 boothr=settingValue.toInt();
 }
    if(settingName == "boothlight green") {
 boothg=settingValue.toInt();
 } 
    if(settingName == "boothlight blue") {
 boothb=settingValue.toInt();
 } 
    if(settingName == "boothlight pixel") {
 boothli=settingValue.toInt();
 }  
 }


 // converting string to Float
 float toFloat(String settingValue){
 char floatbuf[settingValue.length()+1];
 settingValue.toCharArray(floatbuf, sizeof(floatbuf));
 float f = atof(floatbuf);
 return f;
 }
 
 long toLong(String settingValue){
 char longbuf[settingValue.length()+1];
 settingValue.toCharArray(longbuf, sizeof(longbuf));
 long l = atol(longbuf);
 return l;
 }
 
 // Converting String to integer and then to boolean
 // 1 = true
 // 0 = false
 boolean toBoolean(String settingValue) {
 if(settingValue.toInt()==1){
 return true;
 } else {
 return false;
 }
 }

  //Einlesen der EEPROM Daten am start
/*
pixel = EEPROM.read(1); //Anzahl Pixel im Ring
sekunden = EEPROM.read(2); //Sekunden Countdown RING
helligkeit = EEPROM.read(3); //helligkeit RING
ring_r_f = EEPROM.read(4);  //Ring ROT wert Vordergrund
ring_g_f = EEPROM.read(5);    //Ring FRÜN wert Vordergrund
ring_b_f = EEPROM.read(6);  //Ring BLAU wert Vordergrund
ring_r_b = EEPROM.read(7);  //Ring ROT wert hintergrund
ring_g_b = EEPROM.read(8);  //Ring GRÜN wert hintergrund
ring_b_b = EEPROM.read(9);  //Ring BLAU wert hintergrund

druckend = EEPROM.read(99);
*/




uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
   return ring.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else if(WheelPos < 170) {
    WheelPos -= 85;
   return ring.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  } else {
   WheelPos -= 170;
   return ring.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  }
}
