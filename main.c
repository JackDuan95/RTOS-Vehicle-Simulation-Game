#include <LPC17xx.h>
#include <RTL.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "GLCD.h"
#include "KBD.h"
// Reference for initializing periherals: https://ece.uwaterloo.ca/~dwharder/icsrts/Keil_board/IO_examples/src/io_example_project.zip												 

int cars_present;//When this is equal to zero, user should be able to move on to next level			
int level;	
int health;
bool got_powerup = 0;
int level_complete;
const unsigned char ledPosArray[8] = { 28, 29, 31, 2, 3, 4, 5, 6 };
uint32_t X_val_car;
uint32_t Y_val_car;
uint32_t X_val_obstacle_car[15];
uint32_t Y_val_obstacle_car[15];
uint32_t x_val_powerup[1];
uint32_t y_val_powerup[1];
bool destroy[15];

volatile unsigned short car_width = 20;
volatile unsigned short car_height = 50;
volatile unsigned short road_lines_width = 5;
volatile unsigned short road_lines_height = 40;
volatile unsigned short solid_lines_width = 5;
volatile unsigned short solid_lines_height = 320;
unsigned short car_draw[20][50]={{Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red},
															{Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red},
															{Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Black, Black, Red, Red, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Red, Red, Red, Red, Red, Red, Red, Red, Black, Black, Black, Black, Black, Red, Red, Red, Red, Red, Red, Red, Red, Red, Yellow, Yellow},
															{Red, Red, Red, Red, Red, Red, Red, Red, Black, Black, Black, Black, Red, Red, Red, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Red, Red, Red, Red, Red, Red, Black, Black, Black, Black, Black, Black, Black, Black, Red, Red, Red, Red, Red, Red, Red, Red, Red, Yellow, Yellow},
															{Red, Red, Red, Red, Red, Red, Black, Black, Black, Black, Black, Black, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Red, Red, Red, Red, Red, Red, Red, Red, Red, Yellow, Yellow},
															{Red, Red, Red, Red, Black, Black, Black, Black, Black, Black, Black, Black, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red},
															{Red, Red, Red, Red, Black, Black, Black, Black, Black, Black, Black, Black, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red},
															{Red, Red, Red, Red, Black, Black, Black, Black, Black, Black, Black, Black, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red},
															{Red, Red, Red, Red, Black, Black, Black, Black, Black, Black, Black, Black, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red},
															{Red, Red, Red, Red, Black, Black, Black, Black, Black, Black, Black, Black, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red},
															{Red, Red, Red, Red, Black, Black, Black, Black, Black, Black, Black, Black, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red},
															{Red, Red, Red, Red, Black, Black, Black, Black, Black, Black, Black, Black, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red},
															{Red, Red, Red, Red, Black, Black, Black, Black, Black, Black, Black, Black, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red},
															{Red, Red, Red, Red, Black, Black, Black, Black, Black, Black, Black, Black, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red},
															{Red, Red, Red, Red, Black, Black, Black, Black, Black, Black, Black, Black, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red},
															{Red, Red, Red, Red, Red, Red, Black, Black, Black, Black, Black, Black, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Red, Red, Red, Red, Red, Red, Red, Red, Red, Yellow, Yellow},
															{Red, Red, Red, Red, Red, Red, Red, Red, Black, Black, Black, Black, Red, Red, Red, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Red, Red, Red, Red, Red, Red, Black, Black, Black, Black, Black, Black, Black, Black, Red, Red, Red, Red, Red, Red, Red, Red, Red, Yellow, Yellow},
															{Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Black, Black, Red, Red, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Red, Red, Red, Red, Red, Red, Red, Red, Black, Black, Black, Black, Black, Red, Red, Red, Red, Red, Red, Red, Red, Red, Yellow, Yellow},
															{Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Black, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red},
															{Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red, Red}};

