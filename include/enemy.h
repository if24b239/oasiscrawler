#pragma once

#include <global.h>

class enemy {
    private:
    pos position;
    pos home;

    Stats stats;

    bool chasing;
    bool is_ally;

    public:
    void move(pos target);
    bool fight();
    pos get_pos() { return position; }
    bool get_chasing() { return chasing; }
    bool get_is_ally() { return is_ally; }
    pos get_home() { return home; }
    Stats get_stats() { return stats; }
    Stats& ref_stats() { return stats; }
    void convert() { is_ally = true; }
    void stop_chasing() { chasing = false; }
    
    enemy(int pow, pos position);

    bool operator == (const enemy& e) {
        return home == e.home;
    }
    
};