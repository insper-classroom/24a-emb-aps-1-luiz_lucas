#ifndef SONS_H
#define SONS_H

void som(int freq, int tempo, int pino);

void looseSound(int buzzpin, int ledr, int ledg, int ledb, int ledy);

void startSound(int buzzpin, int ledr, int ledg, int ledb, int ledy);

void changeModeSound(int buzzpin, int ledr, int ledg, int ledb, int ledy);

void pointsCountingSound(int pontos, int buzzpin, int ledr, int ledg, int ledb, int ledy, int ledi);

void recordCounting(uint8_t pontos, int buzzpin, int ledr, int ledg, int ledb, int ledy, int ledi);

void resetRecordSound (int buzzpin, int ledr, int ledg, int ledb, int ledy);

#endif