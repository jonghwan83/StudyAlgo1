static const int MAX_CORE = 16384;

static const int MAX_GERM = 1'000'000;

static const int CORE_BUF_SIZE = 1024;


extern void info(int coreIndex, int id);

extern void rotate(int coreIndex, int id, int degree);

extern void scale(int coreIndex, int id, int scale);

extern void match(int coreIndex, int id);

extern void gpu_process(int sharedBuffer[MAX_CORE][CORE_BUF_SIZE]);

extern void result(int anwer);



struct Germ
{
    int isTarget;   // 0: false, 1: true, -1: unknown

    int size;

    int color;

    int degree;

    int similarity1;

    int similarity2;

    int usedCore;

    void init()
    {
        isTarget = -1;
        size = -1;
        color = -1;
        degree = 0;
        similarity1 = -1;
        similarity2 = -1;
        usedCore = -1;

        // first rotate: 180 - degree, second rotate: -degree * 2
    }

    void getDegree()
    {
        if (similarity1 == -1) { return; }

        int diff = (similarity2 - similarity1) / 2;

        if (similarity2 > similarity1)
            degree = 90 - diff;
        else
            degree = 90 + diff;

    }

    void initSimilarity()
    {
        similarity1 = -1;
        similarity2 = -1;
    }
};



int buffer[MAX_CORE][CORE_BUF_SIZE];

Germ germData[MAX_GERM];

int targetColor, targetSize;

int currCoreIdx, currInfoIdx, currCheckIdx;

int res;

void initBuffer()
{
    for (int i = 0; i < MAX_CORE; i++)
    {
        for (int j = 0; j < CORE_BUF_SIZE; j++)
        {
            buffer[i][j] = -1;
        }
    }
}

void clearBuffer()
{
    int coreIdx = 0;

    while (coreIdx < MAX_CORE)
    {
        int bufIdx = 0;

        while (bufIdx < CORE_BUF_SIZE)
        {
            if (buffer[coreIdx][bufIdx] == -1) { break; }

            if (buffer[coreIdx][bufIdx] == 0) // info
            {
                int idx = buffer[coreIdx][bufIdx + 1];
                germData[idx].size = buffer[coreIdx][bufIdx + 2];
                germData[idx].degree = buffer[coreIdx][bufIdx + 3];
                germData[idx].color = buffer[coreIdx][bufIdx + 4];

                for (int i = bufIdx; i < bufIdx + 5; i++)
                {
                    buffer[coreIdx][i] = -1;
                }

                bufIdx += 5;
            }
            else if (buffer[coreIdx][bufIdx] == 1) // match
            {
                int idx = buffer[coreIdx][bufIdx + 1];

                if (idx == 32224)
                    int a = 0;

                if (germData[idx].similarity1 == -1)
                {
                    germData[idx].similarity1 = buffer[coreIdx][bufIdx + 2];
                }

                else
                {
                    germData[idx].similarity2 = buffer[coreIdx][bufIdx + 2];
                }

                if (germData[idx].similarity1 >= 370 || germData[idx].similarity2 >= 370)
                    germData[idx].isTarget = 0;

                if (germData[idx].similarity2 == 0 || germData[idx].similarity1 == 0)
                {
                    if (germData[idx].isTarget != 1)
                    {
                        germData[idx].isTarget = 1;
                        if (targetColor == -1) { targetColor = germData[idx].color; }
                        res++;
                    }

                }

                buffer[coreIdx][bufIdx] = -1;
                buffer[coreIdx][bufIdx + 1] = -1;
                buffer[coreIdx][bufIdx + 2] = -1;

                bufIdx += 3;
            }
        }

        coreIdx++;
    }
}

void getData()
{
    gpu_process(buffer);
    clearBuffer();
    currCoreIdx = 0;
}

void calDegree(int st, int ed)
{
    for (int i = st; i < ed; i++)
    {
        germData[i].getDegree();
    }
}

