#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>

class Game{
    private:
        SDL_Surface * IMG_X=nullptr;
        SDL_Surface * IMG_O=nullptr;
        SDL_Surface * IMG_E=nullptr;
        SDL_Surface * IMG_S=nullptr;
        int board[3][3][2]={};//[x][y][0= {0:empty, 1:o, 2:x},1 = mouse hover]
        const int BACKGROUND_SIZE=48;
        const int X_O_SIZE=32;
        const int X_O_PAD=8;
        const int CELL_SCALE=3;
        const int CELL_SIZE=BACKGROUND_SIZE*CELL_SCALE;
        const int SCREEN_WIDTH=CELL_SIZE*3;
        const int SCREEN_HEIGHT=CELL_SIZE*3;
        SDL_Window * window=nullptr;
        SDL_Surface * surface=nullptr;
        int playerturn=0;
        void render();
        int input();//0 do nothing, 1 quit (game closed), 2 tie, 3 x wins, 4 o wins
        bool check_board(int pos,int direction,int val);
        int check_for_win();//0 nothing, 1 tie, 2 x wins 3 o wins
        void reset();
    public:
        Game();
        ~Game();
        void start();
};

#endif // GAME_H
