#include "../headers/gui.h"


void load_css(char *filename)
{
    // Za pomoca 'providera' pobierz wskazany plik .css i go zaimplementuj
    GtkCssProvider *provider = gtk_css_provider_new();

    gtk_css_provider_load_from_path(provider, filename, NULL);
    gtk_style_context_add_provider_for_screen(gdk_screen_get_default(), GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    g_object_unref(provider);
}

void close_window(GtkButton *button, gpointer user_data)
{
    // Usun w³asciwosc uniemozliwiajaca interakcje z pozostalymi oknami
    gtk_window_set_modal(GTK_WINDOW(user_data), FALSE);
    // Usun okno
    gtk_widget_destroy(user_data);
    // Zwolnij pamiec, w ktorej przechowywane bylo okno
    g_object_unref(user_data);
}

void set_transient(GtkWidget *parentWindow, GtkWidget *newWindow)
{
    // Ustaw relacje rodzic - dziecko dla dwoch okien
    gtk_window_set_transient_for(GTK_WINDOW(newWindow), GTK_WINDOW(parentWindow));
    // Umozliw interkcje tylko z oknem, ktore znajduje sie 'najwyzej' (czyli dla 'dziecka')
    gtk_window_set_modal(GTK_WINDOW(newWindow), TRUE);
}

void create_warning_message(char *msg, GtkWidget *parentWindow)
{
    // Z pliku z kodem xml pobierz potrzebne obiekty interfejsu
    GtkBuilder *warningWindowBuilder = gtk_builder_new_from_file("layouts/warningWindow.ui");
    GtkWidget *warningWindow = GTK_WIDGET(gtk_builder_get_object(warningWindowBuilder, "warningWindow"));
    GtkWidget *message = GTK_WIDGET(gtk_builder_get_object(warningWindowBuilder, "warningMessage"));
    GtkWidget *okBtn = GTK_WIDGET(gtk_builder_get_object(warningWindowBuilder, "okBtn"));
    // Ustaw okno jako 'najwyzsze'
    set_transient(parentWindow, warningWindow);
    // Wypisz na oknie wiadomosc
    gtk_label_set_text(GTK_LABEL(message), msg);
    // Po nacisnieciu przycisku 'OK' zamknij okno
    g_signal_connect(okBtn, "clicked", G_CALLBACK(close_window), warningWindow);
    gtk_widget_show_all(warningWindow);
}

void create_event_form(GtkButton *button, gpointer user_data)
{
    struct _windowData *data = user_data;
    GtkImage *img = GTK_IMAGE(gtk_button_get_image(button));
    // Jezeli nacisniety zostal przycisk 'dodaj', wyswietl formularz do dodania wydarzenia
    if (img == data->plusImage)
    {
        // Z pliku z kodem xml pobierz potrzebne obiekty interfejsu i wyswielt formularz
        data->addEditBuilder = gtk_builder_new_from_file("layouts/addEditWindow.ui");
        data->addEditWin = GTK_WIDGET(gtk_builder_get_object(data->addEditBuilder, "addEditWindow"));
        GtkWidget *cancelBtn = GTK_WIDGET(gtk_builder_get_object(data->addEditBuilder, "cancelBtn"));
        GtkWidget *addEditBtn = GTK_WIDGET(gtk_builder_get_object(data->addEditBuilder, "addEditBtn"));
        GtkWidget *addEditTitle = GTK_WIDGET(gtk_builder_get_object(data->addEditBuilder, "addEditTitle"));
        // Ustaw odpowiednie elemnty interfejsu
        gtk_label_set_text(GTK_LABEL(addEditTitle), "Dodaj wydarzenie");
        gtk_button_set_label(GTK_BUTTON(addEditBtn), "Dodaj");
        // Ustaw okno jako 'najwyzsze'
        set_transient(data->mainWin, data->addEditWin);
        // Po nacisnieciu przycisku 'Anuluj' zamknij okno
        g_signal_connect(cancelBtn, "clicked", G_CALLBACK(close_window), data->addEditWin);
        // Po nacisnieciu przycisku 'Dodaj' zacznij procedure dodawania wydarzenia
        g_signal_connect(addEditBtn, "clicked", G_CALLBACK(add_event), data);
        gtk_widget_show_all(data->addEditWin);
    }
    // Jezeli zostal nacisniety przycisk 'edytuj' wyswietl formularz do edycji wydarzenia
    // i tylko jezeli jakies wydarzenie istnieje
    else if (img == data->pencilImage && *data->head)
    {
        struct tm *info;
        // Z pliku z kodem xml pobierz potrzebne obiekty interfejsu i wyswielt formularz
        data->addEditBuilder = gtk_builder_new_from_file("layouts/addEditWindow.ui");
        data->addEditWin = GTK_WIDGET(gtk_builder_get_object(data->addEditBuilder, "addEditWindow"));
        GtkWidget *cancelBtn = GTK_WIDGET(gtk_builder_get_object(data->addEditBuilder, "cancelBtn"));
        GtkWidget *addEditBtn = GTK_WIDGET(gtk_builder_get_object(data->addEditBuilder, "addEditBtn"));
        GtkWidget *addEditTitle = GTK_WIDGET(gtk_builder_get_object(data->addEditBuilder, "addEditTitle"));
        GtkWidget *titleEntryAddEdit = GTK_WIDGET(gtk_builder_get_object(data->addEditBuilder, "titleEntryAddEdit"));
        GtkWidget *calendarAddEdit = GTK_WIDGET(gtk_builder_get_object(data->addEditBuilder, "calendarAddEdit"));
        GtkWidget *hourAddEdit = GTK_WIDGET(gtk_builder_get_object(data->addEditBuilder, "hourAddEdit"));
        GtkWidget *minuteAddEdit = GTK_WIDGET(gtk_builder_get_object(data->addEditBuilder, "minuteAddEdit"));
        GtkWidget *reminderAddEdit = GTK_WIDGET(gtk_builder_get_object(data->addEditBuilder, "reminderAddEdit"));
        GtkWidget *textAddEdit = GTK_WIDGET(gtk_builder_get_object(data->addEditBuilder, "textAddEdit"));
        GtkTextBuffer *buff = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textAddEdit));
        // Ustaw odpowiednie elemnty interfejsu
        gtk_label_set_text(GTK_LABEL(addEditTitle), "Edytuj wydarzenie");
        gtk_button_set_label(GTK_BUTTON(addEditBtn), "Edytuj");
        info = localtime(&(data->current->start_of_event));
        gtk_entry_set_text(GTK_ENTRY(titleEntryAddEdit), data->current->title);
        gtk_calendar_select_month(GTK_CALENDAR(calendarAddEdit), info->tm_mon, info->tm_year + 1900);
        gtk_calendar_select_day(GTK_CALENDAR(calendarAddEdit), info->tm_mday);
        gtk_spin_button_set_value(GTK_SPIN_BUTTON(hourAddEdit), (double)info->tm_hour);
        gtk_spin_button_set_value(GTK_SPIN_BUTTON(minuteAddEdit), (double)info->tm_min);
        gtk_spin_button_set_value(GTK_SPIN_BUTTON(reminderAddEdit), (double)data->current->num);
        gtk_text_buffer_set_text(buff, data->current->description, strlen(data->current->description));
        // Ustaw okno jako 'najwyzsze'
        set_transient(data->mainWin, data->addEditWin);
        // Po nacisnieciu przycisku 'Anuluj' zamknij okno
        g_signal_connect(cancelBtn, "clicked", G_CALLBACK(close_window), data->addEditWin);
        // Po nacisnieciu przycisku 'Dodaj' zacznij procedure dodawania wydarzenia
        g_signal_connect(addEditBtn, "clicked", G_CALLBACK(edit_event), data);
        gtk_widget_show_all(data->addEditWin);
    }
}

