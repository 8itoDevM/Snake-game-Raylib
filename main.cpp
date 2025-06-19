#include "raylib.h"

int main(){
    InitWindow(800, 600, "My First game");
    SetTargetFPS(60);

    // GameLoop
    while(WindowShouldClose() == false){
        // 1. Event Handling

        // 2. Updating Positions

        // 3. Drawing
        BeginDrawing();
        EndDrawing();
    }

    CloseWindow();
    return 0;
}