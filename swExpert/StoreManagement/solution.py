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

class Stock:
    def __init__(self):
        self.product = []
        self.quantity = []

buyOrder = BuyOrder()
buyId = {} # bId - mProduct

def init():
    buyOrder.__init__()
    buyId.clear()
    return

def buy(bId, mProduct, mPrice, mQuantity):
    bIdx = buyOrder.n

    if buyId.get(bId):
        return -1
    else:
        buyId[bId] = mProduct

    buyOrder.mProduct.append(mProduct)
    buyOrder.mPrice.append(mPrice)
    buyOrder.bId.append(bId)
    buyOrder.firstStockedQuantity.append(mQuantity)
    buyOrder.currentQuantity.append(mQuantity)
    buyOrder.isCanceled.append(False)
    return 

def cancel(bId):

    return 

def sell(sId, mProduct, mPrice, mQuantity):

    return 

def refund(sId):

    return
