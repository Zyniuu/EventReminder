#include "../headers/hfuncs.h"


time_t setupTime(char *date_time)
{
    // Funkcja pobiera czas w postaci 'stringu', ktory ma format 'dd:mm:yy hh:mm',
    // a nastepnie konwertuje go do postaci time_t
    int day, month, year, hours, minutes;
    struct tm tm = {0};
    // Sprawdzenie czy podano czas w odpowiednim formacie
    if (sscanf(date_time, "%2d.%2d.%4d %2d:%2d", &day, &month, &year, &hours, &minutes) == 5)
    {
        // konwersja poprzez wypelnienie struktury 'tm' i uzycie funkcji 'mktime'
        tm.tm_mday = day;
        tm.tm_mon = month - 1;
        tm.tm_year = year - 1900;
        tm.tm_hour = hours;
        tm.tm_min = minutes;
        return mktime(&tm);
    }
    else
    {
        perror("Not a valid time format.");
        return -1;
    }
}

void formatData(char *_string, char *title, time_t *time, int *num, char *multiplier, char *description)
{
    int index = 0;
    int elem = 0;
    int elemIndex = 0;
    int isElem = 0;
    char buff[5][1024];
    // Czytaj linie znak po znaku az do napotkania konca linii
    while(_string[index] != '\n')
    {
        // Jezeli znak jest '{', to oznacza, ze zostal napotkany element danych
        if (_string[index] == '{')
        {
            index++;
            isElem = 1;
        }
        // Jezeli znak jest '}', to oznacza koniec odczytywanego elementu
        if (_string[index] == '}')
        {
            buff[elem][elemIndex] = '\0';
            isElem = 0;
            elemIndex = 0;
            elem++;
        }
        // Odczytuj dane tylko jezeli znajdujesz sie pomiedzy znakami '{' i '}'
        if (isElem)
        {
            buff[elem][elemIndex] = _string[index];
            elemIndex++;
        }
        index++;
    }
    // Zapisanie danych do podanych zmiennych
    strcpy(title, buff[0]);
    sscanf(buff[1], "%lld", &(*time));
    sscanf(buff[2], "%d", &(*num));
    *multiplier = buff[3][0];
    strcpy(description, buff[4]);
}
