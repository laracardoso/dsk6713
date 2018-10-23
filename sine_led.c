/**
 * Lara Cardoso Figueirêdo
 * Teste de programa padrão: gerar uma senoide!
 * 30.08.2018
 **/

#include "dsk6713.h"
#include "dsk6713_led.h"
#include "dsk6713_aic23.h"
#include "dsk6713_flash.h"
#include "dsk6713_dip.h"

#define CHIP_6713 1
#define DSK6713_AIC23_INPUT_MIC 0x0015
#define DSK6713_AIC23_INPUT_LINE 0x0011
#define LOOPLENGTH 8
#define BUF_SIZE 44000*5           // this sets length of delay

void comm_poll();
void output_left_sample(short out_data);
void output_right_sample(short out_data);

//Uint32 fs = DSK6713_AIC23_FREQ_8KHZ;   //set sampling rate
Uint32 fs = DSK6713_AIC23_FREQ_44KHZ;
Uint16 inputsource = DSK6713_AIC23_INPUT_LINE;  //select input

short loopindex = 0; //table index
short gain = 2;                      //gain factor
short sine_table[LOOPLENGTH]={0,707,1000,707,0,-707,-1000,-707}; //sine values
short buffer[BUF_SIZE];         // storage for previous samples
short input,output,delayed;
int i = 0;
int test = 1;

void main(){

  comm_poll();
  DSK6713_init();                        //init DSK,codec,McBSP
  DSK6713_LED_init();                 //init LED from BSL
  DSK6713_DIP_init();                 //init DIP from BSL
  while(1)                            //Loop
  {
    if(DSK6713_DIP_get(0)==0)         //=0 if DIP switch #0 pressed
    {
      //DSK6713_LED_on(0);               //turn LED #0 ON
	  input = sine_table[loopindex];  // read new input sample from ADC
	  delayed = buffer[i];          // read delayed value from buffer
	  output = input*gain*test + delayed;     // output sum of input and delayed values
	  output_left_sample(output);
	  output_right_sample(output);
	  buffer[i] = input*gain + delayed*0.8;
	  i++;
	  loopindex++;
	  
	  if(i >= BUF_SIZE) {
		DSK6713_LED_on(0);
		i=0;
		buffer[i] = 0;
	  }

      if (loopindex >= LOOPLENGTH) {
		test = 0;
		loopindex = 0; //reset table index
      }
    }
    else DSK6713_LED_off(0);          //turn LED off if not pressed
  }                                   //end of while(1) infinite loop

}                                     //end of main
