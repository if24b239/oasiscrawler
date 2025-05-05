#include <catch_amalgamated.hpp>
#include <global.h>
#include <enemy.h>

TEST_CASE( "enemy movement" ) {
    
    enemy one(4, pos(0,0));

    one.move(pos(3, 0));

    REQUIRE(one.get_pos() == pos(1,0));

    one.move(pos(0,3));

    REQUIRE(one.get_pos() == pos(1,1));
};

