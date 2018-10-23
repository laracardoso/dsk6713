
#define CHIP_6713 1

#include "dsk6713.h"
#include "dsk6713_led.h"
#include "dsk6713_aic23.h"
#include "dsk6713_flash.h"
#include "dsk6713_dip.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

Uint32 fs = DSK6713_AIC23_FREQ_8KHZ;          // set sampling rate
#define DSK6713_AIC23_INPUT_MIC 0x0015
#define DSK6713_AIC23_INPUT_LINE 0x0011
Uint16 inputsource = DSK6713_AIC23_INPUT_LINE; // select input



void comm_poll();
void output_left_sample(short out_data);
void output_right_sample(short out_data);

#define n 8
#define GAIN 0.8                // fraction (0 - 1) of output fed back
#define BUF_SIZE 2000           // this sets length of delay
short buffer[BUF_SIZE];         // storage for previous samples
short input,output,delayed;
int i = 0;                          // index into buffer

short loopindex = 0; //table index
short sine_table[n]={0,707,1000,707,0,-707,-1000,-707}; //sine values

void main()
{
      DSK6713_init();
      while(1){

          input = sine_table[n++];  // read new input sample from ADC
          delayed = buffer[i];          // read delayed value from buffer
          output = input + delayed;     // output sum of input and delayed values
          output_left_sample(output);
          output_right_sample(output);
          buffer[i] = input*2 + delayed*GAIN;
          i++;
          //loopindex++;

          if(++i >= BUF_SIZE) {
              i=0;
              buffer[i] = 0;
          }
       }
}

