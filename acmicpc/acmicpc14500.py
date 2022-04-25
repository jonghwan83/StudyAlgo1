import sys
import random

n, m = list(map(int, sys.stdin.readline().split()))

arr = []
for i in range(n):
    arr.append(list(map(int, sys.stdin.readline().split())))
ans = 0

def search_vsquare():
    global ans
    crow = 0
    ccol = 0
    
    while crow + 1 < n and ccol < m:
        vshift = 0
        for i in range(2):
            vshift += arr[crow+i][ccol]

        if crow == 0 and ccol == 0: # 1
            vshift += max(
                arr[crow+1][ccol+1] + arr[crow+2][ccol+1],
                arr[crow][ccol+1] + arr[crow+1][ccol+1]
            )
        elif crow == 0 and 0 < ccol < m-1: # 2
            vshift += max(
                arr[crow+1][ccol-1] + arr[crow+2][ccol-1],
                arr[crow+1][ccol+1] + arr[crow+2][ccol+1],
                arr[crow][ccol-1] + arr[crow+1][ccol-1],
                arr[crow][ccol+1] + arr[crow+1][ccol+1]
            )
        elif crow == 0 and ccol == m-1: # 3
            vshift += max(
                arr[crow+1][ccol-1] + arr[crow+2][ccol-1],
                arr[crow][ccol-1] + arr[crow+1][ccol-1]
            )
        elif crow + 1 == n-1 and ccol == 0: # 4
            vshift += max(
                arr[crow-1][ccol+1] + arr[crow][ccol+1],
                arr[crow][ccol+1] + arr[crow+1][ccol+1]
            )
        elif crow + 1 == n-1 and 0 < ccol < m-1: # 5
            vshift += max(
                arr[crow-1][ccol-1] + arr[crow][ccol-1],
                arr[crow-1][ccol+1] + arr[crow][ccol+1],
                arr[crow][ccol-1] + arr[crow+1][ccol-1],
                arr[crow][ccol+1] + arr[crow+1][ccol+1]
            )
        elif crow + 1 == n-1 and ccol == m - 1: # 6
            vshift += max(
                arr[crow-1][ccol-1] + arr[crow][ccol-1],
                arr[crow][ccol-1] + arr[crow+1][ccol-1]
            )
        elif 0 < crow + 1< n-1 and ccol == 0: # 7
            vshift += max(
                arr[crow-1][ccol+1] + arr[crow][ccol+1],
                arr[crow+1][ccol+1] + arr[crow+2][ccol+1],
                arr[crow][ccol+1] + arr[crow+1][ccol+1]
            )
        elif 0 < crow + 1< n-1 and ccol == m-1: # 8
            vshift += max(
                arr[crow-1][ccol-1] + arr[crow][ccol-1],
                arr[crow+1][ccol-1] + arr[crow+2][ccol-1],
                arr[crow][ccol-1] + arr[crow+1][ccol-1]
            )
        else:
            vshift += max(
                arr[crow-1][ccol-1] + arr[crow][ccol-1],
                arr[crow-1][ccol+1] + arr[crow][ccol+1],
                arr[crow+1][ccol-1] + arr[crow+2][ccol-1],
                arr[crow+1][ccol+1] + arr[crow+2][ccol+1],
                arr[crow][ccol-1] + arr[crow+1][ccol-1],
                arr[crow][ccol+1] + arr[crow+1][ccol+1]
            )

        if vshift > ans:
            ans = vshift

        ccol += 1
        if ccol >= m:
            ccol = 0
            crow += 1

    return

