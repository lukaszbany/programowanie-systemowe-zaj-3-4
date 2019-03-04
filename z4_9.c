//Zadanie 9
//Napisz program, który przegląda wybrany katalog wraz z podkatalogami i wyświetla informację o liczbie i
//procentowym udziale plików różnych typów (zwykłe, katalogi, dowiązania symboliczne, itp.). Do przeglądania
//katalogu użyj funkcji bibliotecznej nftw().

#define _XOPEN_SOURCE 1
#define _XOPEN_SOURCE_EXTENDED 1

#include <stdio.h>
#include <getopt.h>
#include <stdbool.h>
#include <stdlib.h>
#include <signal.h>
#include <ftw.h>

static int numberOfFiles = 0;
static int numberOfDirectories = 0;
static int numberOfCharacterFiles = 0;
static int numberOfBlockFiles = 0;
static int numberOfFifos = 0;
static int numberOfSockets = 0;
static int numberOfSymbolicLinks = 0;

int countFilesAndPrint(const char *path, const struct stat *stat_buf, int type, struct FTW *ftwp) {
    switch (type) {
        case FTW_F:
            switch (stat_buf->st_mode & S_IFMT) {
                case S_IFREG:
                    numberOfFiles++;
                    break;
                case S_IFCHR:
                    numberOfFiles++;
                    break;
                case S_IFBLK:
                    numberOfFiles++;
                    break;
                case S_IFIFO:
                    numberOfFiles++;
                    break;
            }
            break;
        case FTW_D:
            numberOfDirectories++;
            break;
        case FTW_SL:
        case FTW_SLN:
            numberOfSymbolicLinks++;
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

void printStats(char *path) {
    printf("W katalogu \"%s\" znajduja sie: \n", path);
    printf("           Pliki zwykle: %d\n", numberOfFiles);
    printf("               Katalogi: %d\n", numberOfDirectories);
    printf("Pliki specjalne znakowe: %d\n", numberOfCharacterFiles);
    printf("Pliki specjalne blokowe: %d\n", numberOfBlockFiles);
    printf("                Sockety: %d\n", numberOfSockets);
    printf("     Łącza nazwane FIFO: %d\n", numberOfFifos);
    printf(" Dowiązania symboliczne: %d\n", numberOfSymbolicLinks);
}

int main(int argc, char *argv[])
{
    char *path;
    if (argc == 1) {
        path = ".";
    } else {
        path = argv[1];
    }

    nftw(path, countFilesAndPrint, FOPEN_MAX, FTW_PHYS);
    printStats(path);

    return EXIT_SUCCESS;
}