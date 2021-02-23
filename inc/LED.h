#ifndef _LED_H_
#define _LED_H_

// Freedom KL25Z LEDs
#define RED_LED		(uint32_t)(18)	//PORT B
#define GREEN_LED   (uint32_t)(19)	//PORT B
#define BLUE_LED    (uint32_t)(1)	//PORT D


//LED Functions
void init_RGB(void);
void toggle(void);
void toggle_r(void);
void toggle_g(void);
void toggle_b(void);
void set_r(char on_off);
void set_g(char on_off);
void set_b(char on_off);

#endif
