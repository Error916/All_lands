#!/usr/bin/env sh

set -xe;

PAGS=24;

:> released.txt;
:> scryfall_uri.txt;
:> images.txt;
:> foil.txt;
:> nonfoil.txt;
:> set_sig.txt;
:> set_name.txt;
:> collector_number.txt;

for i in $(seq 1 $PAGS);
    do 
        curl "https://api.scryfall.com/cards/search?dir=asc&format=json&include_extras=true&include_multilingual=false&include_variations=true&order=released&page=$i&q=t%3Aland+t%3Abasic+include%3Aextras+-layout%3Aart_series+game%3Apaper+in%3Apaper+unique%3Aprints+include%3Aextras+include%3Avariations&unique=prints" | jq > pag"$i".json; 
        grep -Po '"released_at": \K.*'      pag"$i".json >> released.txt;
        grep -Po '"scryfall_uri": \K.*'     pag"$i".json >> scryfall_uri.txt;
        grep -Po '"normal": \K.*'           pag"$i".json >> images.txt;
        grep -Po '"foil": \K.*'             pag"$i".json >> foil.txt; 
        grep -Po '"nonfoil": \K.*'          pag"$i".json >> nonfoil.txt;
        grep -Po '"set": \K.*'              pag"$i".json >> set_sig.txt;
        grep -Po '"set_name": \K.*'         pag"$i".json >> set_name.txt;
        grep -Po '"collector_number": \K.*' pag"$i".json >> collector_number.txt;
done

awk -i inplace '{$1=$1; print}' released.txt;
awk -i inplace '{$1=$1; print}' scryfall_uri.txt;
awk -i inplace '{$1=$1; print}' images.txt;
awk -i inplace '{printf (!/back/&&NR>1?RS:x)"%s",$0} END {print ""}' images.txt;
awk -i inplace '!/back/ {$0=$0" NULL,"} 1' images.txt;
awk -i inplace '{print "{" $0 "},"}' images.txt;
awk -i inplace '{$1=$1; print}' foil.txt;
awk -i inplace '{$1=$1; print}' nonfoil.txt;
awk -i inplace '{$1=$1; print}' set_sig.txt;
awk -i inplace '{$1=$1; print}' set_name.txt;
awk -i inplace '{$1=$1; print}' collector_number.txt;

paste -d ' ' released.txt scryfall_uri.txt images.txt foil.txt nonfoil.txt set_sig.txt set_name.txt collector_number.txt > elements.txt;
awk -i inplace '{print "    {UNINDEXED, " $0 "},"}' elements.txt;

printf '#include "card_type.h"

const Card cards[] = {
' > all_lands_bash.h;

cat elements.txt >> all_lands_bash.h;

printf '};

const size_t land_count = sizeof(cards) / sizeof(cards[0]);
' >> all_lands_bash.h;

if [ -f ./all_lands.h ]; then
    cp all_lands.h all_lands.h.bak
    awk -i inplace '{ gsub(/cards\[/, "cards_old["); print }' all_lands.h
    awk -i inplace '{ gsub(/cards)/, "cards_old)"); print }' all_lands.h
    awk -i inplace '{ gsub(/land_count/, "land_count_old"); print }' all_lands.h
    mv all_lands.h all_lands_old.h
fi

if [ ! -f ./all_lands_old.h ]; then
    printf '#include "card_type.h"

const Card cards_old[] = {};
    
const size_t land_count_old = sizeof(cards_old) / sizeof(cards_old[0]);
' > all_lands_old.h;
fi

gcc normalize.c -o normalize;

./normalize

gcc collection_update.c -o collection_update;

./collection_update > changelog;

gcc sitemaker.c -o sitemaker;

./sitemaker 7 > index.html;

cp all_lands.h all_lands_latest.h
awk -i inplace '{ gsub(/cards\[/, "cards_latest["); print }' all_lands_latest.h
awk -i inplace '{ gsub(/cards)/, "cards_latest)"); print }' all_lands_latest.h
awk -i inplace '{ gsub(/land_count/, "land_count_latest"); print }' all_lands_latest.h

rm all_lands_bash.h;
rm all_lands_normalize.h;
rm all_lands_old.h;
rm released.txt;
rm scryfall_uri.txt;
rm images.txt;
rm foil.txt;
rm nonfoil.txt;
rm set_sig.txt;
rm set_name.txt;
rm collector_number.txt;
rm elements.txt;

for i in $(seq 1 $PAGS);
    do rm pag"$i".json;
done
