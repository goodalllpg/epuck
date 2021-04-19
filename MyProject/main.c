#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "ch.h"
#include "motors.h"
#include "leds.h"
#include "spi_comm.h"
#include "hal.h"
#include "memory_protection.h"
#include <main.h>
#include "sensors/proximity.h"
#include "chprintf.h"
#include "usbcfg.h"

	messagebus_t bus;
	MUTEX_DECL(bus_lock);
	CONDVAR_DECL(bus_condvar);

int main(void)
{

	messagebus_init(&bus, &bus_lock, &bus_condvar);
    halInit();
    chSysInit();
    mpu_init();
    motors_init();
    clear_leds();
    spi_comm_start();
    proximity_start();
    calibrate_ir();
    usb_start();

    /* Infinite loop. */
    while (1) {
    	int motor_speed=300;
    	int i=0;
    	int state=1;
    	//waits 1 second
    	right_motor_set_speed(300);
    	left_motor_set_speed(300);
    	for (i=0; i<8; i++)
    		if (get_calibrated_prox(i)>800)
    		{
    		while (get_calibrated_prox(i)>800)
    		{
    			if (i>3)
    			{
    				state=-1;
    			}
    			else if (i<4)
    			{
    				state=1;
    			}
        	right_motor_set_speed(100*state);
        	left_motor_set_speed(-100*state);
    		}
    		}
    }
}

#define STACK_CHK_GUARD 0xe2dee396
uintptr_t __stack_chk_guard = STACK_CHK_GUARD;

void __stack_chk_fail(void)
{
    chSysHalt("Stack smashing detected");
}
