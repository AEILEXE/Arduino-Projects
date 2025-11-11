#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define JoyX 2
#define PIN_AUTOPLAY 1
#define PIN_READWRITE 10
#define PIN_CONTRAST 12

#define SPRITE_RUN1 1
#define SPRITE_RUN2 2
#define SPRITE_JUMP 3
#define SPRITE_JUMP_UPPER '.' 
#define SPRITE_JUMP_LOWER 4
#define SPRITE_TERRAIN_EMPTY ' '
#define SPRITE_TERRAIN_SOLID 5
#define SPRITE_TERRAIN_SOLID_RIGHT 6
#define SPRITE_TERRAIN_SOLID_LEFT 7
#define SPRITE_SPIKE 8

#define BOY_HORIZONTAL_POSITION 1

#define TERRAIN_WIDTH 16
#define TERRAIN_EMPTY 0
#define TERRAIN_LOWER_BLOCK 1
#define TERRAIN_UPPER_BLOCK 2

#define BOY_POSITION_OFF 0
#define BOY_POSITION_RUN_LOWER_1 1
#define BOY_POSITION_RUN_LOWER_2 2
#define BOY_POSITION_JUMP_1 3
#define BOY_POSITION_JUMP_2 4
#define BOY_POSITION_JUMP_3 5
#define BOY_POSITION_JUMP_4 6
#define BOY_POSITION_JUMP_5 7
#define BOY_POSITION_JUMP_6 8
#define BOY_POSITION_JUMP_7 9
#define BOY_POSITION_JUMP_8 10
#define BOY_POSITION_RUN_UPPER_1 11
#define BOY_POSITION_RUN_UPPER_2 12

LiquidCrystal_I2C lcd(0x27, 16, 2);
static char terrainUpper[TERRAIN_WIDTH + 1];
static char terrainLower[TERRAIN_WIDTH + 1];
static bool buttonPushed = false;

void initializeGraphics() {
  byte Alien[8] = {
    0b11111,
    0b10101,
    0b11111,
    0b11111,
    0b01110,
    0b01010,
    0b11011,
    0b00000
  };

  byte graphics[][8] = {
    
    {
    0b11111, 
    0b10101, 
    0b11111,
    0b11111, 
    0b01110, 
    0b01010, 
    0b11011, 
 
    }, 
    {0b11111, 
    0b10101, 
    0b11111, 
    0b11111, 
    0b01110, 
    0b01010, 
    0b11011, 
    0b00000}, 

    {0b11111, 
    0b10101, 
    0b11111, 
    0b11111, 
    0b01110, 
    0b01010, 
    0b00000, 
    0b00000}, 

    {0b00000, 
    0b01010, 
    0b11111, 
    0b11111, 
    0b01110, 
    0b00100, 
    0b00000, 
    0b00000}, 

    {0b11111, 
    0b11111, 
    0b11111, 
    0b11111, 
    0b11111, 
    0b11111, 
    0b11111, 
    0b11111}, 

    {B00011, B00011, B00011, B00011, B00011, B00011, B00011, B00011}, 

    {0b11000, 
    0b11000, 
    0b11000, 
    0b11000, 
    0b11000, 
    0b11000, 
    0b11000, 
    0b11000}  
  };

  for (int i = 0; i < 7; ++i) {
    lcd.createChar(i + 1, graphics[i]);
  }

  for (int i = 0; i < TERRAIN_WIDTH; ++i) {
    terrainUpper[i] = SPRITE_TERRAIN_EMPTY;
    terrainLower[i] = SPRITE_TERRAIN_EMPTY;
  }
}

