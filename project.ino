#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define i2c_Address 0x3c
#define OLED_RESET -1

Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define BUTTON_PIN PA0   

const unsigned char PROGMEM dinoRun1[] = {
  0x00, 0x00, 0x00, 
  0x00, 0x07, 0xFE, 
  0x00, 0x07, 0xFF, 
  0x00, 0x07, 0xF7, 
  0x00, 0x07, 0xFF, 
  0x00, 0x07, 0xFF, 
  0x00, 0x07, 0xFF, 
  0x00, 0x07, 0xC0, 
  0x00, 0x07, 0xFC, 
  0x80, 0x03, 0xC0, 
  0x80, 0x03, 0xC0, 
  0xC0, 0x03, 0xC0, 
  0xE0, 0x0F, 0xE0, 
  0xF0, 0x03, 0xE0, 
  0xF8, 0x03, 0xE0, 
  0xFE, 0x03, 0xE0, 
  0x7F, 0x03, 0xE0, 
  0x3F, 0xFF, 0xE0, 
  0x1F, 0xFF, 0xE0, 
  0x0F, 0xFF, 0xE0, 
  0x07, 0xC7, 0xC0, 
  0x03, 0x82, 0x00, 
  0x02, 0x03, 0x00, 
  0x02, 0x00, 0x00
};

const unsigned char PROGMEM dinoRun2[] = {
  0x00, 0x00, 0x00, 
  0x00, 0x07, 0xFE, 
  0x00, 0x07, 0xFF, 
  0x00, 0x07, 0xF7, 
  0x00, 0x07, 0xFF, 
  0x00, 0x07, 0xFF, 
  0x00, 0x07, 0xFF, 
  0x00, 0x07, 0xC0, 
  0x00, 0x07, 0xFC, 
  0x80, 0x03, 0xC0, 
  0x80, 0x03, 0xC0, 
  0xC0, 0x03, 0xC0, 
  0xE0, 0x0F, 0xE0, 
  0xF0, 0x03, 0xE0, 
  0xF8, 0x03, 0xE0, 
  0xFE, 0x03, 0xE0, 
  0x7F, 0x03, 0xE0, 
  0x3F, 0xFF, 0xE0, 
  0x1F, 0xFF, 0xE0, 
  0x0F, 0xFF, 0xE0, 
  0x07, 0xC7, 0xC0, 
  0x03, 0x01, 0x80, 
  0x03, 0x00, 0x00, 
  0x00, 0x00, 0x00
};

const unsigned char PROGMEM cactusBitmap[] = {
  0x18, 0x18, 0x18, 0x58, 0x58, 0x58, 0x58, 0x7E, 0x18, 0x18, 0x18, 0x18, 
  0x18, 0x18, 0x18, 0x00
};

const unsigned char PROGMEM bird1[] = {
  0x00, 0x00, 0x00, 
  0x00, 0x40, 0x00, 
  0x00, 0x60, 0x00, 
  0x00, 0x78, 0x00, 
  0x00, 0x7C, 0x00, 
  0x00, 0x7E, 0x00, 
  0x00, 0x5F, 0x00, 
  0x00, 0x4F, 0x80, 
  0x80, 0x67, 0xC0, 
  0xC0, 0x7F, 0xE0, 
  0xE0, 0x7F, 0xE0, 
  0x70, 0x3F, 0x00, 
  0x38, 0x1E, 0x00, 
  0x1C, 0x00, 0x00, 
  0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00  
};

const unsigned char PROGMEM bird2[] = {
  0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 
  0x00, 0x3E, 0x00, 
  0x00, 0x7F, 0x80, 
  0x80, 0x67, 0xC0, 
  0xC0, 0x7F, 0xE0, 
  0xE0, 0x7F, 0xE0, 
  0x70, 0x7F, 0x00, 
  0x38, 0x7E, 0x00, 
  0x1C, 0x7C, 0x00, 
  0x06, 0x78, 0x00, 
  0x03, 0x60, 0x00, 
  0x01, 0x40, 0x00, 
  0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00  
};

const int DINO_W = 24; 
const int DINO_H = 24; 
const int CACTUS_W = 8;
const int CACTUS_H = 16;
const int BIRD_W = 24;
const int BIRD_H = 18;

const int GROUND_Y = 54; 

