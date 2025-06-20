#include "raylib.h"

Color green = {173, 204, 96, 255};
Color darkGreen = {43, 51, 24, 255};

int cellSize = 30;
int cellCount = 25;

class Food{
public:
    Vector2 pos = {5, 6};
    Texture2D texture;

    Food(){
        Image image = LoadImage("Graphics/food.png");
        texture = LoadTextureFromImage(image);
        UnloadImage(image);
    }

    ~Food(){
        UnloadTexture(texture);
    }
    
    void Draw(){
        DrawTexture(texture, pos.x * cellSize, pos.y * cellSize, WHITE);
    }
};

int main(){
    InitWindow(cellSize*cellCount, cellSize*cellCount, "Snake");
    SetTargetFPS(60);

    Food food = Food();

    // GameLoop
    while(WindowShouldClose() == false){
        // 1. Event Handling

        // 2. Updating Positions

        // 3. Drawing
        BeginDrawing();

        ClearBackground(green);

        food.Draw();

        EndDrawing();
    }

    CloseWindow();
    return 0;
}