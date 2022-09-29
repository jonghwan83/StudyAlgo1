# need hash map for speed up

drows = [0, 1, 1, 1, 0, -1, -1, -1]
dcols = [1, 1, 0, -1, -1, -1, 0, 1]

def seat2grid(mSeatNum):
    global size

    r = (mSeatNum - 1) // size[1]
    c = (mSeatNum - 1) % size[1]
    
    return [r, c]

def grid2seat(loc):
    global size
    return loc[0] * size[1] + loc[1] + 1

def assignSeat(mSeatNum, k):
    global seat, size, layerSeat
    r, c = seat2grid(mSeatNum)

    if seat[r][c] == 0:
        seat[r][c] = 1
        layerSeat[mSeatNum] = k
        return grid2seat([r, c])

    if k >= max(size):
        return 0
    
    
    while k < max(size):
        for i in range(8):
            dr = r + drows[i] * k
            dc = c + dcols[i] * k

            if (0 <= dr < size[0]) and (0 <= dc < size[1]):
                if seat[dr][dc] == 0:
                    seat[dr][dc] = 1
                    layerSeat[mSeatNum] = k
                    return grid2seat([dr, dc])

        k += 1

    return 0

def init(W, H):
    global seat, alternatives, size, unavailable, layerSeat

    seat = [[0] * W for _ in range(H)]    # 0: empty 1: occupied
    size = [H, W]
    unavailable = [0] * (W * H + 1)   # index: seat No.   1: return 0
    layerSeat = [1] * (W * H + 1)   # index: seat No. element: k layers

    return

def selectSeat(mSeatNum):
    global unavailable, layerSeat

    if unavailable[mSeatNum]:
        return 0

    k = layerSeat[mSeatNum]
    result = assignSeat(mSeatNum, k)
    # store not available seat
    if result == 0:
        unavailable[mSeatNum] = 1

    return result
