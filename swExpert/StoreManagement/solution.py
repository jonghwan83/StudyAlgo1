from heapq import heappush, heappop

class BuyOrder:
    def __init__(self):
        self.n = 0
        self.mProduct = []
        self.mPrice = []
        self.bId = []
        self.firstStockedQuantity = []
        self.currentQuantity = []
        self.isCanceled = []

    def addOrder(self, bId, mProduct, mPrice, mQuantity):
        buyOrder.mProduct.append(mProduct)
        buyOrder.mPrice.append(mPrice)
        buyOrder.bId.append(bId)
        buyOrder.firstStockedQuantity.append(mQuantity)
        buyOrder.currentQuantity.append(mQuantity)
        buyOrder.isCanceled.append(False)
        self.n += 1

class SellOrder:
    def __init__(self):
        self.n = 0
        self.mProduct = []
        self.sId = []
        self.isRefunded = []
        self.bIds = []
        self.qty = []

    def addOrder(self, sId, mProduct):
        self.sId.append(sId)
        self.mProduct.append(mProduct)
        self.bIds.append([])
        self.qty.append([])
        self.isRefunded.append(False)
        self.n += 1

class Stock:
    def __init__(self):
        self.mProduct = []
        self.mQuantity = []
        self.pQueue = [] # [mPrice, bId]
    
    def getpIdx(self, mProduct):
        for pIdx, val in enumerate(self.mProduct):
            if mProduct == self.mProduct[pIdx]:
                return pIdx

        self.mProduct.append(mProduct)
        self.mQuantity.append(0)
        self.pQueue.append([])

        return len(self.mProduct) - 1


buyOrder = BuyOrder()
sellOrder = SellOrder()
buyId = {} # bId: [mProduct, bIdx]
sellId = {} # sId: [mProduct, sIdx]
stock = Stock()

def init():
    buyOrder.__init__()
    sellOrder.__init__()
    stock.__init__()
    buyId.clear()
    sellId.clear()
    return

def buy(bId, mProduct, mPrice, mQuantity):
    bIdx = buyOrder.n

    if buyId.get(bId):
        return -1
    else:
        buyId[bId] = [mProduct, bIdx]

    buyOrder.addOrder(bId, mProduct, mPrice, mQuantity)

    pIdx = stock.getpIdx(mProduct)
    stock.mQuantity[pIdx] += mQuantity
    heappush(stock.pQueue[pIdx], [mPrice, bId])
    
    return stock.mQuantity[pIdx]

def cancel(bId):
    if buyId.get(bId):
        mProduct, bIdx = buyId[bId]
    else:
        return -1

    if buyOrder.isCanceled[bIdx]:
        return -1
    
    if buyOrder.firstStockedQuantity[bIdx] != buyOrder.currentQuantity[bIdx]:
        return -1
    
    buyOrder.isCanceled[bIdx] = True

    pIdx = stock.getpIdx(mProduct)
    buyOrder.currentQuantity[bIdx] -= buyOrder.firstStockedQuantity[bIdx]
    stock.mQuantity[pIdx] -= buyOrder.firstStockedQuantity[bIdx]
    
    return stock.mQuantity[pIdx]

def sell(sId, mProduct, mPrice, mQuantity):
    pIdx = stock.getpIdx(mProduct)
    if stock.mQuantity[pIdx] < mQuantity:
        return -1

    sIdx = sellOrder.n
    sellOrder.addOrder(sId, mProduct)
    sellId[sId] = [mProduct, sIdx]

    profit = 0
    while mQuantity > 0:
        buyPrice, bId = heappop(stock.pQueue[pIdx])
        _, bIdx = buyId[bId]
        buyQuantity = buyOrder.currentQuantity[bIdx]
        qty = min(mQuantity, buyQuantity)

        mQuantity -= qty
        buyOrder.currentQuantity[bIdx] -= qty
        stock.mQuantity[pIdx] -= qty
        
        sellOrder.bIds[sIdx].append(bId)
        sellOrder.qty[sIdx].append(qty)

        profit += (mPrice - buyPrice) * qty

    if buyOrder.currentQuantity[bIdx] > 0:
        heappush(stock.pQueue[pIdx], [buyPrice, bId])
    
    return profit

def refund(sId):
    if sellId.get(sId):
        mProduct, sIdx = sellId[sId]
        pIdx = stock.getpIdx(mProduct)
    else:

        return -1

    if sellOrder.isRefunded[sIdx]:
        return -1

    nRefunded = 0
    sellOrder.isRefunded[sIdx] = True
    for bId, qty in zip(sellOrder.bIds[sIdx], sellOrder.qty[sIdx]):
        mProduct, bIdx = buyId[bId]
        buyPrice = buyOrder.mPrice[bIdx]

        buyOrder.currentQuantity[bIdx] += qty
        stock.mQuantity[pIdx] += qty
        nRefunded += qty
        heappush(stock.pQueue[pIdx], [buyPrice, bId])

    return nRefunded
