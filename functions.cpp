//============================================================================
// Name        : functions.cpp
// Author      : Ricardo Guerrero Gómez-Olmedo
// Version     : 1.0
// Copyright   : LGPL v.3
// Description : Set of useful functions for working with detections in C++
//============================================================================

#include <opencv2/opencv.hpp>
#include <opencv2/legacy/legacy.hpp>
#include <opencv2/video/tracking.hpp>
#include <opencv/cv.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>

#include "functions.h"


namespace DetectionSpace
{
	void processString (string string_detection, struct detection *detection)
	{
		string aux;

		aux = string_detection.substr(0,string_detection.find("\t"));
		detection->x1 = atoi(aux.c_str());
		string_detection = string_detection.substr( string_detection.find("\t")+1, string_detection.size());
		aux = string_detection.substr(0,string_detection.find("\t"));
		detection->y1 = atoi(aux.c_str());
		string_detection = string_detection.substr( string_detection.find("\t")+1, string_detection.size());
		aux = string_detection.substr(0,string_detection.find("\t"));
		detection->x2 = atoi(aux.c_str());
		string_detection = string_detection.substr( string_detection.find("\t")+1, string_detection.size());
		aux = string_detection.substr(0,string_detection.find("\t"));
		detection->y2 = atoi(aux.c_str());
		string_detection = string_detection.substr( string_detection.find("\t")+1, string_detection.size());
		aux = string_detection.substr(0,string_detection.find("\t"));
		detection->score = atof(aux.c_str());
	}

	inline vector <int> maxMatlab (int tresh, vector<int> list)
	{
		for (unsigned int i=0;i<list.size();i++)
		{
			if (list.at(i)<tresh)
			{
				list.at(i) = tresh;
			}
		}

		return list;
	}


	inline vector <int> minMatlab (int tresh, vector<int> list)
	{
		for (unsigned int i=0;i<list.size();i++)
		{
			if (list.at(i)>tresh)
			{
				list.at(i) = tresh;
			}
		}

		return list;
	}


	vector <int> subsVector (vector<int> &first, vector<int> &second, int scalar)
	{
		vector<int> result;

		for (unsigned int i=0;i<first.size();i++)
		{
			result.push_back(first.at(i)-second.at(i)+scalar);
		}

		return result;
	}

	vector <struct detection> detectionsFiltering (vector <struct detection> &source, int width, int height)
	{
		bool flagWrong = false;
		vector <struct detection> destination;
		unsigned int i;

		for (i=0; i<source.size(); i++)
		{
			//Out of range
			if (source.at(i).x1 < 0 || source.at(i).x1 > width)
			{
				flagWrong = true;
			}
			if (source.at(i).x2 < 0 || source.at(i).x2 > width)
			{
				flagWrong = true;
			}
			if (source.at(i).y1 < 0 || source.at(i).y1 > height)
			{
				flagWrong = true;
			}
			if (source.at(i).y2 < 0 || source.at(i).y2 > height)
			{
				flagWrong = true;
			}

			//Concept error
			if (source.at(i).x1 >= source.at(i).x2)
			{
				flagWrong = true;
			}
			if (source.at(i).y1 >= source.at(i).y2)
			{
				flagWrong = true;
			}

			//Area = 0
			if ( (source.at(i).x2-source.at(i).x1) * (source.at(i).y2-source.at(i).y1) == 0 )
			{
				flagWrong = true;
			}

			//If the detection is correct, then we store it
			if (!flagWrong)
			{
				destination.push_back(source.at(i));
			}
		}

		return destination;
	}

	vector <int> sortMatlab (vector<float> s)
	{
		vector <float> sSource = s;
		vector <int> I;
		bool found = false;

		sort(s.begin(), s.end());

		for (unsigned int i=0; i<s.size(); i++)
		{
			for (unsigned int j=0; j<sSource.size(); j++)
			{
				if (!found)
				{
					if (s.at(i) == sSource.at(j))
					{
						I.push_back (j);
						found = true;
					}
				}

			}

			found = false;
		}

		return I;
	}


