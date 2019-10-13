#include <stdlib.h>
#include <stdio.h>

int main() {
    int used = 0;
    char tmp[2];
    struct states {
        char abbr[5];
    };
    struct states data;
    struct states entries[10];
    
    while (scanf("%s",data.abbr)!= EOF && used < 10) {
        strcpy(entries[used].abbr, data.abbr);
        used++;
    }
    for (int i = 0; i < used -1 ; ++i) {
        for (int j = i + 1; j < used; ++j) {
            if (strcmp(entries[i].abbr, entries[j].abbr) > 0) {
                strcpy(tmp, entries[i].abbr);
                strcpy(entries[i].abbr, entries[j].abbr);
                strcpy(entries[j].abbr, tmp);
            }
        }
    }
    for (int k=0; k < used; ++k) {
        printf("%s\n", entries[k].abbr);
    }
}
