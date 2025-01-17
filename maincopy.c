#include "hardware/gpio.h"
#include "string.h"
#include "pico/stdlib.h"
#include <stdio.h>
#include "flash.h"
#include "flash.c"
#include "constantes.h"
#include "hardware/flash.h"
#include "hardware/sync.h"


#define FLASH_TARGET_OFFSET (1792*1024)                                                         //++ Starting Flash Storage location after 1.8MB ( of the 2MB )

volatile int r=0;
volatile int g=0;
volatile int b=0;
volatile int y=0;
volatile int t=0;
volatile int click=0;

volatile int ingame=0;

void pin_init(void);



void som(int freq, int tempo, int pino){
  int periodo = 1000000/freq;
  for (int i=0; i<tempo* 1000 / periodo;i++){
    gpio_put(pino,1);
    sleep_us(periodo/2);
    gpio_put(pino,0);
    sleep_us(periodo/2);
  }
  
}

void led (int pino, int freq){
  gpio_put(pino, 1);
  som(freq, 500, BUZZPIN);
  gpio_put(pino, 0);
  sleep_ms(200);
}

void gameover(uint8_t rodada){
  printf("RODADA: %d\n", rodada);
  //som(3000, 500, BUZZPIN);
  for (int i=0; i<rodada-1;i++){
    gpio_put(LED_PIN_R,1);
    gpio_put(LED_PIN_B,1);
    gpio_put(LED_PIN_Y,1);
    gpio_put(LED_PIN_G,1);
    som(2000, 500, BUZZPIN);
    gpio_put(LED_PIN_R,0);
    gpio_put(LED_PIN_B,0);
    gpio_put(LED_PIN_Y,0);
    gpio_put(LED_PIN_G,0);
    //som(2000, 500, BUZZPIN);
    sleep_ms(200);
  }
  gpio_put(LED_PIN_I, 1);


}

void confere(int botao, const int v[], int *nc, int *rodada, uint8_t *score, uint32_t flash_data[]){
      

  if (v[*nc]==botao){
    *nc=*nc+1;
  }else{
    printf("ERROU\n");
    if (*rodada>*score){
      *score=*rodada;
      flash_data[0]=*score;
      pico_flash_write(FLASH_TARGET_OFFSET, flash_data, 1);
    }
    *nc=0;
    som(3000, 500, BUZZPIN);
    gameover(*rodada);
    *rodada=1;
    ingame=0;
    
  }

}

void sequencia (int n, int v[], int mode) {
   int i;
   time_t temp;
   srand((unsigned) time(&temp));
   if (!mode){
    v[n-1]=rand() % 4;
   }

   for( i = 0 ; i < n ; i++ ) {
    if (mode){
      v[i]=rand() % 4;
    }
      int a=v[i];
      if (a==0) led(LED_PIN_R,1900);
      if (a==1) led(LED_PIN_G,2000);
      if (a==2) led(LED_PIN_B,2100);
      if (a==3) led(LED_PIN_Y,2200);
   }
   ingame=1;
   return ;
}

void btn_callback(uint gpio, uint32_t events) {
  
  if (events == 0x4) { // fall edge
        if (ingame){
          ingame=1;
          if (gpio == BTN_PIN_R) r=1;
          if (gpio == BTN_PIN_G) g=1;
          if (gpio == BTN_PIN_B) b=1;
          if (gpio == BTN_PIN_Y) y=1;
        }
        if (gpio == BTN_PIN_T) t=1;

    } else if (events == 0x8) { // rise edge
        if (gpio == BTN_PIN_T) t=2;
        
    }
  
}

