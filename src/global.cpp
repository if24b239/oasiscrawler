#include <global.h>
#include <random>

#include <iostream>

int num_abs_(int num) {
    if (num < 0) {
        return -num;
    }
    return num;
}

int attribute::fight() {
    std::random_device rd;
    unsigned int seed = rd();

    std::srand(seed);

    return std::rand() % current + 1;
}

combatOutcome attribute::combat(attribute& first, attribute& second) {
    
    if (!(first == second)) return INCOMPATIBLE_ATTRIBUTES;
    
    int first_pow = first.fight();
    int second_pow = second.fight();

    if ( first_pow == second_pow) {
        return TIE;
    } else if (first_pow > second_pow) {
        first.consume();
        return FIRST_WON;
    } else {
        second.consume();
        return SECOND_WON;
    }
    
    return ERROR;
}