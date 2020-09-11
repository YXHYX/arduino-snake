#include <SPI.h>
#include <Adafruit_GFX.h>
#include <TFT_ILI9163C.h>


// Color definitions
#define	BLACK   0x0000
#define	BLUE    0x001F
#define	RED     0xF800
#define	GREEN   0x07E0
#define   CYAN    0x07FF
#define   MAGENTA 0xF81F
#define   YELLOW  0xFFE0  
#define   WHITE   0xFFFF

enum Directions {UP = 0, DOWN, LEFT, RIGHT};

struct vec2i
{
     int x, y;
     vec2i(){};
     vec2i(const vec2i& copy) : x(copy.x), y(copy.y) {}
     vec2i(int x, int y) { this->x = x; this->y = y;}
     bool operator!=(const vec2i& other) const {
          return this->x != other.x || this->y != other.y;
     }
     bool operator==(const vec2i& other) const {
          return this->x == other.x && this->y == other.y;
     }
};

class Snake
{
private:
     TFT_ILI9163C *tft;
     vec2i position;
     vec2i lastPosition;
     vec2i Size;
     bool alive;

     Directions dir;

     int length;
     int score;
     
     int joystickX, joystickY;
     int joystickButton;
     int buzzer;

     vec2i tailPos[11*11];
    
public:
     Snake(TFT_ILI9163C *tft, int x, int y, int joystickX, int joystickY, int joystickButton, int buzzer)
     :    tft(tft), position(vec2i(x, y)) , joystickX(joystickX), joystickY(joystickY), joystickButton(joystickButton), buzzer(buzzer)
     {
          this->tft = tft;
          this->Size = vec2i(10, 10);
          this->length = -1;
          this->alive = true;
          this->score = 0;
     }
     virtual ~Snake()
     {
          delete this->tft;
     }

     //Getters
     vec2i getSize()
     { return this->Size; }
     
     vec2i getPosition()
     { return this->position; }

     bool getAlive()
     { return this->alive; }
     
     int getScore()
     { return this->score; }
     
     //Functions
     void checkCollision(vec2i target)
     {
          //check collision
          if(target.x >= this->position.x &&
             target.x <= this->position.x + Size.x &&
             target.y >= this->position.y &&
             target.y <= this->position.y + Size.y)
          {
               //increase the length and score
               this->length++;
               this->tailPos[length] = this->tailPos[length-1];
               tone(this->buzzer, 600, 300);
               tone(this->buzzer, 300, 300);
               this->score += 10;
          }
     }
     
     void updateInput()
     {
          //get the joystick's x and y coordinates and map them to the screen resolution
          int rVX = map(analogRead(this->joystickX), 0, 1023, -50, 50);
          int rVY = map(analogRead(this->joystickY), 0, 1023, -50, 50);

          //update the previous coordinates before making changes
          if(this->length == -1)
          {
               this->lastPosition = this->position;
          }

          else if(this->length > -1 && this->length < 122)
          {
               this->lastPosition = this->tailPos[length];
               
               for(int i = this->length; i > -1; i--)
               {
                    if(i == 0)
                         this->tailPos[0] = this->position;
                    
                    else
                         this->tailPos[i] = this->tailPos[i-1];
               }
          }
          
          
          //changes the directions
          if(rVX < -20 && dir != RIGHT)
               this->dir = LEFT;
          if(rVX > 20 && dir != LEFT)
               this->dir = RIGHT;
          if(rVY > 20 && dir != DOWN)
               this->dir = UP;
          if(rVY < -20 && dir != UP)
               this->dir = DOWN;

          //increase or decrease the coordinates depending on the direction
          //coordinates should not be greater than the screen resolution
          switch(this->dir)
          {
               case UP:
                    this->position.y += 10;
                    
                    if(this->position.y >= 130)
                         this->position.y = 16;
                    break;
               
               case DOWN:
                    this->position.y += -10;
                    
                    if(this->position.y <= 16)
                         this->position.y = 130;
                    break;
               
               case RIGHT:
                    this->position.x += 10;
                    
                    if(this->position.x >= 130)
                         this->position.x = 0;
                    break;
               
               case LEFT:
                    this->position.x += -10;
                    
                    if(this->position.x <= 0)
                         this->position.x = 130;
                    break;
          }
              
          for(int i = 0; i <= this->length; i++)
          {
               if(this->position == this->tailPos[i])
               {
                    tone(this->buzzer, 250, 400);
                    this->alive = false;
               }
          }
     }
     
     void update(vec2i target)
     {
          this->updateInput();
          this->checkCollision(target);
     }
     
