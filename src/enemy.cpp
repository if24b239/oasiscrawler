#include <enemy.h>
#include <global.h>
#include <random>


enemy::enemy(int pow, pos position) : position(position), home(position), stats(), chasing(true), is_ally(false) {
    std::random_device rd;
    unsigned int seed = rd();

    std::srand(seed);

    int minStat = 1 + pow / 2;
    int maxVariance = (pow + 5) / 2;

    stats.luck.setLevel((std::rand() % maxVariance) + minStat);
    stats.charisma.setLevel((std::rand() % maxVariance) + minStat);
    stats.stamina.setLevel((std::rand() % maxVariance) + minStat);

    stats.luck.setCurrent(stats.luck.getLevel());
    stats.charisma.setCurrent(stats.charisma.getLevel());
    stats.stamina.setCurrent(stats.stamina.getLevel());
}

void enemy::move(pos target) {
    
    // calculate distance between entity and target
    pos distance = position - target;

    // if it arrives at its home it will return to chasing
    if (position == home) chasing = true;

    // only walk if the enemies stamina is > 0 otherwise rest
    if (stats.stamina.getCurrent() <= 0) {
        stats.stamina.addCurrent(4);
        return;
    }

    // do nothing if it is already at the target position
    if (distance == pos(0, 0)) return;

    // create random seeding
    std::random_device rd;
    unsigned int seed = rd();

    std::srand(seed);

    // logic to move towards target
    if (num_abs_(distance.x) == num_abs_(distance.y)) {
        //when same choose randomly which axis is decreased
        if (std::rand() % 2) {
            position.x -= distance.x/num_abs_(distance.x);
        } else {
            position.y -= distance.y/num_abs_(distance.y);
        }
    } else if (num_abs_(distance.x) > num_abs_(distance.y)) {
        position.x -= distance.x/num_abs_(distance.x);
    } else {
        position.y -= distance.y/num_abs_(distance.y);  
    }

    // consume one stamina if enemy moved
    stats.stamina.consume();
}

// when true enemy won and it returns home, otherwise it dies
bool enemy::fight() {
    
    // create random seeding
    std::random_device rd;
    unsigned int seed = rd();

    std::srand(seed);

    if (std::rand() % 2) {
        chasing = false;
        return true;
    }
    return false;
}