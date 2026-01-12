#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "card_type.h"
#include "all_lands_normalize.h"
#include "all_lands_old.h"

int card_eq(const Card *c1, const Card *c2) {
       if (c1->foil == c2->foil &&
           c1->nonfoil == c2->nonfoil &&
           strcmp(c1->set_sig, c2->set_sig) == 0 &&
           strcmp(c1->collector_number, c2->collector_number) == 0 &&
           strcmp(c1->released, c2->released) == 0 &&
           strcmp(c1->image[0], c2->image[0]) == 0)
           return 1;
       return 0;
}

void update_changes(const Card *old, int old_len, Card *new, int new_len) {
    for (int iold = 0; iold < old_len; iold++) {
        if (card_eq(&old[iold], &new[iold])) {
            // we set the collection_status to the new array
            new[iold].status = old[iold].status;
        } else {
            int find = 0;
            int inew = 0;
            for (; inew < new_len; inew++) {
                if (card_eq(&old[iold], &new[inew])) { 
                    find = 1;
                    break;
                }
            }
            if (find) {
                // we set the collection_status to the new array
                new[inew].status = old[iold].status;
                printf("Card %d(old) (%s %s #%s) been moved to %d(new) position\n", iold, old[iold].set_sig, old[iold].foil ? "*" : " ", old[iold].collector_number, inew);
            } else {
                // card removed from collection!!
                printf("Card %d(old) (%s %s #%s) been removed\n", iold, old[iold].set_sig, old[iold].foil ? "*" : " ", old[iold].collector_number);
            } 
        }
    }
    
    for (int inew = 0; inew < new_len; inew++) {
        if (new[inew].status == UNINDEXED) {
            new[inew].status = NOT_OWNED;
            printf("Card %d(new) (%s %s #%s) added\n", inew, new[inew].set_sig, new[inew].foil ? "*" : " ", new[inew].collector_number);
        }
    }
}

int main(int argc, char** argv) {
    int coll = 9;
    if (argc > 1) {
        coll = atoi(argv[1]);
    }

    Card *cards_full = malloc(sizeof(Card) * land_count);
    memcpy(cards_full, cards, sizeof(Card) * land_count);
    const Card *cards_full_old = cards_old;
    int land_tot = land_count;
    int land_tot_old = land_count_old;

    update_changes(cards_full_old, land_tot_old, cards_full, land_tot);
    FILE *f = fopen("all_lands.h", "w");
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
            fprintf(f, "\t{%s, \"%s\", \"%s\", {\"%s\", %s%s%s}, %s, %s, \"%s\", \"%s\", \"%s\",},\n",
                    class, cards_full[i].released, cards_full[i].scryfall_uri,
                    cards_full[i].image[0], !cards_full[i].image[1] ? "" : "\"", !cards_full[i].image[1] ? "NULL" : cards_full[i].image[1], !cards_full[i].image[1] ? "" : "\"",
                    cards_full[i].foil ? "true" : "false",  cards_full[i].nonfoil ? "true" : "false",
                    cards_full[i].set_sig, cards_full[i].set_name, cards_full[i].collector_number);
    }
    fprintf(f, "};\n\nconst size_t land_count = sizeof(cards) / sizeof(cards[0]);");
    
    return 0;
}
