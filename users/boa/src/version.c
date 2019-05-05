/*
 *      File to define f/w version number
 *
 *      Authors: David Hsu	<davidhsu@realtek.com.tw>
 *
 *      $Id: version.c,v 1.23 2009/08/06 01:10:22 davidhsu Exp $
 *
 */
#ifdef CONFIG_RTL_8196B
#ifdef CONFIG_RTL8196B_TLD
 unsigned char *fwVersion="v1.4_TD";
#else
 unsigned char *fwVersion="v1.4";
#endif
#elif defined(CONFIG_RTL_8198C) || defined(CONFIG_RTL_8196C) || defined(CONFIG_RTL_8198) || defined(CONFIG_RTL_819XD) 
unsigned char *fwVersion="v3.4.11d-CMCC-pre5";
//unsigned char *fwVersion="v3.0";
#elif defined(CONFIG_RTL_8196E)
unsigned char *fwVersion="HN100_V1.0.1";
#elif defined(CONFIG_RTL_8197F)
	unsigned char *JSVersion="（中国移动江苏公司1.0版本）";
#if defined (CONFIG_MODEL_HN100_340EA)
	unsigned char *fwVersion="HN100_V1.0.1";
#elif defined (CONFIG_MODEL_HN120_341FA)
	unsigned char *fwVersion="HN120_V1.0.2(涓浗绉诲姩姹熻嫃鍏徃1.0鐗堟湰)";
#elif defined (CONFIG_MODEL_HN120_341BA)
	unsigned char *fwVersion="HN120_V1.0.2";
#elif defined (CONFIG_MODEL_HN350_339EA)
	unsigned char *fwVersion="HN350_V1.0.1";	
#elif defined (CONFIG_MODEL_HN330_331EA)
	unsigned char *fwVersion="HN330_V1.0.1";
#elif defined (CONFIG_MODEL_HN140_361EA)
	unsigned char *fwVersion="HN140_V1.0.1";
#else
	unsigned char *fwVersion="ZW_V1.0.1";
#endif
#else
 unsigned char *fwVersion="v1.2f";
#endif
#define SDK_VERSION "Realtek SDK v3.4.11d-CMCC-pre5"
