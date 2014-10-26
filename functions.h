//============================================================================
// Name        : functions.h
// Author      : Ricardo Guerrero Gómez-Olmedo
// Version     : 1.0
// Copyright   : LGPL
// Description : Set of useful functions for working with detections in C++
//============================================================================

#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_

using namespace std;

namespace DetectionSpace
{

	//We will store in this structure the information we read from the output of a detector
	struct detection
	{
		int x1;
		int y1;
		int x2;
		int y2;
		float score;
	};

	//With this function we will ensure there is not any wrong value in our detections
	vector <struct detection> detectionsFiltering (vector <struct detection> &source, int width, int height);

	//This function return true if there is any overlap between all detections.
	//In case of a video with a low number of overlapped detections is much	more faster
	//call first collisionsAll and only call fastNms in case of collisionsAll returns true.
	//Even if there are many detections, the overhead introduced by collisionsAll is really low
	//and could be a good idea call it everytime
	bool collisionsAll (vector <struct detection> &source);

	//The same as above but between two detections
	bool collisions1vs1 (struct detection &car1, struct detection &car2);

	//This is the Tomasz Malisiewicz's version of Non-Maximum Suppression based on Pedro Felzenszwalb's version
	//The default parameter for overlap is 0.5, the same used in Pascal database
	//IMPORTANT: ensure your are not giving a detection with area=0 or it this function will fail
	//detectionsFiltering() could be use for this purpose
	vector <struct detection> fastNms (const vector <struct detection> &source, float overlap = 0.5);

	//The input string of this function stores all the data we need to feed a struct detection, so here
	//we cut this string and store it into detection
	void processString (string string_detection, struct detection *detection);

	//It acts exactly as if we use a=max(tresh, b) in Matlab, if there are values lower than tresh in the list
	//automatically are changed to tresh value
	inline vector <int> maxMatlab (int tresh, vector<int> list);

	//It acts exactly as if we use a=min(tresh, b) in Matlab, if there are values greater than tresh in the list
	//automatically are changed to tresh value
	inline vector <int> minMatlab (int tresh, vector<int> list);

	//It performs the operation first(i)-second(i)+scalar. If we didn't provide any scalar, it will only substract second from first
	vector <int> subsVector (vector<int> &first, vector<int> &second, int scalar=0);

	//If we execute [b c] = sort(a) in Matlab, the returned variable b would content the position each element
	//has before being sorted. This function behaves similar, but only returns b, not c.
	//It sorts the elements in ascending order -> source={9, 5, 1, 6}, destination={1, 5, 6, 9}
	vector <int> sortMatlab (vector<float> s);

}




#endif /* FUNCTIONS_H_ */
