// Lab9Main.c
// Runs on MSPM0G3507
// Lab 9 solution
// Sarah Gruber
// Last Modified: 11/6/2023

#include <ADC.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <ti/devices/msp/msp.h>
#include "../inc/ST7735.h"
#include "../inc/Clock.h"
#include "../inc/LaunchPad.h"
#include "../inc/TExaS.h"
#include "../inc/Timer.h"
#include "DAC5.h"
#include "FIFO1.h"
#include "UART1.h"
#include "UART2.h"
#include "SmallFont.h"
#include "LED.h"
#include "Switch.h"
#include "JoyStick.h"
#include "Sound.h"
#include "SoundData.h"
#include "images/images.h"

/* defining variables and others */

// bullet struct
typedef struct {
    int x;          // X position
    int y;          // Y position
    int prevX;      // previous X position
    int prevY;      // previous Y position
    int dx;         // change in X per update (speed and direction)
    int dy;         // change in Y per update (speed and direction)
    bool active;    // whether the bullet is active
} Bullet;

// enum
enum Language { ENGLISH, SPANISH };
enum GameState { DIALOGUE, MENU, GAME, GAME_OVER, GAME_GOOD, RESET };
enum Language currentLanguage = ENGLISH;
enum GameState currentGameState = MENU; // default

// function prototypes
void PLL_Init(void);                        // done
uint32_t Random(uint32_t n);                // done
void SysTick_IntArm(uint32_t period, uint32_t priority);               // done
void SysTick_Handler(void);
uint8_t TExaS_LaunchPadLogicPB27PB26(void); // done

void ShowMainMenu();                        // done
void DrawMenu(enum Language lang);          // done

void StartGame();                           // done
void DrawGameScreen();                      // done
void ChangeLanguage();                      // done

unsigned long GetCurrentTime();             // done
void StartGameTimer();                      // done
void IsGameOver();                          // done

bool isButtonPressed();                     // done
void healPlayer();                          // done

void UpdatePlayerPosition();                // done
void UpdateBullets();                       // done
bool CheckForActiveBullets();               // done
void CheckCollisions();                     // done
bool IsCollidingWithPlayer(Bullet bullet);  // done
bool IsOutOfBound(Bullet bullet);           // done

void ShowGameOver();                        // done
void DrawGameOver(enum Language lang);      // done
void ShowGameGood();                        // done
void DrawGameGood(enum Language lang);      // done

void ShowDialogue(enum Language lang);

void PerformAttack(int typeAttack);         // done
void HandleBossAnimations();

void FourDirectionalBurst();                // done
void ExpandingCircle();                     // done
void TargetedBurstAttack();                 // done
void FlappyBirdAttack();                    // wip
void WallAttackSequence();                  // done
void WallAttackSequence2();                  // done

void AddBullet(Bullet bullet);              // done

void UpdateHealthBar();                     // done
void TakeDamage();                          // done

void ResetGame();                           // done

// defined variables
#define SCREEN_WIDTH  128
#define SCREEN_HEIGHT 160
#define ADC_MAX       4095
#define JOYSTICK_DEAD_ZONE 1000
#define MAX_GAME_TIME 5                   // 2 minutes, will probably change later
#define PLAYER_AREA_WIDTH  118              // might change, playerArea dimensions
#define PLAYER_AREA_HEIGHT 75
#define TIMER_FREQUENCY 80000000
#define MAX_BULLETS 100                      // maximum number of bullets on screen, might decrease
#define BULLET_WIDTH 4
#define BULLET_HEIGHT 5
#define maxPlayerHealth 150
#define M_PI 3.14159265358979323846
#define CIRCLE_BULLETS 10 // Number of bullets in the circle
#define CIRCLE_RADIUS 10  // Starting radius of the circle
#define SPEED_FACTOR 0.25
#define LINE_BULLETS 3       // Number of bullets in the line
#define BULLET_SPACING 2     // Spacing between bullets in the line
#define FLAPPY_BIRD_LINE_START 8
#define FLAPPY_BIRD_LINE_END_Y 150
#define FLAPPY_BIRD_GAP_SIZE 25
#define FLAPPY_BIRD_SPEED 3


// global variables
int playerHealth = maxPlayerHealth;
int numAttacks = 0;