     void render()
     {
          
          if(length == -1)
          {
               //render the snake's head
               this->tft->fillRect(this->lastPosition.x, this->lastPosition.y, 10, 10, BLACK);
               this->tft->fillRect(this->position.x, this->position.y, 10, 10, GREEN);
          }
          else
          {
               //render the snake's head and body               
               for(int i = 0; i <= length; i++)
                    this->tft->fillRect(this->tailPos[i].x, this->tailPos[i].y, 10, 10, YELLOW);
                    
               this->tft->fillRect(this->lastPosition.x, this->lastPosition.y, 10, 10, BLACK);
               
               this->tft->fillRect(this->position.x, this->position.y, 10, 10, GREEN);
          }
     }
     
     void reset()
     {
          //reset the snake's proprities
          this->score = 0;
          for(int i = 0; i <= this->length; i++)
               this->tailPos[i] = vec2i(0, 0);
          this->position = vec2i(40, 40);
          this->length = -1;
          this->alive = true;
     }
};

class Fruit
{
private:
     TFT_ILI9163C *tft;

     bool eaten;
     vec2i position;
     vec2i Size;
     
public:
     Fruit(TFT_ILI9163C *tft, int x, int y)
     :    tft(tft), position(vec2i(x, y))
     {
          this->eaten = false;
          this->Size = vec2i(10, 10);
     }
     virtual ~Fruit()
     {
          delete this->tft;
     }
     
     //Getters
     vec2i getSize()
     { return this->Size;}
     
     vec2i getPosition()
     { return this->position; }
     
     //Functions
     void generateFruit()
     {
          //vanish the eaten fruit
          this->tft->fillRect(this->position.x, this->position.y, 10, 10, BLACK);
          //generate new position for the fruit
          position = vec2i(random(0, 11) * 10, random(2, 11) * 10);
          eaten = false;
     }
     
     void update(vec2i target)
     {
          if(eaten)
               generateFruit();
          
          //check collision
          if(target.x >= this->position.x &&
             target.x <= this->position.x + Size.x &&
             target.y >= this->position.y &&
             target.y <= this->position.y + Size.y)
                    this->eaten = true;
     }
     void render()
     {
          if(!eaten)
               this->tft->fillRect(this->position.x+2, this->position.y+2, 6, 6, RED);
     }
};

long currentMillis = 0;
long previousMillis  = 0;
long interval = 150;

int highScore = 0;
int score = 0;

bool gameOver = false;

//Defines for TFT
#define __CS 53
#define __DC 9

TFT_ILI9163C tft = TFT_ILI9163C(__CS, __DC);
Snake snake(&tft, 120/2, 120/2, A1, A2, 7, 6);
Fruit fruit(&tft, random(0, 11) * 10, random(3, 11) * 10);

void setup() {

     tft.begin();
	tft.fillScreen();
	
	for(int i = 1; i < 4; i++)
	{
		tft.setRotation(i);
		tft.fillScreen();
	}

     pinMode(A1, INPUT);
     pinMode(A2, INPUT);
     pinMode(7, INPUT_PULLUP);
     
     pinMode(6, OUTPUT);
     
     Serial.begin(9600);
     randomSeed(analogRead(0));
}

void loop() {

     while(!gameOver){
          currentMillis = millis();
          if(currentMillis - previousMillis >= interval)
          {
               snake.update(vec2i(fruit.getPosition().x + (fruit.getSize().x/2), fruit.getPosition().y + (fruit.getSize().y / 2)));
               fruit.update(vec2i(snake.getPosition().x + (snake.getSize().x/2), snake.getPosition().y + (fruit.getSize().y / 2)));
               
               snake.render();
               fruit.render();
               if(!snake.getAlive())
                    gameOver = true;
               showScore();
               previousMillis = currentMillis;
          }
     }
     resetGame();
}

void showScore()
{
     if(snake.getScore() == score)
          return;

     score = snake.getScore();
     if(highScore < score)
          highScore = score;

     tft.fillRect(0, 0, 128, 16, BLACK);
     tft.setTextSize(2);
     tft.setCursor(8, 0);
     tft.setTextColor(CYAN);
     tft.print("Score:");
     tft.print(score);
     
}

void resetGame()
{
     tft.clearScreen();
     tft.setTextSize(2);

     tft.setCursor(8, 4);
     tft.setTextColor(GREEN);
     tft.print("Game Over!");
     
     tft.setCursor(8, 32);
     tft.setTextColor(RED);
     tft.print("Your score is:");
     tft.setTextColor(YELLOW);
     tft.print(snake.getScore());
     
     tft.setCursor(8, 80);
     tft.setTextColor(RED);
     tft.print("High score is:");
     tft.setTextColor(YELLOW);
     tft.print(highScore);
     
     while(!digitalRead(7))
     {}
     tft.clearScreen();
     snake.reset();
     gameOver = false;
}
