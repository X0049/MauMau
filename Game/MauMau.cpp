#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include <time.h>
#include <conio.h>

#define ERRORC() errorc(__FILE__, __LINE__) 
// -------------------------------------------------------
const char* karten_KPHK[] = {
    "Karo Ass",  "Karo Koenig",  "Karo Koenigin",  "Karo Bube",  "Karo 10",  "Karo 9",  "Karo 8" , "Karo 7",
    "Peak Ass",  "Peak Koenig",  "Peak Koenigin",  "Peak Bube",  "Peak 10",  "Peak 9",  "Peak 8",  "Peak 7",
    "Herz Ass",  "Herz Koenig",  "Herz Koenigin",  "Herz Bube",  "Herz 10",  "Herz 9",  "Herz 8",  "Herz 7",
    "Kreuz Ass", "Kreuz Koenig", "Kreuz Koenigin", "Kreuz Bube", "Kreuz 10", "Kreuz 9", "Kreuz 8", "Kreuz 7",
};
const int karte_menge = 32;
const int start_karte = 5;
const int karte_zeichen = 4;
const int karte_symbole = 8;
// -------------------------------------------------------
int tutorial = 0;
int kontern = 1;
int BAB = 0;
int LNZ = 1;
int beta = 0;
// -------------------------------------------------------
const int karten_kr[] = { 0,  1,  2,  3,  4,  5,  6,  7 };
const int karten_ka[] = { 8,  9,  10, 11, 12, 13, 14, 15 };
const int karten_he[] = { 16, 17, 18, 19, 20, 21, 22, 23 };
const int karten_pe[] = { 24, 25, 26, 27, 28, 29, 30, 31 };
// -------------------------------------------------------
typedef struct {
           // Die Karte: https://www.spiele-palast.de/app/uploads/2023/10/32_playing_cards_sp_deck-1110x694.jpg
          // F r Kartenstappel:
         // hand = 1, deck = 0, Tisch = -1
        // Karo  : karten[0][0-7]  (Indizes  0-7)
       // Peak  : karten[1][0-7]  (Indizes  8-15)
      // Herz  : karten[2][0-7]  (Indizes 16-23)
     // Kreuz : karten[3][0-7]  (Indizes 24-31)
    int karten[karte_menge];
    int kartenstappel[karte_menge];
    int karte_hand_s[karte_menge];
    int karte_hand_b[karte_menge];
    int tisch;

    int wunschfarbe;
    int strafkarten;
    int karten_teilen;
    int Spiel_zug;
    int skbgz;
    int bkbgz;
}spiel_karte;
// -------------------------------------------------------
void credit();
void spielanleitung();
void errorc(const char* datei, int zeile);
int menu_c(int wahl, int size, const char* menu[], const char* menu_name[]);
// -------------------------------------------------------
void spiel_st(spiel_karte* sk);
void spiel_menu(spiel_karte sk);
void spiel_SZ(spiel_karte* sk);
void spiel_EBZ(spiel_karte* sk);
void spiel_MBZ(spiel_karte* sk);
void spiel_SBZ(spiel_karte* sk);
// -------------------------------------------------------
int hatKarte(int karte_hand[karte_menge]);
int kartemischen(spiel_karte* sk);
int karteziehen(spiel_karte* sk);
int karte_legbar(int karte, int tisch, int wunschfarbe);
void wunschfarbe_waehlen(spiel_karte* sk, int ist_bot);

