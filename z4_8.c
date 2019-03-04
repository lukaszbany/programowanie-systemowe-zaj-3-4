//Zadanie 8
//Napisz program, który jest uproszczoną wersją polecenia du:
//a) działa tylko we wskazanym katalogu,
//b) wyświetla albo wielkości poszczególnych plików, albo ich sumę
//c) wielkości mogą być wyświetlane w różnych jednostkach (patrz polecenie du) w zależności od podanej opcji

#define _XOPEN_SOURCE 1
#define _XOPEN_SOURCE_EXTENDED 1

#include <stdio.h>
#include <getopt.h>
#include <stdbool.h>
#include <stdlib.h>
#include <signal.h>
#include <ftw.h>
#include <memory.h>

static long currentSize;
static char currentPath[150];
static bool printFiles;
static bool kilobytes;
static bool megabytes;

long formatSize (long size) {
    if (kilobytes) {
        return size / (1024);
    }
    if (megabytes) {
        return size / (1024*1024);
    }

    return size;
}

char getUnit() {
    if (kilobytes) {
        return 'K';
    }
    if (megabytes) {
        return 'M';
    }

    return 'B';
}


bool shouldPrintFileInfo(const char *path) {
    return printFiles && strcmp(path, currentPath) != 0;
}

int sumSizes(const char *path, const struct stat *stat_buf, int type, struct FTW *ftwp) {
    currentSize += stat_buf->st_size;
    if (shouldPrintFileInfo(path))
        printf("%ld%c\t%s\n", formatSize(stat_buf->st_size), getUnit(), path);

    return 0;
}

int sumSizeAndPrint(const char *path, const struct stat *stat_buf, int type, struct FTW *ftwp) {
    strcpy(currentPath, path);

    switch (type) {
        case FTW_D:
            currentSize = 0;
            nftw(path, sumSizes, FOPEN_MAX, FTW_PHYS);
            printf("%ld%c\t%s\n", formatSize(currentSize), getUnit(), path);
            break;
        case FTW_DNR:
            printf("Blad odczytu katalogu \"%s\"!\n", path);
            break;
        case FTW_NS:
            printf("Blad odczytu typu pliku \"%s\"!\n", path);
            break;
    }

    return 0;
}

void checkOption(int argc, char *argv[]) {
    printFiles = false;
    kilobytes = false;
    megabytes = false;

    int option;
    char options[] = "akm";

    while ( (option = getopt(argc, argv, options)) != -1) {
        switch (option) {
            case 'a':
                printFiles = true;
                break;
            case 'k':
                if (!megabytes) {
                    kilobytes = true;
                } else {
                    printf("Wykluczajace sie argumenty!\n");
                    exit(EXIT_FAILURE);
                }
                break;
            case 'm':
                if (!kilobytes) {
                    megabytes = true;
                } else {
                    printf("Wykluczajace sie argumenty!\n");
                    exit(EXIT_FAILURE);
                }
                break;
            default:
                printf("Opcja nieznana. Opcje obslugiwane przez program: -a -k -m\n");
                exit(EXIT_FAILURE);
        }
    };
}

/*
 * Opcje:
 *  -a  wyswietla wielkosc plikow i ich nazwy
 *  -k  wielkosc w kilobajtach
 *  -m  wielkosc w megabajtach
 *
 *  argument: katalog do zbadania
 *  brak argumentu: obecny katalog
 *
 *  KOMENTARZ: zauwazylem, ze wielkosci sa inne niz pokazywane
 *  przez polecenie du. Wydaje mi sie, ze du zaokragla do 4 kilobajtow
 *  w gore. Moj program pokazuje pelne wybrane przez uzytkownika jednostki.
 * */
int main(int argc, char *argv[])
{
    checkOption(argc, argv);

    char *path;
    if (argc == optind) {
        path = ".";
    } else if (argc == optind + 1) {
        path = argv[optind];
    }

    nftw(path, sumSizeAndPrint, FOPEN_MAX, FTW_PHYS);


    return EXIT_SUCCESS;
}