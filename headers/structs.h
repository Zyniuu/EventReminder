#ifndef __STRUCTS_H
#define __STRUCTS_H
#include <gtk/gtk.h>


// Struktura przechowujaca informacje o wydarzeniu
struct _event
{
    char title[40];
    time_t start_of_event;
    int num;
    char multiplier;
    char description[2048];
    int isDone;
    GtkWidget *eventButton;
    struct _event *next;
};
// Struktura, ktora bedzie podawana jako argument roznych funkcji programu
struct _windowData
{
    GtkImage *plusImage;
    GtkImage *pencilImage;
    GtkWidget *mainWin;
    GtkWidget *addEditWin;
    GtkWidget *askWin;
    GtkWidget *currentButton;
    GtkBuilder *mainBuilder;
    GtkBuilder *addEditBuilder;
    struct _event **head;
    struct _event *current;
    struct _event *currentLoopEvent;
};

#endif
