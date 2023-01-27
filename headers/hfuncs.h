/* Funkcje pomocnicze */

#ifndef __HFUNCS_H
#define __HFUNCS_H
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdio.h>


// Konwersja 'stringu' do postaci time_t
time_t setupTime(char *date_time);

// Wyciaganie danych z linii pliku, wiedzac, ze jest on zapisany w odpowiednim formacie
void formatData(char *_string, char *title, time_t *time, int *num, char *multiplier, char *description);

#endif
