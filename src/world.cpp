#include <world.h>
#include <random>
#include <algorithm>
/*
World constructor
*/
world::world(int level) : relic_count(0), level(level) {

    int max_enemies =  2 + level / 2;

    int temp_x = 0;
    int temp_y = 0;

    // generate all fields on the map
    for (auto& row : map) {

        // reset y coord
        temp_y = 0;

        for (field& cur_field : row) {

            int rand_num = std::rand() % 5;
            fieldType gen_field_type;

            if (rand_num == 0) {
                gen_field_type = danger;
            } else if (rand_num == 1 && std::rand() % level == 0) {
                gen_field_type = fountain;
            } else if (rand_num == 2 && std::rand() % (level/5 + 1) == 0) {
                gen_field_type = relic;
                relic_count++;
            } else if (rand_num == 3 && all_enemies.size() < (size_t) max_enemies && std::rand() % 2) {
                gen_field_type = bandit;
                enemy new_enemy(level, pos(temp_x, temp_y));
                all_enemies.push_back(new_enemy);
            } else {
                gen_field_type = empty;
            }

            field generated_field = field(gen_field_type);
            cur_field = std::move(generated_field);

            //next y coord
            temp_y++;
        }

        //next x coord
        temp_x++;
    }

    // set starting field to empty if its a relic
    fieldType& start_field = map[0][0].getObject();

    if (start_field == relic) {
        relic_count--;
        start_field = empty;
    };
    

    // set only relic location if none have been generated
    if (relic_count == 0) {
        int x = std::rand() % WORLD_SIZE;
        int y = std::rand() % WORLD_SIZE;

        fieldType& relic_loc = map[x][y].getObject();
        relic_loc = relic;
        relic_count++;
    }
}

bool world::enemyOnTile(pos tile_position, bool* is_chasing, bool* is_ally) {

    for (auto bandit : all_enemies) {

        if (bandit.get_pos() == tile_position) {
            
            *is_chasing = bandit.get_chasing();
            *is_ally = bandit.get_is_ally();
            return true;
        }
    }

    return false;
}

void world::removeEnemy(enemy& target) {
    all_enemies.erase(std::remove(all_enemies.begin(), all_enemies.end(), target), all_enemies.end());
}