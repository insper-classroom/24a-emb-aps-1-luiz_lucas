#ifndef SONS_H
#define SONS_H

void som(int freq, int tempo, int pino){
  int periodo = 1000000/freq;
  for (int i=0; i<tempo* 1000 / periodo;i++){
    gpio_put(pino,1);
    sleep_us(periodo/2);
    gpio_put(pino,0);
    sleep_us(periodo/2);
  }
}

void playStartupMusic(int buzzpin) {
  int melody[] = {262+200, 330+200, 392+200, 523+200, 392+200, 330+200, 262+200, 196+200};
  int tempo[] = {200, 200, 200, 200, 200, 200, 200, 200}; 

  for (int i = 0; i < sizeof(melody)-1 / sizeof(melody[0]); i++) {
    som(melody[i], tempo[i], buzzpin);
  }

}

void looseSound(int buzzpin, int ledr, int ledg, int ledb, int ledy) {
    int freqs_loose[] = {1000, 800, 600, 400};
    int tempos_loose[] = {200, 200, 200, 400};
    int leds[] = {ledy, ledb, ledg, ledr};
    int num_sons = sizeof(freqs_loose) / sizeof(freqs_loose[0]);

    for (int i = 0; i < num_sons; i++) {
      gpio_put(leds[i], 1);
      som(freqs_loose[i], tempos_loose[i], buzzpin);
      busy_wait_ms(100); 
    }
    for (int i = 0; i < num_sons; i++) {
      gpio_put(leds[i], 0);
    }
    busy_wait_ms(200); 
}

void startSound(int buzzpin, int ledr, int ledg, int ledb, int ledy) {
  int freqs[] = {1500, 1500, 1500};
  int tempos[] = {220, 220, 220};
  int leds[] = {ledr, ledg, ledb, ledy};

  // Contagem regressiva
  for (int i = 0; i < 3 ; i++) {
    gpio_put(leds[i], 1);
    som(freqs[i], tempos[i], buzzpin);
    busy_wait_ms(400); // Atraso de 500ms entre cada som
  }
  // Som para o início da corrida
  gpio_put(leds[3], 1);
  som(2300, 800, buzzpin);
  for (int i = 0; i < 4; i++) {
    gpio_put(leds[i], 0);
  }
  busy_wait_ms(400);
}

void changeModeSound(int buzzpin, int ledr, int ledg, int ledb, int ledy){
  gpio_put(ledr,1);
  gpio_put(ledy,1);
  som(1000, 200, buzzpin); // Primeira parte do som
  busy_wait_ms(100); // Pausa de 100ms entre as partes do som
  gpio_put(ledg,1);
  gpio_put(ledb,1);
  som(1500, 200, buzzpin); // Segunda parte do som
  gpio_put(ledr,0);
  gpio_put(ledy,0);
  gpio_put(ledg,0);
  gpio_put(ledb,0);
}

void pointsCountingSound(int pontos, int buzzpin, int ledr, int ledg, int ledb, int ledy, int ledi){
  int freq_inicial = 400; 
  int freq_minima = 1000; 
  int tempo = 300; 

  int freq_atual = freq_inicial;

  for (int i = 0; i < pontos; i++) {
    gpio_put(ledr,1);
    gpio_put(ledy,1);
    gpio_put(ledg,1);
    gpio_put(ledb,1);
    som(freq_atual, tempo, buzzpin);
    gpio_put(ledr,0);
    gpio_put(ledy,0);
    gpio_put(ledg,0);
    gpio_put(ledb,0);
    busy_wait_ms(200);
    freq_atual += (freq_minima - freq_inicial) / pontos;
  }
  gpio_put(ledi,1);
}

void recordCounting(uint8_t pontos, int buzzpin, int ledr, int ledg, int ledb, int ledy, int ledi){
  int freq_inicial = 400; 
  int freq_minima = 1000; 
  int tempo = 300; 

  int freq_atual = freq_inicial;

  for (int i = 0; i < pontos; i++) {
    gpio_put(ledr,1);
    gpio_put(ledy,1);
    gpio_put(ledg,1);
    gpio_put(ledb,1);
    som(freq_atual, tempo, buzzpin);
    gpio_put(ledr,0);
    gpio_put(ledy,0);
    gpio_put(ledg,0);
    gpio_put(ledb,0);
    busy_wait_ms(200);
    freq_atual += (freq_minima - freq_inicial) / pontos;
  }
  gpio_put(ledi,1);
}

void resetRecordSound (int buzzpin, int ledr, int ledg, int ledb, int ledy){
  for (int i = 0; i < 3; i++){
    gpio_put(ledr,1);
    gpio_put(ledy,1);
    gpio_put(ledg,1);
    gpio_put(ledb,1);
    som(2000, 100, buzzpin);
    gpio_put(ledr,0);
    gpio_put(ledy,0);
    gpio_put(ledg,0);
    gpio_put(ledb,0);
    busy_wait_ms(100);
  }
}

#endif