const int MAX_DEVICE = 256;

const int MAX_ROOM = 52;



struct DetectedDevice

{

	int id;

	int power;

	int tx;

	int ty;

};



extern int scan_device(int mDeviceId, int mScanPower, DetectedDevice mDetected[]);


void scan(int mDeviceId, int mTotalDevice)

{

	return;

}

void result(int mDeviceIds[][MAX_DEVICE])

{

	return;

}