void getTargetSize()
{
    int idx = -1;
    for (int i = 0; i < MAX_CORE; i++)
    {
        if (germData[i].size == 9)
        {
            idx = i;
            break;
        }
    }

    match(currCoreIdx, idx);    // +5
    scale(currCoreIdx, idx, -germData[idx].size);   // +3
    match(currCoreIdx, idx);    // +5
    currCoreIdx++;

    while (currCoreIdx < MAX_CORE)
    {
        for (int i = 0; i < 13; i++)
        {
            info(currCoreIdx, currInfoIdx++);
        }
        currCoreIdx++;
    }

    getData();

    int diff = germData[idx].similarity2 - germData[idx].similarity1;

    targetSize = (diff + 9) / 2;

    germData[idx].initSimilarity();
}



void process()
{
    for (int i = 0; i < MAX_GERM; i++)
        germData[i].init();

    res = 0;

    initBuffer();

    targetSize = targetColor = -1;

    currCoreIdx = currCheckIdx = 0;

    for (int i = 0 ; i < MAX_CORE; i++)
    {
        info(i, i);
    }

    currInfoIdx = MAX_CORE;

    gpu_process(buffer);

    clearBuffer();

    getTargetSize();


    while (currCheckIdx < MAX_GERM)
    {
        if (targetColor > -1) { break; }

        // scale to target
        for (int i = currCheckIdx; i < currInfoIdx; i++)
        {
            if (germData[i].size == -1) { break;}

            if (germData[i].size != targetSize)
            {
                scale(currCoreIdx, i, targetSize - germData[i].size);   // +3
                germData[i].size = targetSize;
            }

            match(currCoreIdx, i);  // +5

            currCoreIdx++;
            if (currCoreIdx == MAX_CORE) { break; }
        }

        while (currCoreIdx < MAX_CORE)
        {
            for (int i = 0; i < 8; i++)
            {
                info(currCoreIdx, currInfoIdx++);
            }
            currCoreIdx++;
        }

        gpu_process(buffer);
        getData();

        // rotate and match

        for (int i = currCheckIdx; i < currInfoIdx; i++)
        {
            currCheckIdx = i;

            if (targetColor > -1)
                if (germData[i].color != targetColor) { continue; }
            if (germData[i].similarity1 >= 370) { continue; }
            if (germData[i].similarity1 == -1) { break; }
            if (germData[i].size != targetSize) { break; }

            int degree = germData[i].similarity1;
            germData[i].initSimilarity();

            rotate(currCoreIdx, i, degree); // +2
            match(currCoreIdx, i);  // +5
            rotate(currCoreIdx, i, -2 * degree);    // +2
            match(currCoreIdx, i);  //+5

            currCoreIdx++;
            if (currCoreIdx == MAX_CORE) { break; }
        }

        while (currCoreIdx < MAX_CORE)
        {
            for (int i = 0; i < 14; i++)
            {
                info(currCoreIdx, currInfoIdx++);
            }
            currCoreIdx++;
        }

        gpu_process(buffer);
        getData();
    }

    for (int i = currInfoIdx; i < MAX_GERM; i++)
    {
        currInfoIdx++;
        info(currCoreIdx, i);
        currCoreIdx++;
        if (currCoreIdx == MAX_CORE) { currCoreIdx = 0; }
    }

    gpu_process(buffer);
    getData();

    for (int i = currCheckIdx; i < MAX_GERM; i++)
    {
        if (germData[i].color != targetColor) { continue; }

        if (germData[i].size != targetSize)
        {
            scale(currCoreIdx, i, targetSize - germData[i].size);   // +3
            germData[i].size = targetSize;
        }

        match(currCoreIdx, i);  // +5

        currCoreIdx++;
        if (currCoreIdx == MAX_CORE) { currCoreIdx = 0; }
    }

    gpu_process(buffer);
    getData();

    for (int i = currCheckIdx; i < MAX_GERM; i++)
    {
        if (germData[i].color != targetColor) { continue; }
        if (germData[i].similarity1 >= 370) { continue; }


        int degree = germData[i].similarity1;
        germData[i].initSimilarity();

        rotate(currCoreIdx, i, degree); // +2
        match(currCoreIdx, i);  // +5
        rotate(currCoreIdx, i, -2 * degree);    // +2
        match(currCoreIdx, i);  //+5

        currCoreIdx++;
        if (currCoreIdx == MAX_CORE) { currCoreIdx = 0; }
    }

    gpu_process(buffer);
    clearBuffer();


    result(res);
}