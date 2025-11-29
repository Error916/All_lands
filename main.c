#include <stdio.h>
#include <stdlib.h>

#include "all_lands.h"

int main(int argc, char** argv) {
    int coll = 9;
    if (argc > 1) {
        coll = atoi(argv[1]);
    }

    int land_tot = 0;
    for (int i = 0; i < land_count; ++i) {
        land_tot += cards[i].foil;
        land_tot += cards[i].nonfoil;
        if (cards[i].image[1] != NULL) {
            land_tot += cards[i].foil;
            land_tot += cards[i].nonfoil;
        }
    }

    Card *cards_full = malloc(sizeof(Card) * land_tot);
    int count = 0; 
    for (int i = 0; i < land_count; ++i) {
        if (cards[i].nonfoil) {
            cards_full[count] = cards[i];
            cards_full[count].foil = false;
            count++;
        }
        if (cards[i].foil) {
            cards_full[count] = cards[i];
            cards_full[count].nonfoil = false;
            count++;
        }
        if (cards[i].image[1] != NULL) {
            if (cards[i].nonfoil) {
                cards_full[count] = cards[i];
                cards_full[count].foil = false;
                cards_full[count].image[0] = cards[i].image[1];
                count++;
            }
            if (cards[i].foil) {
                cards_full[count] = cards[i];
                cards_full[count].nonfoil = false;
                cards_full[count].image[0] = cards[i].image[1];
                count++;
            }
        }
    }

    int rows = (land_tot / coll);
    rows += land_tot % coll ? 1 : 0; 
    printf("<!DOCTYPE html>\n");
    printf("<html>\n");
    printf("    <head>\n");
    printf("        <link rel=\"stylesheet\" href=\"web.css\">\n");
    printf("        <title> ALL BASIC LANDS </title>\n");
    printf("    </head>\n");
    printf("    <body>\n");
    printf("        <table>\n");
    printf("            <th colspan=%d>tot: %d, unique: %d, rows: %d, coll: %d</th>\n", coll, land_tot, land_count, rows, coll);
    for (int i = 0; i < rows; ++i) {
        printf("            <tr>\n");
        for(int j = i * coll; j < i * coll + coll && j < land_tot; ++j)
            printf("                <td>%04d<br /><a href=\"%s\"><img loading=\"lazy\" fetchpriority=\"high\" src=\"%s\"></a>(%s %s #%s)<br />\"%s\"<br />(%s)<br /></td>\n", j, cards_full[j].scryfall_uri, cards_full[j].image[0], cards_full[j].set_sig, cards_full[j].foil ? "*" : " ", cards_full[j].collector_number, cards_full[j].set_name, cards_full[j].released);
        printf("            </tr>\n");
    } 
    printf("        </table>\n");
    printf("    </body>\n");
    printf("</html>\n");

    return 0;
}
