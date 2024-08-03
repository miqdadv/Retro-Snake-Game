# include <bits/stdc++.h>

# include <raylib.h>

# include <deque>

# include <raymath.h>

using namespace std;

Color Green={173,204,96,255};

Color DarkGreen={43,51,24,255};

int cellSize=30;

int cellCount=25;

int offSet=75;

double lastUpdateTime=0;

bool ElementInDeque(Vector2 element,deque<Vector2> deque){

 for(unsigned int i=0;i<deque.size();i++){

   if(Vector2Equals(deque[i],element)){

      return true;

   }

 }

 return false;

}

bool eventTriggered(double interval){

    double currentTime=GetTime();

    if(currentTime-lastUpdateTime>=interval){

           lastUpdateTime=currentTime;

           return true;

    }

    return false;

}

class Snake{

 public:

  deque<Vector2>body={Vector2{6,9},Vector2{5,9},Vector2{4,9}};

  Vector2 direction={1,0};

  bool addSegment=false;

  void Draw(){

    for(long long unsigned int i=0;i<body.size();i++){

       float x=body[i].x;

       float y=body[i].y;

      Rectangle segment=Rectangle{offSet+x*cellSize,offSet+y*cellSize,(float)cellSize,(float)cellSize};

      DrawRectangleRounded(segment,0.5,6,DarkGreen); 

    }

  }

  void Update(){

     body.push_front(Vector2Add(body[0],direction));

     if(addSegment==true){

       addSegment=false;

     }else{

     body.pop_back();

     }

     

  }

  void Reset(){

    body={Vector2{6,9},Vector2{5,9},Vector2{4,9}};

    direction={1,0};

  }

};

class Food{

 public:

  Vector2 position;

  Texture2D texture;

 Food(deque<Vector2> snakeBody){

   Image image=LoadImage("Graphics/food.png");
 
   texture=LoadTextureFromImage(image);

   UnloadImage(image);

   position=GenerateRandomPos(snakeBody);

 }

 ~Food(){

   UnloadTexture(texture);

 }

 void Draw(){

    DrawTexture(texture,offSet+position.x*cellSize,offSet+position.y*cellSize,WHITE);

 }

 Vector2 GenerateRandomCell(){

     float x=GetRandomValue(0,cellCount-1);

     float y=GetRandomValue(0,cellCount-1);      

     return Vector2{x,y};

 }

 Vector2 GenerateRandomPos(deque<Vector2> snakeBody){



     Vector2 position=GenerateRandomCell();

     while(ElementInDeque(position,snakeBody)){

          position=GenerateRandomCell();

     }

     return position;

   

 }

};

class Game{

  public:

  Snake snake=Snake();

  Food food=Food(snake.body);

  bool running=true;

  int score=0;

  Sound eatSound;

  Sound wallSound;

  Game(){

   InitAudioDevice();

   eatSound=LoadSound("Sounds/eat.mp3");

   wallSound=LoadSound("Sounds/wall.mp3");

  }

  ~Game(){

      InitAudioDevice();

      UnloadSound(eatSound);

      UnloadSound(wallSound);

      CloseAudioDevice();

  }

  void Draw(){

      food.Draw();

      snake.Draw();

  }

  void Update(){

    if(running){

       snake.Update();

     checkCollisionWithFood();

     checkCollisonWithEdges();

     checkCollisionWithTail();

    }

    

  }

  void checkCollisionWithFood(){

      if(Vector2Equals(snake.body[0],food.position)){

        food.position=food.GenerateRandomPos(snake.body);

        snake.addSegment=true;

        score++;

        PlaySound(eatSound);

      }

  }

  void checkCollisonWithEdges(){

      if(snake.body[0].x==cellCount || snake.body[0].x==-1){

           GameOver();

      }

      if(snake.body[0].y==25 || snake.body[0].y==-1){

          GameOver();

      }

     

  }

  void GameOver(){

       snake.Reset();

       food.position=food.GenerateRandomPos(snake.body);

       running=false;

       score=0;

       PlaySound(wallSound);

  }

  void checkCollisionWithTail(){

        deque<Vector2> headLessBody=snake.body;

        headLessBody.pop_front();

        if(ElementInDeque(snake.body[0],headLessBody)){

           GameOver();

        }

  }

};

int main(){

 InitWindow(2*offSet+cellSize*cellCount,2*offSet+cellSize*cellCount,"Snake Game");

 SetTargetFPS(60);

 Game game=Game();

 //Game Loop

 while(WindowShouldClose()==false){

     //1. Event handling
     //2. Updating Positions

     if(eventTriggered(0.2)){

         game.Update();

     }

     if(IsKeyPressed(KEY_UP) && game.snake.direction.y!=1){

         game.snake.direction={0,-1};

         game.running=true;

     }

      if(IsKeyPressed(KEY_DOWN)&& game.snake.direction.y!=-1){

         game.snake.direction={0,1};

         game.running=true;


     }

      if(IsKeyPressed(KEY_RIGHT) && game.snake.direction.x!=-1){

         game.snake.direction={1,0};

          game.running=true;


     }

      if(IsKeyPressed(KEY_LEFT) && game.snake.direction.x!=1){

         game.snake.direction={-1,0};

          game.running=true;


     }   

     //3. Drawing

     BeginDrawing();

     ClearBackground(Green);

     DrawRectangleLinesEx(Rectangle{(float)offSet-5,(float)offSet-5,(float)cellSize*cellCount+10,(float)cellSize*cellCount+10},5,DarkGreen);

     DrawText("Retro Snake",offSet-5,20,40,DarkGreen);

     DrawText(TextFormat("%i",game.score),offSet-5,offSet+cellSize*cellCount+10,40,DarkGreen);

     game.Draw();

     EndDrawing();


 }

 CloseWindow();

 return 0;

}