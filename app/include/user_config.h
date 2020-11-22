#ifndef __USER_CONFIG_H__
#define __USER_CONFIG_H__

#include "os_type.h"

#define VERSION "v0.1.0"

#define TYPE 7
#define TYPE_NAME "zMOPS"

#define DEVICE_NAME "zMOPS_%02X%02X"
#define MDNS_DEVICE_NAME "zMOPS_%s"

#define USER_CONFIG_VERSION 1

#define SETTING_MQTT_STRING_LENGTH_MAX  64      //必须 4 字节对齐。
#define PLUG_NAME_LENGTH 32		//插座名称字符串最大长度

#define PLUG_TIME_TASK_NUM 5    //每个插座最多5组定时任务

typedef struct {
	int8_t hour;      //小时
	int8_t minute;    //分钟
	uint8_t repeat; //bit7:一次   bit6-0:周日-周一
	int8_t action;    //动作
	int8_t on;    //开关
} user_plug_task_config_t;

//用户保存参数结构体
typedef struct {
	char version;
	uint8_t name[PLUG_NAME_LENGTH];
	uint8_t mqtt_ip[SETTING_MQTT_STRING_LENGTH_MAX];   //mqtt service ip
	uint16_t mqtt_port;        //mqtt service port
	uint8_t mqtt_user[SETTING_MQTT_STRING_LENGTH_MAX];     //mqtt service user
	uint8_t mqtt_password[SETTING_MQTT_STRING_LENGTH_MAX];     //mqtt service user

	uint8_t on;    //记录当前开关
	user_plug_task_config_t task[PLUG_TIME_TASK_NUM];

} user_config_t;

extern char rtc_init;
extern user_config_t user_config;
extern uint8_t child_lock;
extern uint8_t led_lock;
#endif

