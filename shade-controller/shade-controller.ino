#include <dht.h>
#include <Servo.h>

#define stepPin 4
#define dirPin 7

#define soilWet 500
#define soilDry 300

#define sensorPower 7
#define sensorPin A0

#define microDelay 850

dht DHT;
Servo servo;

//servo
const int servoPin = 9;

//temp & hum
const int DHT11_PIN = 8;
const int TEMP_THRESHOLD = 25; // Adjust these thresholds according to your needs
const int HUM_THRESHOLD = 50; // Adjust these thresholds according to your needs

// Soil moisture thresholds
const int soilWetThreshold = 500;
const int soilDryThreshold = 750;

// shade bool
bool isShadePulled = false;
bool isShadeRetracted = true;

void setup() {
  // Sets the two pins as Outputs
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);

  pinMode(sensorPower, OUTPUT);

  // Initially keep the sensor OFF
	digitalWrite(sensorPower, LOW);

  Serial.begin(9600);
}

void loop() {
  int chk = DHT.read11(DHT11_PIN);
  float temperature = DHT.temperature;
  float humidity = DHT.humidity;
  int moisture = readSensor();

  displayReadings(temperature, humidity, moisture);

  // Check if temperature and humidity are valid (not NaN)
  if (!isnan(temperature) && !isnan(humidity)) {
    // Check if temperature and humidity are above the thresholds to pull the shade
    if (temperature > TEMP_THRESHOLD && humidity > HUM_THRESHOLD && !isShadePulled) {
      pullShade();
      retractShade();

    }
    // Check if temperature and humidity are below the thresholds to retract the shade
    else if (temperature <= TEMP_THRESHOLD && humidity <= HUM_THRESHOLD && !isShadeRetracted) {
      retractShade();
    }
  }

  if(!isnan(moisture)){
    if (moisture < soilWetThreshold) {
      Serial.println("Status: Soil is too wet");
    } else if (moisture >= soilWetThreshold && moisture < soilDryThreshold) {
      Serial.println("Status: Soil moisture is perfect");
    } else {
      Serial.println("Status: Soil is too dry - time to water!");
      waterThePlants();
    }
  }

  delay(5000); // Delay before next reading
}

void pullShade() {
  digitalWrite(dirPin, HIGH); // Enables the motor to move in a particular direction (change on testing)
  for (int x = 0; x < 2000; x++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(microDelay);    // by changing this time delay between the steps we can change the rotation speed
    digitalWrite(stepPin, LOW);
    delayMicroseconds(microDelay);
  }
    isShadePulled = true;
    isShadeRetracted = false;
}

void retractShade() {
  digitalWrite(dirPin, LOW); // Changes the rotations direction (change on testing)
  // Makes 400 pulses for making two full cycle rotation
  for (int x = 0; x < (10 * 200); x++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(microDelay);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(microDelay);
  }
    isShadePulled = false;
    isShadeRetracted = true;
  delay(1000);
}

// Function to water the plants using the servo
void waterThePlants() {
  int startPos = 0;   
  int endPos = 60;   
  int increment = 10;  
  int delayTime = 20; 

  for (int pos = startPos; pos <= endPos; pos += increment) {
    servo.write(pos);
    delay(delayTime);
  }


  delay(3000);

  for (int pos = endPos; pos >= startPos; pos -= increment) {
    servo.write(pos);
    delay(delayTime);
  }
}

int readSensor() {
  digitalWrite(sensorPower, HIGH);
  delay(20);                           // Allow power to settle
  int val = analogRead(sensorPin);      // Read the analog value from sensor
  digitalWrite(sensorPower, LOW);   // Turn the sensor OFF
  return val;                          // Return analog moisture value
}

void displayReadings(float temp, float hum, int moisture){
  Serial.print("Temperature: ");
  Serial.print(temp);
  Serial.println(" °C");

  Serial.print("Humidity: ");
  Serial.print(hum);
  Serial.println(" %");

  Serial.print("Soil Moisture: ");
  Serial.println(moisture);

  Serial.print("Shade status: ");
  Serial.println(isShadePulled == true ? "Open" : "Closed");
}


