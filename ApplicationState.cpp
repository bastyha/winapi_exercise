enum RotationDirection
{	
	LEFT  = -1,
	NONE  =  0,
	RIGHT =  1,
};

struct ApplicationState
{
	RotationDirection direction = RotationDirection::NONE;
	int isTimerRunning = 0;
	int rotationDegree = 0;
};