#include <game.h>
#include <iostream>
#include <random>

#define BANDIT_SLEEP 2

void Game::startGame() {

    std::cout << "Welcome to Oasiscrawler!\nPress Enter to begin:";
    
    std::cin.get();

    while (running) {

        render_screen();
        evaluate_input(); // game pauses here
        game_logic();

        turn++;
    }
}

void Game::render_screen() {
    
    system("clear");

    std::string display = "  OASISCRAWLER    \n     ";
    display.append(validInputs);
    display.append("\n     WINS:");
    display.append(std::to_string(win));
    display.append("\n _______________ \n");
    
    for (int row = 0; row < WORLD_SIZE; row++) {
        
        display.append("|");

        for (int column = 0; column < WORLD_SIZE; column++) {

            field cur_field = gWorld->accessField(pos(row,column));
            bool is_chasing;
            bool is_ally;
            bool draw_enemy = gWorld->enemyOnTile(pos(row, column), &is_chasing, &is_ally);
            pos player_pos = gPlayer->getPositon();

            // draw Fight
            if (player_pos == pos(row, column) && draw_enemy) {
                
                if (is_ally) {
                    display.append(" Ò ");
                } else {
                    display.append(" X ");
                }
                continue;
            }

            // draw player
            if (gPlayer->getPositon() == pos(row, column)) {
                display.append(" O ");
                continue;
            }

            // draw Enemies only after they wake up
            if (draw_enemy && turn >= BANDIT_SLEEP) {
                
                if (is_ally) {
                    if (is_chasing) {
                        display.append(" A ");
                    } else {
                        display.append(" À ");
                    }
                } else {
                    if (is_chasing) {
                        display.append(" E ");
                    } else {
                        display.append(" È ");
                    }
                }
                continue;
            }

            // debug display
            /*
            if (cur_field.getObject() == relic) {
                display.append(" R ");
                continue;
            }
            
            if (cur_field.getObject() == fountain) {
                display.append(" F ");
                continue;
            }
            
            if (cur_field.getObject() == danger) {
                display.append(" D ");
                continue;
            }*/
            

            // draw bandit home tile
            if (cur_field.getObject() == bandit) {
                display.append(" B ");
                continue;
            }

            // draw seen tile
            if (cur_field.was_seen()) {
                
                if (cur_field.getObject() == relic) {
                    display.append(" R ");
                    continue;
                }

                display.append(" + ");
                continue;
            }

            // draw unexplored tile
            display.append(" # ");
        }
        // go to newline
        display.append("|\n");
    }

    display.append(" _______________ \n\n");

    //draw display
    std::cout << display 
        << "Health: " << gPlayer->getHealth() << std::endl
        << "--------------" << std::endl
        << "Stamina: " << gPlayer->getStats().stamina.getCurrent() << std::endl
        << "Charisma: " << gPlayer->getStats().charisma.getCurrent() << std::endl
        << "Luck: " << gPlayer->getStats().luck.getCurrent() << std::endl
        << "--------------" << std::endl
        << "Relics found: " << gPlayer->getRelics() << std::endl 
        << "Relics left: " << gWorld->getRelics() << std::endl
        << "--------------" << std::endl
        << "GAME LOG:" << std::endl;
    
    std::string event_out = gPlayer->getString();
    gPlayer->resetString();

    std::cout << event_out << "\n";

    // bandit position debugging
    /*for (auto bandit : gWorld->getAllEnemies()) {
        std::cout << bandit.get_pos().x << "|" << bandit.get_pos().y << "||" << bandit.get_is_ally() << "\t";
    }*/

    // bandit stats debugging
    /*for (auto bandit : gWorld->getAllEnemies()) {
        std::cout << bandit.get_stats().charisma.getCurrent() << "|" << bandit.get_stats().luck.getLevel() << "|" << bandit.get_stats().stamina.getLevel() << "||" << bandit.get_is_ally() << "\t";
    }*/
}

void Game::evaluate_input() {

    std::cin >> input;

    for (auto& c : validInputs) {

        // test if the input is in the validInputs string, if it is end the function
        if (c == input) return;
    }

    std::cout << "\nInvalid Input Try Again: ";
    evaluate_input();
}

void Game::game_logic() {
    
    player_logic();

    enemy_logic();

    /*
    * WINCONDITIONS
    */
    // 0 hp game lost
    if (gPlayer->getHealth() <= 0) {
        game_lost();
    }

    // were all relics found?
    if (gWorld->getRelics() <= gPlayer->getRelics()) {
        game_won();
    }
}

