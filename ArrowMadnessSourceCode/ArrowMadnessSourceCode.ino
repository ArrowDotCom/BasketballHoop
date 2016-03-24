/***********************************
 * It's time for ARROW MADNESS!
 * This code controls a 32x64 LED Matrix from Adafruit, a 1m strip of NeoPixels
 * from Adafruit, and accepts input from a SHARP 3 wire IR sensor.  
 * 
 * The longs strip of LEDs acts only to illuminate the backboard and is not
 * controlled directly by the Mega.  
 * 
 * This cannot be done on an Arduino Uno, it MUST use a MEGA or
 * a device with equivalent RAM.  
 */

#include <Adafruit_GFX.h>       // Core graphics library from Adafruit Industries
#include <RGBmatrixPanel.h>     // Hardware-specific library from Adafruit Industries
#include <Adafruit_NeoPixel.h>  // NeoPixel library required to control LED strip
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define NUMPIXELS      60       // Tell the computer how many pixels we're working with here
#define PIN 6                   // Set PIN 6 as the data pin for the NeoPixels
#define OE   9                  // These pins are defined in the hookup guide for the LED Matrix
#define LAT 10                  
#define CLK 11
#define A   A0
#define B   A1
#define C   A2
#define D   A3

int sensorpin = 7;               // Pin 7 is the data line for the IR sensor.    
int sensor = 0;
int timer = 30;
int threshold = 351;
int HIGHscore = 0;
int score = 1;
int timer2 = 30;
int start = 0;
int spacer = 0;
int h = 3;
int r = 0;                      // R, G, and B are variables we'll use to set the color
int g = 155;                    // blend of the NeoPixels.  Initialize to a blueish green
int b = 100;
int i = 0;

