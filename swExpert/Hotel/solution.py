from typing import List

from heapq import heappush, heappop, heapify

class Hotel:
    def __init__(self) -> None:
        self.price = {} # key: [region(10), beds(10), roomtype(4), scape(4)], value: [price, room ID, hotel ID]]
        self.priceIdx = {} # key: hotel ID, value: [region, beds, roomtype, sight]
        # 여기서 check in, check out 은 어떻게 구분할 수 있을지?? 
        self.reserved = {}  # key: hotel ID, value: [check in, check out]


def init(N:int, mRoomCnt:List[int]) -> None:
    global hotel

    hotel = Hotel()
    
    return

def addRoom(mHotelID:int, mRoomID:int, mRoomInfo:List[int]) -> None:
    # RoomInfo: region, beds, room, sight, price
    global hotel

    region, beds, roomtype, scape, price = mRoomInfo
    if hotel.price.get((region, beds, roomtype, scape)):
        heappush(hotel.price[(region, beds, roomtype, scape)], [price, mRoomID, mHotelID])
    else:
        hotel.price[(region, beds, roomtype, scape)] = [[price, mRoomID, mHotelID]]
    
    if hotel.priceIdx.get(mHotelID):
        hotel.priceIdx[mHotelID].add((region, beds, roomtype, scape))
    else:
        hotel.priceIdx[mHotelID] = set()
        hotel.priceIdx[mHotelID].add((region, beds, roomtype, scape))

    return

def findRoom(mFilter:List[int]) -> int:
    global hotel

    checkIn = mFilter[0]
    checkOut = mFilter[1]
    priceKey = (mFilter[2], mFilter[3], mFilter[4], mFilter[5])
    temp = []
    isCompleted = False
    ans = -1

    if hotel.price.get(priceKey):
        pass
    else:
        return -1

    temp_val = []

    print('input:', mFilter)
    while hotel.price[priceKey]:
        print(priceKey, hotel.price[priceKey])
        if isCompleted:
            hotel.reserved[reservedKey].add((temp_val[0], temp_val[1]))
            break
        price, roomID, hotelID = heappop(hotel.price[priceKey])
        reservedKey = (hotelID, roomID)
        temp.append([price, roomID, hotelID])
        
    
        if hotel.reserved.get(reservedKey):
            print(reservedKey, hotel.reserved[reservedKey])  
            for reservedIn, reservedOut in hotel.reserved[reservedKey]:
                if (checkOut < reservedIn) or (checkIn > reservedOut):
                    temp_val = [checkIn, checkOut]
                    print(temp_val, reservedIn, reservedOut)
                    isCompleted = True
                    ans = roomID
                else:
                    break
        else:
            hotel.reserved[reservedKey] = set()
            temp_val = [checkIn, checkOut]
            isCompleted = True
            ans = roomID
        

    while temp:
        heappush(hotel.price[priceKey], temp.pop())

    return ans
        
def riseCosts(mHotelID:int) -> int:
    global hotel

    isChanged = False
    ans = 0
    for region, beds, roomtype, scape in hotel.priceIdx[mHotelID]:
        priceKey = (region, beds, roomtype, scape)
        for idx, [price, roomID, hotelID] in enumerate(hotel.price[priceKey]):
            if hotelID == mHotelID:
                hotel.price[priceKey][idx][0] = int(1.1 * hotel.price[priceKey][idx][0])
                ans += hotel.price[priceKey][idx][0]
                isChanged = True
        
        if isChanged:
            heapify(hotel.price[priceKey])
            isChanged = False

    return ans
