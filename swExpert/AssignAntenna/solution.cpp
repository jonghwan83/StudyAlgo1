#define ABS(x) ((x<0)?(-(x)):(x))



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



void init(Coordinates antenna_list[])

{



}



void scanUE(Coordinates UE_list[], int antenna_range[], int assign_antenna[])

{



}
