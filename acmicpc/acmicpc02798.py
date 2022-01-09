'''
In “Blackjack”, a popular card game, 
the goal is to have cards which sum up to largest number not exceeding 21. 
Mirko came up with his own version of this game.

In Mirko"s game, cards have positive integers written on them. 
The player is given a set of cards and an integer M. 
He must choose three cards from this set so that their sum comes as close as possible to M without exceeding it. 
This is not always easy since there can be a hundred of cards in the given set.

Help Mirko by writing a program that finds the best possible outcome of given game.

입력
first line:N, number of cards, M, the number noe exceed
second line: card numbers
-> choose 3 cards
출력
the largest possilbe sum
'''

import sys

N1 = list(map(int, sys.stdin.readline().split()))
N2 = list(map(int, sys.stdin.readline().split()))

result = []

for i in range(len(N2)):
    for j in range(len(N2)):
        for k in range(len(N2)):
            if (i != j) and (j != k) and (i != k):
                temp = N2[i] + N2[j] + N2[k]
                if (temp <= N1[1]):
                    result.append(N2[i] + N2[j] + N2[k])

print(max(result))