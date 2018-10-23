
#define CHIP_6713 1

#include "dsk6713.h"
#include "dsk6713_led.h"
#include "dsk6713_aic23.h"
#include "dsk6713_flash.h"
#include "dsk6713_dip.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

Uint32 fs = DSK6713_AIC23_FREQ_44KHZ;          // set sampling rate
#define DSK6713_AIC23_INPUT_MIC 0x0015
#define DSK6713_AIC23_INPUT_LINE 0x0011
Uint16 inputsource = DSK6713_AIC23_INPUT_LINE; // select input

void comm_poll();
void output_left_sample(short out_data);
void output_right_sample(short out_data);

#define a 8
#define gain 0.8            // fraction (0 - 1) of output fed back

double output[a];
int i = 1;
int n = 1;
double sine_table[a]={0,707,1000,707,0,-707,-1000,-707}; //sine values
int sr = 44100.f;
double delaytime = 1;
int d;

int elements = sizeof(sine_table)/sizeof(sine_table[0]);



void main()
{
      comm_poll();
      DSK6713_init();
      while(1){

        /*  for (n = 1; n < elements; n = n+1) {
          output[n] = sine_table[n] + gain*sine_table[n-i];

          }*/

      d = delaytime*sr;                         /* delaytime is the delay in secs */
      for(n=0; n < elements; n++){              /* we'll make the echo 1/2 amp */
          output[n] = sine_table[n] + (n >= d ? 0.5*sine_table[n-d] : 0.f);
          output_left_sample(output[n]*0.5);
          printf("Hello World!\n");
      }
    }
}

