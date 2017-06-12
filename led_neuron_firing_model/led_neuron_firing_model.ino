// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// released under the GPLv3 license to match the rest of the AdaFruit NeoPixel library

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1
#define PIN            4

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      60

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

int delayval = 500; // delay for half a second

int nums1[] = {0, 101, 798, 1139, 1717, 2383, 3049, 3715, 4388, 5060, 5755, 6459, 7167, 7885, 8610, 9332, 10055, 10774, 11492, 12214, 12927, 13641, 14349, 15048, 15746, 16444, 17137, 17828, 18512, 19196, 19880, 20555, 21235, 21913}; 
int nums2[] = {0, 107, 646, 1139, 1721, 2397, 3072, 3752, 4441, 5275, 6036, 6779, 7517, 8249, 8982, 9706, 10434, 11157, 11875, 12593, 13307, 14018, 14722, 15421, 16119, 16813, 17506, 18195, 18880, 19564, 20243, 20923, 21602, 22281};


void setup() {
  // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
#if defined (__AVR_ATtiny85__)
  if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
#endif
  // End of trinket special code
  pixels.begin(); // This initializes the NeoPixel library.
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < pixels.numPixels(); i=i+3) {
        pixels.setPixelColor(i+q, c);    //turn every third pixel on
      }
      pixels.show();

      delay(wait);

      for (uint16_t i=0; i < pixels.numPixels(); i=i+3) {
        pixels.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return pixels.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return pixels.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return pixels.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

void turnoff(){
  for(int i=0;i<NUMPIXELS/2;i++){
      pixels.setPixelColor(i, pixels.Color(0,0,0));  
      pixels.setPixelColor(NUMPIXELS-i, pixels.Color(0,0,0));
      delay((delayval- (i*8))/10);
      pixels.show();      
    }
}




// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< pixels.numPixels(); i++) {
      pixels.setPixelColor(i, Wheel(((i * 256 / pixels.numPixels()) + j) & 255));
    }
    pixels.show();
    delay(wait);
  }
}

void colorCycle(){
      for(int i=0;i<NUMPIXELS/2;i++){
        // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
        pixels.setPixelColor(i, pixels.Color(255,0,i*3)); 
        
        pixels.setPixelColor(NUMPIXELS-i, pixels.Color(0,i*8,255)); 

  
        pixels.show(); // This sends the updated pixel color to the hardware.

        pixels.setBrightness((i*8)); 
        if (i==NUMPIXELS/2){
          pixels.setBrightness((255)); 
        }

        delay((delayval- (i*16))/10); // Delay for a period of time (in milliseconds) between the lights in the current round.
      }
}

void loop() {

  // For a set of NeoPixels the first NeoPixel is 0, second is 1, all the way up to the count of pixels minus one.
  int currentWait = 0;
  while (1){

    //rainbowCycle(20);
    if (random(0, 100) == 50){
      theaterChase(pixels.Color(127, 127, 127), 50); // White
      theaterChase(pixels.Color(127, 0, 0), 50); // Red
      theaterChase(pixels.Color(0, 0, 127), 50); // Blue
    }

    
    colorCycle();
    delay(9*(nums1[currentWait+1]-nums1[currentWait])); // REMI: This is the line to change. This is the wait between rounds, and it gets the numbers from the above array nums.
    // it gets the time from nums[currentWait+1] and subtracts it from nums[currentWait]
    currentWait++;
    if (currentWait > sizeof(nums1)/sizeof(10) - 2){ // reset once it gets to the end of the array
      currentWait = 0;
    }
    turnoff();
    delay(1*(nums1[currentWait+1]-nums1[currentWait]));
  }
}
