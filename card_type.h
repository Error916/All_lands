#pragma once 
#include <stddef.h>
#include <stdbool.h>

typedef enum {
    UNINDEXED,
    NOT_OWNED,
    OWNED,
    ORDERED,
    POOR_QUALITY,
    WRONG_LANGUAGE,
} collection_status_t;

typedef struct {
    collection_status_t status;
    const char* released;
    const char* scryfall_uri;
    const char* image[2];
    bool  foil;
    bool  nonfoil;
    const char* set_sig;
    const char* set_name;
    const char* collector_number;
} Card;
