struct Road

{

	int city1;

	int city2;

	int status;

	int id;

};



struct Freight

{

	int pos;

	int dest;

	int weight;

};



extern bool move(int mRoadID, int repairNum);

extern bool load(int mFreightID);

extern bool unload();

extern void getRoadInfo(Road mRet[]);

extern void getFreightInfo(Freight mRet[]);





void process() {

}
