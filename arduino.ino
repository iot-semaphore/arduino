// Internal headers
#include "PinningComponents.h"
#include "Enums.h"

// External libraries
#include <SPI.h>
#include <WiFi101.h>
#include <ArduinoJson.h>

bool beeping = false;

char ssid[] = ""; // WiFi Network name
char pass[] = ""; // WiFi password

int keyIndex = 0;
int wireless_status = WL_IDLE_STATUS;

char server[] = "semaphore.vapor.cloud";

WiFiClient client;

// Life cycle
void setup() {
  setupButton();
  setupBuzzer();
  setupFirstLed();
  setupSecondLed();

  Serial.begin(9600);
  while(!Serial){};

  while (wireless_status != WL_CONNECTED) {
    echo("Attempting to connect to SSID: ");
    echoln(ssid);

    wireless_status = WiFi.begin(ssid, pass);
    delay(5000);
  }

  echo("Connected to WiFi: ");
  echoln(ssid);
  
  long signal_strengh = WiFi.RSSI();
  echo("Signal strengh: ");
  Serial.print(signal_strengh);
  echoln(" dBm");
  echoln("################################");
}

void loop() {
  // Connecting to server
  client.connect(server, 80) || die("Connection failed");
  echo("Connected to ");
  echoln(server);

  // Requesting data from endpoint '/'
  client.println("GET / HTTP/1.1");
  client.print("Host: ");
  client.println(server);
  client.println("Connection: close");
  client.println() || die("Failed to request data");

  if (client) {
    bool success = readRequest(client);
    if (success) {
      const size_t BUFFER_SIZE = JSON_OBJECT_SIZE(3) + JSON_ARRAY_SIZE(2) + 60;
      DynamicJsonBuffer jsonBuffer(BUFFER_SIZE);

      JsonArray &root = jsonBuffer.parseArray(client);

      if (!root.success())
        die("Parsing failed");

      echo("Extract values: ");
      Serial.println(root.size());
      echoln("##########################################");
      
      int jobsLength = root.size();
      for (int i = 0; i < jobsLength; i++) {
        JsonObject &job = root[i];
        String _name = job.get<String>("name");
        String _status = job.get<String>("status");
        status jobStatus = getStatus(_status);
        
        if (_name == "example1") {
          setStatus(firstLed, jobStatus);
        }
        else if (_name == "example2") {
          setStatus(secondGroup, jobStatus);
        }
        
        echo("Job name: ");
        Serial.println(_name);
        echo("Status: ");
        Serial.println(_status);
        echoln("##########################################");
      }

      echoln("##########################################");
      echoln("Waiting 10 seconds before the next request");
      echoln("##########################################");
      delay(10000);
    }
  }
}

bool readRequest(WiFiClient &client) {
  bool currentLineIsBlank = true;
  while (client.connected()) {
    if (client.available()) {
      char c = client.read();

      if (c == '\n' && currentLineIsBlank)
        return true;
      else if (c == '\n')
        currentLineIsBlank = true;
      else if (c != '\r')
        currentLineIsBlank = false;
    }
  }  

  return false;
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

status getStatus(String value) {
  if (value == "RUNNING")
    return running;
  
  else if (value == "SUCCESS")
    return success;

  else
    return failure;
}

void setStatus(components pin, status status) {
  echoln("setStatus called");
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

// Utils (thanks ArduinoJSON)
void echoln(const char *message) {
  Serial.println(message);  
}

void echo(const char *message) {
  Serial.print(message);  
}

bool die(const char *message) {
  echo(message);
  while(true);

  return false;
}
