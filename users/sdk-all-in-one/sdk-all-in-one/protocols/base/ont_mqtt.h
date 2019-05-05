#ifndef ONT_SRC_COMM_ONT_MQTT_H_
#define ONT_SRC_COMM_ONT_MQTT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "ont/platform.h"
#include "ont/device.h"

/**
 * ����mqtt�豸
 * @param device �ⲿ���ýӿڵ�ont_device_tָ��
 * @return �ɹ��򷵻�ONT_ERR_OK
 */
int ont_mqtt_create(ont_device_t **dev);

/**
 * ����mqtt�豸
 * @param device ������mqtt�豸��ָ��
 */
void ont_mqtt_destroy(ont_device_t *device);


/**
 * ����Server
 * @param device ������mqtt�豸��ָ��,������dev��ip��ַ�Ͷ˿�
 * @param auth_info ��Ȩ��Ϣ
 * @return �ɹ��򷵻�ONT_ERR_OK
 */
int ont_mqtt_connect(ont_device_t *device,const char *auth_info);

/**
 * �Ͽ�����
 * @param device ������mqtt�豸��ָ��
 */
void ont_mqtt_disconnect(ont_device_t *device);

/**
 * ���ӱ���
 * @param device ������mqtt�豸��ָ��
 * @return �ɹ��򷵻�ONT_ERR_OK
 */
int ont_mqtt_keepalive(ont_device_t *device);

/**
 * ��������
 * @param device ������mqtt�豸��ָ��
 * @param payload �������ݵ���ʼ��ַ
 * @param bytes �������ݵĳ���
 * @return �ɹ��򷵻�ONT_ERR_OK
 */
int ont_mqtt_send_datapoints(ont_device_t *device,
								 const char* payload,
								 size_t bytes);

/**
 * ��ȡƽ̨�·�������
 * @param device ������mqtt�豸��ָ��
 * @return �ɹ��򷵻�ont_device_cmd_t�����ַ�����򷵻�NULL
 * ע���ڻ�ȡ��������ͷ��ڴ棬�ȵ���ont_platform_free(cmd->req),
 * ͬʱ�� ont_device_cmd_t->reqָ�� ����ΪNULL,
 * Ȼ�����ont_platform_free(ont_device_cmd_t*);��ont_device_cmd_t ����ΪNULL
 */
ont_device_cmd_t* ont_mqtt_get_cmd(ont_device_t *device);

/**
 * �ظ�����
 * @param device ������mqtt�豸��ָ��
 * @param cmdid �����id
 * @param resp �ظ����������
 * @param bytes �ظ�����ĳ���
 * @param qos_level qos����1������ȷ��ack��0��������
 * @return �ɹ��򷵻�ONT_ERR_OK
 */
int ont_mqtt_reply_cmd(ont_device_t *device,
						   const char *cmdid,
						   const char *resp,
						   size_t bytes);


#ifdef __cplusplus
} /* extern "C" */
#endif
#endif
