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

void comm_poll();
void output_left_sample(short out_data);
void output_right_sample(short out_data);

Uint32 fs = DSK6713_AIC23_FREQ_8KHZ;   //set sampling rate
Uint16 inputsource = DSK6713_AIC23_INPUT_LINE;  //select input

short loopindex = 0; //table index
short gain = 2;                      //gain factor
short sine_table[LOOPLENGTH]={0,707,1000,707,0,-707,-1000,-707}; //sine values

void main(){

  comm_poll();
  DSK6713_init();                        //init DSK,codec,McBSP
  DSK6713_LED_init();                 //init LED from BSL
  DSK6713_DIP_init();                 //init DIP from BSL
  while(1)                            //Loop
  {
    if(DSK6713_DIP_get(0)==0)         //=0 if DIP switch #0 pressed
    {
      DSK6713_LED_on(0);               //turn LED #0 ON
      output_left_sample(sine_table[loopindex++]*gain); //output sample
      output_right_sample(sine_table[loopindex++]*gain);

      if (loopindex >= LOOPLENGTH) {
          loopindex = 0; //reset table index
      }
    }
    else DSK6713_LED_off(0);          //turn LED off if not pressed
  }                                   //end of while(1) infinite loop

}                                     //end of main
