/*
 * test.c
 */

#include "./polyaness.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
    int             i       = 0,
                    j       = 0;

    char*           speaker = NULL,
        *           quote   = NULL;

    FILE*           fp      = NULL;

    polyaness_t*    pt      = NULL;

    if (argc <= 2)
        return 1;

    if ((fp = fopen(argv[2], "r")) == NULL)
        return 1;

    if (init_polyaness(fp, atoi(argv[1]), &pt) < 0) {
        fclose(fp);

        return 2;
    }
    if (parse_polyaness(fp, atoi(argv[1]), &pt) < 0) {
        fclose(fp);
        release_polyaness(pt);
        
        return 3;
    }
    fclose(fp);

    fprintf(stdout, "*** pt->recs = %d ***\n", pt->recs);
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
    i = 0;
    while (i < pt->recs) {
        speaker = get_polyaness("speaker", i, &pt);
        quote = get_polyaness("quote", i, &pt);
        fprintf(stdout, "%s「%s」\n",
                speaker, quote);

        speaker = quote = NULL;
        i++;
    }
    release_polyaness(pt);

    return 0;
}