int karte_farbe(int karte) { return karte / karte_symbole; }
int karte_wert(int karte) { return karte % karte_symbole; }
// -------------------------------------------------------
int main() {
    srand((unsigned int)time(NULL));
    while (1) {

        int wahl = 0;
        const char* menu[] = {
            " \033[5;37mStart Game\033[0m",
            " Einstellung",
            " Credits",
            " Spielanleitung",
            " Exit"
        };
        const char* tmenu[] = {
            " \033[5;37mStart Game\033[0m (Startet das Spiel)",
            " Einstellung (Oeffnet die Einstellungen)",
            " Credits (Zeigt die Credits an)",
            " Spielanleitung (Zeigt dir wie man spielen soll)",
            " Exit (Verlaesst das Spiel)"
        };
        const char* menu_name[] = { "Mau Mau", };
        int size = sizeof(menu) / sizeof(menu[0]);
        if (!tutorial) wahl = menu_c(wahl, size, menu, menu_name);
        else if (tutorial) wahl = menu_c(wahl, size, tmenu, menu_name);

        if (wahl == 0) {

            while (1) {

                int swahl = 0;
                const char* smenu[] = {
                    " PvE",
                    " Zurueck"
                };
                const char* tsmenu[] = {
                    " PvE (Gegen Bot Spielen)",
                    " Zurueck (Hauptmenue)"
                };
                const char* bsmenu[] = {
                    " PvE",
                    " PvP",
                    "\n\t\t\t\t\t\t Zurueck"
                };
                const char* btsmenu[] = {
                    " PvE (Gegen Bot Spielen)",
                    " PvP (Gegen Spieler Spielen)",
                    "\n\t\t\t\t\t\t Zurueck (Hauptmenue)"
                };

                const char* smenu_name[] = { "Start Game", };
                int ssize = sizeof(smenu) / sizeof(smenu[0]);
                int bssize = sizeof(bsmenu) / sizeof(bsmenu[0]);
                if (!tutorial && !beta) swahl = menu_c(swahl, ssize, smenu, smenu_name);
                else if (tutorial && !beta) swahl = menu_c(swahl, ssize, tsmenu, smenu_name);
                else if (!tutorial && beta) swahl = menu_c(swahl, bssize, bsmenu, smenu_name);
                else if (tutorial && beta) swahl = menu_c(swahl, bssize, btsmenu, smenu_name);

                if (swahl == 0) {
                    
                    int vewahl = 0;
                    const char* vemenu[] = {
                        " Einfach",
                        " Mittel",
                        " Schwere",
                        "\n\t\t\t\t\t\t Zurueck"
                    };
                    const char* tvemenu[] = {
                        " Einfach (Keine Schwierigkeit)",
                        " Mittel  (Normale Schwierigkeit)",
                        " Schwere (Ist nicht einfach)",
                        "\n\t\t\t\t\t\t Zurueck (Start Menue)"
                    };
                    const char* vemenu_name[] = { "PvE", };
                    int vesize = sizeof(vemenu) / sizeof(vemenu[0]);
                    if (!tutorial) vewahl = menu_c(vewahl, vesize, vemenu, vemenu_name);
                    else if (tutorial) vewahl = menu_c(vewahl, vesize, tvemenu, vemenu_name);
                    
                    if (vewahl == 0) {

                        spiel_karte sk; spiel_st(&sk);

                        while ((hatKarte(sk.karte_hand_s) && hatKarte(sk.karte_hand_b))) {
                            system("cls");
                            // -------------------------------------------------------
                            FILE* datei;
                            errno_t err;

                            err = fopen_s(&datei, "Spiel_Karten_PvE_E.txt", "w");
                            if (err != 0) {
                                printf("Fehler beim  ffnen der Datei!\n");
                            }
                            else {
                                fprintf(datei, "\t\t\t\t=========================================================\n");
                                fprintf(datei, "\t\t\t\t                     >  Mau Mau  <\n");
                                fprintf(datei, "\t\t\t\t=========================================================\n\n");
                                fprintf(datei, "\t\t\t\tTischkarte: %s", karten_KPHK[sk.tisch]);
                                fprintf(datei, "\n\n\t\t\t\tDeine Karten:\n");
                                for (int i = 0; i < karte_menge; i++) if (sk.karte_hand_s[i] == 1) fprintf(datei, "\t\t\t\t  %s\n", karten_KPHK[i]);
                                fprintf(datei, "\n\t\t\t\tEinfacher Bot Karten:\n");
                                for (int i = 0; i < karte_menge; i++) if (sk.karte_hand_b[i] == 1) fprintf(datei, "\t\t\t\t  %s\n", karten_KPHK[i]);
                                fprintf(datei, "\n\t\t\t\tDie Zuordnung:");
                                fprintf(datei, "\n\t\t\t\tHand = 1, Deck = 0, Tisch = -1\n");
                                fprintf(datei, "\n\t\t\t\tKartenstapel:");
                                fprintf(datei, "\n\t\t\t\tKaro:  KA  KK  KQ  KB  K10  K9  K8  K7");
                                fprintf(datei, "\n\t\t\t\tPeak:  PA  PK  PQ  PB  P10  P9  P8  P7");
                                fprintf(datei, "\n\t\t\t\tHerz:  HA  HK  HQ  HB  H10  H9  H8  H7");
                                fprintf(datei, "\n\t\t\t\tKreuz: KA  KK  KQ  KB  K10  K9  K8  K7");
                                fprintf(datei, "\n\n\t\t\t\tDie Rest:");
                                for (int i = 0; i < karte_zeichen; i++) {
                                    if (i == 0) fprintf(datei, "\n\t\t\t\tKaro: ");
                                    if (i == 1) fprintf(datei, "\n\t\t\t\tPeak: ");
                                    if (i == 2) fprintf(datei, "\n\t\t\t\tHerz: ");
                                    if (i == 3) fprintf(datei, "\n\t\t\t\tKreuz:");
                                    for (int j = 0; j < karte_symbole; j++) {
                                        int index = i * karte_symbole + j;
                                        fprintf(datei, " %2d", sk.karten[index]);
                                    }
                                }
                                fclose(datei);
                            }
                            // -------------------------------------------------------
                            if (sk.Spiel_zug)      spiel_SZ(&sk);
                            else if (!sk.Spiel_zug)spiel_EBZ(&sk);
                            else { ERRORC(); return 1; }
                        }

                        if (!hatKarte(sk.karte_hand_s)) {

                            system("cls");
                            printf("\n\n\n\t\t\t\t=========================================================\n");
                            printf("\t\t\t\t                     Spieler gewinnt!\n");
                            printf("\t\t\t\t=========================================================\n");
                            printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
                            system("pause");

                        }
                        else if (!hatKarte(sk.karte_hand_b)) {

                            system("cls");
                            printf("\n\n\n\t\t\t\t=========================================================\n");
                            printf("\t\t\t\t                      Bot gewinnt!\n");
                            printf("\t\t\t\t=========================================================\n");
                            printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
                            system("pause");

                        }
                        else { ERRORC(); return 1; }

                    }

                    else if (vewahl == 1) {

                        spiel_karte sk; spiel_st(&sk);

                        while ((hatKarte(sk.karte_hand_s) && hatKarte(sk.karte_hand_b))) {
                            system("cls");
                            // -------------------------------------------------------
                            FILE* datei;
                            errno_t err;

                            err = fopen_s(&datei, "Spiel_Karten_PvE_M.txt", "w");
                            if (err != 0) {
                                printf("Fehler beim  ffnen der Datei!\n");
                            }
                            else {
                                fprintf(datei, "\t\t\t\t=========================================================\n");
                                fprintf(datei, "\t\t\t\t                     >  Mau Mau  <\n");
                                fprintf(datei, "\t\t\t\t=========================================================\n\n");
                                fprintf(datei, "\t\t\t\tTischkarte: %s", karten_KPHK[sk.tisch]);
                                fprintf(datei, "\n\n\t\t\t\tDeine Karten:\n");
                                for (int i = 0; i < karte_menge; i++) if (sk.karte_hand_s[i] == 1) fprintf(datei, "\t\t\t\t  %s\n", karten_KPHK[i]);
                                fprintf(datei, "\n\t\t\t\tMittel Bot Karten:\n");
                                for (int i = 0; i < karte_menge; i++) if (sk.karte_hand_b[i] == 1) fprintf(datei, "\t\t\t\t  %s\n", karten_KPHK[i]);
                                fprintf(datei, "\n\t\t\t\tDie Zuordnung:");
                                fprintf(datei, "\n\t\t\t\tHand = 1, Deck = 0, Tisch = -1\n");
                                fprintf(datei, "\n\t\t\t\tKartenstapel:");
                                fprintf(datei, "\n\t\t\t\tKaro:  KA  KK  KQ  KB  K10  K9  K8  K7");
                                fprintf(datei, "\n\t\t\t\tPeak:  PA  PK  PQ  PB  P10  P9  P8  P7");
                                fprintf(datei, "\n\t\t\t\tHerz:  HA  HK  HQ  HB  H10  H9  H8  H7");
                                fprintf(datei, "\n\t\t\t\tKreuz: KA  KK  KQ  KB  K10  K9  K8  K7");
                                fprintf(datei, "\n\n\t\t\t\tDie Rest:");
                                for (int i = 0; i < karte_zeichen; i++) {
                                    if (i == 0) fprintf(datei, "\n\t\t\t\tKaro: ");
                                    if (i == 1) fprintf(datei, "\n\t\t\t\tPeak: ");
                                    if (i == 2) fprintf(datei, "\n\t\t\t\tHerz: ");
                                    if (i == 3) fprintf(datei, "\n\t\t\t\tKreuz:");
                                    for (int j = 0; j < karte_symbole; j++) {
                                        int index = i * karte_symbole + j;
                                        fprintf(datei, " %2d", sk.karten[index]);
                                    }
                                }
                                fclose(datei);
                            }
                            // -------------------------------------------------------
                            if (sk.Spiel_zug)      spiel_SZ(&sk);
                            else if (!sk.Spiel_zug)spiel_MBZ(&sk);
                            else { ERRORC(); return 1; }

                        }

                        if (!hatKarte(sk.karte_hand_s)) {

                            system("cls");
                            printf("\n\n\n\t\t\t\t=========================================================\n");
                            printf("\t\t\t\t                     Spieler gewinnt!\n");
                            printf("\t\t\t\t=========================================================\n");
                            printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
                            system("pause");

                        }
                        else if (!hatKarte(sk.karte_hand_b)) {

                            system("cls");
                            printf("\n\n\n\t\t\t\t=========================================================\n");
                            printf("\t\t\t\t                      Bot gewinnt!\n");
                            printf("\t\t\t\t=========================================================\n");
                            printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
                            system("pause");

                        }
                        else { ERRORC(); return 1; }

                    }
                    
                    else if (vewahl == 2) {

                        spiel_karte sk; spiel_st(&sk);

                        while ((hatKarte(sk.karte_hand_s) && hatKarte(sk.karte_hand_b))) {
                            system("cls");
                            // -------------------------------------------------------
                            FILE* datei;
                            errno_t err;

                            err = fopen_s(&datei, "Spiel_Karten_PvE_S.txt", "w");
                            if (err != 0) {
                                printf("Fehler beim  ffnen der Datei!\n");
                            }
                            else {
                                fprintf(datei, "\t\t\t\t=========================================================\n");
                                fprintf(datei, "\t\t\t\t                     >  Mau Mau  <\n");
                                fprintf(datei, "\t\t\t\t=========================================================\n\n");
                                fprintf(datei, "\t\t\t\tTischkarte: %s", karten_KPHK[sk.tisch]);
                                fprintf(datei, "\n\n\t\t\t\tDeine Karten:\n");
                                for (int i = 0; i < karte_menge; i++) if (sk.karte_hand_s[i] == 1) fprintf(datei, "\t\t\t\t  %s\n", karten_KPHK[i]);
                                fprintf(datei, "\n\t\t\t\tSchwerer Bot Karten:\n");
                                for (int i = 0; i < karte_menge; i++) if (sk.karte_hand_b[i] == 1) fprintf(datei, "\t\t\t\t  %s\n", karten_KPHK[i]);
                                fprintf(datei, "\n\t\t\t\tDie Zuordnung:");
                                fprintf(datei, "\n\t\t\t\tHand = 1, Deck = 0, Tisch = -1\n");
                                fprintf(datei, "\n\t\t\t\tKartenstapel:");
                                fprintf(datei, "\n\t\t\t\tKaro:  KA  KK  KQ  KB  K10  K9  K8  K7");
                                fprintf(datei, "\n\t\t\t\tPeak:  PA  PK  PQ  PB  P10  P9  P8  P7");
                                fprintf(datei, "\n\t\t\t\tHerz:  HA  HK  HQ  HB  H10  H9  H8  H7");
                                fprintf(datei, "\n\t\t\t\tKreuz: KA  KK  KQ  KB  K10  K9  K8  K7");
                                fprintf(datei, "\n\n\t\t\t\tDie Rest:");
                                for (int i = 0; i < karte_zeichen; i++) {
                                    if (i == 0) fprintf(datei, "\n\t\t\t\tKaro: ");
                                    if (i == 1) fprintf(datei, "\n\t\t\t\tPeak: ");
                                    if (i == 2) fprintf(datei, "\n\t\t\t\tHerz: ");
                                    if (i == 3) fprintf(datei, "\n\t\t\t\tKreuz:");
                                    for (int j = 0; j < karte_symbole; j++) {
                                        int index = i * karte_symbole + j;
                                        fprintf(datei, " %2d", sk.karten[index]);
                                    }
                                }
                                fclose(datei);
                            }
                            // -------------------------------------------------------
                            if (sk.Spiel_zug)      spiel_SZ(&sk);
                            else if (!sk.Spiel_zug)spiel_SBZ(&sk);
                            else { ERRORC(); return 1; }

                        }

                        if (!hatKarte(sk.karte_hand_s)) {

                            system("cls");
                            printf("\n\n\n\t\t\t\t=========================================================\n");
                            printf("\t\t\t\t                     Spieler gewinnt!\n");
                            printf("\t\t\t\t=========================================================\n");
                            printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
                            system("pause");

                        }
                        else if (!hatKarte(sk.karte_hand_b)) {

                            system("cls");
                            printf("\n\n\n\t\t\t\t=========================================================\n");
                            printf("\t\t\t\t                      Bot gewinnt!\n");
                            printf("\t\t\t\t=========================================================\n");
                            printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
                            system("pause");

                        }
                        else { ERRORC(); return 1; }

                    }

                    else if (vewahl == --vesize);
                    else { ERRORC(); return 1; }

                }
                
                else if (swahl == 1 && beta) {

                    int vpwahl = 0;
                    const char* vpmenu[] = {
                        " Beitreten",
                        " Erstellen",
                        " Zuschauen",
                        " Zurueck"
                    };
                    const char* tvpmenu[] = {
                        " Beitreten (Tritt eine Lobby bei)",
                        " Erstellen (Erstellst eine Lobby)",
                        " Zuschauen (Schaust Spieler zu)",
                        " Zurueck (Start Menue)"
                    };
                    const char* vpmenu_name[] = { "PvP", };
                    int vpsize = sizeof(vpmenu) / sizeof(vpmenu[0]);
                    if (!tutorial) vpwahl = menu_c(vpwahl, vpsize, vpmenu, vpmenu_name);
                    else if (tutorial) vpwahl = menu_c(vpwahl, vpsize, tvpmenu, vpmenu_name);

                    if (vpwahl == 0) {

                    }

                    else if (vpwahl == 1) {

                    }

                    else if (vpwahl == 2) {

                    }

                    else if (vpwahl == --vpsize);
                    else { ERRORC(); return 1; }
                }

                else if (swahl == 1 || swahl == 2) break;
                else { ERRORC(); return 1; }

            }

        }

        else if (wahl == 1) {

            int ewahl = 0;
            const char* emenu[] = {
            " Tutorial",
            " Kontern",
            " Bube auf Bube",
            " Legen nach Ziehen",
            " Beta Modus",
            "   Zurueck"
            };
            const char* temenu[] = {
            " Tutorial (Zeigt Tips an)",
            " Kontern (Mit 7 Kontern)",
            " Bube (Erlaubt Bube auf Bube)",
            " LNZ (Erlaubt Karten zu Legen nach dem man gezogen hat)",
            " Beta (Fugt eine Beta Modus hinzu)",
            "   Zurueck (Hauptmenue)"
            };
            int esize = sizeof(emenu) / sizeof(emenu[0]);
            int etaste;

            while (1) {
                system("cls");
                printf("\n\n\n\t\t\t\t=========================================================\n");
                printf("\t\t\t\t                  >  Einstellung  <\n");
                printf("\t\t\t\t=========================================================\n\n");
                const char* tcheckbox = (tutorial == 1) ? "[\033[32mX\033[0m]" : "[ ]";
                const char* kcheckbox = (kontern == 1) ? "[\033[32mX\033[0m]" : "[ ]";
                const char* bcheckbox = (BAB == 1) ? "[\033[32mX\033[0m]" : "[ ]";
                const char* lcheckbox = (LNZ == 1) ? "[\033[32mX\033[0m]" : "[ ]";
                const char* beheckbox = (beta == 1) ? "[\033[32mX\033[0m]" : "[ ]";
                const char* checkboxes[] = { tcheckbox, kcheckbox, bcheckbox, lcheckbox, beheckbox };

                for (int i = 0; i < esize; i++) {
                    const char* prefix = (i < esize - 1) ? checkboxes[i] : " ";
                    if (!tutorial) {
                        if (i == ewahl) printf("\t\t\t\t\t\t-> %s%s\n", prefix, emenu[i]);
                        else printf("\t\t\t\t\t\t %s%s\n", prefix, emenu[i]);
                    }
                    if (tutorial) {
                        if (i == ewahl) printf("\t\t\t\t\t\t-> %s%s\n", prefix, temenu[i]);
                        else printf("\t\t\t\t\t\t %s%s\n", prefix, temenu[i]);
                    }
                }

                etaste = _getch();
                if (etaste == 224) {
                    etaste = _getch();
                    if (etaste == 72 && ewahl > 0)         ewahl--;
                    if (etaste == 80 && ewahl < esize - 1) ewahl++;
                }
                else if (etaste == 13 || etaste == ' ') {
                    if (ewahl == 0) tutorial = !tutorial;
                    if (ewahl == 1) kontern = !kontern;
                    if (ewahl == 2) BAB = !BAB;
                    if (ewahl == 3) LNZ = !LNZ;
                    if (ewahl == 4) beta = !beta;
                    if (ewahl == 5) break;
                }
                else {
                    if ((etaste == 'w' || etaste == 'W') && ewahl > 0)         ewahl--;
                    if ((etaste == 's' || etaste == 'S') && ewahl < esize - 1) ewahl++;
                }
            }

        }

        else if (wahl == 2) { credit(); }
        else if (wahl == 3) { spielanleitung(); }
        else if (wahl == 4) {
            system("cls");
            printf("\n\n\n\n\n\n\n\n\t\t\t\t\t\033[1;37m__________________________________________\033[0m\n\n");
            printf("\t\t\t\t\t\033[1;37m         >  Danke fuers Spielen!  <\033[0m\n\n");
            printf("\t\t\t\t\t\033[1;37m__________________________________________\033[0m\n\n\n\n\n\n\n\n\n\n\n\n\n");
            Sleep(2000);
            return 0;
        }
        else { ERRORC(); return 1; }

    }

    return 1;
}
// -------------------------------------------------------
void credit() {

    system("cls");
    printf("\n\n\t\t\t\t  ====================================================\n");
    printf("\t\t\t\t                        Credits \n");
    printf("\t\t\t\t  ====================================================\n\n\n"); Sleep(400);
    printf("\t\t\t\t\t------------------------------------\n");                     Sleep(100);
    printf("\t\t\t\t\t        Projektinformationen\n");                             Sleep(100);
    printf("\t\t\t\t\t------------------------------------\n\n");                   Sleep(100);
    printf("\t\t\t\t\tTitel: Mau Mau\n");                                           Sleep(100);
    printf("\t\t\t\t\tVersion: 1.1.3\n");                                           Sleep(100);
    printf("\t\t\t\t\tKurzbeschreibung: Das ist ein Karten Spiel\n\n\n");           Sleep(100);

    printf("\t\t\t\t\t------------------------------------\n");                     Sleep(100);
    printf("\t\t\t\t\t         Entwicklerteam\n");                                  Sleep(100);
    printf("\t\t\t\t\t------------------------------------\n\n");                   Sleep(100);
    printf("\t\t\t\t\tProgrammierer: Name\n");                                      Sleep(100);
    printf("\t\t\t\t\tDesigner: Name\n\n");                                         Sleep(100);

    printf("\t\t\t\t\t------------------------------------\n");                     Sleep(100);
    printf("\t\t\t\t\t        Ressourcen & Tools\n");                               Sleep(100);
    printf("\t\t\t\t\t------------------------------------\n\n");                   Sleep(100);
    printf("\t\t\t\t\tProgrammiersprachen: C\n");                                   Sleep(100);
    printf("\t\t\t\t\tEngines: Visual Studio\n");                                   Sleep(100);
    printf("\t\t\t\t\tBibliotheken: stdio.h, stdlib.h,\n");
    printf("\t\t\t\t\t              time.h, windows.h,\n");
    printf("\t\t\t\t\t              conio.h,\n\n");                                 Sleep(100);

    printf("\t\t\t\t\t------------------------------------\n");                     Sleep(100);
    printf("\t\t\t\t\t            Danksagungen\n");                                 Sleep(100);
    printf("\t\t\t\t\t------------------------------------\n\n");                   Sleep(100);
    printf("\t\t\t\t\tNoch Nimand\n\n");                                            Sleep(100);

    system("pause");

}
void spielanleitung() {
    system("cls");
    printf("\n\n\t\t\t\t  ====================================================\n");
    printf("\t\t\t\t                    Spielanleitung\n");
    printf("\t\t\t\t  ====================================================\n\n");
    printf("\t\t\t\tZiel des Spiels:\n");
    printf("\t\t\t\t  Wer zuerst alle Karten ablegt, gewinnt.\n");
    printf("\t\t\t\t  Wer nur noch eine Karte hat, muss 'Mau' rufen!\n\n");
    printf("\t\t\t\tSpielablauf:\n");
    printf("\t\t\t\t  - Jeder Spieler bekommt %d Karten.\n", start_karte);
    printf("\t\t\t\t  - Eine Karte wird als Startkarte aufgedeckt.\n");
    printf("\t\t\t\t  - Reihum legt man eine passende Karte ab\n");
    printf("\t\t\t\t    oder zieht eine Karte vom Stapel.\n\n");
    printf("\t\t\t\tEine Karte passt wenn:\n");
    printf("\t\t\t\t  - Sie die gleiche Symbole hat  (z.B. Herz auf Herz)\n");
    printf("\t\t\t\t  - Sie den gleichen Wert hat  (z.B. 7 auf 7)\n\n");
    printf("\t\t\t\tSonderkarten:\n");
    printf("\t\t\t\t  7  -> Naechster muss 2 Karten ziehen (stapelbar).\n");
    printf("\t\t\t\t  8  -> Naechster Spieler setzt eine Runde aus.\n");
    printf("\t\t\t\t  Bube -> Darf immer gelegt werden.\n");
    printf("\t\t\t\t          Leger wuenscht sich eine Farbe.\n\n");
    printf("\t\t\t\tSteuerung:\n");
    printf("\t\t\t\t  Pfeiltasten / W-S  ->  Auswahl bewegen\n");
    printf("\t\t\t\t  Enter / Leertaste  ->  Bestaetigen\n\n");
    system("pause");
}
void errorc(const char* datei, int zeile) {
    system("cls");
    printf("\n\n\n\n\n\n\n\n");
    printf("\t\t\t\t=========================================================\n");
    printf("\t\t\t\tLeider ist ein Fehler aufgetreten.\n");
    printf("\t\t\t\tBitte versuchen Sie es erneut.\n");
    printf("\t\t\t\tFehlercode in:\n");
    printf("\t\t\t\tZeile %d\n", zeile);
    printf("\t\t\t\tDatei %s\n", datei);
    printf("\t\t\t\t=========================================================\n\n\n\n");
}
int menu_c(int wahl, int size, const char* menu[], const char* menu_name[]) {
    int taste;
    while (1) {
        system("cls");
        printf("\n\n\n\t\t\t\t=========================================================\n");
        printf("\t\t\t\t                     >  \033[1;37m%s\033[0m  <\n", menu_name[0]);
        printf("\t\t\t\t=========================================================\n\n");
        for (int i = 0; i < size; i++) {
            if (i == wahl) printf("\t\t\t\t\t\t\033[1;37m%s\033[0m\n", menu[i]);
            else            printf("\t\t\t\t\t\t \033[90;40m%s\033[0m\n", menu[i]);
        }                                                     //
                                                             //
        taste = _getch();                                   //
        if (taste == 224) {                                // 224 steht f r Prefix ist daf r da sonst pfeil taste hoch/runter machen kann
            taste = _getch();                             //
            if (taste == 72 && wahl > 0)        wahl--;  // 72 steht f r pfeil taste hoch
            if (taste == 80 && wahl < size - 1) wahl++; // 80 steht f r pfeil taste runter
        }                                              //
        else if (taste == 13 || taste == ' ') {       // 13 ist Enter
            return wahl;                             //
        }                                           //
        else {                                     //
            if ((taste == 'w' || taste == 'W') && wahl > 0)        wahl--;
            if ((taste == 's' || taste == 'S') && wahl < size - 1) wahl++;
        }
    }
}
// -------------------------------------------------------
void spiel_st(spiel_karte* sk){

    sk->karten_teilen = 0; sk->Spiel_zug = rand() % 2; sk->tisch = 0; sk->wunschfarbe = -1; sk->strafkarten = 0; sk->skbgz = 0; sk->bkbgz = 0;
    for (int i = 0; i < karte_menge; i++) {
        sk->karten[i] = -1;
        sk->kartenstappel[i] = -1;
        sk->karte_hand_s[i] = 0;
        sk->karte_hand_b[i] = 0;
    }
    
    for (int i = 0; i < karte_menge; i++) sk->kartenstappel[i] = kartemischen(sk);
    if (sk->karten_teilen == 0) {
        for (int i = 0; i < start_karte; i++) { int k = karteziehen(sk); if (k != -1) sk->karte_hand_s[k] = 1; }
        for (int i = 0; i < start_karte; i++) { int k = karteziehen(sk); if (k != -1) sk->karte_hand_b[k] = 1; }
        int k; do { k = karteziehen(sk); } while (k != -1 && karte_wert(k) == 3);
        if (k != -1) sk->tisch = k;
        sk->karten_teilen = 1;
    }

}
void spiel_menu(spiel_karte sk) {
    const char* farbnamen[] = { "Karo", "Peak", "Herz", "Kreuz" };
    system("cls");
    printf("\n\n\n\t\t\t\t=========================================================\n");
    printf("\t\t\t\t                     >  Mau Mau  <\n");
    printf("\t\t\t\t=========================================================\n\n");
    printf("\t\t\t\tTischkarte        : %s\n", karten_KPHK[sk.tisch]);
    if (sk.wunschfarbe >= 0) printf("\t\t\t\tWunschfarbe       : %s\n", farbnamen[sk.wunschfarbe]);
    if (sk.strafkarten > 0)  printf("\t\t\t\t!! Strafkarten    : %d !!\n", sk.strafkarten);
    int kmvb = 0;
    for (int i = 0; i < karte_menge; i++) if (sk.karte_hand_b[i] == 1) kmvb++;
    printf("\n\t\t\t\tKarten des Bot    : %d\n\n", kmvb);
    printf("\t\t\t\tDeine Karten:\n");
    for (int i = 0; i < karte_menge; i++) if (sk.karte_hand_s[i] == 1) printf("\t\t\t\t  %s\n", karten_KPHK[i]);
    printf("\n");
}
void spiel_SZ(spiel_karte* sk) {

    if (LNZ && sk->bkbgz == 1) sk->bkbgz = 0;
    if (kontern && sk->strafkarten > 0) {
        int hat7 = 0;
        for (int i = 0; i < karte_menge; i++) if (sk->karte_hand_s[i] == 1 && karte_wert(i) == 7) { hat7 = 1; break; }
        if (!hat7) {
            spiel_menu(*sk);
            printf("\t\t\t\tDu musst %d Strafkarten ziehen\n", sk->strafkarten); Sleep(1500);
            for (int n = 0; n < sk->strafkarten; n++) { int k = karteziehen(sk); if (k != -1) sk->karte_hand_s[k] = 1; }
            sk->strafkarten = 0; sk->Spiel_zug = !sk->Spiel_zug; return;
        }
    }
    else if (!kontern && sk->strafkarten > 0) {
        spiel_menu(*sk);
        printf("\t\t\t\tDu musst %d Strafkarten ziehen\n", sk->strafkarten); Sleep(1500);
        for (int n = 0; n < sk->strafkarten; n++) { int k = karteziehen(sk); if (k != -1) sk->karte_hand_s[k] = 1; }
        sk->strafkarten = 0; sk->Spiel_zug = !sk->Spiel_zug; return;
    }

    int optionen[karte_menge + 1];
    int opt_count = 0;
    for (int i = 0; i < karte_menge; i++) {
        if (sk->karte_hand_s[i] == 1) {
            if (sk->strafkarten > 0) { if (kontern && karte_wert(i) == 7) optionen[opt_count++] = i; }
            else { if (karte_legbar(i, sk->tisch, sk->wunschfarbe)) optionen[opt_count++] = i; }
        }
    }
    if (sk->strafkarten == 0) optionen[opt_count++] = -1;

    int wahl = 0; int taste;
    while (1) {
        spiel_menu(*sk);
        if (tutorial) {
            printf("\t\t\t\t[Tutorial] Lege eine passende Karte oder waehle 'Karte ziehen'.\n");
            printf("\t\t\t\tWaehle eine Aktion (W/S + Enter):\n\n");
        }
        printf("\t\t\t\tSpielbaren Karten: \n\n");
        for (int i = 0; i < opt_count; i++) {
            if (optionen[i] == -1) {
                if (i == wahl) printf("\t\t\t\t  [ Karte ziehen ] <-\n");
                else            printf("\t\t\t\t    Karte ziehen\n");
            }
            else {
                if (i == wahl) printf("\t\t\t\t  [ %s ] <-\n", karten_KPHK[optionen[i]]);
                else            printf("\t\t\t\t    %s\n", karten_KPHK[optionen[i]]);
            }
        }
        taste = _getch();
        if (taste == 224) {
            taste = _getch();
            if (taste == 72 && wahl > 0)             wahl--;
            if (taste == 80 && wahl < opt_count - 1) wahl++;
        }
        else if (taste == 13 || taste == ' ') break;
        else {
            if ((taste == 'w' || taste == 'W') && wahl > 0)             wahl--;
            if ((taste == 's' || taste == 'S') && wahl < opt_count - 1) wahl++;
        }
    }

    int gewaehlt = optionen[wahl];
    if (gewaehlt == -1) {
        int k = -1;
        if (sk->skbgz == 0) k = karteziehen(sk);
        if (k != -1) {
            sk->karte_hand_s[k] = 1;
            system("cls");
            printf("\n\t\t\t\tDu hast gezogen: %s\n", karten_KPHK[k]);
            Sleep(1000);
            if (LNZ && sk->skbgz == 0) { sk->skbgz = 1; return; }
        }
        if (k == -1) {
            system("cls");
            printf("\n\n\n\t\t\t\tBot ist dran, weil du keine karte ziehen konntest.\n");
            Sleep(1500);
        }
    }
    else {
        sk->karte_hand_s[gewaehlt] = 0;
        sk->karten[gewaehlt] = -1;
        sk->tisch = gewaehlt;
        sk->wunschfarbe = -1;
        int wert = karte_wert(gewaehlt);

        if (wert == 7) {
            sk->strafkarten += 2;
            system("cls");
            printf("\n\t\t\t\tDu hast eine 7 gelegt! Bot muss %d Karten ziehen.\n", sk->strafkarten);
            Sleep(1200);
        }
        else if (wert == 6) {
            system("cls");
            printf("\n\t\t\t\tDu hast eine 8 gelegt! Bot setzt aus.\n");
            Sleep(1200);
            return;
        }
        else if (wert == 3) { wunschfarbe_waehlen(sk, 0); }

        int verbleibend = 0;
        for (int i = 0; i < karte_menge; i++) if (sk->karte_hand_s[i] == 1) verbleibend++;
        if (verbleibend == 1) {
            system("cls");
            printf("\n\t\t\t\t*** Mau Mau! Du hast nur noch eine Karte! ***\n");
            Sleep(1200);
        }
    }
    sk->Spiel_zug = !sk->Spiel_zug;
}
void spiel_EBZ(spiel_karte* sk) {

    system("cls");
    printf("\n\n\t\t\t\t=========================================================\n");
    printf("\t\t\t\t                     Bot ist dran ...\n");
    printf("\t\t\t\t=========================================================\n\n");
    Sleep(900);

    if (LNZ && sk->skbgz == 1) sk->skbgz = 0;
    if (kontern && sk->strafkarten > 0) {
        int hat7 = 0;
        for (int i = 0; i < karte_menge; i++) if (sk->karte_hand_b[i] == 1 && karte_wert(i) == 7) { hat7 = 1; break; }
        if (!hat7) {
            printf("\t\t\t\tBot zieht %d Strafkarten\n", sk->strafkarten); Sleep(1000);
            for (int n = 0; n < sk->strafkarten; n++) { int k = karteziehen(sk); if (k != -1) sk->karte_hand_b[k] = 1; }
            sk->strafkarten = 0; sk->Spiel_zug = !sk->Spiel_zug; return;
        }
    }
    else if (!kontern && sk->strafkarten > 0) {
        printf("\t\t\t\tBot zieht %d Strafkarten\n", sk->strafkarten); Sleep(1000);
        for (int n = 0; n < sk->strafkarten; n++) { int k = karteziehen(sk); if (k != -1) sk->karte_hand_b[k] = 1; }
        sk->strafkarten = 0; sk->Spiel_zug = !sk->Spiel_zug; return;
    }

    int optionen[karte_menge];
    int opt_count = 0;

    for (int i = 0; i < karte_menge; i++) {
        if (sk->karte_hand_b[i] == 1) {
            if (sk->strafkarten > 0) { if (kontern && karte_wert(i) == 7) optionen[opt_count++] = i; }
            else { if (karte_legbar(i, sk->tisch, sk->wunschfarbe)) optionen[opt_count++] = i; }
        }
    }

    if (opt_count == 0) {
        int k = karteziehen(sk);
        if (k != -1) { sk->karte_hand_b[k] = 1; printf("\t\t\t\tBot zieht eine Karte.\n"); }
        else                                     printf("\t\t\t\tDeck leer - Bot passt.\n");
        Sleep(1000);
        if (k == -1) sk->Spiel_zug = !sk->Spiel_zug;
        if (LNZ) return;
    }
    else {
        int beste = optionen[rand() % opt_count];

        sk->karte_hand_b[beste] = 0;
        sk->karten[beste] = -1;
        sk->tisch = beste;
        sk->wunschfarbe = -1;
        printf("\t\t\t\tBot legt: %s\n", karten_KPHK[beste]); Sleep(1000);
        int wert = karte_wert(beste);
        if (wert == 7) {
            sk->strafkarten += 2;
            printf("\t\t\t\tBot legt eine 7. Du musst %d Karten ziehen.\n", sk->strafkarten); Sleep(1200);
        }
        else if (wert == 6) {
            printf("\t\t\t\tBot legt eine 8. Du setzt aus.\n"); Sleep(1200);
            return;
        }
        else if (wert == 3) { wunschfarbe_waehlen(sk, 1); }

        int verbleibend = 0;
        for (int i = 0; i < karte_menge; i++) if (sk->karte_hand_b[i] == 1) verbleibend++;
        if (verbleibend == 1) { printf("\t\t\t\tBot ruft: Mau Mau!\n"); Sleep(1000); }
    }
    sk->Spiel_zug = !sk->Spiel_zug;
}
void spiel_MBZ(spiel_karte* sk) {

    system("cls");
    printf("\n\n\t\t\t\t=========================================================\n");
    printf("\t\t\t\t                     Bot ist dran ...\n");
    printf("\t\t\t\t=========================================================\n\n");
    Sleep(900);

    if (LNZ && sk->skbgz == 1) sk->skbgz = 0;
    if (kontern && sk->strafkarten > 0) {
        int hat7 = 0;
        for (int i = 0; i < karte_menge; i++) if (sk->karte_hand_b[i] == 1 && karte_wert(i) == 7) { hat7 = 1; break; }
        if (!hat7) {
            printf("\t\t\t\tBot zieht %d Strafkarten\n", sk->strafkarten); Sleep(1000);
            for (int n = 0; n < sk->strafkarten; n++) { int k = karteziehen(sk); if (k != -1) sk->karte_hand_b[k] = 1; }
            sk->strafkarten = 0; sk->Spiel_zug = !sk->Spiel_zug; return;
        }
    }
    else if (!kontern && sk->strafkarten > 0) {
        printf("\t\t\t\tBot zieht %d Strafkarten\n", sk->strafkarten); Sleep(1000);
        for (int n = 0; n < sk->strafkarten; n++) { int k = karteziehen(sk); if (k != -1) sk->karte_hand_b[k] = 1; }
        sk->strafkarten = 0; sk->Spiel_zug = !sk->Spiel_zug; return;
    }

    int beste = -1;
    if (sk->strafkarten > 0 && kontern) {
        for (int i = 0; i < karte_menge && beste == -1; i++)
            if (sk->karte_hand_b[i] == 1 && karte_wert(i) == 7) beste = i;
    }
    if (beste == -1) {
        for (int i = 0; i < karte_menge && beste == -1; i++) if (sk->karte_hand_b[i] == 1 && karte_wert(i) == 7 && karte_legbar(i, sk->tisch, sk->wunschfarbe)) beste = i;
        for (int i = 0; i < karte_menge && beste == -1; i++) if (sk->karte_hand_b[i] == 1 && karte_wert(i) == 6 && karte_legbar(i, sk->tisch, sk->wunschfarbe)) beste = i;
        for (int i = 0; i < karte_menge && beste == -1; i++) if (sk->karte_hand_b[i] == 1 && karte_wert(i) == 3) beste = i;
        for (int i = 0; i < karte_menge && beste == -1; i++) if (sk->karte_hand_b[i] == 1 && karte_legbar(i, sk->tisch, sk->wunschfarbe)) beste = i;
    }

    if (beste == -1) {
        int k = karteziehen(sk);
        if (k != -1) { sk->karte_hand_b[k] = 1; printf("\t\t\t\tBot zieht eine Karte.\n"); }
        else                                     printf("\t\t\t\tDeck leer.\n");
        Sleep(1000);
        if (LNZ) return;
    }
    else {
        sk->karte_hand_b[beste] = 0;
        sk->karten[beste] = -1;
        sk->tisch = beste;
        sk->wunschfarbe = -1;
        printf("\t\t\t\tBot legt: %s\n", karten_KPHK[beste]); Sleep(1000);
        int wert = karte_wert(beste);
        if (wert == 7) {
            sk->strafkarten += 2;
            printf("\t\t\t\tBot legt eine 7. Du musst %d Karten ziehen.\n", sk->strafkarten); Sleep(1200);
        }
        else if (wert == 6) {
            printf("\t\t\t\tBot legt eine 8. Du setzt aus.\n"); Sleep(1200);
            return;
        }
        else if (wert == 3) { wunschfarbe_waehlen(sk, 1); }

        int verbleibend = 0;
        for (int i = 0; i < karte_menge; i++) if (sk->karte_hand_b[i] == 1) verbleibend++;
        if (verbleibend == 1) { printf("\t\t\t\tBot ruft: Mau Mau!\n"); Sleep(1000); }
    }
    sk->Spiel_zug = !sk->Spiel_zug;
}
void spiel_SBZ(spiel_karte* sk) {

    system("cls");
    printf("\n\n\t\t\t\t=========================================================\n");
    printf("\t\t\t\t                     Bot ist dran ...\n");
    printf("\t\t\t\t=========================================================\n\n");
    Sleep(900);

    if (LNZ && sk->skbgz == 1) sk->skbgz = 0;
    if (kontern && sk->strafkarten > 0) {
        int hat7 = 0;
        for (int i = 0; i < karte_menge; i++) if (sk->karte_hand_b[i] == 1 && karte_wert(i) == 7) { hat7 = 1; break; }
        if (!hat7) {
            printf("\t\t\t\tBot zieht %d Strafkarten\n", sk->strafkarten); Sleep(1000);
            for (int n = 0; n < sk->strafkarten; n++) { int k = karteziehen(sk); if (k != -1) sk->karte_hand_b[k] = 1; }
            sk->strafkarten = 0; sk->Spiel_zug = !sk->Spiel_zug; return;
        }
    }
    else if (!kontern && sk->strafkarten > 0) {
        printf("\t\t\t\tBot zieht %d Strafkarten\n", sk->strafkarten); Sleep(1000);
        for (int n = 0; n < sk->strafkarten; n++) { int k = karteziehen(sk); if (k != -1) sk->karte_hand_b[k] = 1; }
        sk->strafkarten = 0; sk->Spiel_zug = !sk->Spiel_zug; return;
    }

    int beste = -1;
    if (sk->strafkarten > 0 && kontern) {
        for (int i = 0; i < karte_menge && beste == -1; i++)
            if (sk->karte_hand_b[i] == 1 && karte_wert(i) == 7) beste = i;
    }
    if (beste == -1) {
        int anzahl = 0;
        for (int i = 0; i < karte_menge; i++) if (sk->karte_hand_b[i] == 1 && karte_wert(i) == 6 && karte_legbar(i, sk->tisch, sk->wunschfarbe)) beste = i;
        for (int i = 0; i < karte_menge && beste == -1; i++) if (sk->karte_hand_b[i] == 1 && karte_legbar(i, sk->tisch, sk->wunschfarbe)) beste = i;
        if (!kontern) for (int i = 0; i < karte_menge && beste == -1; i++) if (sk->karte_hand_b[i] == 1 && karte_wert(i) == 7 && karte_legbar(i, sk->tisch, sk->wunschfarbe)) beste = i;
        if (beste == -1) for (int i = 0; i < karte_menge && beste == -1; i++) if (sk->karte_hand_b[i] == 1 && karte_wert(i) == 3) beste = i;
        if (kontern) for (int i = 0; i < karte_menge; i++) if (sk->karte_hand_s[i] == 1) anzahl++;
        if (anzahl < 4) for (int i = 0; i < karte_menge && beste == -1; i++) if (sk->karte_hand_b[i] == 1 && karte_wert(i) == 7 && karte_legbar(i, sk->tisch, sk->wunschfarbe)) beste = i;
        if (karte_wert(beste) != 7 && anzahl == 1 && LNZ && sk->bkbgz == 0) {
            anzahl = 0; int k = karteziehen(sk);
            if (k != -1) { sk->karte_hand_b[k] = 1; printf("\t\t\t\tBot zieht eine Karte.\n"); }
            else                                     printf("\t\t\t\tDeck leer - Bot passt.\n"); Sleep(1000);
            for (int i = 0; i < karte_menge; i++) if (sk->karte_hand_b[i] == 1 && karte_wert(i) == 6 && karte_legbar(i, sk->tisch, sk->wunschfarbe)) beste = i;
            for (int i = 0; i < karte_menge && beste == -1; i++) if (sk->karte_hand_b[i] == 1 && karte_legbar(i, sk->tisch, sk->wunschfarbe)) beste = i;
            if (!kontern) for (int i = 0; i < karte_menge && beste == -1; i++) if (sk->karte_hand_b[i] == 1 && karte_wert(i) == 7 && karte_legbar(i, sk->tisch, sk->wunschfarbe)) beste = i;
            if (beste == -1) for (int i = 0; i < karte_menge && beste == -1; i++) if (sk->karte_hand_b[i] == 1 && karte_wert(i) == 3) beste = i;
            if (kontern) for (int i = 0; i < karte_menge; i++) if (sk->karte_hand_s[i] == 1) anzahl++;
            if (anzahl < 4) for (int i = 0; i < karte_menge && beste == -1; i++) if (sk->karte_hand_b[i] == 1 && karte_wert(i) == 7 && karte_legbar(i, sk->tisch, sk->wunschfarbe)) beste = i;
        }
    }

    if (beste == -1) {
        int k = karteziehen(sk);
        if (k != -1) { sk->karte_hand_b[k] = 1; printf("\t\t\t\tBot zieht eine Karte.\n"); }
        else                                     printf("\t\t\t\tDeck leer.\n");
        Sleep(1000);
        if (LNZ && sk->bkbgz == 0) { sk->bkbgz = 1; return; }
    }
    else {
        sk->karte_hand_b[beste] = 0;
        sk->karten[beste] = -1;
        sk->tisch = beste;
        sk->wunschfarbe = -1;
        printf("\t\t\t\tBot legt: %s\n", karten_KPHK[beste]); Sleep(1000);
        int wert = karte_wert(beste);
        if (wert == 7) {
            sk->strafkarten += 2;
            printf("\t\t\t\tBot legt eine 7. Du musst %d Karten ziehen.\n", sk->strafkarten); Sleep(1200);
        }
        else if (wert == 6) {
            printf("\t\t\t\tBot legt eine 8. Du setzt aus.\n"); Sleep(1200);
            return;
        }
        else if (wert == 3) { wunschfarbe_waehlen(sk, 1); }

        int verbleibend = 0;
        for (int i = 0; i < karte_menge; i++) if (sk->karte_hand_b[i] == 1) verbleibend++;
        if (verbleibend == 1) { printf("\t\t\t\tBot ruft: Mau Mau!\n"); Sleep(1000); }
    }
    sk->Spiel_zug = !sk->Spiel_zug;

}
// -------------------------------------------------------
int hatKarte(int karte_hand[karte_menge]) {
    for (int i = 0; i < karte_menge; i++) if (karte_hand[i] > 0) return 1;
    return 0;
}
int kartemischen(spiel_karte* sk) {
    int Karte;
    do { Karte = rand() % karte_menge; } while (sk->karten[Karte] != -1);
    sk->karten[Karte] = 0;
    return Karte;
}
int karteziehen(spiel_karte* sk) {
    for (int i = 0; i < karte_menge * 4; i++) {
        int Karte = rand() % karte_menge;
        if (sk->karten[Karte] == 0) { sk->karten[Karte] = 1; return Karte; }
    }

    int neu_gemischt = 0;
    for (int i = 0; i < karte_menge; i++) {
        if (i != sk->tisch && sk->karten[i] == -1) {
            sk->karten[i] = 0;
            neu_gemischt++;
        }
    }

    if (neu_gemischt == 0) return -1;

    for (int i = 0; i < karte_menge * 4; i++) {
        int Karte = rand() % karte_menge;
        if (sk->karten[Karte] == 0) { sk->karten[Karte] = 1; return Karte; }
    }
    return -1;
}
int karte_legbar(int karte, int tisch, int wunschfarbe) {
    if (wunschfarbe >= 0) return karte_farbe(karte) == wunschfarbe;
    if (BAB == 0) if (karte_wert(karte) == 3 && karte_wert(tisch) != 3) return 1;
    if (BAB == 1) if (karte_wert(karte) == 3) return 1;
    if (karte_farbe(karte) == karte_farbe(tisch)) return 1;
    if (karte_wert(karte) == karte_wert(tisch)) return 1;
    return 0;
}
void wunschfarbe_waehlen(spiel_karte* sk, int ist_bot) {
    const char* farbnamen[] = { "Karo", "Peak", "Herz", "Kreuz" };
    if (ist_bot) {
        int z[4] = { 0, 0, 0, 0 };
        for (int i = 0; i < karte_menge; i++) if (sk->karte_hand_b[i] == 1) z[karte_farbe(i)]++;
        int b = 0;
        for (int f = 1; f < 4; f++) if (z[f] > z[b]) b = f;
        sk->wunschfarbe = b;
        printf("\n\t\t\t\tBot wuenscht sich: %s\n", farbnamen[b]); Sleep(1200);
    }
    else {
        const char* fmenu[] = { " Karo", " Peak", " Herz", " Kreuz" };
        const char* fname[] = { "Wunschfarbe waehlen" };
        sk->wunschfarbe = menu_c(0, 4, fmenu, fname);
    }
}
// -------------------------------------------------------