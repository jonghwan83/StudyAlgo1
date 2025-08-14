#ifndef _CRT_SECURE_NO_WARNINGS

#define _CRT_SECURE_NO_WARNINGS

#endif



#include <stdio.h>



/* seed and pseudo_rand function will be changed */

static unsigned long long seed;



static int pseudo_rand(void) {

	seed = seed * 25214903917ULL + 11ULL;

	return (seed >> 16) & 0x3fffffff;

}



static long long gTotalScore = 0;

static long long PENALTY = 1000000000000;



/* These constant variables will NOT be changed */

static const int CITY_NUM = 1000;

static const int ROAD_NUM = 3000;

static const int FREIGHT_NUM = 2000;

static const int MAX_WEIGHT = 90;



struct Road {

	int city1;

	int city2;

	int status;

	int id;

};





struct Freight {

	int pos;

	int dest;

	int weight;

};



struct Truck {

	int pos;

	Freight* freight;

};



static Truck truck;

static Road roads[ROAD_NUM];

static Freight freights[FREIGHT_NUM];



////////////////////////////////////



bool move(int mRoadID, int repairNum)

{

	if (mRoadID < 0 || mRoadID >= ROAD_NUM || repairNum < 0 || repairNum + roads[mRoadID].status > 1000)

		return false;



	int totalWeight = 10 + ((truck.freight == nullptr) ? 0 : truck.freight->weight);

	int score = 10 + repairNum * totalWeight;





	if (repairNum + roads[mRoadID].status >= totalWeight) {

		if (truck.pos != roads[mRoadID].city1 && truck.pos != roads[mRoadID].city2)

			return false;



		if (truck.pos == roads[mRoadID].city1)

			truck.pos = roads[mRoadID].city2;

		else

			truck.pos = roads[mRoadID].city1;



		gTotalScore += score;

		roads[mRoadID].status += (repairNum - totalWeight);



		return true;

	}

	else {

		return false;

	}

}



bool load(int mFreightID)

{

	if (truck.freight != nullptr || truck.pos != freights[mFreightID].pos)

		return false;



	truck.freight = &freights[mFreightID];

	gTotalScore += truck.freight->weight * truck.freight->weight;



	return true;

}



bool unload()

{

	if (truck.freight == nullptr)

		return false;



	gTotalScore += truck.freight->weight * truck.freight->weight;



	truck.freight->pos = truck.pos;

	truck.freight = nullptr;



	return true;

}



void getRoadInfo(Road mRet[])

{

	for (int i = 0; i < ROAD_NUM; i++) {

		mRet[i] = roads[i];

	}

}



void getFreightInfo(Freight mRet[])

{

	for (int i = 0; i < FREIGHT_NUM; i++) {

		mRet[i] = freights[i];

	}

}



////////////////////////////////////

static void init()

{

	truck.pos = 0;

	truck.freight = nullptr;

	int roadIdx = 0;



	int citylist[CITY_NUM] = { 0, };



	for (int i = 0; i < CITY_NUM; i++)

		citylist[i] = i;



	int shuffle_cnt = 3000;



	for (int i = 0; i < 2; i++) {

		for (int j = 0; j < shuffle_cnt; j++) {

			int x = pseudo_rand() % CITY_NUM;

			int y = pseudo_rand() % CITY_NUM;



			int t = citylist[x];

			citylist[x] = citylist[y];

			citylist[y] = t;

		}



		for (int j = 0; j < CITY_NUM - 1; j++) {

			roads[roadIdx].city1 = citylist[j];

			roads[roadIdx].city2 = citylist[j + 1];

			roads[roadIdx].id = roadIdx;

			roads[roadIdx].status = pseudo_rand() % 300 + 100;



			roadIdx++;

		}

	}



	while (roadIdx < ROAD_NUM) {

		for (int j = 0; j < shuffle_cnt; j++) {

			int x = pseudo_rand() % CITY_NUM;

			int y = pseudo_rand() % CITY_NUM;



			int t = citylist[x];

			citylist[x] = citylist[y];

			citylist[y] = t;

		}



		int l = pseudo_rand() % CITY_NUM;



		if (roadIdx + l > ROAD_NUM) {

			l = ROAD_NUM - roadIdx;

		}



		int startIdx = pseudo_rand() % CITY_NUM;



		if (startIdx + l >= CITY_NUM) {

			startIdx = CITY_NUM - l - 1;

		}



		for (int j = 0; j < l; j++) {

			roads[roadIdx].city1 = citylist[j + startIdx];

			roads[roadIdx].city2 = citylist[j + startIdx + 1];

			roads[roadIdx].id = roadIdx;

			roads[roadIdx].status = pseudo_rand() % 300 + 100;



			roadIdx++;

		}

	}



	for (int i = 0; i < FREIGHT_NUM; i++) {

		freights[i].pos = pseudo_rand() % CITY_NUM;

		freights[i].dest = pseudo_rand() % CITY_NUM;

		freights[i].weight = pseudo_rand() % MAX_WEIGHT + 1;

	}



}



static bool score()

{

	for (int i = 0; i < FREIGHT_NUM; i++) {

		if (freights[i].pos != freights[i].dest)

			return false;

	}



	return true;

}



extern void process();



int main(int argc, char* argv[])

{

	setbuf(stdout, NULL);

//	freopen("sample_input.txt", "r", stdin);



	bool result = false;

	int TC;



	gTotalScore = 0;



	scanf("%d%lld", &TC, &seed);

	for (int tc = 0; tc < TC; ++tc) {

		init();

		process();

		if ((result = score()) == false)

			break;

	}

	long long SCORE = (result) ? gTotalScore : PENALTY;

	printf("SCORE: %lld\n", SCORE);



	return 0;

}
