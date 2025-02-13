#define GREEN_LED 12
#define RED_LED 11
#define BLUE_LED 10
#define YELLOW_LED 9

#define GREEN_BUTTON 6
#define RED_BUTTON 5
#define BLUE_BUTTON 4
#define YELLOW_BUTTON 3

#define GREEN_TONE 415
#define RED_TONE 310
#define BLUE_TONE 250
#define YELLOW_TONE 200

#define BUZZER_PIN 2

#define DELAY 500
#define INITIAL_SEQUENCE_LENGTH 3

unsigned short current_sequence_length = INITIAL_SEQUENCE_LENGTH;

enum LedColor {
  red = RED_LED,
  green = GREEN_LED,
  blue = BLUE_LED,
  yellow = YELLOW_LED
};

void blinkLed(LedColor color, int delayTime);
void getRandomSequence(LedColor* sequence, unsigned short sequence_length);
void displaySequence(const LedColor* sequence, unsigned short sequence_length);
bool handleUserInput(const LedColor* sequence, unsigned short sequence_length);
void announceMatch();
void announceMismatch();

void setup() {
  Serial.begin(115200);
  
  pinMode(BLUE_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  
  pinMode(BLUE_BUTTON, INPUT);
  pinMode(GREEN_BUTTON, INPUT);
  pinMode(RED_BUTTON, INPUT);
  pinMode(YELLOW_BUTTON, INPUT);
  
  pinMode(BUZZER_PIN, OUTPUT);
}

void loop() {
  LedColor sequence[current_sequence_length];
  
  getRandomSequence(sequence, current_sequence_length);
  displaySequence(sequence, current_sequence_length);
  
  bool isWinner = handleUserInput(sequence, current_sequence_length);
  if (isWinner) {
  	announceMatch();
    current_sequence_length++;
  }
  else {
  	announceMismatch();
    current_sequence_length = INITIAL_SEQUENCE_LENGTH;
  }
}

void blinkLed(LedColor color, int delayTime) {
  switch (color) {
  	case blue: tone(BUZZER_PIN, BLUE_TONE, delayTime); break;
    case green: tone(BUZZER_PIN, GREEN_TONE, delayTime); break;
    case red: tone(BUZZER_PIN, RED_TONE, delayTime); break;
    case yellow: tone(BUZZER_PIN, YELLOW_TONE, delayTime); break;
  }
  digitalWrite(color, HIGH);
  delay(delayTime);
  digitalWrite(color, LOW);
  delay(delayTime);
}

void getRandomSequence(LedColor* sequence, unsigned short sequence_length) {
  for (unsigned short i = 0; i < sequence_length; i++)
    *(sequence + i) = (LedColor)(random(YELLOW_LED, (GREEN_LED + 1)));
}

void displaySequence(const LedColor* sequence, unsigned short sequence_length) {
  for (unsigned short i = 0; i < sequence_length; i++) {
  	blinkLed(sequence[i], DELAY);
  }
}

bool handleUserInput(const LedColor* sequence, unsigned short sequence_length) {
  uint8_t blueButtonState = 1;
  uint8_t greenButtonState = 1;
  uint8_t redButtonState = 1;
  uint8_t yellowButtonState = 1;
  
  for (unsigned short i = 0; i < sequence_length; i++) {
  	do {
      blueButtonState = digitalRead(BLUE_BUTTON);
      greenButtonState = digitalRead(GREEN_BUTTON);
      redButtonState = digitalRead(RED_BUTTON);
      yellowButtonState = digitalRead(YELLOW_BUTTON);
    } while ((blueButtonState + greenButtonState + redButtonState + yellowButtonState) != 3);
    
    if (!blueButtonState) {
      blinkLed(blue, DELAY);
      if (sequence[i] != blue) return false;
    }
    else if (!greenButtonState) {
      blinkLed(green, DELAY);
      if (sequence[i] != green) return false;
    }
    else if (!redButtonState) {
      blinkLed(red, DELAY);
      if (sequence[i] != red) return false;
    }
    else if (!yellowButtonState) {
      blinkLed(yellow, DELAY);
      if (sequence[i] != yellow) return false;
    }
    
    do {
  	  blueButtonState = digitalRead(BLUE_BUTTON);
      greenButtonState = digitalRead(GREEN_BUTTON);
      redButtonState = digitalRead(RED_BUTTON);
      yellowButtonState = digitalRead(YELLOW_BUTTON);
  	} while ((blueButtonState + greenButtonState + redButtonState + yellowButtonState) != 4);
  
  }
  
  return true;
}

void announceMatch() {
  for (unsigned short i = GREEN_LED; i >= YELLOW_LED; i--)
    blinkLed((LedColor)i, 100);
  for (unsigned short i = YELLOW_LED; i <= GREEN_LED; i++)
    blinkLed((LedColor)i, 100);
  delay(800);
}

void announceMismatch() {
  digitalWrite(GREEN_LED, HIGH);
  digitalWrite(RED_LED, HIGH);
  digitalWrite(BLUE_LED, HIGH);
  digitalWrite(YELLOW_LED, HIGH);
  
  delay(300);
  
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(RED_LED, LOW);
  digitalWrite(BLUE_LED, LOW);
  digitalWrite(YELLOW_LED, LOW);
  
  delay(800);
}