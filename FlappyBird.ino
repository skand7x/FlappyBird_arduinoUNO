/*
 * Flappy Bird Game for Arduino Uno 
 */

 #include <Adafruit_GFX.h>    
 #include <Adafruit_TFTLCD.h> 
 #include <TouchScreen.h>     
 
 #define LCD_CS A3 
 #define LCD_CD A2 
 #define LCD_WR A1 
 #define LCD_RD A0 
 #define LCD_RESET A4 
 
 #define YP A1  
 #define XM A2  
 #define YM 7   
 #define XP 6   
 
 #define TS_MINX 150
 #define TS_MINY 120
 #define TS_MAXX 920
 #define TS_MAXY 940
 #define MINPRESSURE 10
 #define MAXPRESSURE 1000
 
 #define SCREEN_WIDTH 240
 #define SCREEN_HEIGHT 320
 
 // Color definitions
 #define BLACK   0x0000
 #define BLUE    0x001F
 #define RED     0xF800
 #define GREEN   0x07E0
 #define CYAN    0x07FF
 #define MAGENTA 0xF81F
 #define YELLOW  0xFFE0
 #define WHITE   0xFFFF
 #define SKYBLUE 0x867D 
 #define BROWN   0xA145 
 
 // Game constants
 #define GRAVITY 0.6
 #define JUMP_FORCE -7.0
 #define BIRD_WIDTH 20
 #define BIRD_HEIGHT 15
 #define PIPE_WIDTH 30
 #define PIPE_GAP 80
 #define GROUND_HEIGHT 30
 #define MAX_PIPES 3
 #define PIPE_SPACING 110 
 
 Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);
 TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300); 
 
 // Game
 boolean gameStarted = false;
 boolean gameOver = false;
 int score = 0;
 unsigned long lastPipeTime = 0;
 
 // Bird properties
 float birdX = 60;
 float birdY = SCREEN_HEIGHT / 2;
 float birdVelocity = 0;
 int birdFrame = 0;
 
 // Pipe structure
 struct Pipe {
   int x;
   int topHeight;
   boolean counted;
 };
 
 // Array of pipes
 Pipe pipes[MAX_PIPES];
 
 // Sprite data for bird animation (3 frames)
 static const uint8_t PROGMEM bird_sprite[][BIRD_WIDTH * BIRD_HEIGHT] = {
 
   {
     0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0,
     0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 3, 3, 0, 0, 0, 0,
     0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 3, 3, 0, 0, 0, 0, 0,
     1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 0, 0, 0, 0, 0, 0,
     1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0,
     1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0,
     1, 1, 1, 1, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0,
     0, 1, 1, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
   },
 
   {
     0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0,
     0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 3, 3, 0, 0, 0, 0,
     0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 3, 3, 0, 0, 0, 0, 0,
     1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 0, 0, 0, 0, 0, 0,
     1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0,
     1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
     1, 1, 1, 1, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0,
     0, 1, 1, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
     0, 0, 0, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
   },
 
   {
     0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0,
     0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 3, 3, 0, 0, 0, 0,
     0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 3, 3, 0, 0, 0, 0, 0,
     1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 0, 0, 0, 0, 0, 0,
     1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0,
     1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0,
     1, 1, 1, 1, 2, 2, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
     0, 1, 1, 2, 2, 2, 2, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 2, 2, 2, 2, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
   }
 };
 
 // Bird color
 const uint16_t birdColors[] = {
   BLACK,   
   YELLOW,  
   WHITE,   
   RED      
 };
 
 void setup() {
   Serial.begin(9600);
   tft.reset();
   tft.begin(0x9341); 
   tft.setRotation(0); 
 
   resetGame();
   showStartScreen();
 }
 
 void loop() {
   // Check for touch input
   if (isTouched()) {
     if (!gameStarted) {
       gameStarted = true;
       resetGame();
       lastPipeTime = millis();
     } else if (!gameOver) {
       birdVelocity = JUMP_FORCE;
     } else {
       gameOver = false;
       resetGame();
       lastPipeTime = millis();
     }
     
     delay(100);
   }
   
   if (gameStarted && !gameOver) {
     static unsigned long lastUpdateTime = 0;
     if (millis() - lastUpdateTime > 33) { // ~30fps
       updateGame();
       lastUpdateTime = millis();
     }
   }
 }
 
 boolean isTouched() {
   TSPoint p = ts.getPoint();
   
   pinMode(YP, OUTPUT);
   pinMode(XM, OUTPUT);
   
   return (p.z > MINPRESSURE && p.z < MAXPRESSURE);
 }
 
 void resetGame() {
   score = 0;
   gameOver = false;
   birdY = SCREEN_HEIGHT / 2;
   birdVelocity = 0;
   birdFrame = 0;
   
   for (int i = 0; i < MAX_PIPES; i++) {
     pipes[i].x = SCREEN_WIDTH + (i * PIPE_SPACING);
     pipes[i].topHeight = random(40, SCREEN_HEIGHT - GROUND_HEIGHT - PIPE_GAP - 40);
     pipes[i].counted = false;
   }
   
   if (gameStarted) {
     drawGame();
   }
 }
 
 void showStartScreen() {
   tft.fillScreen(SKYBLUE);
 
   tft.fillRect(0, SCREEN_HEIGHT - GROUND_HEIGHT, SCREEN_WIDTH, GROUND_HEIGHT, BROWN);
   
   tft.setTextColor(WHITE);
   tft.setTextSize(3);
   tft.setCursor(25, 80);
   tft.print("FLAPPY BIRD");
   
   
   tft.setTextSize(1);
   tft.setCursor(40, 130);
   tft.print("Touch screen to start");
   tft.setCursor(55, 150);
   tft.print("and to make bird fly");
   
 
   drawBird(SCREEN_WIDTH/2 - BIRD_WIDTH/2, SCREEN_HEIGHT/2, 0);
 }
 
 void updateGame() {
   birdVelocity += GRAVITY;
   birdY += birdVelocity;
   
   
   static int frameCount = 0;
   if (++frameCount > 5) {
     birdFrame = (birdFrame + 1) % 3;
     frameCount = 0;
   }
   
   
   if (birdY >= SCREEN_HEIGHT - GROUND_HEIGHT - BIRD_HEIGHT || birdY <= 0) {
     gameOver = true;
   }
   
   if (millis() - lastPipeTime > 2000) {
     for (int i = 0; i < MAX_PIPES; i++) {
       if (pipes[i].x < -PIPE_WIDTH) {
         pipes[i].x = SCREEN_WIDTH;
         pipes[i].topHeight = random(40, SCREEN_HEIGHT - GROUND_HEIGHT - PIPE_GAP - 40);
         pipes[i].counted = false;
       }
     }
     lastPipeTime = millis();
   }
   
   
   for (int i = 0; i < MAX_PIPES; i++) {
     pipes[i].x -= 2;
     
     
     if (birdX + BIRD_WIDTH > pipes[i].x && birdX < pipes[i].x + PIPE_WIDTH) {
       if (birdY < pipes[i].topHeight || birdY + BIRD_HEIGHT > pipes[i].topHeight + PIPE_GAP) {
         gameOver = true;
       }
     }
     
     
     if (!pipes[i].counted && birdX > pipes[i].x + PIPE_WIDTH) {
       score++;
       pipes[i].counted = true;
     }
   }
   
   
   drawGame();
   
   
   if (gameOver) {
     displayGameOver();
   }
 }
 
 void drawGame() {
   
   tft.fillScreen(SKYBLUE);
   
   
   for (int i = 0; i < MAX_PIPES; i++) {
     drawPipe(pipes[i].x, pipes[i].topHeight);
   }
   
   
   tft.fillRect(0, SCREEN_HEIGHT - GROUND_HEIGHT, SCREEN_WIDTH, GROUND_HEIGHT, BROWN);
   
   
   drawBird(birdX, birdY, birdFrame);
   
   
   tft.setTextColor(WHITE);
   tft.setTextSize(2);
   tft.setCursor(10, 10);
   tft.print("Score: ");
   tft.print(score);
 }
 
 void drawBird(int x, int y, int frame) {
   for (int py = 0; py < BIRD_HEIGHT; py++) {
     for (int px = 0; px < BIRD_WIDTH; px++) {
       uint8_t pixel = pgm_read_byte(&bird_sprite[frame][py * BIRD_WIDTH + px]);
       if (pixel > 0) { 
         tft.drawPixel(x + px, y + py, birdColors[pixel]);
       }
     }
   }
 }
 
 void drawPipe(int x, int topHeight) {
   
   tft.fillRect(x, 0, PIPE_WIDTH, topHeight, GREEN);
   tft.drawRect(x, 0, PIPE_WIDTH, topHeight, BLACK);
   
   
   tft.fillRect(x - 3, topHeight - 10, PIPE_WIDTH + 6, 10, GREEN);
   tft.drawRect(x - 3, topHeight - 10, PIPE_WIDTH + 6, 10, BLACK);
   
   
   int bottomY = topHeight + PIPE_GAP;
   int bottomHeight = SCREEN_HEIGHT - GROUND_HEIGHT - bottomY;
   tft.fillRect(x, bottomY, PIPE_WIDTH, bottomHeight, GREEN);
   tft.drawRect(x, bottomY, PIPE_WIDTH, bottomHeight, BLACK);
   
   
   tft.fillRect(x - 3, bottomY, PIPE_WIDTH + 6, 10, GREEN);
   tft.drawRect(x - 3, bottomY, PIPE_WIDTH + 6, 10, BLACK);
 }
 
 void displayGameOver() {
   
   tft.fillRect(30, 100, 180, 80, RED);
   tft.drawRect(30, 100, 180, 80, WHITE);
   
   tft.setTextColor(WHITE);
   tft.setTextSize(2);
   tft.setCursor(60, 115);
   tft.print("GAME OVER");
   
   tft.setTextSize(1);
   tft.setCursor(75, 140);
   tft.print("Your score: ");
   tft.print(score);
   
   tft.setCursor(50, 160);
   tft.print("Touch to play again");
 }