def search_hsqaure(): # shift 추가
    global ans
    crow = 0
    ccol = 0
    while crow < n and ccol + 1 < m:
        hshift = 0
        for i in range(2):
            hshift += arr[crow][ccol+i]
        
        if crow == 0 and ccol == 0: # 1
            hshift += (
                arr[crow+1][ccol+1] + arr[crow+1][ccol+2]
            )
        elif crow == 0 and 0 < ccol + 1 < m-1: # 2
            hshift += max(
                arr[crow+1][ccol+1] + arr[crow+1][ccol+2],
                arr[crow+1][ccol-1] + arr[crow+1][ccol]
            )
        elif crow == 0 and ccol + 1 == m-1: # 3
            hshift += (
                arr[crow+1][ccol-1] + arr[crow+1][ccol]
            )
        elif crow == n - 1 and ccol == 0: # 4
            hshift += (
                arr[crow-1][ccol+1] + arr[crow-1][ccol+2]
            )
        elif crow == n -1 and 0 < ccol + 1 < m -1: # 5
            hshift += max(
                arr[crow-1][ccol-1] + arr[crow-1][ccol],
                arr[crow-1][ccol+1] + arr[crow-1][ccol+2]
            )
        elif crow == n - 1 and ccol + 1 == m - 1: # 6
            hshift += (
                arr[crow-1][ccol-1] + arr[crow-1][ccol]
            )
        elif 0 < crow  < n-1 and ccol == 0: # 7
            hshift += max(
                arr[crow-1][ccol+1] + arr[crow-1][ccol+2],
                arr[crow+1][ccol+1] + arr[crow+1][ccol+2]
            )
        elif 0 < crow  < n - 1 and ccol + 1 == m-1: # 8
            hshift += max(
                arr[crow-1][ccol-1] + arr[crow-1][ccol],
                arr[crow+1][ccol-1] + arr[crow+1][ccol]
            )
        else:
            hshift += max(
                arr[crow-1][ccol-1] + arr[crow-1][ccol],
                arr[crow-1][ccol+1] + arr[crow-1][ccol+2],
                arr[crow+1][ccol+1] + arr[crow+1][ccol+2],
                arr[crow+1][ccol-1] + arr[crow+1][ccol]
            )

        if hshift > ans:
            ans = hshift
        
        ccol += 1
        if ccol + 1 >= m:
            ccol = 0
            crow += 1
    return

def search_vstick():
    global ans
    crow = 0
    ccol = 0
    while crow + 2 < n and ccol < m:
        temp = 0
        for i in range(3):
            temp += arr[crow+i][ccol]

        if crow == 0 and ccol == 0: # 1
            temp += max(
                arr[crow][ccol+1], arr[crow+1][ccol+1],
                arr[crow+2][ccol+1], arr[crow+3][ccol]
            )

        elif crow == 0 and 0 < ccol < m - 1: # 2
            temp += max(
                arr[crow][ccol+1], arr[crow+1][ccol+1], 
                arr[crow+2][ccol+1], arr[crow+3][ccol], 
                arr[crow+2][ccol-1], arr[crow+1][ccol-1], 
                arr[crow][ccol-1]
            )
        
        elif crow == 0 and ccol == m - 1: # 3
            temp += max(
                arr[crow+2][ccol-1], arr[crow+1][ccol-1], 
                arr[crow][ccol-1], arr[crow+3][ccol]
            )
        
        elif crow+2 == n-1 and ccol == 0: # 4
            temp += max(
                arr[crow-1][ccol], arr[crow][ccol+1],
                arr[crow+1][ccol+1], arr[crow+2][ccol+1]
            )
        
        elif crow+2 == n-1 and 0 < ccol < m - 1:  # 5
            temp += max(
                arr[crow-1][ccol], arr[crow][ccol+1],
                arr[crow+1][ccol+1], arr[crow+2][ccol+1],
                arr[crow+2][ccol-1], arr[crow+1][ccol-1], 
                arr[crow][ccol-1]
            )

        elif crow+2 == n-1 and ccol == m-1: # 6
            temp += max(
                arr[crow-1][ccol], arr[crow+2][ccol-1],
                arr[crow+1][ccol-1], arr[crow][ccol-1]
            )

        elif 0 < crow + 2 < n-1 and ccol == 0: # 7
            temp += max(
                arr[crow-1][ccol], arr[crow][ccol+1],
                arr[crow+1][ccol+1], arr[crow+2][ccol+1],
                arr[crow+3][ccol]
            )
        
        elif 0 < crow + 2 < n-1 and ccol == m-1: # 8
            temp += max(
                arr[crow-1][ccol], arr[crow+3][ccol], 
                arr[crow+2][ccol-1], arr[crow+1][ccol-1], 
                arr[crow][ccol-1]
            )
        else:
            temp += max(
                arr[crow-1][ccol], arr[crow][ccol+1],
                arr[crow+1][ccol+1], arr[crow+2][ccol+1],
                arr[crow+3][ccol], arr[crow+2][ccol-1],
                arr[crow+1][ccol-1], arr[crow][ccol-1]
            )

        if temp > ans:
            ans = temp
        
        ccol += 1
        if ccol >= m:
            ccol = 0
            crow += 1
    return

