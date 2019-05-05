#include "utils.h"
#include <gtest/gtest.h>

TEST( UtilsTest, Normal ) {
    const char *str1 = "hello,world!";
    EXPECT_EQ( strlen( str1 ), ont_strlen( str1 ) );

    char str2[32] = { '\0' };
    char str3[32] = { '\0' };
    EXPECT_STREQ( strcpy( str2, str1 ), ont_strcpy( str3, str1 ) );
    EXPECT_EQ( 0, ont_strcmp( str2, str3, ont_strlen( str3 ) ) );
    EXPECT_STREQ( strchr( str2, ',' ), ont_strchr( str3, ',' ) );
    EXPECT_STREQ( strstr( str2, "wo" ), ont_strstr( str3, "wo" ) );

    EXPECT_EQ( atoi( "-241243" ), ont_stoi( "-241243" ) );
    EXPECT_EQ( atoi( "345234234" ), ont_stoi( "345234234" ) );

    long long int val = 0;
    ont_stoi64( (int32_t*)&val, "-345236345123432" );
    EXPECT_EQ( atoll( "-345236345123432" ), val );

    val = 0;
    ont_stoi64( (int32_t*)&val, "2436523453255464" );
    EXPECT_EQ( atoll( "2436523453255464" ), val );

    ont_memzero( str3, sizeof(str3) );
    for ( auto i = 0; i < sizeof(str3); ++i ) {
        EXPECT_EQ( 0, str3[i] );
    }

    ont_memmove( str3, str2, sizeof(str2) );
    for ( auto i = 0; i < sizeof(str2); ++i ) {
        EXPECT_EQ( str3[i], str2[i] );
    }
}