unsigned short obstacle_car_draw[20][50];
unsigned short power_up_draw [20][20] = {{DarkGreen,DarkGreen,DarkGreen,DarkGreen,DarkGreen,DarkGreen,DarkGreen,DarkGreen,DarkGreen,DarkGreen,DarkGreen,DarkGreen,DarkGreen,DarkGreen,DarkGreen,DarkGreen,DarkGreen,DarkGreen,DarkGreen,DarkGreen},
																				{DarkGreen,DarkGreen,DarkGreen,DarkGreen,Red,Red,Red,DarkGreen,DarkGreen,DarkGreen,DarkGreen,DarkGreen,DarkGreen,Red,Red,Red,DarkGreen,DarkGreen,DarkGreen,DarkGreen},
																				{DarkGreen,DarkGreen,Red,Red,Red,Red,Red,Red,Red,DarkGreen,DarkGreen,Red,Red,Red,Red,Red,Red,Red,DarkGreen,DarkGreen},
																				{DarkGreen,Red,Red,Red,Red,Red,Red,Red,Red,Red,Red,Red,Red,Red,Red,Red,Red,Red,Red,DarkGreen},
																				{DarkGreen,Red,Red,Red,Red,Red,Red,Red,Red,Red,Red,Red,Red,Red,Red,Red,Red,Red,Red,DarkGreen},
																				{DarkGreen,Red,Red,Red,Red,Red,Red,Red,Red,Red,Red,Red,Red,Red,Red,Red,Red,Red,Red,DarkGreen},
																				{DarkGreen,Red,Red,Red,Red,Red,Red,Red,Red,Red,Red,Red,Red,Red,Red,Red,Red,Red,Red,Red},
																				{DarkGreen,Red,Red,Red,Red,Red,Red,Red,Red,Red,Red,Red,Red,Red,Red,Red,Red,Red,Red,Red},
																				{DarkGreen,Red,Red,Red,Red,Red,Red,Red,Red,Red,Red,Red,Red,Red,Red,Red,Red,Red,Red,DarkGreen},
																				{DarkGreen,Red,Red,Red,Red,Red,Red,Red,Red,Red,Red,Red,Red,Red,Red,Red,Red,Red,Red,DarkGreen},
																				{DarkGreen,Red,Red,Red,Red,Red,Red,Red,Red,Red,Red,Red,Red,Red,Red,Red,Red,Red,Red,DarkGreen},
																				{DarkGreen,DarkGreen,Red,Red,Red,Red,Red,Red,Red,Red,Red,Red,Red,Red,Red,Red,Red,Red,DarkGreen,DarkGreen},
																				{DarkGreen,DarkGreen,DarkGreen,Red,Red,Red,Red,Red,Red,Red,Red,Red,Red,Red,Red,Red,Red,DarkGreen,DarkGreen,DarkGreen},
																				{DarkGreen,DarkGreen,DarkGreen,DarkGreen,Red,Red,Red,Red,Red,Red,Red,Red,Red,Red,Red,Red,DarkGreen,DarkGreen,DarkGreen,DarkGreen},
																				{DarkGreen,DarkGreen,DarkGreen,DarkGreen,DarkGreen,Red,Red,Red,Red,Red,Red,Red,Red,Red,Red,DarkGreen,DarkGreen,DarkGreen,DarkGreen,DarkGreen},
																				{DarkGreen,DarkGreen,DarkGreen,DarkGreen,DarkGreen,DarkGreen,Red,Red,Red,Red,Red,Red,Red,Red,DarkGreen,DarkGreen,DarkGreen,DarkGreen,DarkGreen,DarkGreen},
																				{DarkGreen,DarkGreen,DarkGreen,DarkGreen,DarkGreen,DarkGreen,DarkGreen,Red,Red,Red,Red,Red,Red,DarkGreen,DarkGreen,DarkGreen,DarkGreen,DarkGreen,DarkGreen,DarkGreen},
																				{DarkGreen,DarkGreen,DarkGreen,DarkGreen,DarkGreen,DarkGreen,DarkGreen,DarkGreen,Red,Red,Red,Red,DarkGreen,DarkGreen,DarkGreen,DarkGreen,DarkGreen,DarkGreen,DarkGreen,DarkGreen},
																				{DarkGreen,DarkGreen,DarkGreen,DarkGreen,DarkGreen,DarkGreen,DarkGreen,DarkGreen,DarkGreen,Red,Red,DarkGreen,DarkGreen,DarkGreen,DarkGreen,DarkGreen,DarkGreen,DarkGreen,DarkGreen,DarkGreen},
																				{DarkGreen,DarkGreen,DarkGreen,DarkGreen,DarkGreen,DarkGreen,DarkGreen,DarkGreen,DarkGreen,DarkGreen,DarkGreen,DarkGreen,DarkGreen,DarkGreen,DarkGreen,DarkGreen,DarkGreen,DarkGreen,DarkGreen,DarkGreen}};
