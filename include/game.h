#pragma once

#include <player.h>
#include <world.h>
#include <string>

class Game {
    private:
    world* gWorld;
    player* gPlayer;
    
    char input;
    std::string validInputs;

    bool running;
    int win;
    int turn;

    void render_screen();
    void evaluate_input();
    void game_logic();
    void game_won();
    void game_lost();
    void player_logic();
    void enemy_logic();

    public:
    void startGame();

    //constructor
    Game(): validInputs("wasder"), running(true), win(0), turn(0) {
        gWorld = new world();
        gPlayer = new player();
    };

    //destructor
    ~Game() {
        delete gWorld;
        delete gPlayer;
    }
};