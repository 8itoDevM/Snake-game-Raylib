#include "raylib.h"
#include <deque>
#include <iostream>

using namespace std;

Color green = {173, 204, 96, 255};
Color darkGreen = {43, 51, 24, 255};

int cellSize = 30;
int cellCount = 25;

class Snake{
public:
    deque<Vector2> body = {Vector2{6, 9}, Vector2{7,9}, Vector2{8, 9}};   

    void Draw(){
        for(unsigned int i = 0; i < body.size(); i++){
            float x = body[i].x;
            float y = body[i].y; 

            Rectangle segment = Rectangle{
                (x * cellSize),
                (y * cellSize),
                static_cast<float>(cellSize),
                static_cast<float>(cellSize)
            };

            DrawRectangleRounded(segment, 0.5f, 6, darkGreen);
        }
    }
};

class Food{
public:
    Vector2 pos;
    Texture2D texture;

    Food(){
        Image image = LoadImage("Graphics/food.png");
        texture = LoadTextureFromImage(image);
        UnloadImage(image);

        pos = GenerateRandomPos();
    }

    ~Food(){
        UnloadTexture(texture);
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
        // 1. Event Handling

        // 2. Updating Positions

        // 3. Drawing
        BeginDrawing();

        ClearBackground(green);

        food.Draw();
        snake.Draw();

        EndDrawing();
    }

    CloseWindow();
    return 0;
}