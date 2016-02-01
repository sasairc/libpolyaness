/*
 * test.c
 */

#include "src/polyaness.h"
#include <stdio.h>

int main(void)
{
    int             i   = 0,
                    j   = 0;

    FILE*           fp  = NULL;

    polyaness_t*    pt  = NULL;

    fp = fopen("test.txt", "r");

    init_polyaness(fp, &pt);
    parse_polyaness(fp, &pt);
    fclose(fp);

    fprintf(stdout, "pt->recs = %d\n", pt->recs);
    while (i < pt->recs) {
        j = 0;
        while (j < pt->record[i]->keys) {
            fprintf(stdout, "pt->record[%d]->key[%d] = %s, pt->record[%d]->value[%d] = %s\n",
                    i, j, pt->record[i]->key[j],
                    i, j, pt->record[i]->value[j]);
            j++;
        }
        i++;
    }
    release_polyaness(pt);

    return 0;
}
