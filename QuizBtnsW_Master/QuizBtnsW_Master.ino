// Wireless Quiz Buttons - Master
// assembled by S.Eames with help from the internet (thanks team :) )

/* 

   * System uses NRF24L01 for comms between buttons and master station
   * When a button is pushed, it transmits a byte to the master to say so
      > If it receives a response byte saying it was first the button flashes for 2 seconds then returns to the original state


   * Button States
      * Game Play
      * Idle

   * Game Modes
      * Button Test     --> all buttons respond is if they were first (no blocking)
      * Normal Trivia   --> First button pressed flashes and blocks other buttons until reset
      * True/False      --> Teams press button to cycle through T/F resonse (indicated on LEDs) - whatever they leave it on is locked in once game master sees all have chosen
      * Multi Choice    --> Same as T/F mode but with 4 (or more?) options


   * Master Station communciates with computer (vMix software) over usb-midi 


TODO


*/
 

// SETUP DEBUG MESSAGES
// #define DEBUG   //If you comment this line, the DPRINT & DPRINTLN lines are defined as blank.
#ifdef DEBUG
   #define DPRINT(...)     Serial.print(__VA_ARGS__)     //DPRINT is a macro, debug print
   #define DPRINTLN(...)   Serial.println(__VA_ARGS__)   //DPRINTLN is a macro, debug print with new line
#else
   #define DPRINT(...)                                   //now defines a blank line
   #define DPRINTLN(...)                                 //now defines a blank line
#endif


// Include libraries
#include "FastLED.h"
#include "RF24.h"
// #include "ENUMVars.h"


///////////////////////// IO /////////////////////////
// #define NUM_ADDR_PINS   3

#define BEEP_PIN        5
#define LED_PIN         6
// #define BTN_PIN         10
// const uint8_t id_addr_pin[NUM_ADDR_PINS] = {7, 8, 9}; // Pins used to assign ID of station
#define RF_CSN_PIN      18
#define RF_CE_PIN       19

//////////////////// RF VARIABLES ////////////////////
RF24 radio(RF_CE_PIN, RF_CSN_PIN);     // CE, CSN

#define MAX_BTNS 8
uint8_t addrStartCode[] = "QUIZ";      // First 4 bytes of node addresses (5th byte on each is node ID - set later)
uint8_t nodeAddr[MAX_BTNS][5];         // Holds addresses of each node

#define RF_BUFF_LEN 1                  // Number of bytes to transmit / receive
uint8_t radioBuf_RX[RF_BUFF_LEN];
uint8_t radioBuf_TX[RF_BUFF_LEN];
bool newRFData = false;                // True if new data over radio just in

uint8_t btnsPresent_flag = 0x01;       // Flags of detected buttons (always at least 1)
uint8_t numBtnsPresent = 1;            // Number of buttons being used

uint8_t myID = 0;                      // ID = 0 for master station

//////////////////// PIXEL SETUP ////////////////////
#define NUM_LEDS     12

CRGB leds[NUM_LEDS];                   // Define the array of leds

#define LED_BRIGHTNESS 255

// Colours!
#define COL_RED      0xFF0000
#define COL_YELLOW   0xFF8F00
#define COL_GREEN    0x00FF10
#define COL_BLUE     0x0000FF
#define COL_PINK     0xFF002F
#define COL_CYAN     0x00AAAA
#define COL_ORANGE   0xFF1C00

#define COL_WHITE    0xFFFF7F
#define COL_BLACK    0x000000

const uint32_t btnCols[] = {COL_RED, COL_YELLOW, COL_GREEN, COL_PINK, COL_CYAN, COL_ORANGE, COL_BLUE, COL_WHITE};


void setup() 
{
   // Initialise beeper
   pinMode(BEEP_PIN, OUTPUT);
   digitalWrite(BEEP_PIN, LOW);

   // Initialise LEDs
   FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
   fill_solid( leds, NUM_LEDS, COL_BLACK);
   FastLED.setBrightness(LED_BRIGHTNESS);
   FastLED.show();

   // Initialise Radio

   // Generate node addresses
   for (uint8_t i = 0; i < MAX_BTNS; ++i)
   {
      for (uint8_t j = 0; j < 4; ++j)  // Set first four bytes of address to same code
         nodeAddr[i][j] = addrStartCode[j];

      nodeAddr[i][4] = i;              // Unique 5th byte according to node address
   }

   radio.begin();  
   radio.setAutoAck(1);                // Ensure autoACK is enabled
   radio.enableAckPayload();           // Allow optional ack payloads
   radio.setRetries(0,15);             // Smallest time between retries, (max no. of retries is 15)
   radio.setPayloadSize(RF_BUFF_LEN);  // Here we are sending 1-byte payloads to test the call-response speed
 
   radio.setChannel(111);              // Keep out of way of common wifi frequencies
   radio.setPALevel(RF24_PA_HIGH);     // Let's make this powerful... later
   radio.setDataRate( RF24_2MBPS );    // Let's make this quick

   // Opening Listening pipe
   radio.openReadingPipe(1, nodeAddr[myID]);                    
   radio.writeAckPayload(1, radioBuf_TX, RF_BUFF_LEN);   // Setup AckPayload
   
   // Initialise Serial debug
   #ifdef DEBUG
      Serial.begin(115200);                  // Open comms line
      while (!Serial) ;                      // Wait for serial port to be available

      DPRINTLN(F("THE MASTER HAS ARRIVED!"));
      DPRINT(F("Saved highscore = "));
      DPRINTLN(highscore);
   #endif

   // Let the games begin!
}

void loop() 
{
   // Listen for button presses

   // 



}
