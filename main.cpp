#include "raylib.h"
#include <deque>                                                            // A good library to control lists better.
#include <iostream>
#include "raymath.h"                                                        // Math library meant to support raylib structs.

using namespace std;

Color green = {173, 204, 96, 255};
Color darkGreen = {43, 51, 24, 255};

int cellSize = 30;
int cellCount = 25;

/*
    I still have to program a piece of code that checks how much time has passed
    since the last snake update, that's why the snake runs like crazy
*/

class Snake{
public:
    deque<Vector2> body = {Vector2{6, 9}, Vector2{7,9}, Vector2{8, 9}};    // Body of the snake, made out of a Vector2 list.
    Vector2 direction = {1, 0};                                            // Defines the direction the snake is going

    void Draw(){
        for(unsigned int i = 0; i < body.size(); i++){                     // For loop that defines the grid system for the snake
            float x = body[i].x;
            float y = body[i].y; 

            Rectangle segment = Rectangle{                                 // Creates a rectangle that fills up an entire cell
                (x * cellSize),
                (y * cellSize),
                static_cast<float>(cellSize),
                static_cast<float>(cellSize)
            };

            DrawRectangleRounded(segment, 0.5f, 6, darkGreen);             // Draw the rounded rectangle
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
        DrawTexture(texture, pos.x * cellSize, pos.y * cellSize, WHITE);
    }

    Vector2 GenerateRandomPos(){
        float x = GetRandomValue(0, cellCount - 1);
        float y = GetRandomValue(0, cellCount - 1);

        return Vector2{x, y};
    }
};

int main(){
    InitWindow(cellSize*cellCount, cellSize*cellCount, "Snake");
    SetTargetFPS(60);

    Food food = Food();
    Snake snake = Snake();

    // GameLoop
    while(WindowShouldClose() == false){
        BeginDrawing();

        snake.Update();

        ClearBackground(green);

        food.Draw();
        snake.Draw();

        EndDrawing();
    }

    CloseWindow();
    return 0;
}