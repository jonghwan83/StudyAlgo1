from heapq import heappush, heappop

class Train:
    def __init__(self) -> None:
        self.nCars = 0
        self.passengers = []    # index: 승객 id, value: [point, jobID, car, v0]
        self.passengersByJob = {}   # key : job ID, value: 승객 ID

        self.minHeap = {}
        self.maxHeap = {}


# N : 총 승객 수, M: 승객 수/객차, J : 직업의 수
def init(N, M, J, mPoint, mJobID):
    global train

    train = Train()

    train.nCars = int(N/M)
    
    for i in range(N):
        car = int(i / M)
        train.passengers.append([mPoint[i], mJobID[i], car, 0, 1])
        if train.passengersByJob.get(mJobID[i]):
            train.passengersByJob[mJobID[i]].append(i)
        else:
            train.passengersByJob[mJobID[i]] = [i]

        if train.minHeap.get(car):
            heappush(train.minHeap[car], [mPoint[i], -i, 0])
        else:
            train.minHeap[car] = [[mPoint[i], -i, 0]]
        
        if train.maxHeap.get(car):
            heappush(train.maxHeap[car], [-mPoint[i], i, 0])
        else:
            train.maxHeap[car] = [[-mPoint[i], i, 0]]

    return

def destroy():

    return

def update(mID, mPoint):
    global train

    car = train.passengers[mID][2]
    train.passengers[mID][0] += mPoint
    train.passengers[mID][3] += 1

    heappush(train.minHeap[car], [train.passengers[mID][0], -mID, train.passengers[mID][3]])
    heappush(train.maxHeap[car], [-train.passengers[mID][0], mID, train.passengers[mID][3]])

    return train.passengers[mID][0]

def updateByJob(mJobID, mPoint):
    global train

    ans = 0
    for pID in train.passengersByJob[mJobID]:
        update(pID, mPoint)
        ans += train.passengers[pID][0]

    return ans

def move(mNum):
    global train

    ans = 0
    tempMin = {}
    tempMax = {}
    for c in range(train.nCars):
        tempMin[c] = []
        tempMax[c] = []
        for _ in range(mNum):
            if c < train.nCars - 1:
                while True:
                    point, pID, ver = train.minHeap[c][0]
                    if ver == train.passengers[-pID][3] and train.passengers[-pID][4] and c == train.passengers[-pID][2]:
                        ans += point
                        tempMin[c].append(heappop(train.minHeap[c]))
                        train.passengers[-pID][4] = 0
                        break
                    else:
                        heappop(train.minHeap[c])
            if c > 0:
                while True:
                    point, pID, ver = train.maxHeap[c][0]
                    if ver == train.passengers[pID][3] and train.passengers[pID][4] and c == train.passengers[pID][2]:
                        ans -= point
                        tempMax[c].append(heappop(train.maxHeap[c]))
                        train.passengers[pID][4] = 0
                        break
                    else:
                        heappop(train.maxHeap[c])

    for c in range(train.nCars):
        if c < train.nCars - 1:
            while tempMin[c]:
                point, pID, ver = tempMin[c].pop()
                train.passengers[-pID][2] = c + 1
                heappush(train.minHeap[c + 1], [point, pID, ver])
                heappush(train.maxHeap[c + 1], [-point, -pID, ver])
                train.passengers[-pID][4] = 1
        
        if c > 0:
            while tempMax[c]:
                point, pID, ver = tempMax[c].pop()
                train.passengers[pID][2] = c - 1
                heappush(train.minHeap[c - 1], [-point, -pID, ver])
                heappush(train.maxHeap[c - 1], [point, pID, ver])
                train.passengers[pID][4] = 1

    return ans