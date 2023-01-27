/* Funkcje wchodzace w interakcje z interfejsem graficznym */

#ifndef __GUI_C
#define __GUI_C
#include "funcs.h"


// Pobranie i zaimplementowanie pliku .css dla styli okien
void load_css(char *filename);

// zamkniecie wybranego okna
void close_window(GtkButton *button, gpointer user_data);

// Uniemozliwienie interakcji z oknem podrzednym
void set_transient(GtkWidget *parentWindow, GtkWidget *newWindow);

// Wyswietlenie na ekranie informacji o bledzie
void create_warning_message(char *msg, GtkWidget *parentWindow);

// Wyswietlenie okna formularza z danymi potrzebnymi do utworzenia wydarzenia
void create_event_form(GtkButton *button, gpointer user_data);

// Pobranie danych wpisanych przez uzytkownika w formularzu
void get_form_data(GtkBuilder *windowBuilder, const char **titleText, char **comboText,
                   char **text, GtkTextIter *start, GtkTextIter *end, guint *year,
                   guint *month, guint *day, guint *hour, guint *minute, guint *reminder);

// Dodanie wydarzenia do listy
void add_event(GtkWidget *button, gpointer user_data);

// Edycja wydarzenia z listy
void edit_event(GtkWidget *button, gpointer user_data);

// Dodanie graficznej reprezentacji wydarzenia w postaci przycisku
void add_event_button(const char *title, gpointer user_data);

// Pokazanie informacji zawartych w wydarzeniu w postaci etykiet
void show_current_event(GtkBuilder *mainBuilder, struct _event *current);

// Zaktualizowanie wskaznika przechowuj¹cego aktualnie przegladane wydarzenie
void update_current_event(GtkButton *button, gpointer user_data);

// Zaladowanie wydarzen z pliku
void load_events_from_file(gpointer user_data);

// Utworzenie okna z zapytaniem o usuniecie wydarzenia
void create_delete_window(GtkButton *button, gpointer user_data);

// Usuniecie wydarzenia
void delete_event(GtkButton *button, gpointer user_data);

// Utworzenie okna powiadomienia
void create_reminder_window(gpointer user_data);

// Wyswietlanie powiadomien dla wydarzen
gboolean check_time_of_events(gpointer user_data);

#endif
