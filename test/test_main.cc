#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>

#include <cstdint>

#include "play_main.hpp"


TEST_CASE( "Factorials are computed", "[factorial]" ) {
    REQUIRE( factorial( 1) == 1 );
    REQUIRE( factorial( 2) == 2 );
    REQUIRE( factorial( 3) == 6 );
    REQUIRE( factorial(10) == 3'628'800 );
}