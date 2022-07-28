
def init():
    global loginQueue, queueIdx, queueSt, idxDict, charDict

    loginQueue = []
    queueIdx = 0
    queueSt = 0
    idxDict = {}

    charDict = {}   # key: 3 chars, value: [mID?, queueIdx]

    return

def loginID(mID):
    global loginQueue, queueIdx, idxDict, queueSt, charDict
    
    if idxDict.get(mID):
        loginQueue[idxDict[mID]][1] = 1

    loginQueue.append([mID, 0])
    idxDict[mID] = queueIdx
    if charDict.get(mID[:3]):
        charDict[mID[:3]].append([mID, queueIdx])
    else:
        charDict[mID[:3]] = [[mID, queueIdx]]

    queueIdx += 1

    return     

def closeIDs(mStr):
    global loginQueue, queueIdx, idxDict, queueSt

    count = 0
    for mID, qIdx in charDict[mStr[:3]]:
        if mStr == mID[:len(mStr)] and not loginQueue[qIdx][1]:
            count += 1
            loginQueue[qIdx][1] = 1

    return count

def connectCnt(mCnt):
    global loginQueue, queueIdx, idxDict, queueSt

    k = 0
    while k < mCnt:
        if loginQueue[queueSt][1]:
            loginQueue[queueSt][1] = 1
            queueSt += 1
        else:
            loginQueue[queueSt][1] = 1
            queueSt += 1
            k += 1

    return 

def waitOrder(mID):
    global loginQueue, queueIdx, queueSt, idxDict
    if not idxDict.get(mID):
        return 0

    idx = idxDict[mID]
    if loginQueue[idx][1]:
        return 0

    count = 0
    for i in range(queueSt, queueIdx):
        if not loginQueue[i][1]:
            count += 1
        if mID == loginQueue[i][0] and not loginQueue[i][1]:
            break

    return count
