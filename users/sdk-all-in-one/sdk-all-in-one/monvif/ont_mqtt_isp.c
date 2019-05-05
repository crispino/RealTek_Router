#include <stdio.h>
#include <string.h>
#include <ont/mqtt.h>
#include <ont/log.h>
#include "isp_info.h"
#include "cJSON.h"



int serialize_to_buf(uint8_t *buf, uint32_t ispid)
{
	if(NULL == buf){
		return -1;
	}

	buf[0] = (ispid >> 24);
	buf[1] = (ispid >> 16);
	buf[2] = (ispid >> 8);
	buf[3] = ispid;

	return 0;
}


int ont_isp_publish(ont_device_t *dev, int ispid)
{
    int err = 0;
    uint8_t isp_publish_content[4];
    const  char *isp_publish_topic_name = "$Video_ispid";
    uint8_t qos_level = 1;

    serialize_to_buf(isp_publish_content, (uint32_t) ispid);

    size_t isp_pubilsh_content_size = sizeof(isp_publish_content);

    err = ont_mqtt_publish(dev, isp_publish_topic_name,
            isp_publish_content, isp_pubilsh_content_size, qos_level);

    return err;
}
