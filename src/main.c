#include <zephyr.h>
#include <kernel.h>
#include <sys/printk.h>
#include <device.h>
#include <drivers/can.h>
#include <drivers/gpio.h>
#include <sys/byteorder.h>

struct gpio_dt_spec led = GPIO_DT_SPEC_GET_OR(DT_ALIAS(led0), gpios, {0});
const struct device *can_dev;

const struct zcan_filter message_filter = {
		.id_type = CAN_STANDARD_IDENTIFIER,
		.rtr = CAN_DATAFRAME,
		.id = 0x123,
		.rtr_mask = 1,
		.id_mask = 0x000
	};

    struct zcan_frame frame = {
        .id_type = CAN_STANDARD_IDENTIFIER,
        .rtr = CAN_DATAFRAME,
        .id = 0x123,
        .dlc = 8,
        .data = {1,2,3,4,5,6,7,8}
};

void tx_irq_callback(uint32_t error_flags, void *arg)
{
	char *sender = (char *)arg;
	if (error_flags) {
		printk("Callback! error-code: %d\nSender: %s\n",
		       error_flags, sender);
	}
}

void rx_led_callback(uint32_t error_flags, void *arg)
{
    
}

void rx_callback_function(struct zcan_frame *frame, void *arg)
{
    
    gpio_pin_set(led.port,led.pin,0);
    

}

void CAN_send(void){

gpio_pin_set(led.port,led.pin,1);
can_send(can_dev, &frame, K_MSEC(1000), NULL, "Sender 1");
}

void CAN_send_two(void){

gpio_pin_set(led.port,led.pin,1);
can_send(can_dev, &frame, K_MSEC(1000), NULL, "Sender 1");
can_send(can_dev, &frame, K_MSEC(1000), NULL, "Sender 1");

}
void main(void)
{
    
    int can_status;
    int filter_id;

    can_dev = device_get_binding("CAN_1");
    can_status =  can_set_mode(can_dev, CAN_LOOPBACK_MODE);
    gpio_pin_configure_dt(&led,GPIO_OUTPUT_HIGH);
    
    filter_id = can_attach_isr(can_dev, rx_callback_function, NULL, &message_filter);
    if (filter_id < 0) {
        printk("Unable to attach isr [%d]", filter_id);
    }

    while(1){
    CAN_send();
    k_msleep(1000);

    }

    
}