	vector <struct detection> fastNms (const vector <struct detection> &source, float overlap)
	{
		unsigned int k;

		if (source.size()<2)
		{
			return source;
		}

		vector <int> x1, x2, y1, y2;
		vector <float> s;

		for (k=0;k<source.size();k++)
		{
			x1.push_back(source.at(k).x1);
			x2.push_back(source.at(k).x2);
			y1.push_back(source.at(k).y1);
			y2.push_back(source.at(k).y2);
			s.push_back(source.at(k).score);
		}

		vector <int> area;
		int areaTemp;

		/** Compute the area of each detection **/
		for (k=0; k<source.size();k++)
		{
			areaTemp = (source.at(k).x2-source.at(k).x1) * (source.at(k).y2-source.at(k).y1);
			area.push_back (areaTemp);
		}


		/** Sorting the scores from lower to greater **/
		vector <int> I = sortMatlab (s);


		vector <int> pick (source.size(), 0);
		int counter = 0, i;
		vector <int> xx1, yy1, xx2, yy2;
		vector <float> o (I.size());
		vector <int> aux, w, h;

		while (!I.empty())
		{
			i = I.back();
			pick.at(counter) = i;
			counter++;

			aux.clear();
			for (k=0;k<I.size()-1;k++)
			{
				aux.push_back( x1.at(I.at(k)) );
			}
			xx1 = maxMatlab(x1.at(i), aux);

			aux.clear();
			for (k=0;k<I.size()-1;k++)
			{
				aux.push_back( y1.at(I.at(k)) );
			}
			yy1 = maxMatlab(y1.at(i), aux);

			aux.clear();
			for (k=0;k<I.size()-1;k++)
			{
				aux.push_back( x2.at(I.at(k)) );
			}
			xx2 = minMatlab(x2.at(i), aux);

			aux.clear();
			for (k=0;k<I.size()-1;k++)
			{
				aux.push_back( y2.at(I.at(k)) );
			}
			yy2 = minMatlab(y2.at(i), aux);

			w = maxMatlab(0, subsVector(xx2, xx1, 1) );
			h = maxMatlab(0, subsVector(xx2, xx1, 1) );


			for (k=0;k<(I.size()-1);k++) //The last element's overlap is not computed
			{
				o.at(k) = (float)w.at(k)*h.at(k)/area.at(I.at(k));
			}

			I.pop_back(); //Erase the last element

			for (k=0;k<I.size();k++) //The size of I is changing, but is evaluated every iteration, so there is no problem
			{
				if (o.at(k)>overlap)
				{
					I.erase(I.begin()+k);
				}
			}

		}

		aux.clear();
		aux.insert(aux.begin(), pick.begin(), pick.begin()+counter);
		//insert method copy the interval [begin, end). It means the last element is not copied.
		//The original implementation copy pick from begining to end-1, so it's ok, we don't need to do any change.
		pick = aux;

		vector <struct detection> top;

		for (k=0;k<pick.size();k++)
		{
			top.push_back( source.at(pick.at(k)) );
		}

		return top;
	}


	bool collisionsAll (vector <struct detection> &source)
	{
		for (unsigned int i=0; i<source.size();i++)
		{
			for (unsigned int j=0; j<source.size();j++)
			{
				if (i!=j)
				{
					if ( collisions1vs1(source.at(i), source.at(j)) )
					{
						return true;
					}
				}
			}
		}

		return false;
	}


	bool collisions1vs1 (struct detection &car1, struct detection &car2)
	{
		int xmin1 = car1.x1;
		int ymin1 = car1.y2;
		int xmax1 = car1.x2;
		int ymax1 = car1.y2;

		int xmin2 = car2.x1;
		int ymin2 = car2.y1;
		int xmax2 = car2.x2;
		int ymax2 = car2.y2;

		bool flag_x = 0;
		bool flag_y = 0;

		/** Check if there is overlap in X axis **/
		if (xmin1 < xmin2) //car1 is to the left of car2
		{
			if (xmin2 < xmax1)
			{
				flag_x = true;
			}
		}
		else //car2 is to the left of car1
		{
			if (xmin1 < xmax2)
			{
				flag_x = true;
			}
		}

		/** Check if there is overlap in Y axis **/
		if (ymin1 < ymin2) //car1 is above car2
		{
			if (ymin2 < ymax1)
			{
				flag_y = true;
			}
		}
		else //car2 is above car1
		{
			if (ymin1 < ymax2)
			{
				flag_y = true;
			}
		}

		return (flag_x && flag_y);
	}

}
