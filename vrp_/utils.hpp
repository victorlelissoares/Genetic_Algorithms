#include <cmath>
#include <iostream>

using namespace std;

//função para gerar números(double) aleatórios 
inline double fRand(double fMin, double fMax){
    return fMin + ((double)rand() / RAND_MAX) * (fMax - fMin);
}

inline double distEuclidiana(pair<int, int> cl1, pair<int, int>cl2){	
	//( (c1x - c2x)^2 + (c1y - c2y)^2 )^(1/2)
 	return sqrt( pow( (cl1.first - cl2.first), 2) + pow( (cl1.second - cl2.second), 2) );
}


