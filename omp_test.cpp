/*****************************/
/* 行列積 */
/*****************************/
#include <iostream>
#include <stdio.h>
#include <math.h>
#include <random>
#include <chrono>
#include <assert.h>
#include <omp.h>

/* デバッグ用 */
// #define DEBUG

/* 関数切り替え */
#define PARALLEL_ROW
// #define PARALLEL_COL
// #define SERIAL
// #define SERIAL_CASH // キャッシュ考慮

/* 行列サイズ */
#ifdef DEBUG
#define ROW_A 2
#define COL_A 2
#define ROW_B COL_A
#define COL_B 2
#else
#define ROW_A 5000
#define COL_A 5000  
#define ROW_B COL_A
#define COL_B 5000
#endif

// 実行回数
#define TRY 1

#ifdef SERIAL
/* 行列行列積(GEMM) */
inline void gemm (int** a, int** b, long** c) {
#ifdef SERIAL_CASH
    for(size_t i=0; i<ROW_A; i++) {
        for(size_t j=0; j<ROW_B; j++) {
            for(size_t k=0; k<COL_B; k++) {
                c[i][k] += (long)a[i][j] * (long)b[j][k];
            }
        }
    }
#else
    for(size_t i=0; i<ROW_A; i++) {
        for(size_t k=0; k<COL_B; k++) {
            for(size_t j=0; j<ROW_B; j++) {
                c[i][k] += (long)a[i][j] * (long)b[j][k];
            }
        }
    }
#endif
}
#endif

#ifdef PARALLEL_ROW
/* 行列行列積(GEMM)_OpenMP_ROW */
inline void gemm (int** a, int** b, long** c) {
    size_t i, j, k;
    #pragma omp parallel for private(j, k)
    for(i=0; i<ROW_A; i++) {
        for(j=0; j<ROW_B; j++) {
            for(k=0; k<COL_B; k++) {
                c[i][k] += (long)a[i][j] * (long)b[j][k];
            }
        }
    }
}
#endif

#ifdef PARALLEL_COL
/* 行列行列積(GEMM)_OpenMP_COL */
#endif


void func_print() {
    #ifdef SERIAL
    #ifdef SERIAL_CASH
    printf("GEMM(serial_cash)\n");
    #else
    printf("GEMM(serial)\n");
    #endif
    #endif
    #ifdef PARALLEL_ROW
    printf("GEMM_行並列\n");
    #endif
    #ifdef PARALLEL_COL
    printf("GEMM_列並列\n");
    #endif
    printf("行列サイズ(ROW_A) : %u\n", ROW_A);
    printf("試行回数 : %u\n", TRY);
    printf("最大スレッド数 : %d\n", omp_get_max_threads());
    omp_set_num_threads(8);
    printf("最大スレッド数 : %d\n", omp_get_max_threads());
    #ifdef DEBUG
    printf("rand()MAX : %u\n", RAND_MAX);
    printf("sizeof(int) : %lu\n", sizeof(int));
    printf("sizeof(long) : %lu\n", sizeof(long));
    printf("sizeof(long long) : %lu\n", sizeof(long long));
    #endif
    printf("\n");
}


/* main */
int main (void) {
    /* 初期化 */
    func_print();
    assert(COL_A == ROW_B);
    srand(0);
    std::chrono::system_clock::time_point  start, end;
    uint64_t time = 0;

    /* メモリ確保 */
    int** a;    // 入力行列A InputMatrixA
    int** b;    // 入力行列B InputMatrixB
    long** c;   // 出力行列C OutputMatrixC
    // 行先頭位置
    a = (int**) malloc(sizeof(int*) * ROW_A);
    b = (int**) malloc(sizeof(int*) * ROW_B);
    c = (long**) malloc(sizeof(long*) * ROW_A);
    // 行配列
    for(size_t i=0; i<ROW_A; i++) {
        a[i] = (int*) malloc(sizeof(int) * COL_A);
        c[i] = (long*) malloc(sizeof(long) * COL_B);
    }
    for(size_t i=0; i<ROW_B; i++) {
        b[i] = (int*) malloc(sizeof(int) * COL_B);
    }
    
    /* 試行 */
    for(size_t i=0; i<TRY; i++) {
        /* 初期値の決定 */
        #ifdef DEBUG
        a[0][0] = 1; a[0][1] = 3; a[1][0] = 2; a[1][1] = 4;
        b[0][0] = 2; b[0][1] = 4; b[1][0] = 2; b[1][1] = 5;
        #else
        for(size_t i=0; i<ROW_A; i++) {
            for(unsigned int j=0; j<COL_A; j++) {
                a[i][j] = rand();
            }
        }
        for(size_t i=0; i<ROW_B; i++) {
            for(unsigned int j=0; j<COL_B; j++) {
                b[i][j] = rand();
            }
        }
        #endif
        for(size_t i=0; i<ROW_A; i++) {
            for(size_t j=0; j<COL_B; j++) {
                c[i][j] = 0;
            }
        }
        /* 行列-行列乗算 GEMM(General Matrix Multiply)*/
        start = std::chrono::system_clock::now();   // 計測開始時間
        // 行列行列積(GEMM)_シリアル
        gemm(a, b, c);
        end = std::chrono::system_clock::now();     // 計測終了時間
        time += (std::chrono::duration_cast<std::chrono::nanoseconds>(end-start).count());
    }

#ifdef DEBUG
    /* 行列内要素確認 */
    printf("出力行列 : C\n");
    for(size_t i=0; i<ROW_A; i++) {
        for(size_t j=0; j<COL_B; j++) {
            printf("%ld, ", c[i][j]);
        }
        printf("\n");
    }
    /* c正解入力 */
    c[0][0] = 8; c[0][1] = 19; c[1][0] = 12; c[1][1] = 28;
    printf("正解行列 : C\n");
    for(size_t i=0; i<ROW_A; i++) {
        for(size_t j=0; j<COL_B; j++) {
            printf("%ld, ", c[i][j]);
        }
        printf("\n");
    }
#endif

    /* 結果表示 */
    printf("GEMM実行時間 : %lf ns\n", (double)time/TRY);
    printf("GEMM実行時間 : %lf ms\n", (double)((double)time/TRY)/1000000);

    /* メモリ解放 */
    for(size_t i=0; i<ROW_A; i++) {
        free(a[i]);
        free(c[i]);
    }
    for(size_t i=0; i<ROW_B; i++) {
        free(b[i]);
    }
    free(a);
    free(b);
    free(c);

    return 0;
}
/*-- main --*/
