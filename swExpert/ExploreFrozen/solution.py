from collections import deque

drows = [-1, 1, 0, 0]
dcols = [0, 0, -1, 1]

def rev_dir(i):
    if i == 0:
        return 1
    elif i == 1:
        return 0
    elif i == 2:
        return 3
    else:
        return 2

def bfs(sPoint, ePoint):
    global planet, visited, nRow, nCol

    visited = [[0] * nCol for _ in range(nRow)]

    queue = deque()
    queue.append(sPoint + [5])

    if (sPoint[0] == ePoint[0]) and (sPoint[1] == ePoint[1]):
        return 

    while queue:
        r, c, track = queue.popleft()

        visited[r][c] += 1

        for i in range(4):
            dr = r + drows[i]
            dc = c + dcols[i]
            
            if i != track:
                if (0 <= dr < nRow) and (0 <= dc < nCol):
                    if planet[dr][dc] == 0:
                        while (0 <= dr < nRow) and (0 <= dc < nCol):
                            if planet[dr][dc] == 1:
                                if not visited[dr-drows[i]][dc-dcols[i]]:
                                    visited[dr-drows[i]][dc-dcols[i]] = visited[r][c]
                                    queue.append([dr-drows[i], dc-dcols[i]] + [rev_dir(i)])
                                break

                            if (dr == ePoint[0]) and (dc == ePoint[1]):
                                visited[dr][dc] = visited[r][c]
                                return
                            dr += drows[i]
                            dc += dcols[i]

    return 


def init(R, C, M, mStructureR, mStructureC):
    global planet, nRow, nCol
    
    nRow = R
    nCol = C
    
    planet = [[0] * C for _ in range(R)]
    for i in range(M):
        planet[mStructureR[i]][mStructureC[i]] = 1  # set structure

    return

def minDamage(mStartR, mStratC, mEndR, mEndC):
    global planet, visited, nRow, nCol, answer
    answer = -1

    if (mStartR == mEndR) and (mStratC == mEndC):
        return 0

    bfs([mStartR, mStratC], [mEndR, mEndC])
    
    if visited[mEndR][mEndC] >= 0:
        answer = visited[mEndR][mEndC] - 1

    return answer
