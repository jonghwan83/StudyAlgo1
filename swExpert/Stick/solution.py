from collections import deque

drows = [-1, 1, 0, 0]
dcols = [0, 0, -1, 1]

class Board:
    def __init__(self):
        self.n = 0
        self.nStick = 0
        self.grid = []
        self.mID = []
        self.mloc = []
        self.idDict = {}
        self.region = []
        self.nRegion = 1
        self.stack = []
        

    def setGrid(self, N):
        self.n = N
        self.grid = [[0] * N for _ in range(N)]
        self.region = [[0] * N for _ in range(N)]
        return

board = Board()

def init(N):
    board.__init__()
    board.setGrid(N)
    return

def addBar(mID, mLength, mRow, mCol, mDir):
    mRow -= 1
    mCol -= 1
    
    for i in range(mLength):
        r = mRow + drows[mDir]
        c = mCol + dcols[mDir]

        if board.grid[r][c] == 0:
            pass

        board.grid[r][c] += 1
    
    for i in range(mLength):
        for j in range(4):
            pass # 작성 중

    return 0

def removeBar(mID):

    return 0