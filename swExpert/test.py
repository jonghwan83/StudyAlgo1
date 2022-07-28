
def init():
    global loginQueue, queueIdx, queueSt, idxDict

    loginQueue = []
    queueIdx = 0
    queueSt = 0
    idxDict = {}

    return

def loginID(mID):
    global loginQueue, queueIdx, idxDict, queueSt
    
    if idxDict.get(mID):
        loginQueue[idxDict[mID][0]][1] = 1
        idxDict[mID][1] = 1

    loginQueue.append([mID, 0])
    idxDict[mID] = [queueIdx, 0]

    queueIdx += 1
    return     

def closeIDs(mStr):
    global loginQueue, queueIdx, idxDict, queueSt
    print(queueSt, loginQueue)
    count = 0
    for i in range(queueSt, queueIdx):
        if mStr == loginQueue[i][0][:len(mStr)]:
            count += 1
            loginQueue[i][1] = 1
            idxDict[loginQueue[i][0]][1] = 1

    print(count)
    return count

def connectCnt(mCnt):
    global loginQueue, queueIdx, idxDict, queueSt

    k = 0
    while k < mCnt:
        if loginQueue[queueSt][1]:
            idxDict[loginQueue[queueSt][0]][1] = 1
            queueSt += 1
        else:
            idxDict[loginQueue[queueSt][0]][1] = 1
            queueSt += 1
            k += 1

    return 

def waitOrder(mID):
    global loginQueue, queueIdx, queueSt, idxDict
    if not idxDict.get(mID):
        print(0)
        return 0

    if idxDict[mID][1]:
        print(0)
        return 0

    idx = idxDict[mID][0]
    if loginQueue[idx][1]:
        print(0)
        return 0

    count = 0
    for i in range(queueSt, queueIdx):
        if not loginQueue[i][1]:
            count += 1
        if mID == loginQueue[i][0] and not loginQueue[i][1]:
            break
    
    print(count)
    return count
