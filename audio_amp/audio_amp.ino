#include <IRremote.h>
#include <Wire.h>

#define MAX9744_I2CADDR 0x4B
#define PIN_POWER_LED 10
#define PIN_ACTIVITY_LED 11
#define PIN_MUTE 8
#define PIN_SHUTDOWN 9
#define PIN_RECV 12
#define ACTIVITY_INTERVAL 250

#define POWER 0x10EFD827 
#define A 0x10EFF807 
#define B 0x10EF7887
#define C 0x10EF58A7
#define UP 0x10EFA05F
#define DOWN 0x10EF00FF
#define LEFT 0x10EF10EF
#define RIGHT 0x10EF807F
#define SELECT 0x10EF20DF

IRrecv irrecv(PIN_RECV);
decode_results results;

int8_t volume = 20;
int8_t mute = 1;
int8_t powered = 1;
int8_t activity = 0;
unsigned long previousMillis = 0;

void setup() {
  pinMode(PIN_POWER_LED, OUTPUT);
  pinMode(PIN_ACTIVITY_LED, OUTPUT);
  pinMode(PIN_MUTE, OUTPUT);
  pinMode(PIN_SHUTDOWN, OUTPUT);

//  Serial.begin(9600);
//  Serial.println("Audio amp");
  
  Wire.begin();

  digitalWrite(PIN_SHUTDOWN, powered);
  digitalWrite(PIN_POWER_LED, powered);
  digitalWrite(PIN_ACTIVITY_LED, 0);

  delay(250);
  
  if (!setvolume(volume)) {
    Serial.println("Failed to set volume, MAX9744 not found!");
    while (1);
  }

  digitalWrite(PIN_MUTE, mute);
  irrecv.enableIRIn();
}

void toggleMute() {
  digitalWrite(PIN_MUTE, mute);
//  Serial.print("Mute: ");
//  Serial.println(!mute);
}

void toggleActivityLed() {
  activity = 1;
  previousMillis = millis();
  
  digitalWrite(PIN_POWER_LED, 0);
  digitalWrite(PIN_ACTIVITY_LED, 1);
}

boolean setvolume(int8_t v) {
  if (v > 63) v = 63;
  if (v < 0) v = 0;
  
//  Serial.print("Setting volume to ");
//  Serial.println(v);

  Wire.beginTransmission(MAX9744_I2CADDR);
  Wire.write(v);
  if (Wire.endTransmission() == 0) 
    return true;
  else
    return false;
}

void loop() {
  unsigned long currentMillis = millis();

  if (activity && currentMillis - previousMillis >= ACTIVITY_INTERVAL) {
    activity = 0;

    digitalWrite(PIN_POWER_LED, powered);
    digitalWrite(PIN_ACTIVITY_LED, 0);
  }
  
  if (irrecv.decode(&results)) {
    if (results.value == POWER) {
      powered ^= 1;
      mute = powered;
      
      setvolume(volume);
      digitalWrite(PIN_MUTE, mute);
      digitalWrite(PIN_SHUTDOWN, powered);
      digitalWrite(PIN_POWER_LED, powered);

//      Serial.print("Shutdown: ");
//      Serial.println(!powered);

    } else if (results.value == SELECT && powered) {
      mute ^= 1;

      if (mute) {
        toggleActivityLed();
      } else {
        digitalWrite(PIN_POWER_LED, 0);
        digitalWrite(PIN_ACTIVITY_LED, 1);
      }
      toggleMute();
      setvolume(volume);
    } else if (results.value == LEFT && powered) {
      if (volume - 1 >= 0) {
        mute = 1;
        volume--;
  
        toggleActivityLed();
        toggleMute();
        setvolume(volume);
      }
    } else if (results.value == RIGHT && powered) {
      if (volume + 1 <= 63) {
        mute = 1;
        volume++;
  
        toggleActivityLed();
        toggleMute();
        setvolume(volume);
      }
    }

    irrecv.resume();
  }
}