unsigned short power_up_erase [20][20];																
unsigned short road_lines[200], road_lines_erase[200], car_erase[1000], solid_lines[1600];
																
OS_SEM sem, sem2;
OS_TID task_car, task_power_ups, task_obstacles, task_moving_road;
																

//Led Initialization
void LEDInit( void ) 
{
	LPC_SC->PCONP     |= (1 << 15);            
	LPC_PINCON->PINSEL3 &= ~(0xCF00);
	LPC_PINCON->PINSEL4 &= (0xC00F);
	LPC_GPIO1->FIODIR |= 0xB0000000;           
	LPC_GPIO2->FIODIR |= 0x0000007C;           
}

// Turn on the LED in a position within 0..7
void turnOnLED( unsigned char led ) 
{
	unsigned int mask = (1 << ledPosArray[led]);
	if ( led < 3 ) 
		LPC_GPIO1->FIOSET |= mask;
	else 
		LPC_GPIO2->FIOSET |= mask;
}

// Turn off the LED in the position within 0..7
void turnOffLED( unsigned char led ) 
{
	unsigned int mask = (1 << ledPosArray[led]);
	if ( led < 3 ) 
		LPC_GPIO1->FIOCLR |= mask;
	else 
		LPC_GPIO2->FIOCLR |= mask;
}

//Initializing Joystick
void KBD_Init (void) 
{
 LPC_SC->PCONP |= (1 << 15); /* enable power to GPIO & IOCON */
/* P1.20, P1.23..26 is GPIO (Joystick) */
 LPC_PINCON->PINSEL3 &= ~((3<< 8)|(3<<14)|(3<<16)|(3<<18)|(3<<20));
/* P1.20, P1.23..26 is input */
 LPC_GPIO1->FIODIR &= ~((1<<20)|(1<<23)|(1<<24)|(1<<25)|(1<<26));
}

//Get Joystick Value(Part of get_button)
uint32_t KBD_get (void) 
{
 uint32_t kbd_val;
 kbd_val = (LPC_GPIO1->FIOPIN >> 20) & KBD_MASK;
 return (kbd_val);
}

//Get Joystick Value 
uint32_t get_button (void) 
{
 uint32_t val = 0;
 val = KBD_get(); /* read Joystick state */
 val = (~val & KBD_MASK); /* key pressed is read as a non '0' value*/
 return (val);
} 

//Initialize INT0
void INT0Init( void ) 
{
	LPC_PINCON->PINSEL4 &= ~(3<<20); 
	LPC_GPIO2->FIODIR   &= ~(1<<10); 
	LPC_GPIOINT->IO2IntEnF |= (1 << 10);
	NVIC_EnableIRQ( EINT3_IRQn );
}

// INT0 interrupt handler
void EINT3_IRQHandler( void ) {

	// Check whether the interrupt is called on the falling edge. GPIO Interrupt Status for Falling edge.
	if ( LPC_GPIOINT->IO2IntStatF && (0x01 << 10) ) {
		LPC_GPIOINT->IO2IntClr |= (1 << 10); // clear interrupt condition
	}
	
	if (level_complete == 1)//if int0 is pressed and level is complete
	{
		level_complete=0;
	}
	
}

void initializeDisplay(){
	int i;
	GLCD_Clear(DarkGreen);//make background green	
	for (i=0; i<2; i++)//drawing the two solid lines
	{
		GLCD_Bitmap(0,225*i+5,solid_lines_height,solid_lines_width, (unsigned char*)solid_lines);
	}
}

