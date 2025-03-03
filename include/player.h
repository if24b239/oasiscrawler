#pragma once

#include <global.h>
#include <string>

enum direction {
    up,
    down,
    left,
    right
};

class player {
    private:
    pos position;
    int health;
    int relics_found;

    Stats stats;

    std::string event_display;

    void event_fountain();
    void event_danger();
    void event_relic();

    public:
    void event_item();
    void move(direction dir);
    void event(fieldType type);
    void lost_fight();
    void fight_outcome(combatOutcome cO);
    pos getPositon() { return position; }
    int getRelics() { return relics_found; }
    int getHealth() { return health; }
    Stats getStats() { return stats; }
    Stats& refStats() { return stats; }

    std::string getString() { return event_display; }
    void resetString() { event_display = ""; }
    
    player(): position(0, 0), health(5), relics_found(0), stats(6, 6, 6), event_display("") {}
};