int main() {
  stdio_init_all();
  pin_init();


  int rodada=1;
  int nc=0;
  int vetor[100];
  int mode=0;
  uint8_t score = 0;
  uint32_t flash_data[FLASH_PAGE_SIZE];  

  //pico_flash_erase(FLASH_TARGET_OFFSET);                                                      //++ Flash operation to erase entire flash page ( 256 locations together )

  gpio_put(LED_PIN_I, 1);
  
  

  gpio_set_irq_enabled_with_callback(BTN_PIN_R, GPIO_IRQ_EDGE_FALL, true, &btn_callback);
  gpio_set_irq_enabled(BTN_PIN_G, GPIO_IRQ_EDGE_FALL, true);
  gpio_set_irq_enabled(BTN_PIN_B, GPIO_IRQ_EDGE_FALL, true);
  gpio_set_irq_enabled(BTN_PIN_Y, GPIO_IRQ_EDGE_FALL, true);
  gpio_set_irq_enabled(BTN_PIN_T, GPIO_IRQ_EDGE_FALL|GPIO_IRQ_EDGE_RISE, true);

  //scanf("mode (0,1): %d", &mode);
  uint32_t rise, click1, click2;
  click1=0;
  click2=0;
  int cop=0;
  int sub=0;

  uint32_t *a = (uint32_t*)malloc(1+1);
  a=pico_flash_read(FLASH_TARGET_OFFSET, 1); 
  score=a[0];

  while (true) {
    if (r){
      busy_wait_ms(jit);
      if (ingame){
        //printf("apertouR\n");
        led(LED_PIN_R,1900);
        confere(0, vetor, &nc, &rodada, &score, flash_data);
      }
      r=0;
    }
    if (g){
      busy_wait_ms(jit);
      if (ingame){
        //printf("apertouG\n");
        led(LED_PIN_G,2000);
        confere(1, vetor, &nc, &rodada, &score, flash_data);
      }
      g=0;
    }
    if (b){
      busy_wait_ms(jit);
      if (ingame){
        //printf("apertouB\n");
        led(LED_PIN_B,2100);
        confere(2, vetor, &nc, &rodada, &score, flash_data);
      }
      b=0;
    }
    if (y){
      busy_wait_ms(jit);
      if (ingame){
        //printf("apertouY\n");
        led(LED_PIN_Y, 2200);
        confere(3, vetor, &nc, &rodada, &score, flash_data);
      }
      y=0;
    }
    if (nc==rodada){
      rodada = rodada + 1;
      nc=0;
      ingame=0;
      sleep_ms(150);
      sequencia(rodada, vetor, mode);
      
    }
    if (to_ms_since_boot(get_absolute_time())-click1>200 && click1!=0 && sub){
      cop=1;
      t=1;
      sub=0;
      click1=0;
      click2=0;
    }

    if (t==1){
      t=0;
      if (click1==0)  {
      click1 = to_ms_since_boot(get_absolute_time());
      }
      else{
        click2 = to_ms_since_boot(get_absolute_time());
          if (click2-click1<200){
            printf("MUDOU %d\n", click2-click1);
            mode = !mode;
            click1=0;
            click2=0;
          }
      }
      if (cop){
        click1=0;
        click2=0;
        cop=0;
        if (!ingame){
          for (int i=0;i<4;i++){
          led(LED_PIN_I, 2000);
          }
          sleep_ms(300);
          sequencia(rodada, vetor, mode);
        }
      }

    }
    else if (t==2){ 
      //printf("apertouT2\n");
      sub=1;
      rise = to_ms_since_boot(get_absolute_time());

      printf("hold: %d\n",rise-click1);
      if (rise - click1 > 500) {
        a=pico_flash_read(FLASH_TARGET_OFFSET, 1); 
        printf("RECORDE: %d\n",a[0]);
        gameover(a[0]);
        click1=0;
        sub=0;
      }
      t=0;
    }

    // if (t==1){
    //   //printf("apertouT1\n");
    //   t = 0;
    //   fall = to_ms_since_boot(get_absolute_time());
    // }
    // else if (t==2){ 
    //   //printf("apertouT2\n");
    //   rise = to_ms_since_boot(get_absolute_time());

    //   printf("hold: %d\n",rise-fall);
    //   if (rise - fall > 500) {
    //     a=pico_flash_read(FLASH_TARGET_OFFSET, 1); 
    //     printf("RECORDE: %d\n",a[0]);
    //     gameover(a[0]);
    //     t=0;
    //   }
    //   else{
    //     busy_wait_ms(jit);
    //     if (!ingame){
    //       for (int i=0;i<4;i++){
    //       led(LED_PIN_I, 2000);
    //       }
    //       sleep_ms(300);
    //       sequencia(rodada, vetor, mode);
    //     }
    //   }
      
    //   t=0;
    // }
  }
}

void pin_init(void) {
  gpio_init(BTN_PIN_R);
  gpio_set_dir(BTN_PIN_R, GPIO_IN);
  gpio_pull_up(BTN_PIN_R);

  gpio_init(BTN_PIN_G);
  gpio_set_dir(BTN_PIN_G, GPIO_IN);
  gpio_pull_up(BTN_PIN_G);

  gpio_init(BTN_PIN_B);
  gpio_set_dir(BTN_PIN_B, GPIO_IN);
  gpio_pull_up(BTN_PIN_B);

  gpio_init(BTN_PIN_Y);
  gpio_set_dir(BTN_PIN_Y, GPIO_IN);
  gpio_pull_up(BTN_PIN_Y);

  gpio_init(BTN_PIN_T);
  gpio_set_dir(BTN_PIN_T, GPIO_IN);
  gpio_pull_up(BTN_PIN_T);

  gpio_init(LED_PIN_B);
  gpio_set_dir(LED_PIN_B, GPIO_OUT);

  gpio_init(LED_PIN_G);
  gpio_set_dir(LED_PIN_G, GPIO_OUT);

  gpio_init(LED_PIN_R);
  gpio_set_dir(LED_PIN_R, GPIO_OUT);

  gpio_init(LED_PIN_Y);
  gpio_set_dir(LED_PIN_Y, GPIO_OUT);

  gpio_init(LED_PIN_I);
  gpio_set_dir(LED_PIN_I, GPIO_OUT);

  gpio_init(BUZZPIN);
  gpio_set_dir(BUZZPIN, GPIO_OUT);
}