void get_form_data(GtkBuilder *windowBuilder, const char **titleText, char **comboText,
                   char **text, GtkTextIter *start, GtkTextIter *end, guint *year,
                   guint *month, guint *day, guint *hour, guint *minute, guint *reminder)
{
    // Z pliku z kodem xml pobierz potrzebne obiekty interfejsu
    GtkWidget *titleEntryAddEdit = GTK_WIDGET(gtk_builder_get_object(windowBuilder, "titleEntryAddEdit"));
    GtkWidget *calendarAddEdit = GTK_WIDGET(gtk_builder_get_object(windowBuilder, "calendarAddEdit"));
    GtkWidget *hourAddEdit = GTK_WIDGET(gtk_builder_get_object(windowBuilder, "hourAddEdit"));
    GtkWidget *minuteAddEdit = GTK_WIDGET(gtk_builder_get_object(windowBuilder, "minuteAddEdit"));
    GtkWidget *reminderAddEdit = GTK_WIDGET(gtk_builder_get_object(windowBuilder, "reminderAddEdit"));
    GtkWidget *comboAddEdit = GTK_WIDGET(gtk_builder_get_object(windowBuilder, "comboAddEdit"));
    GtkWidget *textAddEdit = GTK_WIDGET(gtk_builder_get_object(windowBuilder, "textAddEdit"));
    GtkTextBuffer *buff = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textAddEdit));
    // Zapisz dane z pol formularza do odpowiednich zmiennych podanych w atrybutach funkcji
    *titleText = gtk_entry_get_text(GTK_ENTRY(titleEntryAddEdit));
    gtk_calendar_get_date(GTK_CALENDAR(calendarAddEdit), year, month, day);
    *hour = (guint)gtk_spin_button_get_value(GTK_SPIN_BUTTON(hourAddEdit));
    *minute = (guint)gtk_spin_button_get_value(GTK_SPIN_BUTTON(minuteAddEdit));
    *reminder = (guint)gtk_spin_button_get_value(GTK_SPIN_BUTTON(reminderAddEdit));
    *comboText = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(comboAddEdit));
    gtk_text_buffer_get_bounds(buff, start, end);
    *text = gtk_text_buffer_get_text(buff, start, end, FALSE);
}

