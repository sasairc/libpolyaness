libpolyaness
===

[![license](https://img.shields.io/badge/License-MIT%20or%20WTFPL2-blue.svg?style=flat)](https://raw.githubusercontent.com/sasairc/libpolyaness/master/LICENSE)
[![build](https://img.shields.io/travis/sasairc/libpolyaness.svg?style=flat)](https://travis-ci.org/sasairc/libpolyaness)

polyanessもといLTSVパーサ。

## Usage

```c
#include <polyaness.h>

typedef struct POLYANESS_CELL {
    int     keys;
    char**  key;
    char**  value;
} polyaness_cell;

typedef struct POLYANESS_T {
    int                     recs;
    struct POLYANESS_CELL** record;
} polyaness_t;

int init_polyaness(FILE* fp, int offset, polyaness_t** polyaness);
int parse_polyaness(FILE* fp, int offset, polyaness_t** polyaness);
char* get_polyaness(const char* key, int record, polyaness_t** polyaness);
void release_polyaness(polyaness_t* polyaness);
```

## Example

#### LTSV file (text.txt)

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
:
:
:
```

#### Source code

```c
/*
 * example.c
 */

#include <polyaness.h>
#include <stdio.h>

int main(void)
{
    int             i       = 0,
                    j       = 0;

    char*           speaker = NULL,
        *           quote   = NULL;

    FILE*           fp      = NULL;

    polyaness_t*    pt      = NULL;

    if ((fp = fopen("test.txt", "r")) == NULL)
        return 1;

    if (init_polyaness(fp, 0, &pt) < 0) {
        fclose(fp);

        return 2;
    }
    if (parse_polyaness(fp, 0, &pt) < 0) {
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
```

#### Result

```
% gcc example.c -o example -lpolyaness
% ./exsample | headtail --pretty -n 10
*** pt->recs = 49 ***
pt->record[0]->key[0] = filename, pt->record[0]->value[0] = test.txt
pt->record[0]->key[1] = original_author, pt->record[0]->value[1] = keepoff07
pt->record[0]->key[2] = ltsv_author, pt->record[0]->value[2] = 844196
pt->record[1]->key[0] = speaker, pt->record[1]->value[0] = WiiFitU トレーナー
pt->record[1]->key[1] = quote, pt->record[1]->value[1] = うん。とても安定してますよ！
pt->record[2]->key[0] = speaker, pt->record[2]->value[0] = WiiFitU トレーナー
pt->record[2]->key[1] = quote, pt->record[2]->value[1] = お尻を突き出さないように、まっすぐ立ちます。
pt->record[3]->key[0] = speaker, pt->record[3]->value[0] = WiiFitU トレーナー
pt->record[3]->key[1] = quote, pt->record[3]->value[1] = お疲れさまでした。毎日つづけて、身体のゆがみを改善しましょう。
 :
 :
 :
WiiFitU トレーナー「笛にあわせて、すばやく足と上半身をVの字に起こしましょう。」
WiiFitU トレーナー「難しければ、鼻から吸って口から吐きましょう。」
WiiFitU トレーナー「背筋がまっすぐになるイメージを思い浮かべましょう。」
WiiFitU トレーナー「背骨を伸ばすことで、自律神経を整えます。」
WiiFitU トレーナー「鼻から吸って、鼻から吐いて。」
WiiFitU トレーナー「毎日つづけて、身体を引きしめましょう。」
WiiFitU トレーナー「理想の体型を意識して！」
WiiFitU トレーナー「両手を天井に向け、ひざを曲げます。」
WiiFitU トレーナー「力を入れる時に、呼吸を止めないようにしましょう。」
WiiFitU トレーナー「腕が傾いてます。まっすぐ上に向けましょう。」
```


## Ref

* [844196/polyaness - Polyaness 辞書ファイル標準仕様](https://github.com/844196/polyaness/blob/master/dictionary_spec.md)
* [keepoff07/trainer_say.txt - from WiiFitU](https://gist.github.com/keepoff07/b16a61141c1fd8a81c45)


## License

Dual licensed under the MIT or WTFPL Version 2.

See also [LICENSE](https://raw.githubusercontent.com/sasairc/libpolyaness/master/LICENSE)


## Author

sasairc (https://github.com/sasairc)
