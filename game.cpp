#include <stdexcept>
#include <string>
#include "game.h"

Game::Game(){
    //initialize sdl2
    if(SDL_Init(SDL_INIT_VIDEO)<0){
        std::string error("SDL could not initialize, SDL_Error: ");
        error+=SDL_GetError();
        error+="\n";
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,"Error",error.c_str(),nullptr);
        throw std::runtime_error(error);
    }
    //initialize window
    window=SDL_CreateWindow("TicTacToe",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,SCREEN_WIDTH,SCREEN_HEIGHT,SDL_WINDOW_SHOWN);
    if(window==nullptr){
        std::string error("SDL_Window could not be created, SDL_Error: ");
        error+=SDL_GetError();
        error+="\n";
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,"Error",error.c_str(),nullptr);
        throw std::runtime_error(error);
    }
    //set surface, clear window
    surface=SDL_GetWindowSurface(window);
    //load textures
    IMG_X=SDL_LoadBMP("res/x.bmp");
    if(IMG_X==nullptr){
        std::string error("res/x.bmp could not be found/loaded");
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,"Error",error.c_str(),nullptr);
        throw std::runtime_error(error);
    }
    IMG_O=SDL_LoadBMP("res/o.bmp");
    if(IMG_O==nullptr){
        std::string error("res/o.bmp could not be found/loaded");
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,"Error",error.c_str(),nullptr);
        throw std::runtime_error(error);
    }
    IMG_E=SDL_LoadBMP("res/e.bmp");
    if(IMG_E==nullptr){
        std::string error("res/e.bmp could not be found/loaded");
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,"Error",error.c_str(),nullptr);
        throw std::runtime_error(error);
    }
    IMG_S=SDL_LoadBMP("res/s.bmp");
    if(IMG_S==nullptr){
        std::string error("res/s.bmp could not be found/loaded");
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,"Error",error.c_str(),nullptr);
        throw std::runtime_error(error);
    }
    SDL_UpdateWindowSurface(window);
    //credits
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION,"TicTacToe","Made by RicardoLuis0",window);
}
Game::~Game(){
    //free sdl data and quit sdl
    SDL_FreeSurface(IMG_X);
    SDL_FreeSurface(IMG_O);
    SDL_FreeSurface(IMG_E);
    SDL_FreeSurface(IMG_S);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
void Game::render(){
    int x,y;
    for(x=0;x<3;x++){
        for(y=0;y<3;y++){
            //draw background
            SDL_Rect source_rect_background={0,0,BACKGROUND_SIZE,BACKGROUND_SIZE};
            SDL_Rect dest_rect_background={x*CELL_SIZE,y*CELL_SIZE,CELL_SIZE,CELL_SIZE};
            SDL_BlitScaled(board[x][y][1]?IMG_S:IMG_E,&source_rect_background,surface,&dest_rect_background);
            if(board[x][y][0]>0){
                //draw cells
                SDL_Rect source_rect_xo={0,0,X_O_SIZE,X_O_SIZE};
                SDL_Rect dest_rect_xo={x*CELL_SIZE+X_O_PAD*CELL_SCALE,y*CELL_SIZE+X_O_PAD*CELL_SCALE,X_O_SIZE*CELL_SCALE,X_O_SIZE*CELL_SCALE};
                SDL_BlitScaled(board[x][y][0]==1?IMG_X:IMG_O,&source_rect_xo,surface,&dest_rect_xo);
            }
        }
    }
    SDL_UpdateWindowSurface(window);
}
void Game::start(){
    while(true){
        int result=input();
        render();
        switch(result){
        case 1:
            return;
            break;
        case 2:
            SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION,"Game Ended","Tie!",window);
            reset();
            break;
        case 3:
            SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION,"Game Ended","X wins!",window);
            reset();
            break;
        case 4:
            SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION,"Game Ended","O wins!",window);
            reset();
            break;
        }
    }
}
int Game::input(){
    for(int x=0;x<3;x++){//clear hover flags
        for(int y=0;y<3;y++){
            board[x][y][1]=0;
        }
    }
    int mx,my;
    SDL_GetMouseState(&mx,&my);//get mouse position into mx & my
    if(mx>0&&mx<SCREEN_WIDTH&&my>0&&my<SCREEN_HEIGHT){//check if mouse is on the window
        board[mx/CELL_SIZE][my/CELL_SIZE][1]=1;//if it is, set the hover flag of the cell to true
    }
    SDL_Event e;
    while(SDL_PollEvent(&e)!=0){
        if(e.type==SDL_QUIT){//return quit code
            return 1;
        }else if(e.type==SDL_MOUSEBUTTONDOWN){//place x or o on mouse(depending on turn)
            int px=e.button.x/CELL_SIZE;
            int py=e.button.y/CELL_SIZE;
            if(board[px][py][0]==0){//check if cell is empty
                board[px][py][0]=playerturn+1;//if it is, place x or o on the cell
                playerturn=!playerturn;//switch turn
                int res=check_for_win();//check for wins
                if(res>0) return res+1;//if there is a win, return win code
            }
        }else if(e.type==SDL_KEYDOWN){
            switch(e.key.keysym.sym){
            case SDLK_r://reset board
                reset();
                break;
            }
        }
    }
    return 0;
}
bool Game::check_board(int direction/*0 horiz,1 vert, 2 diagonal '\', 3 diagonal '/', 4 tie */,int pos,int val){
    switch(direction){
    case 0:
        //check horizontal score
        if(board[pos][0][0]==val&&board[pos][1][0]==val&&board[pos][2][0]==val) return true;
        break;
    case 1:
        //check vertical score
        if(board[0][pos][0]==val&&board[1][pos][0]==val&&board[2][pos][0]==val) return true;
        break;
    case 2:
        //check diagonal score '\'
        if(board[0][0][0]==val&&board[1][1][0]==val&&board[2][2][0]==val) return true;
        break;
    case 3:
        //check diagonal score '/'
        if(board[0][2][0]==val&&board[1][1][0]==val&&board[2][0][0]==val) return true;
        break;
    case 4:
        for(int x=0;x<3;x++){
            for(int y=0;y<3;y++){
                if(board[x][y][0]==0) return false;
            }
        }
        return true;
        break;
    }
    return false;
}
int Game::check_for_win(){//1 tie, 2 x, 3 o
    //check horizontal x
    if(check_board(0,0,1)) return 2;
    if(check_board(0,1,1)) return 2;
    if(check_board(0,2,1)) return 2;
    //check vertical x
    if(check_board(1,0,1)) return 2;
    if(check_board(1,1,1)) return 2;
    if(check_board(1,2,1)) return 2;
    //check diagonal x
    if(check_board(2,0,1)) return 2;
    if(check_board(3,0,1)) return 2;
    
    //check horizontal o
    if(check_board(0,0,2)) return 3;
    if(check_board(0,1,2)) return 3;
    if(check_board(0,2,2)) return 3;
    //check vertical o
    if(check_board(1,0,2)) return 3;
    if(check_board(1,1,2)) return 3;
    if(check_board(1,2,2)) return 3;
    //check diagonal o
    if(check_board(2,0,2)) return 3;
    if(check_board(3,0,2)) return 3;
    
    //check tie
    if(check_board(4,0,0)) return 1;
    return 0;
}
void Game::reset(){
    //clear board and reset turn
    for(int x=0;x<3;x++){
        for(int y=0;y<3;y++){
            board[x][y][0]=0;
        }
    }
    playerturn=0;
}
