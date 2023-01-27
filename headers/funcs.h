/* Funckje odpowiedzialne za 'logike' programu */

#ifndef __LOGIC_H
#define __LOGIC_H
#include "structs.h"
#include "hfuncs.h"


// Dodaj dane do listy
void push(struct _event **head, const char *title, char *date_time, int num, char multiplier, char *description);

// Usun dane z listy
void delete_node(struct _event **head, const char *title);

// Zapisz liste do pliku
void save_events(struct _event **head);

// Zaladuj dane z pliku i utworz liste
void load_events(struct _event **head);

// Szukaj elementu listy po podanym tytule
struct _event *search_node(struct _event **head, const char *title);

// Sprawdz czy nalezy przypomniec o wydarzeniu
int check_reminder(struct _event *node);

#endif
