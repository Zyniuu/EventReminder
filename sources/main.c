#include "../headers/gui.h"


int main(int argc, char **argv)
{
    gtk_init(&argc, &argv);
    // 'Glowa' linked listy
    struct _event *head = NULL;
    // Dane ktore beda przetwarzane podczas dzialania programu
    struct _windowData *data = (struct _windowData*)malloc(sizeof(struct _windowData));
    data->mainBuilder = gtk_builder_new_from_file("layouts/mainWindow.ui");
    data->mainWin = GTK_WIDGET(gtk_builder_get_object(data->mainBuilder, "window"));
    data->plusImage = GTK_IMAGE(gtk_builder_get_object(data->mainBuilder, "plusImage"));
    data->pencilImage = GTK_IMAGE(gtk_builder_get_object(data->mainBuilder, "pencilImage"));
    data->head = &head;
    data->current = NULL;
    data->currentButton = NULL;
    data->currentLoopEvent = NULL;
    // Pobranie elementow interfejsu z pliku z kodem xml
    GtkWidget *add = GTK_WIDGET(gtk_builder_get_object(data->mainBuilder, "add"));
    GtkWidget *edit = GTK_WIDGET(gtk_builder_get_object(data->mainBuilder, "edit"));
    GtkWidget *trash = GTK_WIDGET(gtk_builder_get_object(data->mainBuilder, "trash"));
    // Zaladowanie zapisanych wydarzen
    load_events_from_file(data);
    // Umozliwienie zamykania aplikacji
    g_signal_connect(data->mainWin, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    // Umozliwienie dodawania wydarzen
    g_signal_connect(add, "clicked", G_CALLBACK(create_event_form), data);
    // Umozliwienie edycji wydarzen
    g_signal_connect(edit, "clicked", G_CALLBACK(create_event_form), data);
    // Umozliwienie usuwania wydarzen
    g_signal_connect(trash, "clicked", G_CALLBACK(create_delete_window), data);
    // Ustawienie sprawdzania czasu wydarzen
    g_timeout_add(100, G_SOURCE_FUNC(check_time_of_events), data);
    // Zaladowanie plikow styli
    load_css("styles/main.css");
    load_css("styles/add.css");
    load_css("styles/warning.css");
    load_css("styles/ask.css");
    load_css("styles/reminder.css");
    // Wyswietlenie glownego okna
    gtk_widget_show_all(data->mainWin);
    gtk_main();
    free(data);
    return 0;
}