void add_event(GtkWidget *button, gpointer user_data)
{
    // Pobierz dane formularza
    struct _windowData *data = user_data;
    const char *titleText;
    char *comboText, *text;
    GtkTextIter start, end;
    guint year, month, day, hour, minute, reminder;

    get_form_data(data->addEditBuilder, &titleText, &comboText, &text, &start, &end, &year, &month, &day, &hour, &minute, &reminder);

    // Sprawdz poprawnosc pola 'tytul'
    if (!strcmp(titleText, "")) { create_warning_message("Pole 'Tytul' jest wymagane!", data->addEditWin); }
    else
    {
        // Sprawdz czy wydarzenie o podanym tytule juz istnieje i jesli tak to wyswietl
        // stosowny komunikat
        struct _event *check = search_node(data->head, titleText);
        if (check) { create_warning_message("Wydarzenie o takim tytule juz istnieje!", data->addEditWin); }
        else
        {
            // Jesli tekst na opis jest pusty uzupelnij go spacja
            if (!strcmp(text, "")) { strcpy(text, " "); }
            char multiplier = 's';
            char dateTime[18];
            // Podczas tworzenia 'stringu' o odpowiednim formacie do liczby miesiecy dodaj 1
            // bo wartosc z kalendarza jest liczona od 0
            sprintf(dateTime, "%02d.%02d.%4d %02d:%02d", day, month + 1, year, hour, minute);
            // Ustaw odpowiednia wartosc dla zmiennej 'multiplier'. Godzina - h, Minuta - m, Seksunda - s
            if (!strcmp(comboText, "Minuty")) { multiplier = 'm'; }
            else if (!strcmp(comboText, "Godziny")) { multiplier = 'h'; }
            // Dodaj nowe wydarzenie do listy z podanymi wartosciami
            push(data->head, titleText, dateTime, reminder, multiplier, text);
            // zapisz liste do pliku
            save_events(data->head);
            // Ustaw nowo dodane wydarzenie jako aktualnie wyswietlane wydarzenie
            data->current = search_node(data->head, titleText);
            // Dodaj przycisk reprezentujacy wydarzenie
            add_event_button(titleText, data);
            close_window(GTK_BUTTON(button), data->addEditWin);
        }
    }
}

