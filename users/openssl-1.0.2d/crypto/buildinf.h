#ifndef MK1MF_BUILD
    /* auto-generated by util/mkbuildinf.pl for crypto/cversion.c */
    #define CFLAGS cflags
    /*
     * Generate CFLAGS as an array of individual characters. This is a
     * workaround for the situation where CFLAGS gets too long for a C90 string
     * literal
     */
    static const char cflags[] = {
        'c','o','m','p','i','l','e','r',':',' ','m','s','d','k','-','l','i','n',
        'u','x','-','g','c','c',' ','-','I','.',' ','-','I','.','.',' ','-','I',
        '.','.','/','i','n','c','l','u','d','e',' ',' ','-','f','P','I','C',' ',
        '-','D','O','P','E','N','S','S','L','_','P','I','C',' ','-','D','D','S',
        'O','_','D','L','F','C','N',' ','-','D','H','A','V','E','_','D','L','F',
        'C','N','_','H',' ','-','D','T','E','R','M','I','O',' ','-','O','s',' ',
        '-','f','o','m','i','t','-','f','r','a','m','e','-','p','o','i','n','t',
        'e','r',' ','-','W','a','l','l','\0'
    };
    #define PLATFORM "platform: mips-linux"
    #define DATE "built on: Sat May  4 09:45:48 2019"
#endif