volatile bool jumpPressed = false;
int dinoY = GROUND_Y - DINO_H;
float dinoVelocity = 0;

int obstacleX = 128;
int obstacleType = 0;

int score = 0;
bool gameOver = false;
int frameCount = 0; 

void jumpISR() {
  if (!jumpPressed) {
    jumpPressed = true;
  }
}

void setup() {
  Wire.setSDA(PB7);
  Wire.setSCL(PB6);
  Wire.setClock(400000);
  Wire.begin();

  pinMode(BUTTON_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), jumpISR, FALLING);

  delay(100);
  display.begin(i2c_Address, true);
  display.clearDisplay();
  
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(20, 20);
  display.println("READY TO RUN!");
  
  display.drawBitmap(10, GROUND_Y - DINO_H, dinoRun1, DINO_W, DINO_H, SH110X_WHITE);
  display.display();
  
  while(!jumpPressed) { delay(10); } 
  jumpPressed = false;
  display.clearDisplay();
  
  randomSeed(analogRead(PA1)); 
}

void loop() {
  if (gameOver) {
    display.clearDisplay();
    display.setTextSize(2);
    display.setCursor(10, 20);
    display.print("GAME OVER");
    display.setTextSize(1);
    display.setCursor(40, 45);
    display.print("Score: ");
    display.print(score);
    display.display();
    
    if (jumpPressed) {
      gameOver = false;
      score = 0;
      obstacleX = 128;
      obstacleType = 0;
      dinoY = GROUND_Y - DINO_H;
      dinoVelocity = 0;
      jumpPressed = false;
      delay(500);
    }
    return;
  }

  frameCount++;

  if (jumpPressed && dinoY >= (GROUND_Y - DINO_H - 1)) {
    dinoVelocity = -6.0; 
    jumpPressed = false;
  } 
  else if (jumpPressed) {
    jumpPressed = false; 
  }

  dinoY += dinoVelocity;
  if (dinoY < (GROUND_Y - DINO_H)) {
    dinoVelocity += 0.8; 
  } else {
    dinoY = GROUND_Y - DINO_H;
    dinoVelocity = 0;
  }

  obstacleX -= 5; 
  if (obstacleX < -25) {
    obstacleX = 128;
    score++;
    
    if (random(0, 10) > 5) { 
      obstacleType = 1; 
    } else {
      obstacleType = 0; 
    }
  }

  int hitW = (obstacleType == 0) ? CACTUS_W : BIRD_W;
  int hitH = (obstacleType == 0) ? CACTUS_H : BIRD_H;
  
  int obsY = (obstacleType == 0) ? (GROUND_Y - CACTUS_H) : (GROUND_Y - 8 - BIRD_H);

  if (obstacleX < (10 + DINO_W - 8) && (obstacleX + hitW) > (10 + 8) && 
      dinoY > (obsY - hitH + 6)) { 
     gameOver = true;
  }

  display.clearDisplay();
  
  display.drawLine(0, GROUND_Y, 128, GROUND_Y, SH110X_WHITE);

  if (dinoY < (GROUND_Y - DINO_H - 2)) {
    display.drawBitmap(10, dinoY, dinoRun1, DINO_W, DINO_H, SH110X_WHITE);
  } else {
    if ((frameCount / 4) % 2 == 0) {
      display.drawBitmap(10, dinoY, dinoRun1, DINO_W, DINO_H, SH110X_WHITE);
    } else {
      display.drawBitmap(10, dinoY, dinoRun2, DINO_W, DINO_H, SH110X_WHITE);
    }
  }

  if (obstacleType == 0) {
    display.drawBitmap(obstacleX, GROUND_Y - CACTUS_H, cactusBitmap, CACTUS_W, CACTUS_H, SH110X_WHITE);
  } else {
    int birdY = GROUND_Y - 8 - BIRD_H; 
    if ((frameCount / 8) % 2 == 0) { 
       display.drawBitmap(obstacleX, birdY, bird1, BIRD_W, BIRD_H, SH110X_WHITE);
    } else {
       display.drawBitmap(obstacleX, birdY, bird2, BIRD_W, BIRD_H, SH110X_WHITE);
    }
  }

  display.setCursor(0, 0);
  display.print(score);

  display.display(); 
}