void initializeBitmaps(){
	int k, j, i;	
	
	//setting obstacle car bitmap	colours
	for (k=0; k<20; k++){
			for (j=0; j<50; j++){
				obstacle_car_draw[k][j]= car_draw[k][49-j];
				if(car_draw[k][49-j] == Red)
					obstacle_car_draw[k][j] = Blue;
			}
	}	
	
	//setting bitmap colours
	for (i=0; i<1000; i++){
		if(i<200){
			road_lines[i] = Black;
			road_lines_erase[i] = DarkGreen;
		}
		if(i<250){
			solid_lines[i] = Black;
		}		
		car_erase[i] = DarkGreen;
	}

	for( i =0; i<20; i++)
	{
		for(j = 0; j<20;j++)
		{	
			power_up_erase[i][j] = DarkGreen;
		}
	}	
}
__task void moving_road_lines(){
	int i;
	int j = 0;
	
	while(1)
	{
		os_sem_wait(sem, 0xffff);//erase and draw road lines when ready
		for (i=0; i<2; i++)
		{
			if (j<=8)
			{
				//erasing old road lines
				GLCD_Bitmap(40-5*j+5,70*i+80,road_lines_height,road_lines_width, (unsigned char*)road_lines_erase);
				GLCD_Bitmap(120-5*j+5,70*i+80,road_lines_height,road_lines_width, (unsigned char*)road_lines_erase);
				GLCD_Bitmap(200-5*j+5,70*i+80,road_lines_height,road_lines_width, (unsigned char*)road_lines_erase);
				GLCD_Bitmap(280-5*j+5,70*i+80,road_lines_height,road_lines_width, (unsigned char*)road_lines_erase);
				//drawing road lines
				GLCD_Bitmap(40-5*j,70*i+80,road_lines_height,road_lines_width, (unsigned char*)road_lines);
				GLCD_Bitmap(120-5*j,70*i+80,road_lines_height,road_lines_width, (unsigned char*)road_lines);
				GLCD_Bitmap(200-5*j,70*i+80,road_lines_height,road_lines_width, (unsigned char*)road_lines);
				GLCD_Bitmap(280-5*j,70*i+80,road_lines_height,road_lines_width, (unsigned char*)road_lines);
			}
			
			else
			{
				//erasing old road lines
				GLCD_Bitmap(0,70*i+80,road_lines_height+40-5*j+5,road_lines_width, (unsigned char*)road_lines_erase);
				GLCD_Bitmap(120-5*j+5,70*i+80,road_lines_height,road_lines_width, (unsigned char*)road_lines_erase);
				GLCD_Bitmap(200-5*j+5,70*i+80,road_lines_height,road_lines_width, (unsigned char*)road_lines_erase);
				GLCD_Bitmap(280-5*j+5,70*i+80,road_lines_height,road_lines_width, (unsigned char*)road_lines_erase);
				GLCD_Bitmap(360-5*j+5,70*i+80,road_lines_height,road_lines_width, (unsigned char*)road_lines_erase);
				//drawing road lines
				GLCD_Bitmap(0,70*i+80,road_lines_height+40-5*j,road_lines_width, (unsigned char*)road_lines);
				GLCD_Bitmap(120-5*j,70*i+80,road_lines_height,road_lines_width, (unsigned char*)road_lines);
				GLCD_Bitmap(200-5*j,70*i+80,road_lines_height,road_lines_width, (unsigned char*)road_lines);
				GLCD_Bitmap(280-5*j,70*i+80,road_lines_height,road_lines_width, (unsigned char*)road_lines);
				GLCD_Bitmap(360-5*j,70*i+80,road_lines_height,road_lines_width, (unsigned char*)road_lines);
			}
		}

		j++;
		
		if (j==16)//resetting j
		{
			GLCD_Bitmap(0,80,road_lines_height+40-5*j+5,road_lines_width, (unsigned char*)road_lines_erase);
			GLCD_Bitmap(0,150,road_lines_height+40-5*j+5,road_lines_width, (unsigned char*)road_lines_erase);			
			j=0;
		}
		
		GLCD_Bitmap (Y_val_car, X_val_car, car_height, car_width,(unsigned char*)car_draw);//redraw car here so that it doesn't appear car is getting erased by roadlines when user doesnt move car
		os_dly_wait(100);//time buffer
		os_sem_send(sem);
	}
}


