
#ifndef __ONT_MQTT_ISP_H__
#define __ONT_MQTT_ISP_H__


#define ISP_CM  100025LL
#define ISP_CU  100026LL
#define ISP_CT  100017LL
#define ISP_CE  100027LL
/**
 * 运营商标识
 */
typedef enum ont_isp_type_t {
    ONTISP_UK = 0,                  /**未    知= 0 */
    ONTISP_CM = 1,                  /**中国移动= 1 */
    ONTISP_CU = 2,                  /**中国联通= 2 */
    ONTISP_CT = 3,                  /**中国电信= 3 */
    ONTISP_CE = 4,                  /**教育网  = 4 */
} ont_isp_type_t;



/**
 * CURL解析错误标识
 */
typedef enum ont_curl_err_type_t{
    ONTISP_CURL_ERR_OK             =   0,
    ONTISP_CURL_ERR_BAD            =   -1,
    ONTISP_CURL_ERR_OVER_RANGE     =   -2,
} ont_curl_err_type_t;


typedef struct MemoryStruct {
    char *memory;
    size_t size;
} MemoryStruct;



int isp_parse_info();


#endif /*__ONT_MQTT_ISP_H__*/
