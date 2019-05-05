#include "utils.h"
#include "ont/modbus.h"
#include "gtest/gtest.h"

TEST( TestDs, Normal ) {
    ont_device_t dev;
    char api_key[] = "w1Lc0phxiB5PjYMKTtL=SkqvYYA=";

    ont_memzero( &dev, sizeof(dev) );
    dev.device_id = 3013179;
    dev.key = api_key;

    EXPECT_EQ( ONT_ERR_OK, ont_modbus_create_ds( &dev, ONT_SERVER_ADDRESS, ONT_SERVER_PORT, "sdk_test_create_ds", "060300010002947C", 60, "(A0-A1)%A2" ) );
}