void Game::player_logic() {

    pos cur_pos = gPlayer->getPositon();

    switch (input)
    {
    case 'w':
        if (cur_pos.x <= 0) { break; }
        gPlayer->move(up);
        break;
    case 'a':
        if (cur_pos.y <= 0) { break; }
        gPlayer->move(left);
        break;
    case 's':
        if (cur_pos.x >= WORLD_SIZE - 1) { break; }
        gPlayer->move(down);
        break;
    case 'd':
        if (cur_pos.y >= WORLD_SIZE - 1) { break; }
        gPlayer->move(right);
        break;
    case 'e':
        running = false;
        break;
    case 'r':
        gPlayer->refStats().stamina.setCurrent(gPlayer->refStats().stamina.getLevel());
    }

    field& currentField = gWorld->accessField(gPlayer->getPositon());

    // only run events field wasn't seen yet
    if (!currentField.was_seen()) {
        gPlayer->event(currentField.getObject());
        currentField.set_seen();
    }
}

void Game::enemy_logic() {

    if (turn < BANDIT_SLEEP) return;

    // ally movement and combat logic
    for (auto& bandit : gWorld->getAllEnemies()) {

        if (bandit.get_is_ally()) {

            pos shortest_distance(100, 100);
            enemy* target;

            // find closest bandit
            for (auto& ptarget : gWorld->getAllEnemies()) {

                // dont chase other allies and themselves
                if (ptarget.get_is_ally() || ptarget == bandit) {
                    continue;
                }

                pos distance = ptarget.get_pos() - bandit.get_pos();
                if (distance <= shortest_distance) {
                    shortest_distance = distance;
                    target = &ptarget;
                }
            }

            if (shortest_distance == pos(100, 100)) {
                gWorld->removeEnemy(bandit);
                continue;
            }

            // move ally
            if (bandit.get_chasing()) {
                bandit.move(target->get_pos());
            } else {
                bandit.move(bandit.get_home());
            }

            // if it reaches the bandit they fight
            if (bandit.get_pos() == target->get_pos()) {
                
                combatOutcome result;
                
                switch (std::rand() % 3)
                {
                case 0:
                    // charisma
                    result = attribute::combat(bandit.ref_stats().charisma, target->ref_stats().charisma);

                    break;
                case 1:
                    // luck
                    result = attribute::combat(bandit.ref_stats().luck, target->ref_stats().luck);
                    
                    break;
                case 2:
                    //stamina
                    result = attribute::combat(bandit.ref_stats().stamina, target->ref_stats().stamina);
                    
                    break;
                }

                if (result == FIRST_WON) {
                    gWorld->removeEnemy(*target);
                }

                if (result == TIE) {
                    target->stop_chasing();
                }

                // remove the ally no matter the outcome of the fight
                gWorld->removeEnemy(bandit);
            }

        } else { // enemy movement and logic

            if (bandit.get_chasing()) {
                bandit.move(gPlayer->getPositon());
            } else {
                bandit.move(bandit.get_home());
            }

            if (bandit.get_pos() == gPlayer->getPositon()) {

                // create random seeding
                std::random_device rd;
                unsigned int seed = rd();
                std::srand(seed);

                combatOutcome result;

                switch (std::rand() % 3) {
                case 0:
                    //charisma fight on win make an ally
                    result = attribute::combat(gPlayer->refStats().charisma, bandit.ref_stats().charisma);
                    std::cout << result;
                    if (result == FIRST_WON) {
                        bandit.convert();
                    }

                    if (result == SECOND_WON) {
                        gPlayer->lost_fight();
                        bandit.stop_chasing();
                    }

                    if (result == TIE) {
                        bandit.stop_chasing();
                    }

                    break;
                case 1:
                    //luck fight on win 50/50 to get extra relic
                    result = attribute::combat(gPlayer->refStats().luck, bandit.ref_stats().luck);

                    if (result == FIRST_WON) {
                        gWorld->removeEnemy(bandit);
                        gPlayer->event_item();
                    }

                    if (result == SECOND_WON) {
                        gPlayer->lost_fight();
                        bandit.stop_chasing();
                    }

                    if (result == TIE) {
                        bandit.stop_chasing();
                    }

                    break;
                case 2:
                    //stamina fight on win get more max stamina
                    result = attribute::combat(gPlayer->refStats().stamina, bandit.ref_stats().stamina);

                    if (result == FIRST_WON) {
                        gWorld->removeEnemy(bandit);
                        gPlayer->refStats().stamina.levelUp();
                    }

                    if (result == SECOND_WON) {
                        gPlayer->lost_fight();
                        bandit.stop_chasing();
                    }

                    if (result == TIE) {
                        bandit.stop_chasing();
                    }
                    
                    break;
                default:
                    std::cout << "FEHLER!!!";
                    break;
                }

                gPlayer->fight_outcome(result);
            }
        }
    }
}

void Game::game_won() {
    
    win++;
    std::cout << "\nYOU WON!\n";
    
    delete gWorld;
    delete gPlayer;
    gWorld = new world(win + 1);
    gPlayer = new player();
    
    // reset turn number for bandit sleep
    turn = 0;
}

void Game::game_lost() {
    running = false;
}