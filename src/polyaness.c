/*
 * polyaness - standard polyaness parse library for C.
 * 
 * Copyright (c) 2016 sasairc
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE
 * OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "./polyaness.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFLEN  4096

#define LF      0x0a
#define TAB     0x09
#define COL     0x3a

typedef unsigned char   uchar;
typedef unsigned int    uint;

int count_keys(const unsigned char* str);
int add_data_polyaness(int record, int keys, const unsigned char* str, polyaness_t*** polyaness);

int init_polyaness(FILE* fp, polyaness_t** polyaness)
{
    if (fp == NULL)
        return -1;

    uint            i       = 0,
                    code    = 0,
                    recs    = 0;

    polyaness_t*    pt      = NULL;

    rewind(fp);
    while ((code = fgetc(fp)) != EOF)
        if (code == LF)
            recs++;

    if ((pt = (polyaness_t*)
            malloc(sizeof(polyaness_t))) == NULL)
        return -1;

    if ((pt->record = (polyaness_cell**)
            malloc(sizeof(polyaness_cell*) * (recs + 1))) == NULL)
        goto    ERR;

    pt->recs = recs;
    while (i <= recs) {
        if ((pt->record[i] = (polyaness_cell*)
                malloc(sizeof(polyaness_cell))) == NULL)
            goto    ERR;

        pt->record[i]->key = NULL;
        pt->record[i]->key = NULL;

        i++;
    }
    *polyaness = pt;

    return 0;

ERR:

    if (pt->record != NULL) {
        while (i >= 0) {
            if (pt->record[i] != NULL) {
                free(pt->record[i]);
                pt->record[i] = NULL;
            }
            i--;
        }
        free(pt->record);
        pt->record = NULL;
    }
    if (pt != NULL) {
        free(pt);
        pt = NULL;
    }

    return -1;
}

int parse_polyaness(FILE* fp, polyaness_t** polyaness)
{
    if (fp == NULL || *polyaness == NULL)
        return -1;

    uint            i       = 0,
                    code    = 0,
                    recs    = 0;

    uchar*          buf     = NULL;

    size_t          len     = 0,
                    x_bufl  = BUFLEN;

    polyaness_t*    pt      = NULL;

    if ((buf = (uchar*)
            malloc(sizeof(uchar) * x_bufl)) == NULL)
        return -1;

    rewind(fp);
    while ((code = fgetc(fp)) != EOF) {
        if (code == LF) {
            if (add_data_polyaness(recs, count_keys(buf), buf, &polyaness) < 0)
                goto ERR;
            memset(buf, '\0', strlen(buf));
            recs++;
            len = 0;
        } else {
            if (len == (x_bufl - 1)) {
                x_bufl += BUFLEN;
                if ((buf = (uchar*)
                        realloc(buf, sizeof(uchar) * x_bufl)) == NULL)
                    goto ERR;
            }
            buf[len] = code;
            len++;
        }
    }
    free(buf);

    return 0;

ERR:

    if (buf != NULL) {
        free(buf);
        buf = NULL;
    }

    return -1;
}

int count_keys(const uchar* str)
{
    uint    keys    = 1;

    while (*str != '\0') {
        if (*str == TAB)
            keys++;
        str++;
    }

    return keys;
}

int add_data_polyaness(int record, int keys, const uchar* str, polyaness_t*** polyaness)
{
    uint    i       = 0;

    size_t  head    = 0,
            tail    = 0;

    (*(*polyaness))->record[record]->key = (uchar**)
        malloc(sizeof(uchar*) * keys);
    (*(*polyaness))->record[record]->value = (uchar**)
        malloc(sizeof(uchar*) * keys);
    
    if ((*(*polyaness))->record[record]->key == NULL ||
        (*(*polyaness))->record[record]->value == NULL)
        return -1;
        
    (*(*polyaness))->record[record]->keys = keys;
    while (i < keys) {
        if (str[head] == COL) {
            (*(*polyaness))->record[record]->key[i] = (uchar*)
                malloc(sizeof(uchar) * (head - tail + 1));

            memcpy((*(*polyaness))->record[record]->key[i],
                    str + tail, head - tail);
            (*(*polyaness))->record[record]->key[i][head - tail + 1] = '\0';
            head++;
            tail = head;
        } else if (str[head] == TAB || str[head] == '\0') {
            (*(*polyaness))->record[record]->value[i] = (uchar*)
                malloc(sizeof(uchar) * (head - tail + 1));

            memcpy((*(*polyaness))->record[record]->value[i],
                    str + tail, head - tail);
            (*(*polyaness))->record[record]->value[i][head - tail + 1] = '\0';
            head++;
            tail = head;
            i++;
        }
        head++;
    }

    return 0;
}

char* get_polyaness(const unsigned char* key, unsigned int record, polyaness_t** polyaness)
{
    if (polyaness == NULL   ||
            key == NULL     ||
            (*polyaness)->recs < record)
        return NULL;

    uint    i       = 0;
    
    uchar*  match   = NULL;

    while (i < (*polyaness)->record[record]->keys) {
        if (strcmp((*polyaness)->record[record]->key[i], key) == 0) {
            match = (*polyaness)->record[record]->value[i];
            break;
        }
        i++;
    }

    return match;
}

void release_polyaness(polyaness_t* polyaness)
{
    if (polyaness == NULL)
        return;

    uint    i   = 0,
            j   = 0;

    while (i < polyaness->recs) {
        if (polyaness->record[i]->key != NULL) {
            j = 0;
            while (j < polyaness->record[i]->keys) {
                if (polyaness->record[i]->key[j] != NULL)
                    free(polyaness->record[i]->key[j]);
                if (polyaness->record[i]->value[j] != NULL)
                    free(polyaness->record[i]->value[j]);
                j++;
            }
            if (polyaness->record[i]->key != NULL)
                free(polyaness->record[i]->key);
            if (polyaness->record[i]->value != NULL)
                free(polyaness->record[i]->value);
        }
        if (polyaness->record[i] != NULL)
            free(polyaness->record[i]);
        i++;
    }
    if (polyaness->record != NULL)
        free(polyaness->record);

    free(polyaness);
    polyaness = NULL;

    return;
}
