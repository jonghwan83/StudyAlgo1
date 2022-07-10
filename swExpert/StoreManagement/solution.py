class boughtItem:
    def __init__(self):
        self.items = {} # [id, price, quantity]
        self.bid = {}
    def init(self):
        self.items.clear
        self.bid.clear
        

bought = boughtItem()
def init():
    bought.init()
    return;

def buy(bId, mProduct, mPrice, mQuantity):
    result = 0
    if bought.items.get(mProduct):
        bought.items[mProduct].append([bId, mPrice, mQuantity])
    else:
        bought.items[mProduct] = [[bId, mPrice, mQuantity]]

    if bought.bid.get(bId):
        pass
    else:
        pass

    for product in bought.items[mProduct]:
        result += product[2]
    
    print(result)
    return result

def cancel(bID):

    return -1

def sell(sId, mProduct, mPrice, mQuantity):
    
    return -1

def refund(sId):

    return -1
