#ifndef __USER_CONFIG_H__
#define __USER_CONFIG_H__

#include "os_type.h"

#define VERSION "v1.3.2"

#define TYPE 2
#define TYPE_NAME "zDC1"

#define DEVICE_NAME "zDC1_%02X%02X"
#define MDNS_DEVICE_NAME "zDC1_%s"

#define USER_CONFIG_VERSION 5

#define SETTING_MQTT_STRING_LENGTH_MAX  64      //���� 4 �ֽڶ��롣
#define PLUG_NAME_LENGTH 32		//���������ַ�����󳤶�
#define PLUG_NUM 4              //��������
#define PLUG_TIME_TASK_NUM 5    //ÿ���������5�鶨ʱ����

typedef struct {
	int8_t hour;      //Сʱ
	int8_t minute;    //����
	uint8_t repeat; //bit7:һ��   bit6-0:����-��һ
	int8_t action;    //����
	int8_t on;    //����
} user_plug_task_config_t;

typedef struct {
	uint8_t name[PLUG_NAME_LENGTH];
	uint8_t on;    //��¼��ǰ����
	user_plug_task_config_t task[PLUG_TIME_TASK_NUM];
} user_plug_config_t;

//�û���������ṹ��
typedef struct {
	uint8_t name[PLUG_NAME_LENGTH];
	uint8_t mqtt_ip[SETTING_MQTT_STRING_LENGTH_MAX];   //mqtt service ip
	uint16_t mqtt_port;        //mqtt service port
	uint8_t mqtt_user[SETTING_MQTT_STRING_LENGTH_MAX];     //mqtt service user
	uint8_t mqtt_password[SETTING_MQTT_STRING_LENGTH_MAX];     //mqtt service user
	user_plug_config_t plug[PLUG_NUM];
	char version;
    uint8_t interval;   //�ϱ�Ƶ��
} user_config_t;

extern char rtc_init;
extern user_config_t user_config;
extern uint16_t power;
extern uint16_t voltage;
extern uint16_t current;
#endif

