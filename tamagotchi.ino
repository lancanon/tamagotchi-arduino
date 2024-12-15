#include <LiquidCrystal.h>

// LCD setup
LiquidCrystal lcd(3, 4, 5, 6, 7, 8); // Update LCD pins to 3, 4, 5, 6, 7, 8

// Buzzer pin
const int buzzerPin = 9; // Pin connected to the buzzer

// LED Matrix rows and columns
const int rowPins[] = {A1, A2, A3, A4}; // Rows connected to A1-A4
const int colPins[] = {10, 11, 12, 13}; // Columns connected to 10, 11, 12, 13

// Menu options
const char* menuOptions[] = {
  "Happy Face",
  "Hungry",
  "Sleepy",
  "Play"
};
const int numOptions = 4; // Number of menu options

// Pin definitions
const int potPin = A0;    // Potentiometer pin
const int buttonPin = 2;  // Push button connected to pin 2

// Variables
int currentSelection = 0; // Current menu selection
bool buttonPressed = false;

void setup() {
  // LCD setup
  lcd.begin(16, 2);
  lcd.print("Menu Selector");

  // Serial Monitor setup
  Serial.begin(9600); // Initialize the Serial Monitor at 9600 baud rate
  Serial.println("Starting Menu Selector...");
  Serial.println("Use the potentiometer to navigate the menu.");

  // LED Matrix setup
  for (int i = 0; i < 4; i++) {
    pinMode(rowPins[i], OUTPUT);
    pinMode(colPins[i], OUTPUT);
  }
  clearMatrix(); // Ensure all LEDs are off

  // Button setup
  pinMode(buttonPin, INPUT_PULLUP); // Enable internal pull-up resistor for button

  // Buzzer setup
  pinMode(buzzerPin, OUTPUT); // Set buzzer pin as output
}

void loop() {
  // Read raw potentiometer value
  int potValue = analogRead(potPin);
  int newSelection = map(potValue, 0, 1023, 0, numOptions - 1);

  // If the selection has changed, update the display and Serial Monitor
  if (newSelection != currentSelection) {
    currentSelection = newSelection;
    displayMenu();
    Serial.print("Current Selection: ");
    Serial.println(menuOptions[currentSelection]); // Print selected menu option
  }

  // Check if the button is pressed
  if (digitalRead(buttonPin) == LOW) { // Button is pressed when LOW
    delay(200); // Debounce delay
    if (!buttonPressed) {
      buttonPressed = true;
      Serial.print("Executing: ");
      Serial.println(menuOptions[currentSelection]); // Log selected option
      playSound();       // Play sound when button is pressed
      executeSelection();
    }
  } else {
    buttonPressed = false;
  }
}

// Function to display the menu
void displayMenu() {
  lcd.clear(); // Clear the display
  lcd.setCursor(0, 0); // Move to the first line, first column
  lcd.print("Select: ");
  lcd.print(menuOptions[currentSelection]); // Display the current menu option

  lcd.setCursor(0, 1); // Move to the second line, first column
  lcd.print("Press to confirm");
}

// Function to execute the selected option
void executeSelection() {
  lcd.setCursor(0, 1);
  lcd.print("Executing...      ");

  // Display the selected frame on the LED matrix
  switch (currentSelection) {
    case 0:
      drawHappyFace(); // Display Happy Face (static)
      break;
    case 1:
      drawHungryAnimation(); // Display Hungry animation
      break;
    case 2:
      drawSleepyFace(); // Display Sleepy (static)
      break;
    case 3:
      drawPlayAnimation(); // Display Play animation
      break;
  }

  delay(1000); // Wait to show execution
  lcd.setCursor(0, 1);
  lcd.print("Done!             ");
  Serial.println("Done!"); // Log completion to Serial Monitor
  delay(1000); // Show "Done!" for a moment
}

// Function to clear all LEDs
void clearMatrix() {
  for (int i = 0; i < 4; i++) {
    digitalWrite(rowPins[i], LOW);  // Set all rows to LOW
    digitalWrite(colPins[i], HIGH); // Set all columns to HIGH
  }
}

// Function to light up a specific LED
void lightUpLED(int row, int col) {
  clearMatrix();                  // Turn off all LEDs
  digitalWrite(rowPins[row], HIGH); // Activate the selected row
  digitalWrite(colPins[col], LOW);  // Activate the selected column
}

// Function to draw a static Happy Face
void drawHappyFace() {
  int happyFace[4][4] = {
    {0, 1, 1, 0}, // Row 1
    {1, 0, 0, 1}, // Row 2
    {1, 1, 1, 1}, // Row 3
    {0, 1, 1, 0}  // Row 4
  };

  for (int row = 0; row < 4; row++) {
    for (int col = 0; col < 4; col++) {
      if (happyFace[row][col] == 1) {
        lightUpLED(row, col);
      }
    }
    delay(200);
  }
}

// Function to draw Hungry animation
void drawHungryAnimation() {
  for (int i = 0; i < 3; i++) {
    clearMatrix();
    delay(200);
    lightUpLED(i, i);
    lightUpLED(i, 3 - i);
    delay(200);
  }
}

// Function to draw a static Sleepy Face
void drawSleepyFace() {
  int sleepyFace[4][4] = {
    {0, 1, 1, 0},
    {1, 0, 0, 1},
    {0, 1, 1, 0},
    {1, 0, 0, 1}
  };

  for (int row = 0; row < 4; row++) {
    for (int col = 0; col < 4; col++) {
      if (sleepyFace[row][col] == 1) {
        lightUpLED(row, col);
      }
    }
    delay(200);
  }
}

// Function to draw Play animation
void drawPlayAnimation() {
  for (int frame = 0; frame < 4; frame++) {
    for (int i = 0; i < 4; i++) {
      lightUpLED(i, (i + frame) % 4);
    }
    delay(200);
  }
}

// Function to play sound
void playSound() {
  switch (currentSelection) {
    case 0: // Tune for "Happy Face"
      tone(buzzerPin, 262, 200); // C4
      delay(200);
      tone(buzzerPin, 294, 200); // D4
      delay(200);
      tone(buzzerPin, 330, 200); // E4
      delay(200);
      break;

    case 1: // Tune for "Hungry"
      tone(buzzerPin, 392, 200); // G4
      delay(200);
      tone(buzzerPin, 440, 200); // A4
      delay(200);
      tone(buzzerPin, 494, 200); // B4
      delay(200);
      break;

    case 2: // Tune for "Sleepy"
      tone(buzzerPin, 523, 200); // C5
      delay(200);
      tone(buzzerPin, 494, 200); // B4
      delay(200);
      tone(buzzerPin, 440, 200); // A4
      delay(200);
      break;

    case 3: // Tune for "Play"
      tone(buzzerPin, 349, 200); // F4
      delay(200);
      tone(buzzerPin, 392, 200); // G4
      delay(200);
      tone(buzzerPin, 349, 400); // F4 (longer)
      delay(400);
      break;

    default:
      tone(buzzerPin, 262, 200); // Default tone
      delay(200);
      break;
  }
}

