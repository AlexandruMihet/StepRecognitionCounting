#include <MKL25Z4.h>
#include <stdio.h>
#include "PeriodicTimer.h"
#include "LED.h"
#include "UARTCom.h"
#include "i2c.h"
#include "mma8451.h"
#include "Recorder.h"
#include <math.h>



static char buff[50];
void tx_call(void);

double calculate_noise(int x_media, int y_media, int z_media){
	return sqrt( x_media*x_media + y_media * y_media + z_media * z_media );
}

void tx_call(void) {
}

int main(void) {
	int i = 0, j = 0;
	int working = 0;
	
	config_sys_clock();
	
	Com_Init(9600);
	i2c_init();
	init_mma();
	
	init_RGB();
	toggle();
	
	//RecorderInit(0, 0, 1);
	
    //timer0_init(240000);
	timer0_init(24000000);
	int state = 0;
	int steps = 0;
	int x_mediu = 0;
	int y_mediu = 0;
	int z_mediu = 0;
	int pasi = 0;
	
	while(1) {
		
		if(timerEvent) {
			timerEvent = 0;
			
			if(j > 100) {
				if(i & 0x01)
					set_b(1);
				else
					set_b(0);
				
				if(i & 0x02)
					set_g(1);
				else
					set_g(0);
				
				if(i & 0x04) 
					set_r(1);
				else
					set_r(0);	
				i++;
			//	j = 0;
			}
		//	j++;
			//RecorderCallback();
			read_full_xyz();
			sprintf(buff, "x=%d y=%d z=%d\n\r", acc_X, acc_Y, acc_Z);
			stdout_putstr(buff, 50);
			
			/*x_mediu += acc_X;
			y_mediu += acc_Y;
			z_mediu += acc_Z;
			pasi += 1;*/
			
		/*	if(pasi == 50)
			{
				sprintf(buff, "medie x=%d, medie y=%d, medie z=%d\n\r", x_mediu/pasi, y_mediu/pasi, z_mediu/pasi);
				stdout_putstr(buff, 50);
			}*/
			
			//scade offset-ul
			double netmag = calculate_noise(acc_X, acc_Y, acc_Z) - calculate_noise(50, 50, 4160);
			
			sprintf(buff, "netmag = %f\n\r", netmag);
			stdout_putstr(buff, 50);
			
			int z = acc_Z - 4096;
			static const int offset_step_up = 200;
			static const int offset_setp_down = -200;
			
			
			switch(state){
				case 0:
					if(z >= offset_step_up){	//a inceput sa paseasca	(prima jumate de pas)
						state = 1;
					}
					break;
				
				case 1:
					if(z <= offset_setp_down){	//detecteaza cand coboara	(a doua jumatate de pas)
						state = 0;
						steps++;
						sprintf(buff, "steps=%d\n\r", steps);
						stdout_putstr(buff, 20);
					}
					break;
				
				default:
					break;
			}

			
			
		}
		
		//if(Com_HasByte()) {
		//	char c = (char)Com_GetByte();
			//if(c == 'r' || c=='R') {
				//RecorderStart();
			//	working = 1;
		//	}
		//}
		
		//if(working && RecorderFinished()) {
		//	Com_WriteByteArray(RecorderGetBuffer(), RecorderGetSize() * 2, &tx_call);
		//	working = 0;
		//}		
		
		__WFI();
	}
	
	return 0;
}

