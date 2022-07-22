from heapq import heappush, heappop

stockPrices = []
stockIdx = {}

def init(N, mStocks, mPrices):
    global stockPrices, stockIdx
    stockPrices.clear()

    temp = []
    for i in range(N):
        heappush(temp, [(mStocks[i], i), int(mPrices[i])])

    i = 0
    while temp:
        temp_list = heappop(temp)
        stockPrices.append(temp_list)
        stockIdx[temp_list[0][0]] = i
        i += 1
        
    return

def changePrices(mFromStock, mToStock, mPriceDiff):
    global stockPrices, stockIdx

    mFromIdx = stockIdx[mFromStock]
    mToIdx = stockIdx[mToStock]

    for i in range(mFromIdx, mToIdx+1):
        lastPrice = stockPrices[i][-1]
        stockPrices[i].append(lastPrice + int(mPriceDiff))

    return

def getPrice(mStock):
    global stockPrices, stockIdx
    mIdx = stockIdx[mStock]

    return stockPrices[mIdx][-1]

def getMostIncreasedStock(mFromStock, mToStock):
    global stockPrices, stockIdx
    
    mFromIdx = stockIdx[mFromStock]
    mToIdx = stockIdx[mToStock]

    cdiff = stockPrices[mFromIdx][-1] - stockPrices[mFromIdx][1]
    cStock = stockPrices[mFromIdx][0][0]
    cIdx = mFromIdx
    for i in range(mFromIdx, mToIdx+1):
        diffPrice = stockPrices[i][-1] - stockPrices[i][1]
        if cdiff < diffPrice:
            cIdx = i
            cdiff = diffPrice
            cStock = stockPrices[i][0][0]
        elif cdiff == diffPrice:
            if stockPrices[i][0][1] < stockPrices[cIdx][0][1]:
                cIdx = i
                cdiff = diffPrice
                cStock = stockPrices[i][0][0]

    return stockPrices[stockIdx[cStock]][0][1]
