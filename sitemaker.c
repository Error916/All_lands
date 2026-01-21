#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "card_type.h"
#include "all_lands.h"

int main(int argc, char** argv) {
    int coll = 7;
    if (argc > 1) {
        coll = atoi(argv[1]);
    }

    int rows = (land_count / coll);
    rows += land_count % coll ? 1 : 0;

    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    char timebuf[100];

    strftime(timebuf, sizeof(timebuf), "%d %b %Y %H:%M:%S", t);
   
    int count[CARD_STATE_COUNT] = { 0 };
    for (int i = 0; i < land_count; ++i) {
        count[cards[i].status]++;
    }

    printf("<!DOCTYPE html>\n");
    printf("<html>\n");
    printf("    <head>\n");
    printf("        <link rel=\"stylesheet\" href=\"web.css\">\n");
    printf("        <title> ALL BASIC LANDS </title>\n");
    printf("    </head>\n");
    printf("    <body>\n");
    printf("        <table>\n");
    printf("            <th colspan=2> BG color reference table </th>\n");
    printf("            <tr>\n");
    printf("                <td>Not owned</td>\n");
    printf("                <td class = \"not_owned\">%05d</td>\n", count[NOT_OWNED]);
    printf("            </tr>\n");
    printf("            <tr>\n");
    printf("                <td>Owned</td>\n");
    printf("                <td class = \"owned\">%05d</td>\n", count[OWNED]);
    printf("            </tr>\n");
    printf("            <tr>\n");
    printf("                <td>Ordered</td>\n");
    printf("                <td class = \"ordered\">%05d</td>\n", count[ORDERED]);
    printf("            </tr>\n");
    printf("            <tr>\n");
    printf("                <td>Poor quality</td>\n");
    printf("                <td class = \"poor_quality\">%05d</td>\n", count[POOR_QUALITY]);
    printf("            </tr>\n");
    printf("            <tr>\n");
    printf("                <td>Wrong language</td>\n");
    printf("                <td class = \"wrong_language\">%05d</td>\n", count[WRONG_LANGUAGE]);
    printf("            </tr>\n");
    printf("            <tr>\n");
    printf("                <td>Unindexed</td>\n");
    printf("                <td class = \"unindexed\">%05d</td>\n", count[UNINDEXED]);
    printf("            </tr>\n");
    printf("        </table>\n");
    printf("        <table>\n");
    printf("            <th colspan=%d>tot: %ld, owned: %d, missing: %d, rows: %d, coll: %d, lastupdate: %s</th>\n", coll, land_count, count[OWNED] + count[ORDERED] + count[POOR_QUALITY] + count[WRONG_LANGUAGE], count[NOT_OWNED] + count[UNINDEXED], rows, coll, timebuf);
    for (int i = 0; i < rows; ++i) {
        printf("            <tr>\n");
        for(int j = i * coll; j < i * coll + coll && j < land_count; ++j) {
            char* class = NULL;
            switch (cards[j].status) {
                case NOT_OWNED:
                    class = "not_owned";
                    break;
                case OWNED:
                    class = "owned";
                    break;
                case ORDERED:
                    class = "ordered";
                    break;
                case POOR_QUALITY:
                    class = "poor_quality";
                    break;
                case WRONG_LANGUAGE:
                    class = "wrong_language";
                    break;
                case UNINDEXED:
                default:
                    class = "unindexed";
            }
            printf("                <td class=\"%s\">%04d<br /><a href=\"%s\"><img loading=\"lazy\" fetchpriority=\"high\" src=\"%s\"></a>(%s %s #%s)<br />\"%s\"<br />(%s)<br /></td>\n", class, j, cards[j].scryfall_uri, cards[j].image[0], cards[j].set_sig, cards[j].foil ? "*" : " ", cards[j].collector_number, cards[j].set_name, cards[j].released);
        }
        printf("            </tr>\n");
    } 
    printf("        </table>\n");
    printf("    </body>\n");
    printf("</html>\n");

    return 0;
}
