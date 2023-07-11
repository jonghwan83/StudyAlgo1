#ifndef _CRT_SECURE_NO_WARNINGS

#define _CRT_SECURE_NO_WARNINGS

#endif

#include <stdio.h>

#include <string.h>

#include <chrono>



static int PUZZLE[3][3] = { { 1,  2,  3 },{ 4,  5,  6 },{ 7,  8,  0 } };

static int X = 2;

static int Y = 2;

extern void test(const int puzzle[3][3]);



static inline int pseudo_rand(void)

{

static unsigned long long seed = 55;

seed = seed * 25214903917ULL + 11ULL;

return (seed >> 16) & 0x7fffffff;

}



void move(int mode)

{

  switch (mode)
  
  {
  
    case 0: // from top to bottom
    
    if (Y != 0) {
    
      PUZZLE[Y][X] = PUZZLE[Y - 1][X];
      
      Y--;
      
      PUZZLE[Y][X] = 0;
    
    }
    
    return;
    
    case 1: // from bottom to top
    
    if (Y != 2) {
    
      PUZZLE[Y][X] = PUZZLE[Y + 1][X];
      
      Y++;
      
      PUZZLE[Y][X] = 0;
    
    }
    
    return;
    
    case 2: // from left to right
    
    if (X != 0) {
    
      PUZZLE[Y][X] = PUZZLE[Y][X - 1];
      
      X--;
      
      PUZZLE[Y][X] = 0;
    
    }
    
    return;
    
    case 3: // from right to left
    
    if (X != 2) {
    
      PUZZLE[Y][X] = PUZZLE[Y][X + 1];
      
      X++;
      
      PUZZLE[Y][X] = 0;
    
    }
    
    return;
    
  }

}



int main()

{

  int SCORE = 0;
  
  for (int tc = 0; tc < 10000; tc++) {
  
  
  
    for (int c = 0; c < 500; c++) {
    
      move(pseudo_rand() % 4);
    
    }
    
    
    
    std::chrono::system_clock::time_point start = std::chrono::system_clock::now();
    
    test(PUZZLE);
    
    std::chrono::system_clock::time_point end = std::chrono::system_clock::now();
    
    SCORE += (int)std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    
    
    int n = 1;
    
    
    for (int y = 0; y < 3; y++) {
    
      for (int x = 0; x < 3; x++) {
      
        if (PUZZLE[y][x] != n++) {
        
          SCORE += 100000000;
          
          tc = 10000;
        
        }
        
        if (n == 9) {
        
          n = 0;
        
        }
      
      }
    
    }
  
  }
  
  printf("SCORE: %d\n", SCORE);

}
