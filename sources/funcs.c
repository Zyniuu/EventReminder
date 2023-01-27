#include "../headers/funcs.h"


void push(struct _event **head, const char *title, char *date_time, int num, char multiplier, char *description)
{
    // Jezli wskaznik pierwszego elementu jest pusty, to wypelnij go danymi
    if (!(*head))
    {
        struct _event *_new = (struct _event*)malloc(sizeof(struct _event));
        if (!_new) { perror("Memory allocation error."); }
        strcpy(_new->title, title);
        strcpy(_new->description, description);
        _new->start_of_event = setupTime(date_time);
        _new->num = num;
        _new->multiplier = multiplier;
        _new->isDone = 0;
        _new->next = NULL;
        *head = _new;
        return;
    }
    // W innym wypadku szukaj ostatniego elementu listy
    // i dodaj dane do jego kolejnego elementu
    struct _event *current = *head;
    while(current->next) { current = current->next; }

    current->next = (struct _event*)malloc(sizeof(struct _event));
    if (!current->next) { perror("Memory allocation error."); }
    strcpy(current->next->title, title);
    strcpy(current->next->description, description);
    current->next->start_of_event = setupTime(date_time);
    current->next->num = num;
    current->next->multiplier = multiplier;
    current->next->isDone = 0;
    current->next->next = NULL;
}

void delete_node(struct _event **head, const char *title)
{
    // Potrzebne bedzie pilnowanie elementu wstecz
    struct _event *prev = NULL, *temp = *head;
    // Jezeli usuwany element jest jednoczesnie 'glowa'
    if (temp && !strcmp(temp->title, title))
    {
        *head = temp->next;
        free(temp);
        return;
    }
    // W przeciwnym przypadku przeszukuj linked liste do napotkania
    // szukanego elementu, lub do zakonczenia linked listy
    while(temp && strcmp(temp->title, title))
    {
        prev = temp;
        temp = temp->next;
    }
    // Jezeli nie znaleziono elementu, to zakoncz
    if (!temp) { return; }
    // Jezeli element zostal znaleziony, usun go i zwolnij pamiec
    prev->next = temp->next;
    free(temp);
}

void save_events(struct _event **head)
{
    // Otworzenie pliku 'save.dat' do zapisu
    FILE *file = fopen("saves/save.dat", "w");
    struct _event *temp = *head;
    // Dopoki nie zostal osiagniety koniec linked listy
    while(temp)
    {
        // Zapisuj dane aktualnego elementu listy do pliku w odpowiednim formacie
        // i przechodz do nastepnego elementu
        fprintf_s(file, "({%s}{%d}{%d}{%c}{%s})\n", temp->title, temp->start_of_event, temp->num, temp->multiplier, temp->description);
        temp = temp->next;
    }
    fclose(file);
}

void load_events(struct _event **head)
{
    FILE *file = fopen("saves/save.dat", "r");
    struct tm *info;
    char date[20];
    char buff[2048];
    struct _event temp;
    // Czytaj plik linia po linii
    while(fgets(buff, sizeof(buff), file))
    {
        // Wykorzystujac, ze kazda linia jest zapisana w odpowiednim
        // formacie, pobierz dane z pliku do zainicjalizowanych zmiennych
        formatData(buff, temp.title, &temp.start_of_event, &temp.num, &temp.multiplier, temp.description);
        // Konwersja czasu zapisanego w postaci 'time_t' do 'stringu'
        // w odpowiednim formacie
        info = localtime(&(temp.start_of_event));
        sprintf(date, "%02d.%02d.%4d %02d:%02d", info->tm_mday, info->tm_mon + 1, info->tm_year + 1900, info->tm_hour, info->tm_min);
        // Utworzenie linked listy z odczytanych danych
        push(head, temp.title, date, temp.num, temp.multiplier, temp.description);
    }
    fclose(file);
}

struct _event *search_node(struct _event **head, const char *title)
{
    struct _event *temp = *head;
    // Jesli pierwszy element to rowniez szukany element
    if (temp && !strcmp(temp->title, title)) { return *head; }
    // W innym wypadku przeszukaj cala liste
    while(temp && strcmp(temp->title, title)) { temp = temp->next; }
    // Jezeli nie znaleziono elementu zwroc NULL
    if (!temp) { return NULL; }
    // Jezeli znaleziono to zwroc wskaznik
    return temp;
}

int check_reminder(struct _event *node)
{
    int multiplier = 1;
    // Ustaw odpowiedni mnoznik w zaleznosci od jednostki czasu dla przypomnienia
    switch(node->multiplier)
    {
        case 'm':
            multiplier *= 60;
            break;
        case 'h':
            multiplier *= 3600;
            break;
    }
    // Porownaj czas wydarzenia z aktualnym czasem i zwroc 1 jesli ich roznica jest mniejsza badz rowna
    // ilosci ustawionego czasu do przypomnienia
    time_t now = time(0);
    if ((difftime(node->start_of_event, now) <= (node->num * multiplier)) && !node->isDone) { return 1; }
    return 0;
}
