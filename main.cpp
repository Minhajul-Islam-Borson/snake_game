#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <vector>
#include <iostream>

using namespace std; 

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int GRID_SIZE = 20;
const string FONT_PATH = "ami.otf"; 

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
TTF_Font* font = nullptr;

struct Snake {
    vector<pair<int, int>> body;
    char direction;
};

struct Food {
    int x, y;
};

Snake snake;
Food food;

void initialize() {
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("Snake Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    TTF_Init();
    font = TTF_OpenFont(FONT_PATH.c_str(), 24);
    if (!font) {
        cerr << "Failed to load font: " << TTF_GetError() << endl;
        exit(-1);
    }

    snake.body.push_back({SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2});
    snake.direction = 'R';

    food.x = rand() % (SCREEN_WIDTH / GRID_SIZE) * GRID_SIZE;
    food.y = rand() % (SCREEN_HEIGHT / GRID_SIZE) * GRID_SIZE;
}

void close() {
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void generateFood() {
    food.x = rand() % (SCREEN_WIDTH / GRID_SIZE) * GRID_SIZE;
    food.y = rand() % (SCREEN_HEIGHT / GRID_SIZE) * GRID_SIZE;
}

void update(int& score, bool& game_over) {
    std::pair<int, int> newHead = snake.body.front();

    switch (snake.direction) {
        case 'U':
            newHead.second -= GRID_SIZE;
            break;
        case 'D':
            newHead.second += GRID_SIZE;
            break;
        case 'L':
            newHead.first -= GRID_SIZE;
            break;
        case 'R':
            newHead.first += GRID_SIZE;
            break;
    }

    // Check for collision with display borders
    if (newHead.first < 0 || newHead.first >= SCREEN_WIDTH || newHead.second < 0 || newHead.second >= SCREEN_HEIGHT) {
        game_over = true;
        return;
    }

    // Check for collision with itself
    for (size_t i = 1; i < snake.body.size(); ++i) {
        if (newHead.first == snake.body[i].first && newHead.second == snake.body[i].second) {
            game_over = true;
            return;
        }
    }

    snake.body.insert(snake.body.begin(), newHead);

    // Check for collision with food
    if (newHead.first == food.x && newHead.second == food.y) {
        score++;
        generateFood();
    } else {
        
        snake.body.pop_back();
    }
}

void render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Render Snake
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    for (const auto& segment : snake.body) {
        SDL_Rect rect = {segment.first, segment.second, GRID_SIZE, GRID_SIZE};
        SDL_RenderFillRect(renderer, &rect);
    }

    // Render Food
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_Rect foodRect = {food.x, food.y, GRID_SIZE, GRID_SIZE};
    SDL_RenderFillRect(renderer, &foodRect);

    SDL_RenderPresent(renderer);
}

void displayGameOver(int score) {
    SDL_Color textColor = {255, 0, 0, 255};
    SDL_Surface* surfaceMessage = TTF_RenderText_Solid(font, ("Game Over! Score: " + to_string(score)).c_str(), textColor);
    SDL_Texture* gameOverTexture = SDL_CreateTextureFromSurface(renderer, surfaceMessage);

    SDL_Rect messageRect;
    messageRect.x = SCREEN_WIDTH / 4;
    messageRect.y = SCREEN_HEIGHT / 2;
    messageRect.w = SCREEN_WIDTH / 2;
    messageRect.h = SCREEN_HEIGHT / 4;

    SDL_RenderCopy(renderer, gameOverTexture, NULL, &messageRect);
    SDL_RenderPresent(renderer);

    cout << "Game Over! Score: " << score << endl;

    SDL_DestroyTexture(gameOverTexture);
    SDL_FreeSurface(surfaceMessage);
}

int main(int argc, char* argv[]) {
    initialize();

    SDL_Event e;
    bool quit = false;
    bool game_over = false;
    int score = 0;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            } else if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                    case SDLK_UP:
                        if (snake.direction != 'D') snake.direction = 'U';
                        break;
                    case SDLK_DOWN:
                        if (snake.direction != 'U') snake.direction = 'D';
                        break;
                    case SDLK_LEFT:
                        if (snake.direction != 'R') snake.direction = 'L';
                        break;
                    case SDLK_RIGHT:
                        if (snake.direction != 'L') snake.direction = 'R';
                        break;
                }
            }
        }

        if (!game_over) {
            update(score, game_over);
            render();
            SDL_Delay(100);
        } else {
            displayGameOver(score);
            SDL_Delay(2000); 
            quit = true;    
        }
    }

    close();
    return 0;
}