/*include headers*/
#include "gtest/gtest.h"
#include "utils.h"
#include "ont/device.h"
#include "ont/modbus.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main( int argc, char *argv[] ) {
    testing::InitGoogleTest( &argc, argv );

    return RUN_ALL_TESTS();
}