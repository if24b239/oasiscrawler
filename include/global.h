#pragma once

enum fieldType {
    empty = 0,
    relic,
    danger,
    fountain,
    bandit
};

enum attributeTypes {
    a_stamina = 0,
    a_luck,
    a_charisma
};

enum combatOutcome {
    INCOMPATIBLE_ATTRIBUTES = 0,
    FIRST_WON,
    SECOND_WON,
    TIE,
    ERROR
};

int num_abs_(int num);

class field {
    private:
    fieldType object;
    bool player_seen;

    public:
    fieldType& getObject() {
        return object;
    }
    bool was_seen() { return player_seen; }
    void set_seen() { player_seen = true; }
    
    field() : object(empty), player_seen(false) {}
    field(fieldType type) :object(type), player_seen(false) {}  
};

struct pos {
    int x;
    int y;

    pos(int x_start, int y_start) : x(x_start), y(y_start) {}

    bool operator == (const pos& p) const {
        return x == p.x && y == p.y;
    }

    pos operator-(const pos& p) const {
        return pos(x - p.x, y - p.y);
    }

    bool operator <= (const pos& p) {
        return x*x + y*y <= p.x*p.x + p.y*p.y;
    }
};

struct attribute {
    private:
    attributeTypes id;
    int level;
    int current;

    public:
    attribute(attributeTypes id, int level) : id(id), level(level), current(level) {}

    bool operator == (const attribute& a) {
        return id == a.id;
    }

    int getLevel() { return level; }
    int getCurrent() { return current; }
    void setLevel(int lvl) { level = lvl; current = lvl; }
    void setCurrent(int curr) { current = curr; }
    void levelUp() { level++; }
    void addCurrent(int add) { current = current + add > level ? level : current + add; }

    // static function to have two attributes fight return combatOutcome enum
    static combatOutcome combat(attribute& first, attribute& second);

    // return number between current and 0
    int fight();

    void consume() { current--; }
};

struct Stats {

    attribute luck;
    attribute charisma;
    attribute stamina;

    Stats(int lvlLuck = 1, int lvlCharisma = 1, int lvlStamina = 1) : luck(a_luck, lvlLuck), charisma(a_charisma, lvlCharisma), stamina(a_stamina, lvlStamina) {}

};