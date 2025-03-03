#pragma once

#include <global.h>
#include <vector>
#include <enemy.h>

#define WORLD_SIZE 5

class world {
    private:
    field map[WORLD_SIZE][WORLD_SIZE];
    unsigned int relic_count;
    int level;

    std::vector<enemy> all_enemies;

    public:
    field& accessField(pos position) {
        return map[position.x][position.y];
    }

    int getRelics() { return relic_count; }
    bool enemyOnTile(pos tile_position, bool* is_chasing, bool* is_ally);
    std::vector<enemy>& getAllEnemies() { return all_enemies; }
    void removeEnemy(enemy& target);

    world(int level = 1);
};