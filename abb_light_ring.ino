// Define the pins for the 8 LEDs
const int ledPins[8] = {2, 3, 4, 5, 6, 7, 8, 9};

// Delay time in milliseconds for cycling mode
const int delayTime = 200;

// Brightness control
int brightness = 255;
const int step = 25;

// Mode variables
bool turnedOn = true;
bool allOnMode = false;

unsigned long lastUpdateTime = 0;
int currentLed = 0;

void setup() {
    // Initialize each LED pin as OUTPUT
    for (int i = 0; i < 8; i++) {
        pinMode(ledPins[i], OUTPUT);
    }

    // Start serial communication
    Serial.begin(9600);
}

void loop() {
    // Constantly check serial input
    if (Serial.available() > 0) {
        char command = Serial.read();

        // Switch modes based on received command
        if (command == 'A') {
            allOnMode = true;    // Switch to "all on" mode
            Serial.println("Mode set: ALL ON");
        } else if (command == 'C') {
            allOnMode = false;   // Switch back to cycling mode
            Serial.println("Mode set: CYCLE");
        } else if (command == 'O') {
            turnedOn = !turnedOn;
            Serial.print("Lights on: ");
            Serial.println(turnedOn);
        } else if (command == '+') {
            brightness += step;
            if (brightness > 255) {
                brightness = 255;
            }
            Serial.print("Increased brightness: ");
            Serial.println(brightness);
        } else if (command == '-') {
            brightness -= step;
            if (brightness < 0) {
                brightness = 0;
            }
            Serial.print("Decreased brightness: ");
            Serial.println(brightness);
        }
    }

    if (turnedOn) {
        if (allOnMode) {
            // Turn all LEDs on
            for (int i = 0; i < 8; i++) {
                analogWrite(ledPins[i], brightness);
            }
        } else {
            // Check if it's time to update the LED
            if (millis() - lastUpdateTime >= delayTime) {
                // Turn off the previous LED
                digitalWrite(ledPins[currentLed], LOW);

                // Move to the next LED
                currentLed = (currentLed + 1) % 8;

                // Turn on the current LED with the specified brightness
                analogWrite(ledPins[currentLed], brightness);

                // Update the last update time
                lastUpdateTime = millis();
            }
        }
    } else {
        // Turn off all LEDs if turnedOn is false
        for (int i = 0; i < 8; i++) {
            digitalWrite(ledPins[i], LOW);
        }
    }
}