int spriteX = 64;
int spriteY = 128;
int spriteWidth = 16;
int spriteHeight = 15;

int prevSpriteX = 64;
int prevSpriteY = 128;

int bossX = 51;
int bossY = 69;
int bossWidth = 25;
int bossHeight = 34;

int healthFlag = 1;
int bossFlag = 1;
int playerAreaFlag = 1;
int occassionalFlag = 0;
int potionFlag = 2;
bool doneFlag = false;

unsigned long startTime;                    // start time of the game

int attackType = 0;
int attackInterval = 30;         // ticks for an attack

int burstCount = 0;
int bulletInBurstCount = 0;
const int bulletsPerBurst = 10;
const int totalBursts = 5;
const int delayBetweenBursts = 1000; // Ticks
int tickCount = 0;
volatile uint32_t SysTickCount = 0;


// arrays
Bullet bullets[MAX_BULLETS];

/* end defining, functions below */

int main(void){
  __disable_irq();
  PLL_Init(); // set bus speed
  LaunchPad_Init();
  UART1_Init(); // just transmit, PA8, blind synchronization
  UART2_Init(); // just receive, PA22, receiver timeout synchronization
  ST7735_InitPrintf();
    //note: if you colors are weird, see different options for
    // ST7735_InitR(INITR_REDTAB); inside ST7735_InitPrintf()
  ST7735_FillScreen(ST7735_BLACK);
  JoyStick_Init();
  Switch_Init(); // initialize switches
  LED_Init();    // initialize LED
  DAC5_Init();
  //Sound_Init();  // initialize sound
  TExaS_Init(0,0,&TExaS_LaunchPadLogicPB27PB26); // PB27 and PB26
  ADC_InitDual(ADC1,6,5,ADCVREF_VDDA); //accelerometer X,Z (analog)
  ADC0_Init(7,ADCVREF_VDDA); //accelerometer Y (analog)
  SysTick_IntArm(2666667, 7);
  TimerA1_IntArm(10417, 256, 7); // Set TimerA1 to trigger at 30Hz
  __enable_irq();

  for(int i = 0; i < 10000; i++){}  // blind cycle to initialize
  while(1){

  }
}

void SysTick_IntArm(uint32_t period, uint32_t priority){
    SysTick->CTRL = 0x00;
    SysTick->LOAD = period-1;
    SCB->SHP[1] = (SCB->SHP[1]&(~0xC0000000))|priority<<30;
    SysTick->VAL = 0;
    SysTick->CTRL = 0x07;
}
// games  engine runs at 30Hz
void SysTick_Handler(void){
    SysTickCount++;
      switch (currentGameState) {
      case DIALOGUE:
          ShowDialogue(currentLanguage);
          break;
      case MENU:
          ShowMainMenu();
          break;
      case GAME:
          UpdatePlayerPosition();
          UpdateBullets();
          CheckCollisions();
          if(isButtonPressed()){
              if(potionFlag != 0){
                  potionFlag--;
                  healPlayer();
              }
          }
          DrawGameScreen();
          IsGameOver();
          break;
      case GAME_OVER:
          ShowGameOver();
          break;
      case GAME_GOOD:
          ShowGameGood();
          break;
      case RESET:
          ResetGame();
          break;
      default:
          // Other game states (like MENU) can be handled here
          break;
      }
}

// bullets handler, runs at 20MHz
void TIMA1_IRQHandler(void) {
    if((TIMA1->CPU_INT.IIDX) == 1) {
        static uint32_t ticksSinceLastAttack = 0;
        switch (currentGameState) {
            case GAME:
                if (ticksSinceLastAttack >= 28) {
                    PerformAttack(attackType);
                    attackType = (attackType + 1) % 7;
                    numAttacks++;
                    ticksSinceLastAttack = 0;
                }else ticksSinceLastAttack++;
                if(numAttacks >= 30){
                    doneFlag = true;
                }
                break;
            default:
                break;
        }
    }
}

void PLL_Init(void){ // set phase lock loop (PLL)
  // Clock_Init40MHz(); // run this line for 40MHz
  Clock_Init80MHz(0);   // run this line for 80MHz
}

uint32_t M=1;
uint32_t Random32(void){
  M = 1664525*M+1013904223;
  return M;
}
uint32_t Random(uint32_t n){
  return (Random32()>>16)%n;
}

