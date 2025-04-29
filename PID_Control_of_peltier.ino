#include <Wire.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// DS18B20 Temperature Sensor setup
#define ONE_WIRE_BUS_PIN 4
OneWire oneWire(ONE_WIRE_BUS_PIN);
DallasTemperature sensors(&oneWire);

// OLED display setup
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Rotary potentiometer setup
int steeringWheelPin = A0;

// PWM pin for Peltier control
#define PELTIER_PIN 9 // PWM-capable pin connected to MOSFET gate

// PID controller variables
float referenceTemperature;
float measuredTemperature;
float errorValue;
float previousError = 0;
float integral = 0;
float derivative = 0;

// PID constants (adjust as necessary)
float kp = 2.0;  // Proportional gain
float ki = 0.5;  // Integral gain
float kd = 1.0;  // Derivative gain

void setup() {
  Serial.begin(9600);

  // Initialize DS18B20 temperature sensor
  sensors.begin();

  // Initialize OLED display
  if (!display.begin(SSD1306_I2C_ADDRESS, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Initializing...");
  display.display();
  delay(1000);

  pinMode(PELTIER_PIN, OUTPUT);
}

void loop() {
  // Read the rotary potentiometer value for reference temperature
  int steeringValue = analogRead(steeringWheelPin);
  referenceTemperature = map(steeringValue, 0, 1023, 10, 25);

  // Read temperature from DS18B20
  sensors.requestTemperatures();
  measuredTemperature = sensors.getTempCByIndex(0);

  // Calculate the error
  errorValue = referenceTemperature - measuredTemperature;

  // PID control calculations
  integral += errorValue;                  // Integral term
  derivative = errorValue - previousError; // Derivative term
  float pidOutput = kp * errorValue + ki * integral + kd * derivative;
  previousError = errorValue;

  // Constrain the PID output to PWM limits (0-255)
  pidOutput = constrain(pidOutput, 0, 255);

  // Send PWM signal to Peltier
  analogWrite(PELTIER_PIN, pidOutput);

  // Display values on Serial Monitor
  Serial.print("Ref Temp: ");
  Serial.print(referenceTemperature);
  Serial.print(" | Measured Temp: ");
  Serial.print(measuredTemperature);
  Serial.print(" | Error: ");
  Serial.print(errorValue);
  Serial.print(" | PID Output: ");
  Serial.println(pidOutput);

  // Display values on OLED
  display.clearDisplay();
  
  display.setCursor(0, 0);
  display.setTextSize(1);
  display.print("Ref Temp: ");
  display.print(referenceTemperature);
  display.println(" C");
  
  display.setCursor(0, 20);
  display.print("Measured Temp: ");
  display.print(measuredTemperature);
  display.println(" C");
  
  display.setCursor(0, 40);
  display.print("Error: ");
  display.print(errorValue);
  display.println(" C");
  
  display.setCursor(0, 55);
  display.print("PWM Output: ");
  display.print(pidOutput);
  display.println();
  
  display.display();

  delay(1000); // Update every second
}
