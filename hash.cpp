/*hash.cpp*/

//
// Niko Castellana
// U. of Illinois, Chicago
// CS 251: Spring 2020
// 
// Lab Week 11: hashing of dates and lottery numbers (SOLUTION)
// 

#include <iostream>
#include <string>
#include <cctype>  /*isdigit*/
#include <regex>   /*regular expressions*/

#include "hash.h"

using namespace std;

//
// isNumeric
//
// Returns true if the given string is numeric (all digits), false
// if not.  If the string is empty, false is returned since there 
// are no digits present.
//
bool isNumeric(string s)
{
	//
	// A string is numeric if it contains 1 or more digits, so let's
	// use a regular expression to check that.
	//
	// we are using POSIX basic notation for regular expressions, see
	//   https://en.cppreference.com/w/cpp/regex/ecmascript
	// where [:d:] means digit, [[:d:]] means match a single digit.
	// The + means match the preceding sub-expression 1 or more times.
	//
	regex pattern("[[:d:]]+", regex::ECMAScript);

	smatch matchResults;  // provides more details on the match

	if (regex_match(s, matchResults, pattern))
		return true;
	else
		return false;
}


//
// string2int
// 
// Converts a string to an integer, unless string is empty, in
// which case 0 is returned.
// 
int string2int(string s)
{
	if (s == "")
		return 0;
	else
		return stoi(s);
}


// Hashes the station ids
int HashStationID(string ID, int N)
{
	if(ID.length() < 1)
		return -1;
	
	if(!isNumeric(ID))
		return -1;

	int id = string2int(ID);
	int index = id % N;
	
	return index;
}


// Hashes the station abbreviations
int HashStationAbbrev(string abbrev, int N)
{
	if(abbrev.length() < 1)
		return -1;
	
	int sum  = 0;
	
	// Add ascii values for each abbreviation
	for(int i = 0; abbrev[i] != '\0'; i++)
	{
		sum = sum + abbrev[i];
	}
	
	int index = sum;

	return index % N;
}


// Hashes the trip ids
int HashTripID(string tripID, int N)
{

	if(tripID.length() < 3)
		return -1;
	
	// Check for T and R
	if(tripID[0] != 'T' || tripID[1] != 'r') 
			return -1;
	
	tripID.erase(0,2); // Erase 'T' and 'r'
			
	// Check for numbers after 'T' and 'r'
	if(!isNumeric(tripID))
		return -1;
	
	int index = string2int(tripID);
	
	return index % N;
}


// Hashes the bike ids
int HashBikeID(string bikeID, int N)
{
	// Check for B at the beginning
	if(bikeID[0] != 'B')
		return -1;
	
	if(bikeID.length() < 2)
		return -1;
	
	bikeID.erase(0,1); // erase B
	
	int index = string2int(bikeID);
	
	return index % N;
}

