// Include necessary libraries
#include <Adafruit_DotStar.h>
#include <Adafruit_SleepyDog.h>

// Define global variables
#define GAS_DATA   4
#define GAS_ENABLE 3 // opposite expected behavior, high = off, low = on
#define BUZZER     1
#define WATER_DATA 0
#define STATUS_LED 13

// Define water and gas leak conditions
#define water_leak_condition 900
#define gas_leak_condition   400

// Initialize global variables
int GAS_VALUE;
int WATER_VALUE;
bool FORCE_BUZZER;

// Prepare on-board DotStar LED
Adafruit_DotStar strip = Adafruit_DotStar(1, INTERNAL_DS_DATA, INTERNAL_DS_CLK, DOTSTAR_BGR);

// At boot
void setup() {
  strip.begin(); strip.setBrightness(0); strip.setPixelColor(0, 0, 0, 0); strip.show(); // disable on-board LED
  
  // Define pins as inputs and outputs
  pinMode(GAS_ENABLE, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  // Warm up gas sensor for 20 seconds
  digitalWrite(GAS_ENABLE, LOW);  // warm up gas sensor for the first time
  delay(20000);                   // --> 20 seconds total to get warm
  digitalWrite(GAS_ENABLE, HIGH); // gas sensor warmed up, stop heating

  // Set default values
  GAS_VALUE = 0;
  WATER_VALUE = 0;
  FORCE_BUZZER = false;
  digitalWrite(BUZZER, LOW);  // disable the buzzer
}


// Loop forever
void loop() {
  // Status Light Blink
  digitalWrite(STATUS_LED, HIGH); // Show we're awake again
  delay(300);                     // wait for a half a second
  digitalWrite(STATUS_LED, LOW);  // Show we're asleep

  // Warm up gas sensor
  digitalWrite(GAS_ENABLE, LOW);  // begin warming gas sensor
  delay(1500);                    // wait for 1.5 seconds

  // Check both gas & water sensors
  for (int i = 0; i < 10; i++) {
    // read pins
    GAS_VALUE = analogRead(GAS_DATA);
    WATER_VALUE = analogRead(WATER_DATA);

    // Compare input values to constraints
    if (GAS_VALUE >= gas_leak_condition || WATER_VALUE >= water_leak_condition) { 
      digitalWrite(BUZZER, HIGH); // sound the alarm!
    }
  }

  // Disable the gas sensor
  digitalWrite(GAS_ENABLE, HIGH); // stop warming, time to sleep

  // Sleep for 20 seconds
  Watchdog.sleep(5000);
  Watchdog.sleep(5000);
  Watchdog.sleep(5000);
  Watchdog.sleep(5000);
}
