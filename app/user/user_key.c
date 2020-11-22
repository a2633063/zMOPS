#include "ets_sys.h"
#include "osapi.h"
#include "os_type.h"
#include "mem.h"
#include "user_interface.h"
#include "espconn.h"

#include "../cJson/cJSON.h"
#include "driver\key.h"
#include "user_key.h"
#include "user_led.h"
#include "user_wifi.h"
#include "user_json.h"
#include "user_setting.h"

os_timer_t key_50ms;
LOCAL unsigned char key_time = 0;

void ICACHE_FLASH_ATTR
user_relay_set(char level) {
	if (level != -1) {
		GPIO_OUTPUT_SET(GPIO_ID_PIN(GPIO_RELAY_IO_NUM), level);
	} else {
		GPIO_OUTPUT_SET(GPIO_ID_PIN(GPIO_RELAY_IO_NUM), !GPIO_INPUT_GET(GPIO_ID_PIN(GPIO_RELAY_IO_NUM)));
	}
	user_set_led_wifi(user_config.on);
}

LOCAL void ICACHE_FLASH_ATTR
user_key_short_press(void) {
	uint8_t i, result = 1;
	char strJson[128];
	os_printf("user_key_short_press\n");


	user_config.on = !user_config.on;

	os_sprintf(strJson, "{\"mac\":\"%s\",\"on\":%d}", strMac, user_config.on);
	user_json_analysis(false, strJson);
}

LOCAL void ICACHE_FLASH_ATTR
user_key_long_press(void) {
	os_printf("user_key_long_press\n");

}

/******************************************************************************
 * FunctionName : key_50ms_cb
 * Description  : 50ms timer callback to check it's a real key push
 * Parameters   : single_key_param *single_key - single key parameter
 * Returns      : none
 *******************************************************************************/
LOCAL void ICACHE_FLASH_ATTR
key_50ms_cb(void *arg) {

	if(child_lock==1){
		os_timer_disarm(&key_50ms);
		key_time = 0;
		gpio_pin_intr_state_set(GPIO_ID_PIN(GPIO_KEY_0_IO_NUM), GPIO_PIN_INTR_NEGEDGE);
		return;
	}

	if (GPIO_INPUT_GET(GPIO_ID_PIN(GPIO_KEY_0_IO_NUM)) == 0) {
		key_time++;
		if (key_time == 100) {
			user_key_long_press();
			key_time = 110;
		} else if (key_time > 100) {
			key_time = 110;
		}
	} else {
		if (key_time < 100 && key_time > 1)	//>1 Îª°´¼üÈ¥¶¶
			user_key_short_press();
		os_timer_disarm(&key_50ms);
		key_time = 0;
		gpio_pin_intr_state_set(GPIO_ID_PIN(GPIO_KEY_0_IO_NUM), GPIO_PIN_INTR_NEGEDGE);
	}
}
LOCAL void key_intr_handler(void *arg) {
	uint8_t i;
	uint32_t gpio_status = GPIO_REG_READ(GPIO_STATUS_ADDRESS);
	struct keys_param *keys = (struct keys_param *) arg;

	if (gpio_status & BIT(GPIO_KEY_0_IO_NUM)) {
		//disable interrupt
		gpio_pin_intr_state_set(GPIO_ID_PIN(GPIO_KEY_0_IO_NUM), GPIO_PIN_INTR_DISABLE);

		//clear interrupt status
		GPIO_REG_WRITE(GPIO_STATUS_W1TC_ADDRESS, gpio_status & BIT(GPIO_KEY_0_IO_NUM));

		// 50ms, check if this is a real key up
		os_timer_disarm(&key_50ms);
		os_timer_setfn(&key_50ms, (os_timer_func_t *) key_50ms_cb, NULL);
		os_timer_arm(&key_50ms, 50, 1);

	}
}
void ICACHE_FLASH_ATTR
user_key_init(void) {

//	PIN_FUNC_SELECT(GPIO_RELAY_IO_MUX, GPIO_RELAY_IO_FUNC);
//	single_key[0] = key_init_single(GPIO_KEY_0_IO_NUM, GPIO_KEY_0_IO_MUX,
//	GPIO_KEY_0_IO_FUNC, user_key_long_press, user_key_short_press);
//
//	keys.key_num = GPIO_KEY_NUM;
//	keys.single_key = single_key;
//	key_init(&keys);

	ETS_GPIO_INTR_ATTACH(key_intr_handler, NULL);

	ETS_GPIO_INTR_DISABLE();

	PIN_FUNC_SELECT(GPIO_KEY_0_IO_MUX, GPIO_KEY_0_IO_FUNC);

	gpio_output_set(0, 0, 0, GPIO_ID_PIN(GPIO_KEY_0_IO_NUM));

	gpio_register_set(GPIO_PIN_ADDR(GPIO_KEY_0_IO_NUM),
	GPIO_PIN_INT_TYPE_SET(GPIO_PIN_INTR_DISABLE) | GPIO_PIN_PAD_DRIVER_SET(GPIO_PAD_DRIVER_DISABLE) | GPIO_PIN_SOURCE_SET(GPIO_AS_PIN_SOURCE));

	//clear key io status
	GPIO_REG_WRITE(GPIO_STATUS_W1TC_ADDRESS, BIT(GPIO_KEY_0_IO_NUM));

	//enable interrupt
	gpio_pin_intr_state_set(GPIO_ID_PIN(GPIO_KEY_0_IO_NUM), GPIO_PIN_INTR_NEGEDGE);

	ETS_GPIO_INTR_ENABLE();

}