__task void obstacles(){

	int i,j;
	int cars_start;
	cars_start = level*5;
	cars_present = cars_start;
	x_val_powerup[0] = 160;
	y_val_powerup[0] = 280;		
	for(j = 0; j<cars_start; j++)// set initial obstacle car locations
	{
			X_val_obstacle_car[j]=(j%3)*75+35;
			Y_val_obstacle_car[j]=320+j*(135-level*25);
			destroy[j]= false;
	}
	x_val_powerup[0] = (level%3)*75+35;
	y_val_powerup[0] = 280;
	
	while(1)
	{
		os_sem_wait(sem, 0xffff);//move obstacle cars when ready
		
		
		if(!got_powerup)
		{
			GLCD_Bitmap (y_val_powerup[0], x_val_powerup[0], 20, 20,(unsigned char*)power_up_draw);
		}
		
		
		for(i = 0; i<cars_start; i++)
		{
			if (!destroy[i])
			{
				
				if (Y_val_obstacle_car[i]<=320)
				{
					GLCD_Bitmap (Y_val_obstacle_car[i], X_val_obstacle_car[i], car_height, car_width,(unsigned char*)car_erase);
					Y_val_obstacle_car[i]-=5;//DON'T CHANGE
					if((((Y_val_obstacle_car[i]+20)/40)%2)==0)
					{
						X_val_obstacle_car[i]+=(2*level);
					}
					else
					{
						X_val_obstacle_car[i]-=(2*level);
					}
					GLCD_Bitmap (Y_val_obstacle_car[i], X_val_obstacle_car[i], car_height, car_width,(unsigned char*)obstacle_car_draw);
					
	
					if (!destroy[i] && (X_val_car<(X_val_obstacle_car[i]+20)) && ((X_val_car+20)>X_val_obstacle_car[i]) && (Y_val_car<(Y_val_obstacle_car[i]+50)) && ((Y_val_car+50)>Y_val_obstacle_car[i]))//when user hits obstacle car
					{
						health-=1;
						turnOffLED(health);
					
						if (health == 0)//game over
						{
							while (1)
							{
								GLCD_Clear(White);
								printf ("GAME OVER");
								os_sem_wait(sem, 0xffff);//delete tasks when ready
								os_tsk_delete(task_car);
								os_tsk_delete(task_moving_road);
								os_sem_send(sem);
								os_tsk_delete_self();
							}
						}
					
						GLCD_Bitmap (Y_val_obstacle_car[i], X_val_obstacle_car[i], car_height, car_width,(unsigned char*)car_erase);
						destroy[i] = true;
						cars_present-=1;
					}	
					
					if (Y_val_obstacle_car[i]<=0 && !destroy[i])//when obstacle car hits bottom of screen
					{
						x_val_powerup[0] = ((level+1)%3)*75+35;
						y_val_powerup[0] = 280;
						GLCD_Bitmap (Y_val_obstacle_car[i], X_val_obstacle_car[i], car_height, car_width,(unsigned char*)car_erase);
						destroy[i] = true;	
						cars_present-=1;
					}	
				}
				
				else
				{
					Y_val_obstacle_car[i]-=5;//DON'T CHANGE
				}					
			}
			
			else;
		// Checking for collision with powerup
		if (!got_powerup && (X_val_car<(x_val_powerup[0]+20)) && ((X_val_car+20)>x_val_powerup[0]) && (X_val_car<(x_val_powerup[0]+20)) && ((Y_val_car+50)>y_val_powerup[0]))
		{
				GLCD_Bitmap (y_val_powerup[0], x_val_powerup[0], 20, 20,(unsigned char*)power_up_erase);
				got_powerup = 1;
				// Handle Health
				health +=1;
				turnOnLED(health-1);
		}
		

		}
		if(!got_powerup)
		{
			GLCD_Bitmap (y_val_powerup[0], x_val_powerup[0], 20, 20,(unsigned char*)power_up_erase);
			y_val_powerup[0] -=cars_start;
			GLCD_Bitmap (y_val_powerup[0], x_val_powerup[0], 20, 20,(unsigned char*)power_up_draw);
		}
	  if( y_val_powerup[0] < 25)
		{
			got_powerup = 1;
			GLCD_Bitmap (y_val_powerup[0], x_val_powerup[0], 20, 20,(unsigned char*)power_up_erase);
		}	
		

		if (cars_present == 0)//beat the level
		{
				if (level == 3)//just beat the last level
				{
					while (1)
					{
						GLCD_Clear(White);
						printf ("YOU WIN\n SCORE: %d", health);
						os_sem_wait(sem, 0xffff);//delete task when ready
						os_tsk_delete(task_car);
						os_tsk_delete(task_moving_road);
						os_sem_send(sem);
						os_tsk_delete_self();
					}
				}
				
				else//move on to next level
				{
					level+=1;
					got_powerup = 0;
					health+=1;//reward level complete by giving 1 extra health
					turnOnLED(health-1);
					GLCD_Bitmap (Y_val_car, X_val_car, car_height, car_width, (unsigned char*)car_erase);			
					X_val_car=110;//reset car X value
					Y_val_car=60;//reset car Y value
					cars_start = level*5;
					cars_present = cars_start;
					
					for(j = 0; j<cars_start; j++)// set initial obstacle car locations
					{
							X_val_obstacle_car[j]=(j%3)*75+35;
							Y_val_obstacle_car[j]=320+j*120;
							destroy[j]= false;
					}	
					
					level_complete=1;
					GLCD_Clear(White);
					printf ("Press INT0 for LEVEL: %d", level);
					while(level_complete==1);
					initializeDisplay();
				}
					
		}
		
		os_dly_wait(100);//time buffer
		os_sem_send(sem);
	}
}


