//Zadanie 7
//Polecenie tee czyta ze standardowego wejścia do napotkania końca pliku, zapisuje kopię wejścia na standardowe
//wyjście i do pliku podanego w wierszu wywołania. Domyślnie polecenie tee nadpisuje ten plik, jeśli istnieje.
//Napisz program, który
//a) działa zgodnie z domyślnym zachowaniem polecenia tee
//b) obsługuje opcję –a plik , która powoduje dopisanie wczytywanych danych do końca pliku, jeśli ten istnieje.

#include <stdio.h>
#include <getopt.h>
#include <stdbool.h>
#include <stdlib.h>
#include <signal.h>

#define WRITE "w"
#define APPEND "a"

static bool append = false;
static bool end = false;

void endReading(int sig_num) {
    end = true;
}

void checkOption(int argc, char *argv[]) {
    int option;
    char options[] = "a";

    if (argc == 1) {
        printf("Brak argumentu (nazwa pliku)\n");
        exit(EXIT_FAILURE);
    }

    while ( (option = getopt(argc, argv, options)) != -1) {
        if (option == 'a') {
            append = true;
        } else {
            printf("Opcja nieznana. Opcja obslugiwana przez program: -a\n");
            exit(EXIT_FAILURE);
        }
        break;
    };
}

int main(int argc, char *argv[])
{
    checkOption(argc, argv);

    if (argc <= optind) {
        printf("Brak argumentu (nazwa pliku)\n");
        exit(EXIT_FAILURE);
    }
    char *filename = argv[optind];

    FILE *file = fopen(filename, append ? APPEND : WRITE);
    if (!file) {
        perror("Blad otwierania pliku");
        exit(EXIT_FAILURE);
    }

    signal(SIGINT, endReading);
    int c;
    while ( !end && (c = getchar()) != EOF ) {
        fputc(c, file);
        putc(c, stdout);
    }

    if (fclose(file)) {
        printf("Brak zamykania pliku!\n");
        exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}