void edit_event(GtkWidget *button, gpointer user_data)
{
    // Pobierz dane formularza
    struct _windowData *data = user_data;
    const char *titleText;
    char *comboText, *text;
    GtkTextIter start, end;
    guint year, month, day, hour, minute, reminder;

    get_form_data(data->addEditBuilder, &titleText, &comboText, &text, &start, &end, &year, &month, &day, &hour, &minute, &reminder);

    // Sprawdz poprawnosc pola 'tytul'
    if (!strcmp(titleText, "")) { create_warning_message("Pole 'Tytul' jest wymagane!", data->addEditWin); }
    else
    {
        // Sprawdz czy wydarzenie o podanym tytule juz istnieje i jesli tak to wyswietl
        // stosowny komunikat
        struct _event *check = search_node(data->head, titleText);
        if (check && check != data->current) { create_warning_message("Wydarzenie o takim tytule juz istnieje!", data->addEditWin); }
        else
        {
            // Jesli tekst na opis jest pusty uzupelnij go spacja
            if (!strcmp(text, "")) { strcpy(text, " "); }
            char multiplier = 's';
            char dateTime[18];
            // Podczas tworzenia 'stringu' o odpowiednim formacie do liczby miesiecy dodaj 1
            // bo wartosc z kalendarza jest liczona od 0
            sprintf(dateTime, "%02d.%02d.%4d %02d:%02d", day, month + 1, year, hour, minute);
            // Ustaw odpowiednia wartosc dla zmiennej 'multiplier'. Godzina - h, Minuta - m, Seksunda - s
            if (!strcmp(comboText, "Minuty")) { multiplier = 'm'; }
            else if (!strcmp(comboText, "Godziny")) { multiplier = 'h'; }
            // Edytuj wydarzenie z listy
            strcpy(data->current->title, titleText);
            strcpy(data->current->description, text);
            data->current->multiplier = multiplier;
            data->current->num = reminder;
            data->current->start_of_event = setupTime(dateTime);
            gtk_button_set_label(GTK_BUTTON(data->currentButton), titleText);
            show_current_event(data->mainBuilder, data->current);
            // zapisz liste do pliku
            save_events(data->head);
            close_window(GTK_BUTTON(button), data->addEditWin);
        }
    }
}

void add_event_button(const char *title, gpointer user_data)
{
    // Z pliku z kodem xml pobierz potrzebne obiekty interfejsu
    struct _windowData *data = user_data;
    GtkWidget *eventsBox = GTK_WIDGET(gtk_builder_get_object(data->mainBuilder, "eventsBox"));
    GtkWidget *btn = gtk_button_new_with_label(title);
    // Przypisz aktualny przycisk do wydarzenia
    data->current->eventButton = btn;
    // Ustaw dodawany przycisk jako atualny
    data->currentButton = btn;
    // Dodaj klase 'event-button' do przyciksu (potrzebne dla kodu .css)
    gtk_style_context_add_class(gtk_widget_get_style_context(btn), "event-button");
    // Po nacisnieciu na przycisk zaktualizuj wyswietlane informacje
    g_signal_connect(btn, "clicked", G_CALLBACK(update_current_event), data);
    // Dodaj przycisk do kontenera
    gtk_box_pack_start(GTK_BOX(eventsBox), btn, FALSE, FALSE, 0);
    gtk_widget_show_all(eventsBox);
    // Pokaz dane nowo dodanego wydarzenia
    show_current_event(data->mainBuilder, data->current);
}

void update_current_event(GtkButton *button, gpointer user_data)
{
    // Zaktualizuj wyswietlane dane
    struct _windowData *data = user_data;
    data->current = search_node(data->head, gtk_button_get_label(GTK_BUTTON(button)));
    show_current_event(data->mainBuilder, data->current);
    // Zaktualizuj aktualny przycisk
    data->currentButton = GTK_WIDGET(button);
}

