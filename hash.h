/*hash.h*/

//
// Niko Castellana
// U. of Illinois, Chicago
// CS 251: Spring 2020
// 
// Project 6
// 

#include <string>

using namespace std;

bool isNumeric(string s);
int Hash(string theDate, int N);
int string2int(string s);
int HashStationID(string ID, int N);
int HashStationAbbrev(string abbrev, int N);
int HashTripID(string tripID, int N);
int HashBikeID(string bikeID, int N);