//Initialize the Matrix and NeoPixel Strip
RGBmatrixPanel matrix(A, B, C, D, CLK, LAT, OE, false, 64);     
Adafruit_NeoPixel strip = Adafruit_NeoPixel(60, PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void setup() {

  matrix.begin();
  Serial.begin(9600);
  //scrollvs();

  // fill the screen with 'black'
  matrix.fillScreen(matrix.Color333(0, 0, 0));

  pixels.begin(); // This initializes the NeoPixel library.
  
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

  void loop() {
  sensor = analogRead(sensorpin);
  Serial.print("IR Value: ");
  Serial.print(sensor);
  Serial.print("\n");
    delay(10);

  if ((sensor > threshold)&&start==0){
    Serial.print ("we're ready!");
      arrowmadness();
      theaterChaseRainbow(20);
      delay(1000);
      matrix.fillScreen(matrix.Color333(0, 0, 0));
      matrix.setTextSize(1);
      matrix.setCursor(15,12);
      matrix.setTextColor(matrix.Color333(0,7,7));
      matrix.print("READY?");
      delay(1000);
      matrix.fillScreen(matrix.Color333(0, 0, 0));
      matrix.setCursor(15,12);
      matrix.setTextColor(matrix.Color333(4,0,7));
      matrix.print("SHOOT!");
      matrix.drawRect(3, 3, 57, 25, matrix.Color333(7, 7, 7));
      start=1;
      delay(1000);
      sensor = analogRead(sensorpin);
  }
  if((sensor > threshold)&& start==1){
    Serial.println("Start game!");
    matrix.fillScreen(matrix.Color333(0, 0, 0));
    matrix.setCursor(1,1);
    matrix.setTextSize(1);
    matrix.setTextColor(matrix.Color333(7,7,7));
    matrix.print("Time");
    matrix.setCursor(33,1);
    matrix.setTextSize(1);
    matrix.setTextColor(matrix.Color333(7,7,7));
    matrix.print("Score");
    updateDisplay();
    timedgame();
    rollcredits();
    start = 0;
    sensor = analogRead(sensorpin);
    }
 
  }

  
  void timedgame(){
    timer = 30;
    h=3;
    i=0;
    matrix.setTextColor(matrix.Color333(0,7,0));
    for (timer = 30; timer >= 0; timer--){
      if (i>59){
         i=0;
      }
      spacer=0;
      updateDisplay();
      //check sensor and update the colors of the pixels 24 times to make the loop 1 second
      for (int y=0; y<=24; y++){
        checksensor();
        delay (10);
        Serial.print("time: ");
        Serial.print(timer);
        Serial.print("\n");
        pixels.setPixelColor(i, pixels.Color(r,g,b)); // Moderately bright green color.
        pixels.show(); // This sends the updated pixel color to the hardware.
        i++;
        delay(10);
        
      }
      matrix.setTextColor(matrix.Color333(0,7,0));  //Start colored strip as green
      r = 0;
      g = 255;
      b = 0;
      if (timer <= 20){
        matrix.setTextColor(matrix.Color333(7,7,0)); //As timer drops below 20 seconds, cycle in yellow
        h=3;
        r = 125;
        g = 120;
        b = 0;
       }
       if (timer <= 10) {
        matrix.setTextColor(matrix.Color333(7,0,0));  //At 10 seconds to go, cycle in red!  
        h=11;
        r = 255;
        g = 0;
        b = 0;      
       }
    }
    matrix.fillScreen(matrix.Color333(0, 0, 0));
    matrix.setCursor(6, 3);
    matrix.setTextSize(1);
    matrix.setTextColor(matrix.Color333(4, 0, 7));
    matrix.print("GAME OVER");
    matrix.setCursor(22,16);
    matrix.setTextSize(2);
    matrix.print(score);
    delay(2000);
   
  }

  void arrowmadness(){
     
      matrix.setTextColor(matrix.Color333(7,7,7));
      matrix.fillScreen(matrix.Color333(0, 0, 0));
      
      matrix.print("ARROW");
      uint8_t w = 0;
      
      for (w=0; w<20; w=w+2) {
        matrix.setCursor(18, 7);    // start at top left, with 8 pixel of spacing
        matrix.setTextSize(1);
        matrix.setTextColor(matrix.Color333(0,0,0));
        matrix.fillScreen(matrix.Color333(0, 0, 0));
        matrix.fillScreen(Wheel16(w)); 
        matrix.drawRect(0, 0, matrix.width(), matrix.height(), matrix.Color333(7, 7, 7));
        matrix.drawRect(3, 3, 57, 25, matrix.Color333(7, 7, 7));
        matrix.print("ARROW");
        matrix.setCursor(12, 17); 
        matrix.setTextColor(matrix.Color333(0,0,0));
        matrix.print("MADNESS");
        delay(100);
      }
        matrix.fillRect(4, 4, 55, 23, matrix.Color333(0, 0, 0));
        matrix.setTextColor(matrix.Color333(7,7,7));
        matrix.setCursor(18, 7);
        matrix.print("ARROW");
        matrix.setCursor(12, 17); 
        matrix.print("MADNESS");
  }
  void checksensor(){
    sensor = analogRead(sensorpin);
      if ((sensor >= threshold)&&spacer==0){
        score++;
        Serial.print("score: ");
        Serial.print(score);
        Serial.print("\n");
        Serial.print(sensor);
        delay(10);
        sensor = analogRead(sensorpin);
        spacer=1;
      }
      else{
        delay(10);
        Serial.print("score: ");
        Serial.print(score);
        Serial.print("\n");
        Serial.print(sensor);
        
        }
    }
    
void updateDisplay () {
      matrix.fillRect(0, 10, matrix.width(), matrix.height(), matrix.Color333(0, 0, 0));   //fill score/timer with black
      matrix.setCursor(h,12);
      matrix.setTextSize(2);
      Serial.print(timer2);
      matrix.print(timer);
      matrix.setCursor(36,12);
      matrix.setTextColor(matrix.Color333(7,7,7)); 
      matrix.print(score);
      delay(10);
      Serial.print("Display Updated");
      
}
  void scrollvs() {
    int cursor_x = 0;
    int cursor_y = 1;

    while(cursor_x < 64){
      matrix.setTextWrap(false); // Don't wrap at end of line - will do ourselves
      matrix.setTextSize(2);
      matrix.fillScreen(matrix.Color333(0, 0, 0));
      matrix.setCursor(cursor_x, cursor_y);
      matrix.setTextColor(matrix.Color333(7,0,0));
      matrix.print('V');
      matrix.setTextColor(matrix.Color333(7,4,0)); 
      matrix.print('V');
      matrix.setTextColor(matrix.Color333(7,7,0));
      matrix.print('V');
      matrix.setTextColor(matrix.Color333(4,7,0)); 
      matrix.print('V');
      matrix.setTextColor(matrix.Color333(0,7,0));  
      matrix.print('V');
      matrix.setTextColor(matrix.Color333(0,7,7)); 
      matrix.print("V");
      matrix.setTextColor(matrix.Color333(0,4,7)); 
      matrix.print('V');
      matrix.setTextColor(matrix.Color333(0,0,7));
      matrix.print('V');
      matrix.setTextColor(matrix.Color333(4,0,7)); 
      matrix.print("V");
      matrix.setTextColor(matrix.Color333(7,0,4)); 
      matrix.println("V");
      cursor_x++;
      delay(30);
    }
        
      cursor_y = 16;

    while(cursor_x > 0){
      matrix.setTextWrap(false); // Don't wrap at end of line - will do ourselves
      matrix.setTextSize(2);
      matrix.fillScreen(matrix.Color333(0, 0, 0));
      matrix.setCursor(cursor_x, cursor_y);
      matrix.setTextColor(matrix.Color333(7,0,0));
      matrix.print('V');
      matrix.setTextColor(matrix.Color333(7,4,0)); 
      matrix.print('V');
      matrix.setTextColor(matrix.Color333(7,7,0));
      matrix.print('V');
      matrix.setTextColor(matrix.Color333(4,7,0)); 
      matrix.print('V');
      matrix.setTextColor(matrix.Color333(0,7,0));  
      matrix.print('V');
      matrix.setTextColor(matrix.Color333(0,7,7)); 
      matrix.print("V");
      matrix.setTextColor(matrix.Color333(0,4,7)); 
      matrix.print('V');
      matrix.setTextColor(matrix.Color333(0,0,7));
      matrix.print('V');
      matrix.setTextColor(matrix.Color333(4,0,7)); 
      matrix.print("V");
      matrix.setTextColor(matrix.Color333(7,0,4)); 
      matrix.println("V");
      cursor_x--;
      delay(30);
    }
  
  }
  
  
  void rollcredits(){
    
      if (score > HIGHscore){
          //new high score!
          HIGHscore = score;
          matrix.fillScreen(matrix.Color333(0, 0, 0));
          matrix.drawRect(3, 3, 57, 25, matrix.Color333(0, 7, 0));
          matrix.setTextColor(matrix.Color333(0,7,0));
          matrix.setCursor(8,4);
          matrix.setTextSize(1);
          matrix.print("NEW HIGH");
          matrix.setCursor(16,11);
          matrix.print("SCORE");
          matrix.setCursor(14,20);
          matrix.print("(OMG!)");     
          delay(4000);   
       }

      //begin end sequence
        matrix.fillScreen(matrix.Color333(0, 0, 0));
        matrix.setCursor(18,5);
        matrix.setTextSize(1);
        matrix.setTextColor(matrix.Color333(7,0,7));
        matrix.print("GREAT");
        matrix.setCursor(21,16);
        matrix.setTextColor(matrix.Color333(0,4,7));
        matrix.print("JOB!");
        rainbowCycle(1);
      //final/your score sequence 
      for(int z=0; z<2; z++){
        matrix.fillScreen(matrix.Color333(0, 0, 0));
        matrix.setTextSize(1);
        matrix.setCursor(3,3);
        matrix.setTextColor(matrix.Color333(0,0,7));
        matrix.print("YOUR SCORE");
        matrix.setTextColor(matrix.Color333(0,0,7));
        matrix.setTextSize(2);
        matrix.setCursor(22,12);
        matrix.print(score);
        delay(1500);
        matrix.fillScreen(matrix.Color333(0, 0, 0));
        matrix.setTextSize(1);
        matrix.setCursor(3,3);
        matrix.setTextColor(matrix.Color333(0,7,0));
        matrix.print("HIGH SCORE");
        matrix.setTextColor(matrix.Color333(0,7,0));
        matrix.setTextSize(2);
        matrix.setCursor(22,12);
        matrix.print(HIGHscore);
        delay(1500);
      }
      delay(2000);
      score = 1;
      r = 0;
      g = 255;
      b = 0;
      
  }
  
//All code below is pulled from Adafruit example code, and modified to fit our needs:

  uint16_t Wheel16(byte WheelPos) {
  if(WheelPos < 8) {
   return matrix.Color333(7 - WheelPos, WheelPos, 0);
  } else if(WheelPos < 16) {
   WheelPos -= 8;
   return matrix.Color333(0, 7-WheelPos, WheelPos);
  } else {
   WheelPos -= 16;
   return matrix.Color333(0, WheelPos, 7 - WheelPos);
  }
} 
  
  // Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*3; j=j+2) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, c);    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j=j+8) {     // cycle all 256 colors in the wheel.  I upped to j+8 to make this all happen faster.  
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < strip.numPixels(); i=i+2) {
        strip.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
      }
      strip.show();

      delay(10);

      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0); 
}
  
