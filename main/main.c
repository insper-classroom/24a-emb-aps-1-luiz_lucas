#include "hardware/gpio.h"
#include "pico/stdlib.h"
#include <stdio.h>

const int BTN_PIN_R = 20;
const int BTN_PIN_G = 26;
const int BTN_PIN_B = 18;
const int BTN_PIN_Y = 22;
const int BTN_PIN_T = 28;

const double jit=10;

const int LED_PIN_R = 8;
const int LED_PIN_G = 14;
const int LED_PIN_B = 11;
const int LED_PIN_Y = 10;

const int BUZZPIN = 6;

volatile int r=0;
volatile int g=0;
volatile int b=0;
volatile int y=0;
volatile int t=0;

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

// int random (int n) {
//    int i;
//    time_t t;
//    int *vetor = (int *)malloc(n * sizeof(int));
   
//    /* Intializes random number generator */
//    srand((unsigned) time(&t));

//    /* Print 5 random numbers from 0 to 49 */
//    for( i = 0 ; i < n ; i++ ) {
//       //printf("%d\n", rand() % 4);
//       vetor[i]=rand() % 4;
//    }
   
//    return(vetor);
// }

void btn_callback(uint gpio, uint32_t events) {
  if (gpio == BTN_PIN_R) r=1;
  if (gpio == BTN_PIN_G) g=1;
  if (gpio == BTN_PIN_B) b=1;
  if (gpio == BTN_PIN_Y) y=1;
  if (gpio == BTN_PIN_T) t=1;

}

int main() {
  stdio_init_all();
  pin_init();

  gpio_set_irq_enabled_with_callback(BTN_PIN_R, GPIO_IRQ_EDGE_FALL, true, &btn_callback);
  gpio_set_irq_enabled(BTN_PIN_G, GPIO_IRQ_EDGE_FALL, true);
  gpio_set_irq_enabled(BTN_PIN_B, GPIO_IRQ_EDGE_FALL, true);
  gpio_set_irq_enabled(BTN_PIN_Y, GPIO_IRQ_EDGE_FALL, true);
  gpio_set_irq_enabled(BTN_PIN_T, GPIO_IRQ_EDGE_RISE, true);


  while (true) {
    if (r){
      busy_wait_ms(jit);
      printf("apertouR\n");
      gpio_put(LED_PIN_R, 1);
      som(1900, 300, BUZZPIN);
      gpio_put(LED_PIN_R, 0);
      r=0;
    }
    if (g){
      busy_wait_ms(jit);
      printf("apertouG\n");
      gpio_put(LED_PIN_G, 1);
      som(2000, 300, BUZZPIN);
      gpio_put(LED_PIN_G, 0);
      g=0;
    }
    if (b){
      busy_wait_ms(jit);
      printf("apertouB\n");
      gpio_put(LED_PIN_B, 1);
      som(2100, 300, BUZZPIN);
      gpio_put(LED_PIN_B, 0);
      b=0;
    }
    if (y){
      busy_wait_ms(jit);
      printf("apertouY\n");
      gpio_put(LED_PIN_Y, 1);
      som(2200, 300, BUZZPIN);
      gpio_put(LED_PIN_Y, 0);
      y=0;
    }
    if (t){
      busy_wait_ms(jit);
      printf("apertouT\n");
      int a;
      int n=4;
      //for( int i = 0 ; i < n ; i++ ) {
        //printf("%d\n", rand() % 4);
      //}
      a=rand() % 4;
      t=0;
      if (a==0) r=1; 
      if (a==1) g=1;
      if (a==2) b=1;
      if (a==3) y=1;
    }
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

  gpio_init(BUZZPIN);
  gpio_set_dir(BUZZPIN, GPIO_OUT);
}

