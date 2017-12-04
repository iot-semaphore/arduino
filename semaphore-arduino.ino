
// Pin settings
int firstRedPin = 0;
int firstGreenPin = 1;
int firstBluePin = 2;

int secondRedPin = 3;
int secondGreenPin = 4;
int secondBluePin = 5;

int buzzerPin = A6;

enum status {
  running,
  failure,
  success
};

enum components {
  firstLed,
  secondGroup,
  buzzer
};

// Life cycle
void setup() {
  setupBuzzer();
  setupFirstLed();
  setupSecondLed();
}

void loop() {
//  tone(buzzerPin, 1000);
//  delay(300);
//  noTone(buzzerPin);
//  delay(3000);
  
  setStatus(firstLed, running);
  delay(2000);
  
  setStatus(firstLed, success);
  delay(2000);

  setStatus(firstLed, failure);
  delay(2000);
}

// Set colors
void setColor(int redPin, int redValue, int greenPin, int greenValue, int bluePin, int blueValue) {
  analogWrite(redPin, redValue);
  analogWrite(greenPin, greenValue);
  analogWrite(bluePin, blueValue);
}

void setRunningColor(int redPin, int greenPin, int bluePin) {
  setColor(redPin, 255, greenPin, 160, bluePin, 0);
}

void setFailureColor(int redPin, int greenPin, int bluePin) {
  setColor(redPin, 255, greenPin, 0, bluePin, 0);
}

void setSuccessColor(int redPin, int greenPin, int bluePin) {
  setColor(redPin, 0, greenPin, 0, bluePin, 255);
}

void setStatus(components pin, status status) {
  int redPin;
  int greenPin;
  int bluePin;

  if (pin == firstLed) {
    redPin = firstRedPin;
    greenPin = firstGreenPin;
    bluePin = firstBluePin;
  }
  else if (pin == secondGroup) {
    redPin = secondRedPin;
    greenPin = secondGreenPin;
    bluePin = secondBluePin;
  }

  if (status == running) {
    setRunningColor(redPin, greenPin, bluePin);
  }
  else if (status == success) {
    setSuccessColor(redPin, greenPin, bluePin);
  }
  else if (status == failure) {
    setFailureColor(redPin, greenPin, bluePin);
  }
}


// Setup components
void setupBuzzer() {
  pinMode(buzzerPin, OUTPUT);
}

void setupFirstLed() {
  pinMode(firstRedPin, OUTPUT);
  pinMode(firstGreenPin, OUTPUT);
  pinMode(firstBluePin, OUTPUT);
}

void setupSecondLed() {
  pinMode(secondRedPin, OUTPUT);
  pinMode(secondGreenPin, OUTPUT);
  pinMode(secondBluePin, OUTPUT);
}
