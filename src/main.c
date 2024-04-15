/*
 * Copyright (c) 2012-2014 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>

#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/console/console.h>
#include <zephyr/drivers/gpio.h>

#define SLEEP_TIME_MS 50
#define LED0_NODE DT_ALIAS(led0)
static const struct gpio_dt_spec led0 = GPIO_DT_SPEC_GET(LED0_NODE, gpios);

int ret;
static const char* const letters[]={".-/","-.../","-.-./",
					"-../","./","..-./","--./",
					"..../","../",".---/","-.-/"
					,".-../","--/","-./","---/",".--./",
					"--.-/",".-./",".../","-/","..-/","...-/",
					".--/","-..-/","-.--/","--../"};
static const char* const numbers[]={"-----/",".----/","..---/","...--/","....-/","...../","-..../","--.../","---../","----./"};					
static const char* letter2morse(char c){
	if(c>='A'&&c<='Z'){
		return letters[c-'A'];

	}
	else if(c>='a'&&c<='z'){
		return letters[c-'a'];
	}
	else if(c>='0'&&c<='9'){
		return numbers[c-'0'];
	}
	else if(c==' '){
		return "/";
	}
	else return "?";
}

static void morse2led (const char * letter){
	
	for (int i=0;letter[i]!=0;i++){
		
		if(letter[i]=='.'){
			gpio_pin_set_dt(&led0,0);		
			k_msleep(SLEEP_TIME_MS);
			gpio_pin_set_dt(&led0,1);
		}
		else if(letter[i]=='-'){
			gpio_pin_set_dt(&led0,0);		
			k_msleep(3*SLEEP_TIME_MS);
			gpio_pin_set_dt(&led0,1);
		}
		else if(letter[i]=='/'){
			k_msleep(2*SLEEP_TIME_MS);
		}
		k_msleep(SLEEP_TIME_MS);

		

	}
		
}

int main(void)
{

	console_getline_init();

	if (!gpio_is_ready_dt(&led0)) {
		return 0;
	}

	ret = gpio_pin_configure_dt(&led0, GPIO_OUTPUT_ACTIVE);
	if (ret < 0) {
		return 0;
	}

	while(true){
		printf("Enter message:");
		char* linepointer= console_getline();
		// char* linepointer= "test test sq9hyl ";
		for (int i=0;linepointer[i]!=0;i++){
			const char* morse;
			morse=letter2morse(linepointer[i]);
			printf("morse:%s\n", morse);
			morse2led(morse);
		}
}
	return 0;
}
