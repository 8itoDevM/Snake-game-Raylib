#include "raylib.h"
#include <deque>                                                            // A good library to control lists better.
#include <iostream>
#include "raymath.h"                                                        // Math library meant to support raylib structs.

using namespace std;

Color green = {173, 204, 96, 255};
Color dark_green = {43, 51, 24, 255};

int cell_size = 30;
int cell_count = 25;

double last_updated_time = 0;

bool EventTriggered(double interval){                                      // This method returns if the interval since the last update has passed
    double current_time = GetTime();                                       // GetTime retrieves how much time has passed since the GameInit                
    if(current_time - last_updated_time >= interval){
        last_updated_time = current_time;
        return true;
    }
    return false;
}

class Snake{
public:
    deque<Vector2> body = {Vector2{6, 9}, Vector2{7,9}, Vector2{8, 9}};    // Body of the snake, made out of a Vector2 list.
    Vector2 direction = {1, 0};                                            // Defines the direction the snake is going

    void Draw(){
        for(unsigned int i = 0; i < body.size(); i++){                     // For loop that defines the grid system for the snake
            float x = body[i].x;
            float y = body[i].y; 

            Rectangle segment = Rectangle{                                 // Creates a rectangle that fills up an entire cell
                (x * cell_size),
                (y * cell_size),
                static_cast<float>(cell_size),
                static_cast<float>(cell_size)
            };

            DrawRectangleRounded(segment, 0.5f, 6, dark_green);             // Draw the rounded rectangle
        }
    }

    void Update(){
        body.pop_back();                                                   // Removes the last cell from the back of the snake
        body.push_front(Vector2Add(body[0], direction));                   // Adds one cell on front of the snake and applies directions
    }
};

class Food{
public:
    Vector2 pos;
    Texture2D texture;

    Food(){
        Image image = LoadImage("Graphics/food.png");
        texture = LoadTextureFromImage(image);                             // Loads an Image set as a Texture and unloads the Image to manage memory
        UnloadImage(image);

        pos = GenerateRandomPos();          
    }

    ~Food(){
        UnloadTexture(texture);                                            // This is a distructor... it's called when the object is destroyed or loses its reference
    }

    void Draw(){
        DrawTexture(texture, pos.x * cell_size, pos.y * cell_size, WHITE);
    }

    Vector2 GenerateRandomPos(){
        float x = GetRandomValue(0, cell_count - 1);
        float y = GetRandomValue(0, cell_count - 1);

        return Vector2{x, y};
    }
};

class Game{
public:
    Snake snake = Snake();
    Food food = Food();

    void Draw(){
        snake.Draw();
        food.Draw();
    }

    void Update(){
        snake.Update();
        CheckCollisionWithFood();
    }

    void CheckCollisionWithFood(){
        if(Vector2Equals(snake.body[0], food.pos)){
            food.pos = food.GenerateRandomPos();
        }
    }

};

int main(){
    InitWindow(cell_size*cell_count, cell_size*cell_count, "Snake");
    SetTargetFPS(60);

    Game game = Game();

    // GameLoop
    while(WindowShouldClose() == false){
        BeginDrawing();

        if(EventTriggered(0.25)){
            game.Update();
        }

        if(IsKeyPressed(KEY_UP) && game.snake.direction.y != 1){
        game.snake.direction = {0, -1};
        }
        if(IsKeyPressed(KEY_DOWN) && game.snake.direction.y != -1){
            game.snake.direction = {0, 1};
        }
        if(IsKeyPressed(KEY_LEFT) && game.snake.direction.x != 1){
            game.snake.direction = {-1, 0};
        }
        if(IsKeyPressed(KEY_RIGHT) && game.snake.direction.x != -1){
            game.snake.direction = {1, 0};
        }

        ClearBackground(green);

        game.Draw();

        EndDrawing();
    }

    CloseWindow();
    return 0;
}