void show_current_event(GtkBuilder *mainBuilder, struct _event *current)
{
    // Z pliku z kodem xml pobierz potrzebne obiekty interfejsu
    GtkWidget *titleLabel = GTK_WIDGET(gtk_builder_get_object(mainBuilder, "titleLabel"));
    GtkWidget *dateLabel = GTK_WIDGET(gtk_builder_get_object(mainBuilder, "dateLabel"));
    GtkWidget *timeLabel = GTK_WIDGET(gtk_builder_get_object(mainBuilder, "timeLabel"));
    GtkWidget *reminderConstLabel = GTK_WIDGET(gtk_builder_get_object(mainBuilder, "reminderConstLabel"));
    GtkWidget *reminderValLabel = GTK_WIDGET(gtk_builder_get_object(mainBuilder, "reminderValLabel"));
    GtkWidget *descriptionConstLabel = GTK_WIDGET(gtk_builder_get_object(mainBuilder, "descriptionConstLabel"));
    GtkWidget *descriptionLabel = GTK_WIDGET(gtk_builder_get_object(mainBuilder, "descriptionLabel"));
    // Wyswietl dane tylko jesli istnieja
    if (current)
    {
        struct tm *info = localtime(&(current->start_of_event));
        char date[11];
        char time[6];
        char reminderTex[10];
        // Odpowiednio sformatuj wyswietlany tekst
        sprintf(date, "%02d.%02d.%4d", info->tm_mday, info->tm_mon + 1, info->tm_year + 1900);
        sprintf(time, "%02d:%02d", info->tm_hour, info->tm_min);
        sprintf(reminderTex, "%d%c", current->num, current->multiplier);
        // Wyswietl tekst w odpowiednich etykietach
        gtk_label_set_text(GTK_LABEL(titleLabel), current->title);
        gtk_label_set_text(GTK_LABEL(dateLabel), date);
        gtk_label_set_text(GTK_LABEL(timeLabel), time);
        gtk_label_set_text(GTK_LABEL(reminderConstLabel), "Przypomnienie:");
        gtk_label_set_text(GTK_LABEL(reminderValLabel), reminderTex);
        gtk_label_set_text(GTK_LABEL(descriptionConstLabel), "Opis");
        gtk_label_set_text(GTK_LABEL(descriptionLabel), current->description);
    }
    // W przeciwnym razie wyczysc wszystkie etykiety
    else
    {
        gtk_label_set_text(GTK_LABEL(titleLabel), "");
        gtk_label_set_text(GTK_LABEL(dateLabel), "");
        gtk_label_set_text(GTK_LABEL(timeLabel), "");
        gtk_label_set_text(GTK_LABEL(reminderConstLabel), "");
        gtk_label_set_text(GTK_LABEL(reminderValLabel), "");
        gtk_label_set_text(GTK_LABEL(descriptionConstLabel), "");
        gtk_label_set_text(GTK_LABEL(descriptionLabel), "");
    }
}

void load_events_from_file(gpointer user_data)
{
    struct _windowData *data = user_data;
    // Zaladuj dane z pliku do listy
    load_events(data->head);
    struct _event *temp = *(data->head);
    // Przejdz po wszystkich elementach listy
    while(temp)
    {
        // Jesli element istnieje to ustaw go jako aktualnie wyswietlany
        data->current = temp;
        // Dodaj graficzna reprezentacje elementu
        add_event_button(temp->title, user_data);
        temp = temp->next;
    }
}

void create_delete_window(GtkButton *button, gpointer user_data)
{
    struct _windowData *data = user_data;
    // Zezwol na usuwanie wtedy i tylko wtedy kiedy lista nie jest pusta
    if (*data->head)
    {
        // Utworzenie okna z zapytaniem
        GtkBuilder *askWindowBuilder = gtk_builder_new_from_file("layouts/askWindow.ui");
        data->askWin = GTK_WIDGET(gtk_builder_get_object(askWindowBuilder, "askWindow"));
        GtkWidget *eventTitleLabel = GTK_WIDGET(gtk_builder_get_object(askWindowBuilder, "eventTitleLabel"));
        GtkWidget *yesBtn = GTK_WIDGET(gtk_builder_get_object(askWindowBuilder, "yesBtn"));
        GtkWidget *noBtn = GTK_WIDGET(gtk_builder_get_object(askWindowBuilder, "noBtn"));
        // Wypelnij etykiete z tytulem
        gtk_label_set_text(GTK_LABEL(eventTitleLabel), data->current->title);
        // Po nacisnieciu przycisku 'Nie' zamknij okno
        g_signal_connect(noBtn, "clicked", G_CALLBACK(close_window), data->askWin);
        // Po nacisnieciu przycisku  'Tak' usun wydarzenie
        g_signal_connect(yesBtn, "clicked", G_CALLBACK(delete_event), user_data);
        // Ustawienie okna jako 'najwyzsze'
        set_transient(data->mainWin, data->askWin);
        // Pokazanie okna
        gtk_widget_show_all(data->askWin);
    }
}

