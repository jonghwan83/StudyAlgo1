#define ABS(x) ((x<0)?(-(x)):(x))

static const int MAP_SIZE = 100;

static const int ANTENNA_NUM = 150;

static const int ANTENNA_CAPA = 100;

static const int UE_NUM = 10'000;

int max(int a, int b) { return a > b? a : b; }

struct Coordinates

{
    
    int y, x;
    
    Coordinates()
    
    {
        
        y = x = 0;
        
    }
    
    Coordinates(int _y, int _x)
    
    {
        
        y = _y;
        
        x = _x;
        
    }
    
    int operator-(Coordinates& param)
    
    {
        
        return ABS(y - param.y) + ABS(x - param.x);
        
    }
    
};


Coordinates antenna_list[ANTENNA_NUM];

int capa[ANTENNA_NUM];

int subtask;



void init(Coordinates _antenna_list[])
{
    subtask = 0;
    
    for (int i = 0; i < ANTENNA_NUM; i++)
    {
        antenna_list[i] = _antenna_list[i];
    }
    
}



void scanUE(Coordinates UE_list[], int antenna_range[], int assign_antenna[])
{
    for (int i = 0; i < ANTENNA_NUM; i++)
    {
        capa[i] = 0;
     
        antenna_range[i] = 0;
    }
    
    
    for (int ue = 0; ue < UE_NUM; ue++)
    {
        int antenna_id = -1;
     
        int min_dist = MAP_SIZE * MAP_SIZE * ANTENNA_NUM;
        
        for (int antenna = 0; antenna < ANTENNA_NUM; antenna++)
        {
            if (capa[antenna] >= ANTENNA_CAPA) { continue; }
            
            int dist = antenna_list[antenna] - UE_list[ue];
            
            if (antenna_id > -1 && dist + 4 < antenna_range[antenna] && capa[antenna] < capa[antenna_id])
            {
                min_dist = dist;
                
                antenna_id = antenna;
            }
            
            else if (dist < min_dist)
            {
                min_dist = dist;
                
                antenna_id = antenna;
            }
            
        }
        
        assign_antenna[ue] = antenna_id;
        
        antenna_range[antenna_id] = max(antenna_range[antenna_id], min_dist + 4);
        
        capa[antenna_id]++;
    }
    
    subtask++;
}