void advanceTerrain(char* terrain, byte newTerrain) {
  for (int i = 0; i < TERRAIN_WIDTH; ++i) {
    char current = terrain[i];
    char next = (i == TERRAIN_WIDTH - 1) ? newTerrain : terrain[i + 1];
    switch (current) {
      case SPRITE_TERRAIN_EMPTY:
        terrain[i] = (next == SPRITE_TERRAIN_SOLID) ? SPRITE_TERRAIN_SOLID_RIGHT : SPRITE_TERRAIN_EMPTY;
        break;
      case SPRITE_TERRAIN_SOLID:
        terrain[i] = (next == SPRITE_TERRAIN_EMPTY) ? SPRITE_TERRAIN_SOLID_LEFT : SPRITE_TERRAIN_SOLID;
        break;
      case SPRITE_TERRAIN_SOLID_RIGHT:
        terrain[i] = SPRITE_TERRAIN_SOLID;
        break;
      case SPRITE_TERRAIN_SOLID_LEFT:
        terrain[i] = SPRITE_TERRAIN_EMPTY;
        break;
    }
  }
}

bool drawBoy(byte position, char* terrainUpper, char* terrainLower, unsigned int score) {
  bool collide = false;
  char upperSave = terrainUpper[BOY_HORIZONTAL_POSITION];
  char lowerSave = terrainLower[BOY_HORIZONTAL_POSITION];
  byte upper, lower;

  switch (position) {
    case BOY_POSITION_OFF: upper = lower = SPRITE_TERRAIN_EMPTY; break;
    case BOY_POSITION_RUN_LOWER_1: upper = SPRITE_TERRAIN_EMPTY; lower = SPRITE_RUN1; break;
    case BOY_POSITION_RUN_LOWER_2: upper = SPRITE_TERRAIN_EMPTY; lower = SPRITE_RUN2; break;
    case BOY_POSITION_JUMP_1:
    case BOY_POSITION_JUMP_8: upper = SPRITE_TERRAIN_EMPTY; lower = SPRITE_JUMP; break;
    case BOY_POSITION_JUMP_2:
    case BOY_POSITION_JUMP_7: upper = SPRITE_JUMP_UPPER; lower = SPRITE_JUMP_LOWER; break;
    case BOY_POSITION_JUMP_3:
    case BOY_POSITION_JUMP_4:
    case BOY_POSITION_JUMP_5:
    case BOY_POSITION_JUMP_6: upper = SPRITE_JUMP; lower = SPRITE_TERRAIN_EMPTY; break;
    case BOY_POSITION_RUN_UPPER_1: upper = SPRITE_RUN1; lower = SPRITE_TERRAIN_EMPTY; break;
    case BOY_POSITION_RUN_UPPER_2: upper = SPRITE_RUN2; lower = SPRITE_TERRAIN_EMPTY; break;
  }

  if (upper != ' ') {
    terrainUpper[BOY_HORIZONTAL_POSITION] = upper;
    collide = (upperSave != SPRITE_TERRAIN_EMPTY && upperSave != SPRITE_SPIKE);
  }
  if (lower != ' ') {
    terrainLower[BOY_HORIZONTAL_POSITION] = lower;
    collide |= (lowerSave != SPRITE_TERRAIN_EMPTY && lowerSave != SPRITE_SPIKE);
  }

  if (upperSave == SPRITE_SPIKE || lowerSave == SPRITE_SPIKE) {
    collide = true;
  }

  byte digits = (score > 9999) ? 5 : (score > 999) ? 4 : (score > 99) ? 3 : (score > 9) ? 2 : 1;

  terrainUpper[TERRAIN_WIDTH] = '\0';
  terrainLower[TERRAIN_WIDTH] = '\0';
  char temp = terrainUpper[16 - digits];
  terrainUpper[16 - digits] = '\0';
  lcd.setCursor(0, 0);
  lcd.print(terrainUpper);
  terrainUpper[16 - digits] = temp;
  lcd.setCursor(0, 1);
  lcd.print(terrainLower);
  lcd.setCursor(16 - digits, 0);
  lcd.print(score);

  terrainUpper[BOY_HORIZONTAL_POSITION] = upperSave;
  terrainLower[BOY_HORIZONTAL_POSITION] = lowerSave;

  return collide;
}

