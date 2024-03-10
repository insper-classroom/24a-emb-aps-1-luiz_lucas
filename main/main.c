#include "hardware/gpio.h"
#include "string.h"
#include "pico/stdlib.h"
#include <stdio.h>
#include "flash.h"
#include "flash.c"
#include "constantes.h"
#include "hardware/flash.h"
#include "hardware/sync.h"
#include "sons.h"
#include "sons.c"

#define FLASH_TARGET_OFFSET (1792*1024)                                                         //++ Starting Flash Storage location after 1.8MB ( of the 2MB )

volatile int r=0;
volatile int g=0;
volatile int b=0;
volatile int y=0;
volatile int t=0;
volatile int ingame=0;
volatile int reset_record=0;

void pin_init(void);

void led (int pino, int freq){
  gpio_put(pino, 1);
  som(freq, 500, BUZZPIN);
  gpio_put(pino, 0);
  sleep_ms(200);
}

/* void showPoints(uint8_t rodada){
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
 */

void confere(int botao, const int v[], int *nc, int *rodada, uint8_t *score, uint32_t flash_data[]){
  if (v[*nc]==botao){
    *nc=*nc+1;
  }else{
    printf("ERROU\n");
    if (*rodada>*score){
      *score=*rodada;
      flash_data[0]=*score-1;
      pico_flash_write(FLASH_TARGET_OFFSET, flash_data, 1);
    }
    *nc=0;

    looseSound(BUZZPIN, LED_PIN_R, LED_PIN_G, LED_PIN_B, LED_PIN_Y);
    pointsCountingSound(*rodada-1, BUZZPIN, LED_PIN_R, LED_PIN_G, LED_PIN_B, LED_PIN_Y, LED_PIN_I);
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
    } else {
      if (gpio == BTN_PIN_T) t=1;

      if (gpio == BTN_PIN_B) reset_record=1;
      if (gpio == BTN_PIN_G && reset_record==1) reset_record=2;
      if (gpio == BTN_PIN_Y && reset_record==2) reset_record=3;
      if (gpio == BTN_PIN_R && reset_record==3) reset_record=4;
    }

  } else if (events == 0x8) { // rise edge
    if (gpio == BTN_PIN_T) t=2;
  }
}

volatile bool t_fired = false;
int64_t alarm_callback(alarm_id_t id, void *user_data) {
  t_fired = true;
  // Can return a value here in us to fire in the future
  return 0;
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
  uint32_t t_press_duration, fall;
  uint32_t *a = (uint32_t*)malloc(1+1);
  a=pico_flash_read(FLASH_TARGET_OFFSET, 1); 
  score=a[0];
  int t_count = 0;
  while (true) {

    if (reset_record == 4) {
      reset_record = 0;
      t=0;
      t_count=0;
      //pico_flash_erase(FLASH_TARGET_OFFSET);
      flash_data[0]=0;
      score=0;
      pico_flash_write(FLASH_TARGET_OFFSET, flash_data, 1);
      printf("TUDO JUNTO\n");
      resetRecordSound(BUZZPIN, LED_PIN_R, LED_PIN_G, LED_PIN_B, LED_PIN_Y);
    }

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
    
    if (t==1){
      printf("APERTOU PRINTIPAL\n");
      busy_wait_ms(jit);
      //printf("apertouT1\n");
      t = 0;
      reset_record = 0;
      fall = to_ms_since_boot(get_absolute_time());
      t_count ++;
    } else if (t==2){ 
      busy_wait_ms(jit);
      //printf("soltouT1\n");
      t = 0;
      t_press_duration = to_ms_since_boot(get_absolute_time()) - fall;
      //printf("Duração: %u\n", t_press_duration);
      if (t_press_duration >= 1000 && t_count == 1){
        printf("Pressionado por um segundo - MOSTRA RECORDE\n");
        reset_record = 0;
        t_count = 0;
        a=pico_flash_read(FLASH_TARGET_OFFSET, 1); 
        //showPoints(a[1]);
        printf("RECORDE: %u\n", a[0]);
        recordCounting(a[0], BUZZPIN, LED_PIN_R, LED_PIN_G, LED_PIN_B, LED_PIN_Y, LED_PIN_I);


      } else if (t_press_duration < 200 && t_count == 1) {
        if (!add_alarm_in_ms(200, alarm_callback, NULL, false)) {
          printf("Failed to add timer\n");
        }
      }
    }

    if(t_fired){
      t_fired = 0;
      if (t_count == 2){
        printf("Pressionado duas vezes - MUDA O MODO\n");
        t = 0;
        t_count = 0;
        reset_record = 0;

        mode = !mode;
        changeModeSound(BUZZPIN, LED_PIN_R, LED_PIN_G, LED_PIN_B, LED_PIN_Y);

      } else {
        printf("Pressionado uma vez - COMEÇA O JOGO\n");
        t = 0;
        t_count=0;
        reset_record = 0;

        if (!ingame){
          /* for (int i=0;i<4;i++){
          led(LED_PIN_I, 2000);
          }
          sleep_ms(300); */
          gpio_put(LED_PIN_I,0);
          startSound(BUZZPIN, LED_PIN_R, LED_PIN_G, LED_PIN_B, LED_PIN_Y);
          sequencia(rodada, vetor, mode);
        }
      }
    }

      /* //printf("apertouT2\n");
      rise = to_ms_since_boot(get_absolute_time());

      printf("hold: %d\n",rise-fall);
      if (rise - fall > 500) {
        a=pico_flash_read(FLASH_TARGET_OFFSET, 1); 
        printf("RECORDE: %d\n",a[0]);
        gameover(a[0]);
        t=0;
      }
      else{
        busy_wait_ms(jit);
        if (!ingame){
          for (int i=0;i<4;i++){
          led(LED_PIN_I, 2000);
          }
          sleep_ms(300);
          sequencia(rodada, vetor, mode);
        }
      }
      
      t=0; */
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