uint8_t TExaS_LaunchPadLogicPB27PB26(void){
  return (0x80|((GPIOB->DOUT31_0>>26)&0x03));
}

void ShowMainMenu() {
    bool menuActive = true;
    DrawMenu(currentLanguage);

    while(menuActive) {

        // Here you need to implement a way to read button presses
        // For example, if Button 1 is pressed, start the game
        // If Button 2 is pressed, change the language
        uint32_t buttonState = Switch_In();

        if(buttonState == 1) {
            StartGame();
            menuActive = false;
        } else if(buttonState == 2) {
            ChangeLanguage();
        }
    }
}

void DrawMenu(enum Language lang) {
    ST7735_FillScreen(ST7735_BLACK);

    if(lang == ENGLISH) {
        ST7735_SetCursor(0, 0);
        ST7735_OutString("1. Start Game");
        ST7735_SetCursor(0, 1);
        ST7735_OutString("2. Change to Spanish");
    } else if(lang == SPANISH) {
        ST7735_SetCursor(0, 0);
        ST7735_OutString("1. Iniciar Juego");
        ST7735_SetCursor(0, 1);
        ST7735_OutString("2. Cambiar a Ingles");
    }
}

void StartGame() {
    ST7735_FillScreen(ST7735_BLACK);
    currentGameState = DIALOGUE;
}

void ChangeLanguage() {
    if(currentLanguage == ENGLISH) {
        currentLanguage = SPANISH;
    } else {
        currentLanguage = ENGLISH;
    }
    DrawMenu(currentLanguage);
    uint32_t buttonState = Switch_In();
    while(buttonState != 0) { buttonState = Switch_In(); }
}

void IsGameOver() {
    bool isHealthDepleted = (playerHealth <= 0);
    bool isTimeElapsed = doneFlag;

    if(isHealthDepleted){
        currentGameState = GAME_OVER;
    }else if (isTimeElapsed){
        currentGameState = GAME_GOOD;
    }
}


void UpdatePlayerPosition() {

    uint32_t joystickX, joystickY, joystickZ;
    int screenX, screenY, lastScreenX, lastScreenY;
    const uint16_t *sprite = player;

    prevSpriteX = spriteX;
    prevSpriteY = spriteY;

    ADC_InDual(ADC1, &joystickX, &joystickZ);
    joystickY = ADC0_In();

    int playerAreaX1 = 8;
    int playerAreaY1 = 154;
    int playerAreaX2 = 121;
    int playerAreaY2 = 104;

    // Right Movement Boundary
    if(joystickX > (ADC_MAX / 2 + JOYSTICK_DEAD_ZONE)) {
        if(spriteX < playerAreaX2 - 19) spriteX += 5;  // right
    }

    // Left Movement Boundary
    else if(joystickX < (ADC_MAX / 2 - JOYSTICK_DEAD_ZONE)) {
        if(spriteX > playerAreaX1 + 3) spriteX -= 5;  // left
    }

    // Up Movement Boundary
    if(joystickY > (ADC_MAX / 2 + JOYSTICK_DEAD_ZONE)) {
        if(spriteY < playerAreaY1 - 1) spriteY += 5;  // up
    }

    // Down Movement Boundary
    else if(joystickY < (ADC_MAX / 2 - JOYSTICK_DEAD_ZONE)) {
        if(spriteY > playerAreaY2 - 4) spriteY -= 5;  // down
    }
}

void UpdateBullets() {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (bullets[i].active) {
            // update position of the bullet
            bullets[i].x += bullets[i].dx;
            bullets[i].y += bullets[i].dy;

            // check if the bullet has left the play area
            if (IsOutOfBound(bullets[i])) {
                bullets[i].active = false; // deactivate
                ST7735_DrawBitmap(bullets[i].prevX, bullets[i].prevY, bulletBlack, BULLET_WIDTH, BULLET_HEIGHT);
            }
        }
    }
    CheckCollisions();
}


void CheckCollisions() {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (bullets[i].active && IsCollidingWithPlayer(bullets[i])) {
            TakeDamage();
            bullets[i].active = false;
            ST7735_DrawBitmap(bullets[i].prevX, bullets[i].prevY, bulletBlack, BULLET_WIDTH, BULLET_HEIGHT);
        }
    }
}


