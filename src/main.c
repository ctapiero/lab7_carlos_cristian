#include <zephyr.h>
#include <kernel.h>
#include <sys/printk.h>
#include <device.h>
#include <drivers/can.h>
#include <drivers/gpio.h>
#include <sys/byteorder.h>

#define MY_STACK_SIZE 512
#define MY_PRIORITY 5

struct gpio_dt_spec led = GPIO_DT_SPEC_GET_OR(DT_ALIAS(led0), gpios, {0});
const struct device *can_dev;



struct k_work_q my_work_q;
const struct device *can_dev1;
const struct device *can_dev2;
int ret2;
int ret1;
int filter_id1;
int filter_id2;


K_THREAD_STACK_DEFINE(my_stack_area, MY_STACK_SIZE);




    struct zcan_frame frame1 = {
        .id_type = CAN_STANDARD_IDENTIFIER,
        .rtr = CAN_DATAFRAME,
        .id = 0x123,
        .dlc = 8,
        .data = {1,2,3,4,5,6,7,8}
};

struct zcan_frame frame2 = {
        .id_type = CAN_STANDARD_IDENTIFIER,
        .rtr = CAN_DATAFRAME,
        .id = 0x124,
        .dlc = 8,
        .data = {1,2,3,4,5,6,7,8}
};
//send a single message to bus
void send_message(struct zcan_frame *frame)
{
        ret1 = can_send(can_dev1, &frame2, K_MSEC(100), NULL, NULL); //send message 2
        if (ret1 != CAN_TX_OK) {
                printk("Sending failed [%d]", ret1);
                }     
}        


//send 2 messages to bus
void send_message_2(void)
{
        ret2 = can_send(can_dev2, &frame1, K_MSEC(100), NULL, NULL); // send message 1
        if (ret2 != CAN_TX_OK) {
                printk("Sending failed [%d]", ret2);
                }     
        k_sleep(K_MSEC(250)); //wait before sending again

}       

const struct zcan_filter message_filter = {
		.id_type = CAN_STANDARD_IDENTIFIER,
		.rtr = CAN_DATAFRAME,
		.id = 0x123,
		.rtr_mask = 1,
		.id_mask = 0x000
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
can_send(can_dev, &frame2, K_MSEC(1000), NULL, "Sender 1");
}

void CAN_send_two_messages(void){

gpio_pin_set(led.port,led.pin,1);
can_send(can_dev, &frame1,  K_MSEC(100), NULL, "Sender 1");
can_send(can_dev, &frame2, K_MSEC(100), NULL, "Sender 2");
}
void main(void)
{
    
    int can_status;
    int filter_id;

    can_dev = device_get_binding("CAN_1");
    // can_status =  can_set_mode(can_dev, CAN_LOOPBACK_MODE); loopback mode
    can_status =  can_set_mode(can_dev, CAN_NORMAL_MODE);

    gpio_pin_configure_dt(&led,GPIO_OUTPUT_HIGH);
    
    filter_id = can_attach_isr(can_dev, rx_callback_function, NULL, &message_filter);
    if (filter_id < 0) {
        printk("Unable to attach isr [%d]", filter_id);
    }

    while(1){
    CAN_send_two_messages();
    k_msleep(1000);
    }

    // activity 3 
    #if defined (ACTIVITY3)
    can_set_mode(can_dev, CAN_NORMAL_MODE);
    

    struct zcan_filter filter = {
        .id_type = CAN_STANDARD_IDENTIFIER,
        .rtr = CAN_DATAFRAME,
        .id = 0x123,
        .rtr_mask = 1, 
        .id_mask = CAN_STD_ID_MASK
    };

    can_attach_isr(can_dev, rx_callback_function, NULL, &filter);
    void send_loprio_message_periodic(const struct device *dev, uint32_t delay){

    struct zcan_frame msg = {
        .id_type = CAN_STANDARD_IDENTIFIER,
        .rtr = CAN_DATAFRAME,
        .id = 0x129,
        .dlc = 8,
        .data = {1,2,3,4,5,6,7,8}
    };

    struct k_timer t;
    k_timer_init(&t, NULL, NULL);

    while(1){

        can_send(dev, &msg, K_MSEC(100), NULL, NULL);
        k_timer_start(&t, K_MSEC(delay), K_NO_WAIT);
        k_timer_status_sync(&t);
    }
}
    while(1){
        // send_hiprio_message_cont(can_dev);
        send_lowpriorityMessage(can_dev, 1000);
    }
    
    #endif
}
