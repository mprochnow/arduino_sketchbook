#include <LiquidCrystal.h>

// LCD
#define PIN_RS  8
#define PIN_RW  9
#define PIN_D4  4
#define PIN_D5  5
#define PIN_D6  6
#define PIN_D7  7
#define NUM_ROWS 2
#define NUM_COLS 16

// Keypad
#define PIN_KEY_IN 0
#define THRESHOLD 5
// result values
#define SAMPLE_WAIT -1
#define NO_KEY 0
#define UP_KEY 3
#define DOWN_KEY 4
#define LEFT_KEY 2
#define RIGHT_KEY 5
#define SELECT_KEY 1
// ARV => analogue read value
#define UPKEY_ARV 144
#define DOWNKEY_ARV 329
#define LEFTKEY_ARV 505
#define RIGHTKEY_ARV 0
#define SELKEY_ARV 742
#define NOKEY_ARV 1023

class KeyPad
{
public:
  KeyPad(int refreshRate=10):
    _refreshRate(refreshRate),
    _oldTime(0),
    _curInput(NO_KEY),
    _curKey(NO_KEY),
    _prevKey(SAMPLE_WAIT)
  {
  }
  
  int getKey()
  {
    int result = SAMPLE_WAIT;
    
    if (millis() > _oldTime + _refreshRate)
    {
      _curInput = analogRead(PIN_KEY_IN);

      if (_curInput > UPKEY_ARV - THRESHOLD && _curInput < UPKEY_ARV + THRESHOLD )
        _curKey = UP_KEY;
      else if (_curInput > DOWNKEY_ARV - THRESHOLD && _curInput < DOWNKEY_ARV + THRESHOLD )
        _curKey = DOWN_KEY;
      else if (_curInput > RIGHTKEY_ARV - THRESHOLD && _curInput < RIGHTKEY_ARV + THRESHOLD )
        _curKey = RIGHT_KEY;
      else if (_curInput > LEFTKEY_ARV - THRESHOLD && _curInput < LEFTKEY_ARV + THRESHOLD )
        _curKey = LEFT_KEY;
      else if (_curInput > SELKEY_ARV - THRESHOLD && _curInput < SELKEY_ARV + THRESHOLD )
        _curKey = SELECT_KEY;
      else 
        _curKey = NO_KEY;
  
      if (_curKey != _prevKey)
      {
        _prevKey = _curKey;
        
        result = _curKey;
      }
      
      _oldTime = millis();
    }
    
    return result;
  }
  
private:
  int _refreshRate;
  unsigned long _oldTime;
  int _curInput;
  int _curKey;
  int _prevKey;

};

LiquidCrystal lcd(PIN_RS, PIN_RW, PIN_D4, PIN_D5, PIN_D6, PIN_D7); 
KeyPad keyPad;

void setup() 
{ 
  lcd.begin(NUM_COLS, NUM_ROWS);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Current key");
}

void loop() 
{
  int key = keyPad.getKey();
  
  if (key != SAMPLE_WAIT)
  {
    lcd.setCursor(12, 0);
    lcd.print(key);
  }
}
