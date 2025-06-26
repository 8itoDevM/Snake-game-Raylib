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

bool ElementInDeque(Vector2 element, deque<Vector2> deque){
    for(unsigned int i = 0; i < deque.size(); i++){
        if(Vector2Equals(deque[i], element)){
            return true;
        }
    }
    return false;
}

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

    bool add_segment = false;

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
        body.push_front(Vector2Add(body[0], direction));                   // Adds one cell on front of the snake and applies directions

        if(add_segment){
            add_segment = false;
        } else{
            body.pop_back();                                                   // Removes the last cell from the back of the snake
        }
    }

    void Reset(){
        body = {Vector2{6, 9}, Vector2{7,9}, Vector2{8, 9}};
        direction = {1, 0};
    }
};

class Food{
public:
    Vector2 pos;
    Texture2D texture;

    Food(deque<Vector2> snake_body){
        Image image = LoadImage("Graphics/food.png");
        texture = LoadTextureFromImage(image);                             // Loads an Image set as a Texture and unloads the Image to manage memory
        UnloadImage(image);

        pos = GenerateRandomPos(snake_body);          
    }

    ~Food(){
        UnloadTexture(texture);                                            // This is a distructor... it's called when the object is destroyed or loses its reference
    }

    void Draw(){
        DrawTexture(texture, pos.x * cell_size, pos.y * cell_size, WHITE);
    }

    Vector2 GenerateRandomCell(){
        float x = GetRandomValue(0, cell_count - 1);
        float y = GetRandomValue(0, cell_count - 1);

        return Vector2{x,y};
    }

    Vector2 GenerateRandomPos(deque<Vector2> snake_body){
        Vector2 cell = GenerateRandomCell();

        while(ElementInDeque(cell, snake_body)){
            cell = GenerateRandomCell();
        };

        return cell;
    }
};

class Game{
public:
    Snake snake = Snake();
    Food food = Food(snake.body);
    bool running = true;

    void Draw(){
        snake.Draw();
        food.Draw();
    }

    void Update(){
        if(running){
            snake.Update();
            CheckCollisionWithFood();
            CheckCollisionWithEdges();
            CheckCollisionWithTail();
        }        
    }

    void CheckCollisionWithFood(){
        if(Vector2Equals(snake.body[0], food.pos)){
            food.pos = food.GenerateRandomPos(snake.body);
            snake.add_segment = true;
        }
    }

    void CheckCollisionWithEdges(){
        if(snake.body[0].x == cell_count || snake.body[0].x == -1){
            GameOver();
        }

        if(snake.body[0].y == cell_count || snake.body[0].y == -1){
            GameOver();
        }
    }

    void GameOver(){
        snake.Reset();
        food.GenerateRandomPos(snake.body);
        running = false;
    }
    
    void CheckCollisionWithTail(){
        deque<Vector2> headless_body = snake.body;
        headless_body.pop_front();

        if(ElementInDeque(snake.body[0], headless_body)){
            GameOver();
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
            game.running = true;
        }
        if(IsKeyPressed(KEY_DOWN) && game.snake.direction.y != -1){
            game.snake.direction = {0, 1};
            game.running = true;
        }
        if(IsKeyPressed(KEY_LEFT) && game.snake.direction.x != 1){
            game.snake.direction = {-1, 0};
            game.running = true;
        }
        if(IsKeyPressed(KEY_RIGHT) && game.snake.direction.x != -1){
            game.snake.direction = {1, 0};
            game.running = true;
        }

        ClearBackground(green);

        game.Draw();

        EndDrawing();
    }

    CloseWindow();
    return 0;
}