drows = [0, 1, 1, 1, 0, -1, -1, -1]
dcols = [1, 1, 0, -1, -1, -1, 0, 1]

def seat2grid(mSeatNum):
    global size

    r = mSeatNum // size[0]
    c = mSeatNum % size[1] - 1
    if c == -1:
        c = 0
    
    return [r, c]

def grid2seat(loc):
    global size
    return loc[0] * size[0] + loc[1] + 1

def assignSeat(mSeatNum, k):
    global seat, size
    r, c = seat2grid(mSeatNum)

    if seat[r][c] == 0:
        seat[r][c] = 1
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
                    return grid2seat([dr, dc])

        k += 1

    return 0

def init(W, H):
    global seat, alternatives, size

    seat = [[0] * W for _ in range(H)]    # 0: empty 1: occupied
    alternatives = {}   # key: seat No., value: alternative seat No.
    size = [H, W]

    # print(size)
    # for lst in seat:
    #     print(lst)

    return

def selectSeat(mSeatNum):
    result = assignSeat(mSeatNum, 1)
    
    # print('==seat==')
    # for lst in seat:
    #     print(lst)

    return result