void buttonPush() {
  buttonPushed = true;
}

void setup() {
  pinMode(PIN_READWRITE, OUTPUT);
  digitalWrite(PIN_READWRITE, LOW);
  pinMode(PIN_CONTRAST, OUTPUT);
  digitalWrite(PIN_CONTRAST, LOW);
  pinMode(JoyX, INPUT);
  digitalWrite(JoyX, HIGH);
  pinMode(PIN_AUTOPLAY, OUTPUT);
  digitalWrite(PIN_AUTOPLAY, HIGH);

  attachInterrupt(0, buttonPush, FALLING);

  lcd.init();
  lcd.backlight();
  initializeGraphics();
}

void loop() {
  static byte boyPos = BOY_POSITION_RUN_LOWER_1;
  static byte newTerrainType = TERRAIN_EMPTY;
  static byte newTerrainDuration = 1;
  static bool playing = false;
  static bool blink = false;
  static unsigned int distance = 0;

  if (!playing) {
    drawBoy((blink) ? BOY_POSITION_OFF : boyPos, terrainUpper, terrainLower, distance >> 3);
    if (blink) {
      lcd.setCursor(0, 0);
      lcd.print("Press Start      ");
    }
    delay(250);
    blink = !blink;
    if (buttonPushed) {
      initializeGraphics();
      boyPos = BOY_POSITION_RUN_LOWER_1;
      playing = true;
      buttonPushed = false;
      distance = 0;
    }
    return;
  }

  byte terrainBlock = SPRITE_TERRAIN_EMPTY;
  if (newTerrainType == TERRAIN_LOWER_BLOCK) {
    terrainBlock = (random(6) == 0) ? SPRITE_SPIKE : SPRITE_TERRAIN_SOLID;
  }

  advanceTerrain(terrainLower, terrainBlock);
  advanceTerrain(terrainUpper, newTerrainType == TERRAIN_UPPER_BLOCK ? SPRITE_TERRAIN_SOLID : SPRITE_TERRAIN_EMPTY);

  if (--newTerrainDuration == 0) {
    if (newTerrainType == TERRAIN_EMPTY) {
      newTerrainType = (random(3) == 0) ? TERRAIN_UPPER_BLOCK : TERRAIN_LOWER_BLOCK;
      newTerrainDuration = 2 + random(10);
    } else {
      newTerrainType = TERRAIN_EMPTY;
      newTerrainDuration = 10 + random(10);
    }
  }

  if (buttonPushed) {
    if (boyPos <= BOY_POSITION_RUN_LOWER_2) boyPos = BOY_POSITION_JUMP_1;
    buttonPushed = false;
  }

  if (drawBoy(boyPos, terrainUpper, terrainLower, distance >> 3)) {
    playing = false;
  } else {
    if (boyPos == BOY_POSITION_RUN_LOWER_2 || boyPos == BOY_POSITION_JUMP_8) {
      boyPos = BOY_POSITION_RUN_LOWER_1;
    } else if ((boyPos >= BOY_POSITION_JUMP_3 && boyPos <= BOY_POSITION_JUMP_5) && terrainLower[BOY_HORIZONTAL_POSITION] != SPRITE_TERRAIN_EMPTY) {
      boyPos = BOY_POSITION_RUN_UPPER_1;
    } else if (boyPos >= BOY_POSITION_RUN_UPPER_1 && terrainLower[BOY_HORIZONTAL_POSITION] == SPRITE_TERRAIN_EMPTY) {
      boyPos = BOY_POSITION_JUMP_5;
    } else if (boyPos == BOY_POSITION_RUN_UPPER_2) {
      boyPos = BOY_POSITION_RUN_UPPER_1;
    } else {
      ++boyPos;
    }
    ++distance;

    digitalWrite(PIN_AUTOPLAY, terrainLower[BOY_HORIZONTAL_POSITION + 2] == SPRITE_TERRAIN_EMPTY ? HIGH : LOW);
  }

  delay(100);
}
