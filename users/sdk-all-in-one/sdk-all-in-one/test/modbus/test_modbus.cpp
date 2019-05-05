#include "ont_modbus.h"
#include "ont/modbus.h"
#include <gtest/gtest.h>

TEST( TestModbus, Normal ) {
    ont_device_t *dev = NULL;

    EXPECT_EQ( ONT_ERR_OK, ont_device_create( 59261, ONTDEV_MODBUS, "test_modbus", &dev ) );
    EXPECT_EQ( ONT_ERR_OK, ont_device_connect( dev, ONT_SERVER_ADDRESS, ONT_SERVER_PORT, "4q0vXEp99yeGjfEc", "{\"18888888888\":\"12345\"}", 200 ) );

    unsigned char reply_1[] = { 0x01, 0x03, 0x04, 0x00, 0x01, 0x00, 0x02, 0x32, 0x2a };
    unsigned char reply_2[] = { 0x02, 0x03, 0x04, 0x00, 0x03, 0x00, 0x04, 0xf0, 0x38 };
    unsigned char reply_3[] = { 0x03, 0x03, 0x04, 0x00, 0x05, 0x00, 0x06, 0xf0, 0x49 };
    unsigned char reply_4[] = { 0x04, 0x01, 0x05, 0x71, 0x72, 0x73, 0x74, 0x75, 0x21, 0x22 };
    unsigned char reply_5[] = { 0x05, 0x01, 0x01, 0x61, 0x91, 0x50 };
    unsigned char reply_6[] = { 0x06, 0x03, 0x04, 0x00, 0x03, 0x00, 0x04, 0x30, 0x7d };
    unsigned char reply_7[] = { 0x07, 0x03, 0x04, 0x00, 0x03, 0x00, 0x04, 0xf0, 0x6d };
    unsigned char reply_8[] = { 0x08, 0x02, 0x05, 0x71, 0x72, 0x73, 0x74, 0x75, 0x77, 0x12 };
    unsigned char reply_9[] = { 0x09, 0x04, 0x04, 0x00, 0x03, 0x00, 0x04, 0x87, 0x83 };
    unsigned char *replays[] = { reply_1, reply_2, reply_3, reply_4, reply_5, reply_6, reply_7, reply_8, reply_9 };
    unsigned int sizes[] = { sizeof(reply_1), sizeof(reply_2), sizeof(reply_3), sizeof(reply_4), sizeof(reply_5), sizeof(reply_6), sizeof(reply_7), sizeof(reply_8), sizeof(reply_9) };

    auto count = 0;
    ont_device_cmd_t *cmd = NULL;
    while ( 1 ) {
        cmd = ont_device_get_cmd( dev );
        if ( NULL != cmd ) {
            ++count;
            EXPECT_GT( cmd->size, 2 );
            EXPECT_GE( cmd->req[0], 1 );
            EXPECT_LE( cmd->req[0], 9 );
            EXPECT_EQ( ONT_ERR_OK, ont_device_reply_cmd( dev, cmd->id, (const char*)replays[cmd->req[0] - 1], sizes[cmd->req[0] - 1] ) );
            ont_device_cmd_destroy( cmd );
        }

        ont_platform_sleep( 100 );

        if ( count > 10 ) {
            break;
        }
    }

    ont_device_disconnect( dev );
    ont_device_destroy( dev );
}