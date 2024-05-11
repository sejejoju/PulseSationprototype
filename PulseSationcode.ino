#include <Servo.h>
#include <LiquidCrystal.h>
#include <Adafruit_NeoPixel.h>

#define LED_PIN    6
#define NUM_LEDS   30

Adafruit_NeoPixel strip(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

// Pin configurations
const int SERVO_PIN = 9;

// LCD pin configuration
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;

// Global objects
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
Servo heart;

unsigned long startTime = 0;
bool servoMoving = false;

void setup() {
  Serial.begin(115200);

  lcd.begin(16, 2);  // Adjust the size according to your LCD screen
  lcd.print("Looking for");
  lcd.setCursor(0, 1);
  lcd.print("pulse...");

  heart.attach(SERVO_PIN);

  // Record the start time
  startTime = millis(); 

  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

}

void loop() {
  // Start the servo and LCD after 5 seconds
  if (millis() - startTime >= 5000) {
    // Start displaying messages after 5 seconds
    displayMessages();
    
    // Start moving the servo
    servoMoving = true;
  }

  // Move servo to the rhythm of 75 BPM until the last LCD screen
  if (servoMoving && millis() - startTime >= 5000 && millis() - startTime < 55000) {
    moveServo(75);
  }

  // Move servo to the rhythm of 100 BPM for 5 seconds after the last LCD screen
  if (servoMoving && millis() - startTime >= 55000 && millis() - startTime < 65000) {
    moveServo(100);
  } else if (millis() - startTime >= 65000) {
    // If the time exceeds 65 seconds, stop the servo
    servoMoving = false;
    heart.detach(); // Detach servo to stop movement
  }
}

void displayMessages() {
  unsigned long currentTime = millis() - startTime;

  if (currentTime >= 5000 && currentTime < 25000) { // Display "Reading pulse..." for 20 seconds
    lcd.clear();
    lcd.print("Reading pulse...");
  } else if (currentTime >= 25000 && currentTime < 35000) { // Display "350000 heartbeats left" for 10 seconds
    lcd.clear();
    lcd.print("35 million");
    lcd.setCursor(0, 1);
    lcd.print("heartbeats left");
  } else if (currentTime >= 35000 && currentTime < 45000) { // Display "= 1 year until death" for 10 seconds
    lcd.clear();
    lcd.print("= 1 year until");
    lcd.setCursor(0, 1);
    lcd.print("death");
  } else if (currentTime >= 45000 && currentTime < 55000) { // Display "You are number 429 on the list" for 10 seconds
    lcd.clear();
    lcd.print("You are number");
    lcd.setCursor(0, 1);
    lcd.print("429 on the list");
  } else if (currentTime >= 55000 && currentTime < 65000) { // Display "What if this were you" for 10 seconds
    lcd.clear();
    lcd.print("What if this");
    lcd.setCursor(0, 1);
    lcd.print("were you?");
  }
}

void moveServo(int bpm) {
  // Calculate the total interval between heartbeats (including pause)
  int totalInterval = 60000 / bpm; // Total interval in milliseconds
  
  // Pause time between each heartbeat cycle
  int pauseTime = 500; // 500 ms pause
  
  // Remaining time for servo movements
  int remainingTime = totalInterval - pauseTime;
  
  // Delay time for each direction of the servo movement
  int delayTime = remainingTime / 2;

  // Move servo to one side
  heart.write(0); // Move to one side
  delay(delayTime);    // Wait for the first part of the interval
// Fade in LED
  for (int brightness = 0; brightness <= 255; brightness += 40) {
    for (int i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, strip.Color(brightness, 0, 0)); // Set color to red
    }
    strip.show();
    delay(25);  // Adjust the delay to control the speed of the fading
  }
  // Move servo to the other side
  heart.write(180); // Move to the other side
    // Fade out LED
  for (int brightness = 255; brightness >= 0; brightness -= 40) {
    for (int i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, strip.Color(brightness, 0, 0)); // Set color to red
    }
    strip.show();
    delay(20);  // Adjust the delay to control the speed of the fading
  }
  delay(delayTime);      // Wait for the second part of the interval

  // Pause between heartbeat cycles
  delay(pauseTime);
}
