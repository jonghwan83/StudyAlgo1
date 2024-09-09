struct Result {
    int mCost;
    int mContent;
};


#define N_MINE 2
#define N_TYPE 3
#define MAX_MINERAL 3500
#define MAX_COST 300'001

struct Mineral
{
    int cost;
    int content;
    bool isRemoved;

    Mineral()
    {
        cost = MAX_COST;
        content = 0;
        isRemoved = false;
    }
};



int shipFee;

Mineral minerals[N_MINE][N_TYPE][MAX_MINERAL];

int idx_minerals[N_MINE][N_TYPE];

int cnt_minerals[N_MINE][N_TYPE];

int max_minerals;

int max(int a, int b) { return a > b? a: b; }

int min(int a, int b) { return a < b? a: b; }

void init(int mShipFee) {

    max_minerals = 0;
    shipFee = mShipFee;

    for (int mine = 0; mine < N_MINE; mine++)
    {
        for (int type = 0; type < N_TYPE; type++)
        {
            idx_minerals[mine][type] = 0;
            cnt_minerals[mine][type] = 0;
        }
    }

    return;
}


int gather(int mMineId, int mType, int mCost, int mContent) {

    int idx = idx_minerals[mMineId][mType];

    minerals[mMineId][mType][idx].cost = mCost;
    minerals[mMineId][mType][idx].content = mContent;
    minerals[mMineId][mType][idx].isRemoved = false;

    idx_minerals[mMineId][mType]++;
    cnt_minerals[mMineId][mType]++;

    max_minerals = max(max_minerals, mContent);

    return cnt_minerals[mMineId][mType];
}



Result mix(int mBudget) {

    Result res = { 0, 0 };

    int st = 0;
    int ed = max_minerals;

    Mineral candidate[N_MINE][N_TYPE];
    int idx_candidate[N_MINE][N_TYPE];

    int idx0, idx1, idx2;
    int mine0, mine1, mine2;

    while (st != ed)
    {
        int mid = (st + ed) / 2;

        for (int mine = 0; mine < N_MINE; mine++)
        {
            for (int type = 0; type < N_TYPE; type++)
            {
                candidate[mine][type].cost = MAX_COST;
                idx_candidate[mine][type] = -1;
            }
        }

        for (int mine = 0; mine < N_MINE; mine++)
        {
            for (int type = 0; type < N_TYPE; type++)
            {
                int length = idx_minerals[mine][type];

                for (int i = 0; i < length; i++)
                {
                    if (minerals[mine][type][i].isRemoved) { continue; }

                    if (minerals[mine][type][i].content > mid)
                    {
                        if (minerals[mine][type][i].cost < candidate[mine][type].cost)
                        {
                            candidate[mine][type].cost = minerals[mine][type][i].cost;
                            candidate[mine][type].content = minerals[mine][type][i].content;
                            idx_candidate[mine][type] = i;
                        }
                    }
                }
            }
        }

        //
        int cost0 = candidate[0][0].cost + candidate[0][1].cost + candidate[0][2].cost + shipFee;
        int cost1 = candidate[1][0].cost + candidate[1][1].cost + candidate[1][2].cost + shipFee;
        int cost2 = min(candidate[0][0].cost, candidate[1][0].cost) + min(candidate[0][1].cost, candidate[1][1].cost)
                + min(candidate[0][2].cost, candidate[1][2].cost) + 2 * shipFee;

        int min_cost = min(min(cost0, cost1), cost2);

        if (min_cost <= mBudget)
        {
            st = mid + 1;

            if (min_cost == cost0)
            {
                res.mCost = min_cost;
                res.mContent = min(min(candidate[0][0].content, candidate[0][1].content), candidate[0][2].content);
                mine0 = mine1 = mine2 = 0;
            }
            else if (min_cost == cost1)
            {
                res.mCost = min_cost;
                res.mContent = min(min(candidate[1][0].content, candidate[1][1].content), candidate[1][2].content);
                mine0 = mine1 = mine2 = 1;
            }
            else
            {
                res.mCost = min_cost;

                if (candidate[0][0].cost < candidate[1][0].cost) { mine0 = 0 ; }
                else { mine0 = 1; }
                if (candidate[0][1].cost < candidate[1][1].cost) { mine1 = 0 ; }
                else { mine1 = 1; }
                if (candidate[0][2].cost < candidate[1][2].cost) { mine2 = 0 ; }
                else { mine2 = 1; }

                res.mContent = min(min(candidate[mine0][0].content, candidate[mine1][1].content), candidate[mine2][2].content);
            }

            idx0 = idx_candidate[mine0][0];
            idx1 = idx_candidate[mine1][1];
            idx2 = idx_candidate[mine2][2];

        }
        else
        {
            ed = mid;
        }

    }

    if (res.mCost == 0) { return res; }

    minerals[mine0][0][idx0].isRemoved = true;
    minerals[mine1][1][idx1].isRemoved = true;
    minerals[mine2][2][idx2].isRemoved = true;

    cnt_minerals[mine0][0]--;
    cnt_minerals[mine1][1]--;
    cnt_minerals[mine2][2]--;

    return res;
}