void delete_event(GtkButton *button, gpointer user_data)
{
    struct _windowData *data = user_data;
    // Usun element pod warunkiem ze lista nie jest pusta
    // Pobierz kontener, z ktorego ma zostac usuniety przycisk
    GtkWidget *eventsBox = GTK_WIDGET(gtk_builder_get_object(data->mainBuilder, "eventsBox"));
    // Usun wskazany element z linked listy
    delete_node(data->head,  gtk_button_get_label(GTK_BUTTON(data->currentButton)));
    // Usun przycisk reprezentujacy wskazane wydarzenie z kontenera
    gtk_container_remove(GTK_CONTAINER(eventsBox), data->currentButton);
    struct _event *head = *data->head;
    // Ustaw 'glowe' jako aktualny element listy
    data->current = head;
    // Ustaw przycisk 'glowy' jako aktualny przycisk pod warunkiem ze lista nie jest pusta
    if (head) { data->currentButton = head->eventButton; }
    // Pokaz kontener po usunieciu przycisku
    gtk_widget_show_all(eventsBox);
    // Wyswietl dane aktualnego wydarzenia i zapisz liste do pliku
    show_current_event(data->mainBuilder, data->current);
    save_events(data->head);
    // Zamknij okno z zapytaniem
    close_window(button, data->askWin);
}

void create_reminder_window(gpointer user_data)
{
    struct _event *temp = user_data;
    // Pobierz dane interfejsu z pliku z kodem xml
    int rWidth, rHeight;
    char eventTime[20];
    GtkBuilder *reminderWindowBuilder = gtk_builder_new_from_file("layouts/reminderWindow.ui");
    GtkWidget *reminderWindow = GTK_WIDGET(gtk_builder_get_object(reminderWindowBuilder, "reminderWindow"));
    GtkWidget *titleLabel = GTK_WIDGET(gtk_builder_get_object(reminderWindowBuilder, "titleLabel"));
    GtkWidget *timeLabel = GTK_WIDGET(gtk_builder_get_object(reminderWindowBuilder, "timeLabel"));
    GtkWidget *okBtn = GTK_WIDGET(gtk_builder_get_object(reminderWindowBuilder, "okBtn"));
    // Ustaw pozycje okna na prawy dolny rog na glownym monitorze
    GdkMonitor *monitor = gdk_display_get_monitor(gdk_display_get_default(), 0);
    GdkRectangle work = {0};
    gdk_monitor_get_workarea(monitor, &work);
    gtk_window_get_size(GTK_WINDOW(reminderWindow), &rWidth, &rHeight);
    gtk_window_move(GTK_WINDOW(reminderWindow), work.width - rWidth, work.height - rHeight);
    // Ustaw odpowiedni tekst na etykietach
    sprintf(eventTime, "%d%c", temp->num, temp->multiplier);
    gtk_label_set_text(GTK_LABEL(titleLabel), temp->title);
    gtk_label_set_text(GTK_LABEL(timeLabel), eventTime);
    // Umozliw zamkniecie okna poprzez nacisniecie 'OK'
    g_signal_connect(okBtn, "clicked", G_CALLBACK(close_window), reminderWindow);
    gtk_widget_show_all(reminderWindow);
}

gboolean check_time_of_events(gpointer user_data)
{
    struct _windowData *data = user_data;
    struct _event *temp = *data->head;
    // Przechodz po wszystkich dostepnych wydarzeniach
    while(temp)
    {
        // Jesli nadszedl czas na przypomnienie o wydarzeniu to wyswietl stosowne okno
        if (check_reminder(temp))
        {
            // Utworz okno wydarzenia
            create_reminder_window(temp);
            // Ustaw wydarzenie jako juz przypomniane
            temp->isDone = 1;
        }
        temp = temp->next;
    }
    return TRUE;
}
