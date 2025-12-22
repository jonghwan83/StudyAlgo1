#define ABS(x) ((x) >= 0 ? (x) : -(x))

typedef struct {
    
    int y, x;
    
} Coordinates;

typedef struct {
    
    Coordinates src;
    
    Coordinates dest;
    
} Delivery;

extern bool deliver(int mID);


#include <iostream>
#include <queue>
#include <vector>

#define RESTAURANT_NUM 10

#define DELIVERY_NUM 2000

#define MAP_SIZE 100

#define TIME_LIMIT 100000

int getDistance(Coordinates c1, Coordinates c2)
{
    return ABS(c1.x - c2.x) + ABS(c1.y - c2.y);
}


std::pair<int, std::vector<int>> simulate(Coordinates mRider, Delivery mDeliveries[], float p, int k)
{
    int res = 0;
    
    std::vector<int> order;
    
    Coordinates pos = mRider;
    
    int time_left = TIME_LIMIT;
    
    std::vector<Coordinates> restaurants;
    
    std::vector<int> closest_src(DELIVERY_NUM);
    
    std::vector<int> min_dist(DELIVERY_NUM, MAP_SIZE * MAP_SIZE + 1);
    
    std::vector<bool> is_done(DELIVERY_NUM, false);
    
    for(int i = 0; i < DELIVERY_NUM; i++)
    {
        bool flag = true;
        for(int j = 0; j < restaurants.size(); j++)
        {
            if(!getDistance(mDeliveries[i].src, restaurants[j]))
                flag = false;
        }
        
        if(flag) restaurants.push_back(mDeliveries[i].src);
    }
    
    
    for(int i = 0; i < DELIVERY_NUM; i++)
    {
        for(int j = 0; j < RESTAURANT_NUM; j++)
        {
            int dist = getDistance(mDeliveries[i].src, restaurants[j]);
            
            if(dist < min_dist[i])
            {
                min_dist[i] = dist;
                closest_src[i] = j;
            }
        }
    }
    
    while(time_left > 0)
    {
        std::priority_queue<std::pair<float, int>> pq;
        
        for(int i = 0; i < DELIVERY_NUM; i++)
        {
            int score = 3000 + 300 * getDistance(mDeliveries[i].src, mDeliveries[i].dest);
            int cost = getDistance(pos, mDeliveries[i].src);
            int next_trip = getDistance(mDeliveries[i].dest, restaurants[closest_src[i]]);
            
            float total = score / ((float)cost + (float)next_trip);
            
            pq.push({total, i});
        }
        
        std::vector<int> tops;
        
        while(!pq.empty())
        {
            if(!is_done[pq.top().second])
            {
                tops.push_back(pq.top().second);
                if(tops.size() >= k) break;
            }
            pq.pop();
        }
        
        int p_action = rand() % 100;
        
        int deliver_id = tops[0];
        
        if((float)p_action/100.0 > p)
        {
            deliver_id = tops[rand() % k];
        }
        
//        deliver(deliver_id);
        time_left -= getDistance(pos, mDeliveries[deliver_id].src);
        time_left -= getDistance(mDeliveries[deliver_id].src, mDeliveries[deliver_id].dest);
        
        res += 3000 + 300 * getDistance(mDeliveries[deliver_id].src, mDeliveries[deliver_id].dest);
        
        is_done[deliver_id] = true;
        
        order.push_back(deliver_id);
        
        pos = mDeliveries[deliver_id].dest;
    }
    
    return std::make_pair(res, order);
}

void process(Coordinates mRider, Delivery mDeliveries[])
{
    std::vector<float> prob = { 0.99 };
    
    std::vector<int> rank = { 9, 12, 15 };
    
    std::pair<int, std::vector<int>> res;
    res.first = 0;
    res.second = { 0 };
    
    for(auto p : prob)
    {
        for(auto k : rank)
        {
            std::pair<int, std::vector<int>> temp = simulate(mRider, mDeliveries, p, k);
            
            if(temp.first > res.first)
            {
                res = temp;
//                std::cout << p << " " << k << "\n";
            }
        }
    }
    
    for(auto id : res.second)
    {
        deliver(id);
    }
}
