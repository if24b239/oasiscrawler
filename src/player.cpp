#include <player.h>
#include <random>

#define FOUNTAIN_F "You found an oasis but it was protected by wild animals\n"
#define FOUNTAIN_S "You found an oasis and filled your bottles with water\n"
#define DANGER_F "You stepped on a scorpion and it stung you\n"
#define DANGER_S "You stepped on a scorpion but you managed to kick it away before it could sting you\n"
#define RELIC_EVENT "Diggining through the hot sand you found a relic\n"
#define RELIC_FIGHT "The defeated bandits dropped a relic\n"
#define FIGHT_LOST " and got wounded\n"
#define FIGHT_WON " but managed to defeat them\n"
#define FIGHT_TIE " and barely managed to fight them off\n"
#define FIGHT_FLAVOR "You were surprise attacked by fremen buried in the sand"
#define TIRED "You drop to the ground taking a sip from your bottle\n"
#define ITEM_CHARISMA "You found a fancy dress\n"
#define ITEM_LUCK "You found a lucky charm\n"
#define ITEM_STAMINA "You found running shoes\n"

void player::move(direction dir) {
    
    if (stats.stamina.getCurrent() <= 0) {
        event_display += TIRED;
        stats.stamina.addCurrent(3);
        return;
    }

    switch (dir)
    {
    case up:
        position.x--;
        break;
    case down:
        position.x++;
        break;
    case left:
        position.y--;
        break;
    case right:
        position.y++;
        break;
    }

    stats.stamina.consume();
}

void player::event(fieldType type) {
    switch (type)
    {
    case empty:
        break;
    case relic:
        event_relic();
        break;
    case danger:
        event_danger();
        break;
    case fountain:
        event_fountain();
        break;
    case bandit:
        break;
    }
}

/*
what happens when player steps on fountain
*/
void player::event_fountain() {

    if (std::rand() % 2 && health < 5) {
        health++;

        event_display += FOUNTAIN_S;
    } else {
        event_display += FOUNTAIN_F;
    }

    // 50/50 to give an item that resets one stats current to level
    if (std::rand() % 2) event_item();

    stats.stamina.addCurrent(3);
}

/*
what happens when player steps on danger
*/
void player::event_danger() {

    attribute dangerC(a_charisma, (std::rand() % 3) + 4);
    attribute dangerL(a_luck, (std::rand() % 3) + 4);
    attribute dangerS(a_stamina, (std::rand() % 3) + 2);

    combatOutcome result;

    switch (std::rand() % 3)
    {
    case 0:
        result = attribute::combat(stats.charisma, dangerC);
        break;
    case 1:
        result = attribute::combat(stats.luck, dangerL);
        break;
    case 2:
        result = attribute::combat(stats.stamina, dangerS);
        break;
    }

    if (result == SECOND_WON) {
        health--;

        event_display += DANGER_F;
    }

    if (result == FIRST_WON || result == TIE) {
        event_display += DANGER_S;
    }
}

/*
what happens when player steps on relic
*/
void player::event_relic() {
    relics_found++;

    event_display += RELIC_EVENT;
}

void player::event_item() {
    
    switch (std::rand() % 3)
    {
    case 0: // charisma
        stats.charisma.levelUp();
        stats.charisma.setCurrent(stats.charisma.getLevel());
        event_display += ITEM_CHARISMA;
        break;
    case 1: // luck
        stats.luck.levelUp();
        stats.stamina.setCurrent(stats.stamina.getLevel());
        event_display += ITEM_LUCK;
        break;
    case 2: // stamina
        stats.stamina.levelUp();
        stats.stamina.setCurrent(stats.stamina.getLevel());
        event_display += ITEM_STAMINA;
        break;
    default:
        break;
    }
}

/*
what happens when player looses a fight
*/
void player::lost_fight() {
    health--;
}

void player::fight_outcome(combatOutcome cO) {
    event_display += FIGHT_FLAVOR;

    switch (cO)
    {
    case FIRST_WON: // player won
        event_display += FIGHT_WON;
        break;
    case SECOND_WON: // bandit won
        event_display += FIGHT_LOST;
        break;
    case TIE: // tie
        event_display += FIGHT_TIE;
        break;
    case INCOMPATIBLE_ATTRIBUTES:
        break;
    case ERROR:
        break;
    }
}