
drows = [-1, 1, 0, 0]
dcols = [0, 0, -1, 1]

class Civil:
    def __init__(self, N) -> None:
        self.grid = [[0] * N for _ in range(N)]
        self.N = N
        self.territory = {}


def init(N):
    global civil

    civil = Civil(N)
    print('initialized')
    return

def newCivilization(r, c, mID):
    global civil

    r -= 1
    c -= 1

    civil.grid[r][c] = mID
    if civil.territory.get(mID):
        civil.territory[mID].append([r, c])
    else:
        civil.territory[mID] = [[r, c]]

    edges = {}
    for i in range(4):
        dr = r + drows[i]
        dc = c + dcols[i]

        if 0 <= dr < civil.N and 0 <= dc < civil.N:
            if civil.grid[dr][dc] != 0:
                if edges.get(civil.grid[dr][dc]):
                    edges[civil.grid[dr][dc]] += 1
                else:
                    edges[civil.grid[dr][dc]] = 1

    if bool(edges):
        base = 0
        surrounded = 1000000001
        for key, value in edges.items():
            if value > base:
                base = value
                surrounded = key
            elif value == base:
                surrounded = min(key, surrounded)

        civil.grid[r][c] = surrounded
        civil.territory[surrounded].append([r, c])
        civil.territory[mID].clear()

        return surrounded

    return mID

def removeCivilization(mID):
    global civil

    if civil.territory.get(mID):
        if len(civil.territory[mID]) == 0:
            return 0
        else:
            pass
    else:
        return 0

    ans = 0
    for r, c, in civil.territory[mID]:
        civil.grid[r][c] = 0
        ans += 1

    civil.territory[mID].clear()
    return ans

def getCivilization(r, c):
    global civil
    
    r -= 1
    c -= 1

    return civil.grid[r][c] 

def getCivilizationArea(mID):
    global civil

    if civil.territory.get(mID):
        pass
    else:
        return 0

    return len(civil.territory[mID])

def mergeCivilization(mID1, mID2):
    global civil
    
    for r, c in civil.territory[mID2]:
        civil.grid[r][c] = mID1
        civil.territory[mID1].append([r, c])
    civil.territory[mID2].clear()

    return len(civil.territory[mID1])