bool IsCollidingWithPlayer(Bullet bullet) {
    int playerLeft = spriteX;
    int playerRight = spriteX + spriteWidth;
    int playerTop = spriteY - spriteHeight;
    int playerBottom = spriteY;

    int bulletLeft = bullet.x;
    int bulletRight = bullet.x + BULLET_WIDTH;
    int bulletTop = bullet.y;
    int bulletBottom = bullet.y + BULLET_HEIGHT;

    // Check for overlap
    bool collisionX = (playerLeft < bulletRight && playerRight > bulletLeft);
    bool collisionY = (playerTop < bulletBottom && playerBottom > bulletTop);

    return collisionX && collisionY;  // Both X and Y collisions must be true
}


bool IsOutOfBound(Bullet bullet) {
    return (bullet.x + BULLET_WIDTH > 122 || bullet.x - BULLET_WIDTH < 7 ||
            bullet.y + BULLET_HEIGHT > 155 || bullet.y - BULLET_HEIGHT < 83);
}

void ShowGameOver() {
    bool menuActive = true;
    DrawGameOver(currentLanguage);
    while(menuActive) {
        uint32_t buttonState = Switch_In();
        if(buttonState == 1) {
            currentGameState = RESET;
            menuActive = false;
        }
    }
}

void DrawGameOver(enum Language lang) {
    ST7735_FillScreen(ST7735_BLACK);
    ST7735_DrawBitmap(0, 87, gameOver, 128, 60);
    if(lang == ENGLISH) {
        ST7735_SetCursor(0, 9);
        ST7735_OutString("Better go work on");
        ST7735_SetCursor(0, 10);
        ST7735_OutString("your lab.");
        ST7735_SetCursor(0, 11);
        ST7735_OutString("Play Again?");
        ST7735_SetCursor(0, 12);
        ST7735_OutString("(press button)");
    } else if(lang == SPANISH) {
        ST7735_SetCursor(0, 9);
        ST7735_OutString("Mejor ve a trabajar en ");
        ST7735_SetCursor(0, 10);
        ST7735_OutString("tu laboratorio");
        ST7735_SetCursor(0, 11);
        ST7735_OutString("Juega de nuevo?");
        ST7735_SetCursor(0, 12);
        ST7735_OutString("(presiona el boton)");
    }
}

void ShowGameGood() {
    bool menuActive = true;
    DrawGameGood(currentLanguage);
    while(menuActive) {
        uint32_t buttonState = Switch_In();
        if(buttonState == 1) {
            currentGameState = RESET;
            menuActive = false;
        }
    }
}

void DrawGameGood(enum Language lang) {
    ST7735_FillScreen(ST7735_BLACK);
    ST7735_DrawBitmap(0, 87, gameWon, 128, 60);
    if(lang == ENGLISH) {
        ST7735_SetCursor(0, 9);
        ST7735_OutString("Wow... I guess you");
        ST7735_SetCursor(0, 10);
        ST7735_OutString("get a C!");
        ST7735_SetCursor(0, 11);
        ST7735_OutString("Play Again?");
        ST7735_SetCursor(0, 12);
        ST7735_OutString("(press button)");
    } else if(lang == SPANISH) {
        ST7735_SetCursor(0, 9);
        ST7735_OutString("Vaya... Supongo que");
        ST7735_SetCursor(0, 10);
        ST7735_OutString("obtienes una C!");
        ST7735_SetCursor(0, 11);
        ST7735_OutString("Juega de nuevo?");
        ST7735_SetCursor(0, 12);
        ST7735_OutString("(presiona el boton)");
    }
}

int StringLength(const char *str) {
    int length = 0;
    while (str[length] != '\0') {
        length++;
    }
    return length;
}

unsigned long GetCurrentTime() {
    return (SysTickCount * 1000) / 30; // return in ms
}

bool isDialogueActive = false;
int dialogueIndex = 0;
int dialogueLength = 0;
int heartY = 77;
unsigned long lastHeartToggleTime = 0;
unsigned long lastCharDisplayTime = 0;
float charDisplayInterval = 50; // Milliseconds
int heartToggleInterval = 1000; // Milliseconds

// Assuming you have an array of dialogues or a similar structure
const char *dialogue1[] = {
    "It's a beautiful",
    "day outside,",
    "birds are ",
    "singing,",
    "flowers are",
    "blooming,",
    "on days like",
    "this kids like",
    "you...",
    "should be",
    "doing your lab.",
};