__task void car_control(){
	
	int i;
	while (1)
	{
		i = get_button();

		os_sem_wait(sem, 0xffff);//moves car when ready 
		
		if (i == 0x08 && X_val_car>10)// car moving left
		{
			GLCD_Bitmap (Y_val_car, X_val_car, car_height, car_width, (unsigned char*)car_erase);
			X_val_car-=10;
		}
		else if (i == 0x20 && X_val_car<210)// car moving right
		{
			GLCD_Bitmap (Y_val_car, X_val_car, car_height, car_width, (unsigned char*)car_erase);
			X_val_car+=10;	
		}
		else if (i == 0x40 && Y_val_car>0)// car moving down
		{
			GLCD_Bitmap (Y_val_car, X_val_car, car_height, car_width, (unsigned char*)car_erase);
			Y_val_car-=10;	
		}
		else if (i == 0x10 && Y_val_car<270)// car moving up
		{
			GLCD_Bitmap (Y_val_car, X_val_car, car_height, car_width, (unsigned char*)car_erase);
			Y_val_car+=10;
		}

		os_dly_wait(100);//time buffer
		os_sem_send(sem);//
	}
}

 __task void base_task( void ){
	
	os_sem_init(sem, 1);//initialize semaphore with inital token 1(able to use right away)

	task_moving_road=os_tsk_create(moving_road_lines,1);//create moving road task of priority 1
	
	task_obstacles=os_tsk_create(obstacles,1);//create obstacles task of priority 1
	
	task_car=os_tsk_create(car_control,1);//create car task of priority 1
	
	while (1);//loops forever
	//os_dly_wait(10000);
}

 int main( void ){
	int i;
	SystemInit();
	LEDInit();
	health=3;//set health to 3
	for (i=0; i<health; i++)
	{
		turnOnLED(i);
	}
	INT0Init();
  GLCD_Init();
	initializeDisplay();
	initializeBitmaps();
	level=1;//set level to 1
	level_complete=0;//level not complete
	X_val_car=110;//initialize car X value
	Y_val_car=60;//initialize car Y value
	os_sys_init(base_task);
}


// Neutral: 511
// Down: 511
// Left: 503
// Right: 479
// Down: 447
// Up: 495