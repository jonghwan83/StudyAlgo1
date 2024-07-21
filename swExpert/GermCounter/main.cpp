// main.cpp

#ifndef _CRT_SECURE_NO_WARNINGS

#define _CRT_SECURE_NO_WARNINGS

#endif



#include <stdio.h>

#include <time.h>



static unsigned long long seed = 5;

static int pseudo_rand(void)

{

    seed = seed * 25214903917ULL + 11ULL;

    return (seed >> 16) & 0x3fffffff;

}



/* These constant variables will NOT be changed */

static const long long PENALTY = 10'000'000'000'000LL;

static const int MAX_TC = 10;



static const int MAX_CORE = 16384;

static const int CORE_BUF_SIZE = 1024;



static const int MAX_GERM = 1'000'000;

static const int MAX_COLOR = 16;

static const int MAX_GERM_TYPE = 1000;



static long long SCORE;

static long long gCoreCycle[MAX_CORE];

static int gAnswer;



struct Germs {

    int size;

    int color;

    int direction;

    int degree;

    int shape;

    int usedCore;

};



static struct Germs germs[MAX_GERM], target;



static int gGpuBuffer[MAX_CORE][CORE_BUF_SIZE];

static int gGpuBufferIndex[MAX_CORE];



#define ABS(x) (((x) > 0) ? (x) : -(x))



//////////////////////////



void info(int coreIndex, int id)

{

    if (coreIndex < 0 || coreIndex >= MAX_CORE || id < 0 || id >= MAX_GERM)

        return;



    gCoreCycle[coreIndex] += 1;



    if (germs[id].usedCore == -1 || germs[id].usedCore == coreIndex) {

        germs[id].usedCore = coreIndex;



        int& bufferIndex = gGpuBufferIndex[coreIndex];

        gGpuBuffer[coreIndex][bufferIndex++] = 0; // info

        gGpuBuffer[coreIndex][bufferIndex++] = id;

        gGpuBuffer[coreIndex][bufferIndex++] = germs[id].size;

        gGpuBuffer[coreIndex][bufferIndex++] = germs[id].degree;

        gGpuBuffer[coreIndex][bufferIndex++] = germs[id].color;

    }

}



void rotate(int coreIndex, int id, int degree)

{

    if (coreIndex < 0 || coreIndex >= MAX_CORE || id < 0 || id >= MAX_GERM)

        return;



    gCoreCycle[coreIndex] += 2;



    if (germs[id].usedCore == -1 || germs[id].usedCore == coreIndex) {

        germs[id].usedCore = coreIndex;

        germs[id].degree = (germs[id].degree + degree) % 360;

    }

}



void scale(int coreIndex, int id, int scale)

{

    if (coreIndex < 0 || coreIndex >= MAX_CORE || id < 0 || id >= MAX_GERM)

        return;



    gCoreCycle[coreIndex] += 3;



    if (germs[id].usedCore == -1 || germs[id].usedCore == coreIndex) {

        germs[id].usedCore = coreIndex;

        germs[id].size = germs[id].size + scale;

    }

}



void match(int coreIndex, int id)

{

    if (coreIndex < 0 || coreIndex >= MAX_CORE || id < 0 || id >= MAX_GERM)

        return;



    gCoreCycle[coreIndex] += 5;



    if (germs[id].usedCore == -1 || germs[id].usedCore == coreIndex) {

        germs[id].usedCore = coreIndex;



        int similarity = ABS(target.color - germs[id].color);

        similarity += ABS(target.shape - germs[id].shape);

        similarity += ABS(target.size - germs[id].size);

        int direction = ABS(target.direction - (germs[id].direction + germs[id].degree)) % 360;

        similarity += (direction < 180) ? direction : 360 - direction;



        int& bufferIndex = gGpuBufferIndex[coreIndex];

        gGpuBuffer[coreIndex][bufferIndex++] = 1; // match

        gGpuBuffer[coreIndex][bufferIndex++] = id;

        gGpuBuffer[coreIndex][bufferIndex++] = similarity;

    }

}



void gpu_process(int sharedBuffer[MAX_CORE][CORE_BUF_SIZE])

{

    for (int i = 0; i < MAX_CORE; i++)

        for (int j = 0; j < gGpuBufferIndex[i]; j++)

            sharedBuffer[i][j] = gGpuBuffer[i][j];



    for (int i = 0; i < MAX_CORE; i++)

        gGpuBufferIndex[i] = 0;



    int maxScore = 0;

    for (int coreIndex = 0; coreIndex < MAX_CORE; coreIndex++) {

        if (maxScore < gCoreCycle[coreIndex])

            maxScore = gCoreCycle[coreIndex];

        gCoreCycle[coreIndex] = 0;

    }

    SCORE += (maxScore + 10);



    for (int i = 0; i < MAX_GERM; i++)

        germs[i].usedCore = -1;

}



void result(int answer)

{

    gAnswer = answer;

}



//////////////////////////



static void init()

{

    gAnswer = -1;



    for (int i = 0; i < MAX_GERM; i++) {

        germs[i].size = pseudo_rand() % 10;

        germs[i].direction = pseudo_rand() % 360;

        germs[i].degree = 0;

        germs[i].color = pseudo_rand() % MAX_COLOR;

        germs[i].shape = pseudo_rand() % MAX_GERM_TYPE;

        germs[i].usedCore = -1;

    }



    target.size = pseudo_rand() % 10;

    target.direction = pseudo_rand() % 360;

    target.degree = 0;

    target.color = pseudo_rand() % MAX_COLOR;

    target.shape = pseudo_rand() % MAX_GERM_TYPE;



    for (int i = 0; i < MAX_CORE; i++)

    {

        gGpuBufferIndex[i] = 0;

        gCoreCycle[i] = 0;

    }

}



static bool verify()

{

    int count = 0;



    for (int i = 0; i < MAX_GERM; i++)

        if (target.color == germs[i].color && target.shape == germs[i].shape)

            count++;



    if (count == gAnswer)

        return true;



    return false;

}



extern void process();



int main()

{

    clock_t start = clock();

    setbuf(stdout, NULL);



    SCORE = 0;

    for (int tc = 0; tc < MAX_TC; ++tc) {

        init();



        process();



        if (verify() == false) {

            SCORE = PENALTY;

            // break;

        }

    }

    printf("SCORE: %lld\n", SCORE);

    clock_t end = clock() - start;

    printf("elapsed time: %f\n", (float)end / CLOCKS_PER_SEC);

    return 0;

}


