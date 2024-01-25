//snake game
#include<bits/stdc++.h>
#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include<SDL2/SDL_mixer.h>
#include<vector>
#include<SDL2/SDL_ttf.h>
using namespace std;
SDL_Window* window=nullptr;
SDL_Renderer* render=nullptr;
SDL_Surface* imageSurface=nullptr;
SDL_Texture* imageTexture=nullptr;
Mix_Chunk* foodsound=nullptr;
TTF_Font* font=nullptr;

#define screen_w 800
#define screen_h 600
#define z 20

SDL_Event e;
bool game=true;
bool gameover=false;
int score=0;
int genaratedbonus=0;
int bonusfood_time=0;
int present_time=0;

SDL_Rect leftwall={40,40,20,200};
SDL_Rect mleftwall={40,40,200,20};
SDL_Rect rightwall={screen_w-60,screen_h-240,20,200};
SDL_Rect mrightwall={screen_w-240,screen_h-60,200,20};

struct SNAKE
{
    vector<pair<int,int>>body;
    char direction;
};
struct FOOD
{
    int a,b;
};

SNAKE snake;
FOOD food;
FOOD bonusfood;
void shaper_khabar()
{
    food.a=rand()%(screen_w/z)*z;
    food.b=rand()%(screen_h/z)*z;
}
void bonuskhabar()
{
    bonusfood.a=rand()%(screen_w/z)*z;
    bonusfood.b=rand()%(screen_h/z)*z;
}
void createwall();

void initialize()
{
    snake.body.push_back({0,0});
    snake.direction='R';
    shaper_khabar();
    bonusfood.a=10000;
    bonusfood.b=10000;
}

