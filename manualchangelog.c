#include <stdio.h>
#include <string.h>

#include "card_type.h"
#include "all_lands.h"
#include "all_lands_latest.h"

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

char *status_str(collection_status_t cs) {
    switch (cs) {
        case NOT_OWNED:
            return "NOT_OWNED";
        case OWNED:
            return "OWNED";
        case ORDERED:
            return "ORDERED";
        case POOR_QUALITY:
            return "POOR_QUALITY";
        case WRONG_LANGUAGE:
            return "WRONG_LANGUAGE";
        case UNINDEXED:
        default:
            return "UNINDEXED";
    }
}

int main(int argc, char** argv) {
    if (land_count != land_count_latest) {
        printf("Attenzione hai modificato il numero di terre!!!\n");
        return 1;
    }    

    for (int i = 0; i < land_count; ++i) {
        if (!card_eq(&cards[i], &cards_latest[i])) {
            printf("Attenzione hai cambiato l'ordine delle terre (%d)!!!\n", i);
            return 1;
        }
        if (cards[i].status != cards_latest[i].status) {
            printf("Card %d (%s %s #%s) updated status %s -> %s\n", i, cards[i].set_sig, cards[i].foil ? "*" : " ", cards[i].collector_number, status_str(cards_latest[i].status), status_str(cards[i].status));
        }
    }

    return 0;
}
