
// Pin settings
int firstRedPin = 3;
int firstGreenPin = 4;
int firstBluePin = 5;

int secondRedPin = 10;
int secondGreenPin = 9;
int secondYellowPin = 8;

int buzzerPin = 6;
int buttonPin = 7;

bool beeping = false;

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
  setupButton();
  setupBuzzer();
  setupFirstLed();
  setupSecondLed();
}

void loop() {
//  beeping = true;
//  tone(buzzerPin, 1000);
//  delay(300);
//  noTone(buzzerPin);
//  delay(3000);


  setStatus(firstLed, running);
  setStatus(secondGroup, running);
  delay(500);
  
  setStatus(firstLed, success);
  setStatus(secondGroup, success);
  delay(500);

  setStatus(firstLed, failure);
  setStatus(secondGroup, failure);
  delay(500);
}

// RGB LED set colors
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
  setColor(redPin, 0, greenPin, 255, bluePin, 0);
}

void setStatus(components pin, status status) {
  // as we are using one RGB Led and three colored Leds, 
  // we need to do this early return
  if (pin == secondGroup) {
    if (status == running) {
      digitalWrite(secondRedPin, LOW);
      digitalWrite(secondGreenPin, LOW);
      digitalWrite(secondYellowPin, HIGH);
    }
    else if (status == success) {
      digitalWrite(secondRedPin, LOW);
      digitalWrite(secondGreenPin, HIGH);
      digitalWrite(secondYellowPin, LOW);
    }
    else if (status == failure) {
      digitalWrite(secondRedPin, HIGH);
      digitalWrite(secondGreenPin, LOW);
      digitalWrite(secondYellowPin, LOW);
    }

    return;
  }


  // Continue only if the component selected is the first, but
  // we still enable to add more RGB LEDs and set else statement
  
  int redPin;
  int greenPin;
  int bluePin;

  if (pin == firstLed) {
    redPin = firstRedPin;
    greenPin = firstGreenPin;
    bluePin = firstBluePin;
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
void setupButton() {
  pinMode(buttonPin, INPUT);
}

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
  pinMode(secondYellowPin, OUTPUT);
}