const char *dialogue2[] = {
    "Es un dia",
    "hermoso aya",
    "afuera, las",
    "aves cantan,",
    "las fores",
    "florecen, ",
    "en dias como",
    "estos ninos",
    "como tu...",
    "deberia estar",
    "tu laboratoria.",
};

int currentDialogueIndex = 0; // To keep track of the current dialogue part
const char *currentDialogue;

void ShowDialogue(enum Language lang) {
    if (!isDialogueActive) {
        // Initialize dialogue
        ST7735_DrawBitmap(6, 155, playerArea, PLAYER_AREA_WIDTH, PLAYER_AREA_HEIGHT);
        ST7735_DrawBitmap(spriteX, spriteY, player, 16, 15);
        ST7735_DrawBitmap(bossX, bossY, valvanoSans1, bossWidth, bossHeight);
        ST7735_DrawBitmap(13, 91, text, 102, 21);
        isDialogueActive = true;
        dialogueIndex = 0;
        if(currentLanguage == ENGLISH){
            currentDialogue = dialogue1[currentDialogueIndex];
        }else {
            currentDialogue = dialogue2[currentDialogueIndex];
        }
        dialogueLength = StringLength(currentDialogue);
    }

    // toggle heart
    unsigned long currentTime = GetCurrentTime();
    if (currentTime - lastHeartToggleTime >= heartToggleInterval) {
        heartY = (heartY == 77) ? 78 : 77;
        ST7735_DrawBitmap(16, heartY, heart, 3, 3);
        lastHeartToggleTime = currentTime;
    }

       const int maxCharsPerLine = 16;
       const int maxLines = 1;
       int line = dialogueIndex / maxCharsPerLine;
       int charInLine = dialogueIndex % maxCharsPerLine;

       if (dialogueIndex < dialogueLength && currentTime - lastCharDisplayTime >= charDisplayInterval) {
           if (line < maxLines) {
               if (charInLine == 0) {
                   ST7735_SetCursor(3, 8);   // cursor
                   ST7735_SetTextColor(ST7735_WHITE);  // color
               }

               char str[2] = {currentDialogue[dialogueIndex], '\0'}; // single character
               ST7735_OutString(str);
               lastCharDisplayTime = currentTime;
               dialogueIndex++;

               // play sound every 2 characters
               if (dialogueIndex % 2 == 0) {
                   StartSong(3);
               }
           }
       }
       uint32_t buttonState = Switch_In();
       if (buttonState == 1) {
           if (dialogueIndex >= dialogueLength) {
               // next part of the dialogue
               currentDialogueIndex++;
               if (currentDialogueIndex < sizeof(dialogue1) / sizeof(dialogue1[0])) {
                   // start
                   dialogueIndex = 0;
                   if(currentLanguage == ENGLISH){
                       currentDialogue = dialogue1[currentDialogueIndex];
                   }else {
                       currentDialogue = dialogue2[currentDialogueIndex];
                   }
                   dialogueLength = StringLength(currentDialogue);
                   // reset dialogue box
                   ST7735_DrawBitmap(13, 91, text, 102, 21);
               } else {
                   // end dialogue
                   isDialogueActive = false;
                   StartSong(0);
                   ST7735_FillScreen(ST7735_BLACK);
                   currentGameState = GAME; // Change game mode
               }
           }
       }
   }



void HandleBossAnimations() {
    // Switch between different frames of boss sprite
    // Use a timer or counter to change frames periodically
}

void PerformAttack(int typeAttack){
    switch (typeAttack) {
        case 0:
            FourDirectionalBurst();
            break;
        case 1:
            ExpandingCircle();
            break;
        case 2:
            TargetedBurstAttack();
            break;
        case 3:
            WallAttackSequence();
            break;
        case 4:
            WallAttackSequence2();
            break;
        case 5:
            FlappyBirdAttack();
            break;
        case 6:
            FlappyBirdAttack();
            break;
        case 7:
            FlappyBirdAttack();
            break;
        // other attacks...
    }
}

bool CheckForActiveBullets() {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (bullets[i].active) {
            return true; // found an active bullet
        }
    }
    return false; // no active bullets found
}