void eventfunction()
{
    while(SDL_PollEvent(&e)!=0)
        {
            if(e.type==SDL_QUIT)
                game=false;

            else if (e.type == SDL_KEYDOWN)
            {
                switch (e.key.keysym.sym)
                {
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
}

void wallCollision()
{
    if ((snake.body.front().first >= leftwall.x && snake.body.front().first < leftwall.x+leftwall.w &&
    snake.body.front().second >= leftwall.y && snake.body.front().second < leftwall.y+leftwall.h) || 
    (snake.body.front().first >= mleftwall.x && snake.body.front().first < mleftwall.x+mleftwall.w &&
    snake.body.front().second >= mleftwall.y && snake.body.front().second < mleftwall.y+mleftwall.h) || 
    (snake.body.front().first >= rightwall.x && snake.body.front().first < rightwall.x+rightwall.w &&
    snake.body.front().second >= rightwall.y && snake.body.front().second < rightwall.y+rightwall.h) || 
    (snake.body.front().first >= mrightwall.x && snake.body.front().first < mrightwall.x+mrightwall.w &&
    snake.body.front().second >= mrightwall.y && snake.body.front().second < mrightwall.y+mrightwall.h))
    {
        cout<<"Game over ! :"<<score<<endl;
        game = false;
    }

    if ((food.a >= leftwall.x && food.a < leftwall.x+leftwall.w &&
    food.b >= leftwall.y && food.b < leftwall.y+leftwall.h) || 
    (food.a >= mleftwall.x && food.a < mleftwall.x+mleftwall.w &&
    food.b >= mleftwall.y && food.b < mleftwall.y+mleftwall.h) || 
    (food.a >= rightwall.x && food.a < rightwall.x+rightwall.w &&
    food.b >= rightwall.y && food.b < rightwall.y+rightwall.h) || 
    (food.a >= mrightwall.x && food.a < mrightwall.x+mrightwall.w &&
    food.b >= mrightwall.y && food.b < mrightwall.y+mrightwall.h))
        shaper_khabar();
}

void putkimara()
{
    pair<int,int>newblock;
    newblock=snake.body.front();
    switch (snake.direction)
    {
    case 'U':
        if(newblock.second>=0)
            newblock.second-=z;
        else
            newblock.second=screen_h-z;
        break;
    case 'D':
        if(newblock.second<screen_h)
            newblock.second+=z;
        else
            newblock.second=0;
        break;
    case 'L':
         if(newblock.first>=0)
            newblock.first-=z;
        else
            newblock.first=screen_w-z;
        //newblock.first-=z;
        break;
    case 'R':
       if(newblock.first<screen_w)
            newblock.first+=z;
        else
            newblock.first=0;
        break;
    }
    snake.body.insert(snake.body.begin(),newblock);
    //for body collision
    for (int i = 1; i < snake.body.size(); ++i) {
        if (snake.body.front().first == snake.body[i].first && snake.body.front().second == snake.body[i].second) {
            cout<<"Game Over!"<<score<<endl;
            game=false;
        }
    }
    wallCollision();
    /*
    //for border collision
    if(newblock.first<0 || newblock.first>=x || newblock.second<0 || newblock.second>=y)
    {
        cout<<"Game Over ! "<<score;
        game=false;
        //SDL_DestroyWindow(window);
    }*/


    present_time=SDL_GetTicks();
    if(present_time-bonusfood_time>=5000)
    {
        bonusfood.a=10000;
        bonusfood.b=10000;
    }
    //cout << newh.first << " " << newh.second << endl;
    //cout << snake.body.front().first << " " << snake.body.front().second << endl << endl;
    if(score!=genaratedbonus && score%5==0)
    {
        bonuskhabar();
        genaratedbonus=score;
        bonusfood_time=SDL_GetTicks();
    }
        
    if (newblock.first==food.a && newblock.second == food.b)
    {
        Mix_PlayChannel(-1,foodsound,0);
        score++;
        shaper_khabar();
    }
    else if (newblock.first==bonusfood.a && newblock.second ==bonusfood.b)
    {
        score+=3;
        bonusfood.a=10000;
        bonusfood.b=10000;
    }
    else
    {
        snake.body.pop_back();
    }
    
}

void khela_shes()
{
    string s="game over...your score : "+to_string(score);
    SDL_Surface* oversurface=TTF_RenderText_Solid(font,s.c_str(), {139,69,19});
    SDL_Texture* overtex=SDL_CreateTextureFromSurface(render,oversurface);
    SDL_Rect borson={screen_h/2-oversurface->w/2,screen_w/2-oversurface->h/2,oversurface->w,oversurface->h};
    SDL_RenderCopy(render,overtex,NULL,&borson);
    SDL_RenderPresent(render);
}

void score_dekha_mc()
{
    string s="SCORE : "+to_string(score);
    SDL_Surface* scoresurface=TTF_RenderText_Solid(font,s.c_str(), {139,69,19});
    SDL_Texture* scoretex=SDL_CreateTextureFromSurface(render,scoresurface);
    SDL_Rect borson={0,0,scoresurface->w,scoresurface->h};
    SDL_RenderCopy(render,scoretex,NULL,&borson);
}

void allRender()
{
    SDL_SetRenderDrawColor(render,0,20,147,255);
    SDL_RenderClear(render);
    SDL_RenderCopy(render, imageTexture, NULL, NULL);

    score_dekha_mc();
    //bonus food
    SDL_SetRenderDrawColor(render,0,128,0,255);
    SDL_Rect balchal={bonusfood.a,bonusfood.b,z,z};
    SDL_RenderFillRect(render,&balchal);

    //wall
    SDL_SetRenderDrawColor(render,255,20,147,255);

    SDL_RenderFillRect(render,&leftwall);
    SDL_RenderFillRect(render,&mleftwall);
    SDL_RenderFillRect(render,&rightwall);
    SDL_RenderFillRect(render,&mrightwall);

    //snake
    SDL_SetRenderDrawColor(render,0,0,0,255);
    for(int i=0;i<snake.body.size();i++)

    {
        SDL_Rect snakebody={snake.body[i].first,snake.body[i].second,z,z};
        SDL_RenderFillRect(render,&snakebody);
    }
    //food
    SDL_SetRenderDrawColor(render,255,0,0,255);
    SDL_Rect snakefood={food.a,food.b,z,z};
    SDL_RenderFillRect(render,&snakefood);
    //SDL_RenderPresent(render);
    
    
    SDL_RenderPresent(render);
}

int main(int argc, char* argv[])
{
    SDL_Init(SDL_INIT_EVERYTHING);
    IMG_Init(IMG_INIT_PNG);
    Mix_Init(MIX_INIT_MP3);
    TTF_Init();
    window=SDL_CreateWindow("SNAKE GAME",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,screen_w,screen_h,SDL_WINDOW_SHOWN);
    render=SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);
    //SDL_SetRenderDrawColor(render,0,255,0,255);
    //ttf
    font=TTF_OpenFont("ttf_font.otf",60);

    //image show
    imageSurface=IMG_Load("dogesh.png");

    imageTexture = SDL_CreateTextureFromSurface(render, imageSurface);

    //audio
    Mix_OpenAudio(44100,MIX_DEFAULT_FORMAT,2,1024);
    Mix_Music* music=Mix_LoadMUS("snakesound.mp3");
    foodsound=Mix_LoadWAV("foodsound.mp3");
    Mix_PlayMusic(music, -1);
    
    initialize();

    while(game)
    {
        eventfunction();
        //if(!gameover)
        //{
            putkimara();
            allRender();
            SDL_Delay(100);
        //}
        
    }
    SDL_SetRenderDrawColor(render,0,20,147,255);
    SDL_RenderClear(render);
    khela_shes();
    SDL_Delay(2000);
    SDL_FreeSurface(imageSurface);
    SDL_DestroyRenderer(render);
    SDL_DestroyWindow(window);
    SDL_DestroyTexture(imageTexture);
    IMG_Quit();
    SDL_Quit();

    return 0;
}