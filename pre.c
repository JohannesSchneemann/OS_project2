#include <stdlib.h>
#include <stdio.h>

int main() {
    int used = 0;
    struct states {
        char abbr[5];
        int population;
    };
    struct states data;
    struct states entries[10];
   
    while (scanf("%s %d",data.abbr, &data.population)!= EOF
        && used != 10) {
        if (data.population >= 10) {
          entries[used]= data;
          used++;
        }
    }
    for (int i = 0; i < used; ++i) {
        printf("%s\n",entries[i].abbr);
    }
}
