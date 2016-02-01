libpolyaness
===

polyanessもといLTSVパーサ。

## Usage

```c
#include <polyaness.h>

typedef struct POLYANESS_CELL {
    unsigned int            keys;
    unsigned char**         key;
    unsigned char**         value;
} polyaness_cell;

typedef struct POLYANESS_T {
    unsigned int            recs;
    struct POLYANESS_CELL** record;
} polyaness_t;

int init_polyaness(FILE* fp, polyaness_t** polyaness);
int parse_polyaness(FILE* fp, polyaness_t** polyaness);
void release_polyaness(polyaness_t* polyaness);
```

## Example

### LTSV file (text.txt)

```text
filename:test.txt<TAB>original_author:keepoff07<TAB>ltsv_author:844196
speaker:WiiFitU トレーナー<TAB>quote:うん。とても安定してますよ！
speaker:WiiFitU トレーナー<TAB>quote:お尻を突き出さないように、まっすぐ立ちます。
speaker:WiiFitU トレーナー<TAB>quote:お疲れさまでした。毎日つづけて、身体のゆがみを改善しましょう。
speaker:WiiFitU トレーナー<TAB>quote:かかとを押して尾骨を斜め上に。
speaker:WiiFitU トレーナー<TAB>quote:きれいな姿勢です！
speaker:WiiFitU トレーナー<TAB>quote:きれいな動きです。とても安定してますよ！
speaker:WiiFitU トレーナー<TAB>quote:この時、腰もいっしょに動かすことを意識しましょう。
speaker:WiiFitU トレーナー<TAB>quote:そのままの姿勢で、ゆっくり呼吸しましょう。
speaker:WiiFitU トレーナー<TAB>quote:そのまま少し後ろに伸展します。
```

### Source code

```c
#include "./polyaness.h"
#include <stdio.h>

int main(void)
{
    int             i   = 0,
                    j   = 0;

    FILE*           fp  = NULL;

    polyaness_t*    pt  = NULL;

    if ((fp = fopen("test.txt", "r")) == NULL)
        return 1;

    if (init_polyaness(fp, &pt) < 0) {
        fclose(fp);

        return 2;
    }
    if (parse_polyaness(fp, &pt) < 0) {
        fclose(fp);
        release_polyaness(pt);
        
        return 3;
    }
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
```

### Result

```
% ./sample | head
pt->recs = 49
pt->record[0]->key[0] = filename, pt->record[0]->value[0] = test.txt
pt->record[0]->key[1] = original_author, pt->record[0]->value[1] = keepoff07
pt->record[0]->key[2] = ltsv_author, pt->record[0]->value[2] = 844196
pt->record[1]->key[0] = speaker, pt->record[1]->value[0] = WiiFitU トレーナー
pt->record[1]->key[1] = quote, pt->record[1]->value[1] = うん。とても安定してますよ！
pt->record[2]->key[0] = speaker, pt->record[2]->value[0] = WiiFitU トレーナー
pt->record[2]->key[1] = quote, pt->record[2]->value[1] = お尻を突き出さないように、まっすぐ立ちます。
pt->record[3]->key[0] = speaker, pt->record[3]->value[0] = WiiFitU トレーナー
pt->record[3]->key[1] = quote, pt->record[3]->value[1] = お疲れさまでした。毎日つづけて、身体のゆがみを改善しましょう。
```


## License

[MIT](https://github.com/sasairc/libncipher/blob/master/LICENSE)


## Author

sasairc (https://github.com/sasairc)