float sin_approx(float x) {
    // Normalize x to the range [-PI, PI]
    while (x > M_PI) x -= 2 * M_PI;
    while (x < -M_PI) x += 2 * M_PI;

    float x2 = x * x;
    return x - (x2 * x) / 6 + (x2 * x2 * x) / 120;  // Up to the x^5 term
}


// Simple cosine approximation
float cos_approx(float x) {
    return sin_approx(x + M_PI / 2);
}

uint32_t RandomRange(uint32_t min, uint32_t max) {
    if (max <= min) {
        return min; // To handle incorrect ranges
    }
    return min + Random(max - min + 1);
}


void FourDirectionalBurst() {
    int bulletSpeed = 2;  // Speed of the bullets

    // Random position within the player area
    int randomX = RandomRange(7, 122);
    int randomY = RandomRange(83, 155);

    // Upward bullets
    Bullet upBullet = {randomX, randomY, 0, 0, 0, -bulletSpeed, true};
    AddBullet(upBullet);

    // Downward bullets
    Bullet downBullet = {randomX, randomY, 0, 0, 0, bulletSpeed, true};
    AddBullet(downBullet);

    // Leftward bullets
    Bullet leftBullet = {randomX, randomY, 0, 0, -bulletSpeed, 0, true};
    AddBullet(leftBullet);

    // Rightward bullets
    Bullet rightBullet = {randomX, randomY, 0, 0, bulletSpeed, 0, true};
    AddBullet(rightBullet);
}

void ExpandingCircle() {
    int centerX = 64; // Center of the player area
    int centerY = 119;

    for (int i = 0; i < CIRCLE_BULLETS; i++) {
        float angle = 2 * M_PI * i / CIRCLE_BULLETS;
        int dx = (int)(CIRCLE_RADIUS * 10 * cos_approx(angle) * SPEED_FACTOR) / 10;
        int dy = (int)(CIRCLE_RADIUS * 10 * sin_approx(angle) * SPEED_FACTOR) / 10;


        Bullet newBullet = {centerX + dx, centerY + dy, 0, 0, dx, dy, true};
        AddBullet(newBullet);
    }
}

void AddBullet(Bullet bullet) {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (!bullets[i].active) {
            bullets[i] = bullet;
            break;
        }
    }
}

int abs(int x) {
    return (x < 0) ? -x : x;
}

void GetDirectionTowardsPlayer(int originX, int originY, int *dx, int *dy) {
    int deltaX = spriteX - originX;
    int deltaY = spriteY - originY;
    int maxDelta = abs(deltaX) > abs(deltaY) ? abs(deltaX) : abs(deltaY);

    // Normalize the direction to maintain a consistent speed
    *dx = (maxDelta == 0) ? 0 : (deltaX * 3 / maxDelta);
    *dy = (maxDelta == 0) ? 0 : (deltaY * 3 / maxDelta);
}

void TargetedBurstAttack() {
    int originX = 64; // X-coordinate of the attack's origin
    int originY = 90; // Y-coordinate of the attack's origin
    int dx, dy;
    GetDirectionTowardsPlayer(originX, originY, &dx, &dy);

    for (int i = 0; i < LINE_BULLETS; i++) {
        int offsetX = i * dx * BULLET_SPACING;
        int offsetY = i * dy * BULLET_SPACING;
        Bullet newBullet = {originX + offsetX, originY + offsetY, 0, 0, dx, dy, true};
        AddBullet(newBullet);
    }
}

void FlappyBirdAttack() {
    static int lineX = 12; // left edge of the screen
    static int gapStartY = 0; // Y position of the gap

        gapStartY = RandomRange(90, 130); // random gap

    for (int y = 90; y <= FLAPPY_BIRD_LINE_END_Y; y+=5) {
        if (y < gapStartY || y > gapStartY + FLAPPY_BIRD_GAP_SIZE) {
            Bullet newBullet = {lineX, y, 0, 0, FLAPPY_BIRD_SPEED, 0, true};
            AddBullet(newBullet);
        }
    }
}

