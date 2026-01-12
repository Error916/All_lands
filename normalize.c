#include <stdio.h>
#include <stdlib.h>

#include "card_type.h"
#include "all_lands_bash.h"

int main(int argc, char** argv) {
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

    FILE *f = fopen("all_lands_normalize.h", "w");
    fprintf(f, "#include \"card_type.h\"\n\nconst Card cards[] = {\n");
    for (int i = 0; i < land_tot; ++i) {
            char* class = "not_owned";
            switch (cards_full[i].status) {
                case NOT_OWNED:
                    class = "NOT_OWNED";
                    break;
                case OWNED:
                    class = "OWNED";
                    break;
                case ORDERED:
                    class = "ORDERED";
                    break;
                case POOR_QUALITY:
                    class = "POOR_QUALITY";
                    break;
                case WRONG_LANGUAGE:
                    class = "WRONG_LANGUAGE";
                    break;
                case UNINDEXED:
                default:
                    class = "UNINDEXED";
            }
            fprintf(f, "\t{%s, \"%s\", \"%s\", {\"%s\", %s}, %s, %s, \"%s\", \"%s\", \"%s\",},\n",
                    class, cards_full[i].released, cards_full[i].scryfall_uri,
                    cards_full[i].image[0], "NULL",
                    cards_full[i].foil ? "true" : "false",  cards_full[i].nonfoil ? "true" : "false",
                    cards_full[i].set_sig, cards_full[i].set_name, cards_full[i].collector_number);
    }
    fprintf(f, "};\n\nconst size_t land_count = sizeof(cards) / sizeof(cards[0]);");
    return 0;
}
