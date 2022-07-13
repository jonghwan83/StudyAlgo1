from heapq import heappush, heappop
# 큐에 인덱스가 들어가서 풀어야함
class Product:
    def __init__(self):
        self.mProduct = []
        self.mQuantity = []

class BuyOrder:
    def __init__(self) -> None:
        self.bId = []
        self.price = []
        self.firstStockedQuantity = []
        self.currentStockedQuantity = []
        self.isCanceled = []
        self.pIdx = []

class SellOrder:
    def __init__(self) -> None:
        self.sId = []
        self.mQuantity = []
        self.isRefunded = []
        self.subOrderCount = []
        self.subOrder = SubOrder()

    def addSubOrder(self, bIdx, qty):
        self.subOrder.bIdx.append(bIdx)
        self.subOrder.qty.append(qty)
        

class SubOrder:
    def __init__(self) -> None:
        self.bIdx = []
        self.qty = []

def getProductIdx(mProduct):
    global pwp
    for i in range(pwp):
        if product.mProduct[i] == mProduct:
            return i
    product.mProduct.append(mProduct)
    product.mQuantity.append(0)
    pwp += 1
    return pwp - 1


product = Product()
buyOrder = BuyOrder()
sellOrder = SellOrder()
buyDict = {}
sellDict = {}

queue = []

def init():
    global bIdx, pwp, swp
    product.__init__()
    buyOrder.__init__()
    sellOrder.__init__()
    pwp = 0
    swp = 0
    bIdx = 0
    return

def buy(bId, mProduct, mPrice, mQuantity):
    global bIdx

    buyOrder.bId.append(bId)
    buyOrder.price.append(mPrice)
    buyOrder.isCanceled.append(False)
    buyOrder.firstStockedQuantity.append(mQuantity)
    buyOrder.currentStockedQuantity.append(mQuantity)
    buyOrder.pIdx.append(getProductIdx(mProduct))
    product.mQuantity[buyOrder.pIdx[bIdx]] += mQuantity

    heappush(queue, [mPrice, bId])

    buyDict[bId] = bIdx

    result = product.mQuantity[buyOrder.pIdx[bIdx]]
    
    bIdx += 1

    print(result)
    return result

def cancel(bId):
    if buyDict.get(bId):
        bIdx = buyDict[bId]
    else:
        print(-1)
        return -1

    if buyOrder.isCanceled[bIdx]:
        print(-1)
        return -1
    
    if buyOrder.currentStockedQuantity[bIdx] != buyOrder.firstStockedQuantity[bIdx]:
        print(-1)
        return -1

    buyOrder.isCanceled[bIdx] = 1
    product.mQuantity[buyOrder.pIdx[bIdx]] -= buyOrder.currentStockedQuantity[bIdx]

    print(product.mQuantity[buyOrder.pIdx[bIdx]])
    return product.mQuantity[buyOrder.pIdx[bIdx]]

def sell(sId, mProduct, mPrice, mQuantity):
    global swp
    pIdx = getProductIdx(mProduct)
    
    if product.mQuantity[pIdx] < mQuantity:
        print(-1)
        return -1

    sellOrder.sId.append(sId)
    sellOrder.isRefunded.append(0)
    sellOrder.mQuantity.append(0)
    sellOrder.subOrderCount.append(0)

    profit = 0
    while mQuantity > 0:
        _, bId = heappop(queue)
        bIdx = buyDict[bId]
        if buyOrder.isCanceled[bIdx]:
            continue
        qty = min(buyOrder.currentStockedQuantity[bIdx], mQuantity)
        mQuantity -= qty
        buyOrder.currentStockedQuantity[bIdx] -= qty
        product.mQuantity[pIdx] -= qty
        sellOrder.addSubOrder(bIdx, qty)
        sellOrder.mQuantity[swp] += qty
        sellOrder.subOrderCount[swp] += 1
        profit += (mPrice - buyOrder.price[bIdx]) * qty
        print('profit', qty, profit, buyOrder.price[bIdx])
        if buyOrder.currentStockedQuantity[bIdx] > 0:
            heappush(queue, [buyOrder.price[bIdx], bId])

    swp += 1
    
    print(profit)
    return profit

def refund(sId):

    return