def search_hstick():
    global ans
    crow = 0
    ccol = 0
    
    while crow < n and ccol + 2 < m:
        temp = 0
        for i in range(3):
            temp += arr[crow][ccol+i]

        if crow == 0 and ccol == 0: # 1
            temp += max(
                arr[crow][ccol+3], arr[crow+1][ccol+2],
                arr[crow+1][ccol+1], arr[crow+1][ccol]
            )

        elif crow == 0 and 0 < ccol + 2 < m-1: # 2
            temp += max(
                arr[crow][ccol-1], arr[crow][ccol+3], 
                arr[crow+1][ccol+2], arr[crow+1][ccol+1], 
                arr[crow+1][ccol]
            )

        elif crow == 0 and ccol + 2 == m-1: # 3
            temp += max(
                arr[crow][ccol-1], arr[crow+1][ccol+2],
                arr[crow+1][ccol+1], arr[crow+1][ccol]
            )

        elif crow == n-1 and ccol == 0: # 4
            temp += max(
                arr[crow-1][ccol], arr[crow-1][ccol+1], 
                arr[crow-1][ccol+2], arr[crow][ccol+3]
            )

        elif crow == n-1 and 0 < ccol + 2 < m-1: # 5
            temp += max(
                arr[crow][ccol-1], arr[crow-1][ccol],
                arr[crow-1][ccol+1], arr[crow-1][ccol+2],
                arr[crow][ccol+3]
            )
        elif crow == n-1 and ccol + 2 == m-1: # 6
            temp += max(
                arr[crow][ccol-1], arr[crow-1][ccol],
                arr[crow-1][ccol+1], arr[crow-1][ccol+2]
            )

        elif 0 < crow < n-1 and ccol == 0: # 7
            temp += max(
                arr[crow-1][ccol],
                arr[crow-1][ccol+1], arr[crow-1][ccol+2],
                arr[crow][ccol+3], arr[crow+1][ccol+2],
                arr[crow+1][ccol+1], arr[crow+1][ccol]
            )

        elif 0 < crow < n-1 and ccol + 2 == m-1: # 8
            temp += max(
                arr[crow][ccol-1], arr[crow-1][ccol],
                arr[crow-1][ccol+1], arr[crow-1][ccol+2],
                arr[crow+1][ccol+2], arr[crow+1][ccol+1], 
                arr[crow+1][ccol]
            )
        else:
            temp += max(
                arr[crow][ccol-1], arr[crow-1][ccol],
                arr[crow-1][ccol+1], arr[crow-1][ccol+2],
                arr[crow][ccol+3], arr[crow+1][ccol+2],
                arr[crow+1][ccol+1], arr[crow+1][ccol]
            )

        if temp > ans:
            ans = temp

        ccol += 1
        if ccol + 2 >= m:
            ccol = 0
            crow += 1

    return


search_vsquare()
search_hsqaure()
search_vstick()
search_hstick()
print(ans)