void WallAttackSequence() {
    int bulletSpeed = 2;  // Speed of the bullets
    int xmin = 12;
    int ymin = 90;
    int xmax = 115;
    int ymax = 150;

    // Right To left

    for (int y = ymax; y > 130; y-=5){
    Bullet RightToLeft = {xmax, y, 0, 0, -bulletSpeed, 0, true};
    AddBullet(RightToLeft);
    }

    for (int y = ymin; y < 110; y+=5){
        Bullet LeftToRight = {xmin, y, 0, 0, bulletSpeed, 0, true};
        AddBullet(LeftToRight);
      }

}

void WallAttackSequence2() {
    int bulletSpeed = 2;  // Speed of the bullets
    int xmin = 12;
    int ymin = 90;
    int xmax = 115;
    int ymax = 150;

    // Right To left

    for (int y = ymax; y > 130; y-=5){
    Bullet RightToLeft = {xmin, y, 0, 0, bulletSpeed, 0, true};
    AddBullet(RightToLeft);
    }

    for (int y = ymin; y < 110; y+=5){
        Bullet LeftToRight = {xmax, y, 0, 0, -bulletSpeed, 0, true};
        AddBullet(LeftToRight);
      }

}


void UpdateHealthBar() {
    int healthBarWidth = 50;    // Width of the full health bar
    int healthBarHeight = 5;    // Height of the health bar
    int healthBarX = 39;        // X position of the health bar
    int healthBarY = 10;        // Y position of the health bar

    // Calculate the width of the health bar based on current health
    int currentHealthBarWidth = (playerHealth * healthBarWidth) / maxPlayerHealth;

    // Draw the background of the health bar (empty part)
    ST7735_FillRect(healthBarX, healthBarY, healthBarWidth, healthBarHeight, ST7735_WHITE);

    // Draw the current health (filled part)
    ST7735_FillRect(healthBarX, healthBarY, currentHealthBarWidth, healthBarHeight, ST7735_RED);
}

void TakeDamage() {
    healthFlag = 1;
    playerHealth -= 10;
    if (playerHealth < 0) {
        playerHealth = 0;
    }
    UpdateHealthBar();
}

bool isButtonPressed(){
    uint32_t buttonState = Switch_In();
    if(buttonState == 2) {
        return true;
    }
    return false;
}



void healPlayer(){
    healthFlag = 1;
    playerHealth += (maxPlayerHealth/2);
    if (playerHealth > maxPlayerHealth) {
         playerHealth = maxPlayerHealth;
     }
    UpdateHealthBar();
}

void ResetGame() {
    ST7735_FillScreen(ST7735_BLACK);

    playerHealth = maxPlayerHealth;
    spriteX = 64;
    spriteY = 128;

    for (int i = 0; i < MAX_BULLETS; i++) {
        bullets[i].active = false;
    }

    healthFlag = 1;
    bossFlag = 1;
    playerAreaFlag = 1;
    occassionalFlag = 0;
    potionFlag = 2;
    numAttacks = 0;
    doneFlag = false;

    currentGameState = GAME;
}


void DrawGameScreen() {
    // player area
    if(playerAreaFlag == 1){
        playerAreaFlag = 0;
        int playerAreaX = 6;
        int playerAreaY = 155;
        ST7735_DrawBitmap(6, 155, playerArea, PLAYER_AREA_WIDTH, PLAYER_AREA_HEIGHT);
    }

    // player's sprite
    ST7735_DrawBitmap(prevSpriteX, prevSpriteY, playerBlack, 16, 15);   // clear previous
    ST7735_DrawBitmap(spriteX, spriteY, player, 16, 15);

    // draw the boss sprite
    if(bossFlag == 1){
        bossFlag = 0;
        ST7735_DrawBitmap(bossX, bossY, valvanoSans1, bossWidth, bossHeight);
        ST7735_DrawBitmap(52, 56, valvanoHead, 22, 21);
    }

    // draw bullets
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (bullets[i].active) {
            // Clear the previous bullet position
            ST7735_DrawBitmap(bullets[i].prevX, bullets[i].prevY, bulletBlack, BULLET_WIDTH, BULLET_HEIGHT);

            // Draw the bullet at the new position
            ST7735_DrawBitmap(bullets[i].x, bullets[i].y, bulletSprite, BULLET_WIDTH, BULLET_HEIGHT);

            // Update the previous position
            bullets[i].prevX = bullets[i].x;
            bullets[i].prevY = bullets[i].y;
        }
    }

    if(healthFlag == 1){
        healthFlag = 0;
        UpdateHealthBar();
    }
}



