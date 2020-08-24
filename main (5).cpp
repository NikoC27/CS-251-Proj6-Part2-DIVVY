/*main.cpp*/

//
// Niko Castellana
// U. of Illinois, Chicago
// CS 251: Spring 2020
// 
// Project 6: DIVVY Hash Program
// 
// This project reads through files, one of
// the files containing a list of station 
// data, the other containing data for
// trips. Structures are used to store lines
// in each file. A key is taken from each line
// and is used in its own unique Hash function
// to store the data in an array. Collisions
// may occur but are taken care of through probing.
// The program also outputs stations that are 
// "nearby" by calculating the distance based on
// their latitudes and longitudes. Lastly, the
// program can find "similar" trips to the station
// being input based on latitude and longitude.
// 

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <set>
#include <algorithm>
#include <utility>
#include "hash.h"
#include "hashmap.h"
#include "util.h"

using namespace std;

/* Structure used for 
 * Station Data */
struct Station
{
	string ID;
	string Abbrev;
	string Fullname;
	string Latitude;
	string Longitude;
	string Capacity;
	string OnlineDate;
	
		Station()
		{
			ID          = "";
			Abbrev      = "";
			Fullname    = "";
			Latitude    = "";
			Longitude   = "";
			Capacity    = "";
			OnlineDate  = ""; 
		}
};


/* Structure used for 
 * Trip Data */
struct Trip
{
	string TripID;
	string StartTime;
	string StopTime;
	string BikeID;
	string Duration;
	string FromStation;
	string ToStation;
	string Gender;
	string BirthYear;
	
		Trip()
		{
			TripID      = "";
			StartTime   = "";
			StopTime    = "";
			BikeID      = "";
			Duration    = "";
			FromStation = "";
			ToStation   = "";
			Gender      = "";
			BirthYear   = "";
		}
};


/* Structure used for 
 * Bike Data */
struct Bike
{
	string ID;
	int Usage;
	
		Bike()
		{
			ID = "";
			Usage = 0;
		}
};
	

// Reads through the "station" file input in main 
// and sends the content of the file to a hashmap. 
// The number of stations in the file are also calculated 
bool inputStationData(string filename, hashmap<string, Station>& hmapIDStation, 
											hashmap<string, Station>& hmapABStation, int &numStations)
{
		ifstream  infile(filename);
	
		cout << "Reading " << filename << endl;
	
		if (!infile.good())
		{
			cout << "**Error: unable to open '" << filename << "'..." << endl;
			return false;
		}
	
		// file is open, start processing:
		string line;
	
		getline(infile, line);  // input and discard first row --- header row
	
		/* Loop through the file */
		while (getline(infile, line))
		{
			stringstream s(line);
			
			string id, abbrev, fullname, latitude;
			string longitude, capacity, onlinedate;
			
			/* Separtate each line in the file by commas */
			getline(s, id, ',');
			getline(s, abbrev, ',');
			getline(s, fullname, ',');
			getline(s, latitude, ',');
			getline(s, longitude, ',');
			getline(s, capacity, ',');
			getline(s, onlinedate, ',');
			
			/* Declare Station Struct */
			Station sd;
			
			sd.ID         = id;
			sd.Abbrev     = abbrev;
			sd.Fullname   = fullname;
			sd.Latitude   = latitude;
			sd.Longitude  = longitude;
			sd.Capacity   = capacity;
			sd.OnlineDate = onlinedate;
			
			/* Insert both id and abbrev keys with
			 * their data into the hash */
			hmapIDStation.insert(id, sd, HashStationID);
      hmapABStation.insert(abbrev, sd, HashStationAbbrev);
							
			numStations++; // number of stations updated
		}
	
	return true;  // we have data to be processed:
}


// Reads through the "trip" file input in main 
// and sends the content of the file to a hashmap. 
// The number of stations and bikes in the file are also calculated 
bool inputTripData(string filename, hashmap<string, Trip>& hmapTrip,
									 hashmap<string, Bike>& hmapBike, int &numTrips, int &numBikes)
{
		ifstream  infile(filename);
	
		cout << "Reading " << filename << endl;
	
		if (!infile.good())
		{
			cout << "**Error: unable to open '" << filename << "'..." << endl;
			return false;
		}
		
		set<string> bikes; // Declare set to count number of bikes
	
		// file is open, start processing:
		string line;
	
		getline(infile, line);  // input and discard first row --- header row
	
		/* Loop through the file */
		while (getline(infile, line))
		{
			stringstream s(line);
			
			string tripID, startTime, stopTime, bikeID;
			string duration, fromStation, toStation;
			string gender, birthYear;
			
			/* Seperate each line in the file by commas */
			getline(s, tripID, ',');
			getline(s, startTime, ',');
			getline(s, stopTime, ',');
			getline(s, bikeID, ',');
			getline(s, duration, ',');
			getline(s, fromStation, ',');
			getline(s, toStation, ',');
			getline(s, gender, ',');
			getline(s, birthYear, ',');
			
			/* Declare Trip and Bike Structs */
			Trip td;
			Bike bd;
			
			/* Copy data from current line
			 * of the Trips file to the 
			 * Trip struct */
			td.TripID      = tripID;
			td.StartTime   = startTime;
			td.StopTime    = stopTime;
			td.BikeID      = bikeID;
			td.Duration    = duration;
			td.FromStation = fromStation;
			td.ToStation   = toStation;
			td.Gender      = gender;
			td.BirthYear   = birthYear;
			
			/* Copy data from current line
			 * of the Trips file to the 
			 * Bike struct */
			bd.ID = bikeID;
			bikes.insert(bikeID); // insert unique bikeIDs in set
			
			/* Insert both tripID and bikeID keys with
			 * their data into the hash */
			hmapTrip.insert(tripID, td, HashTripID); // insert with probing
			hmapBike.insert2(bikeID, bd, HashBikeID); // insert without probing
			
			numTrips++; // number of trips updated
		}
	
	numBikes = bikes.size(); // number of bikes updated
	
	return true;  // we have data to be processed:
}
	

// Prints the help commands when userInput
// types in "help" from main
void helpCommand()
{
			cout << "Available commands:" << endl;
			cout << "Enter a station id (e.g 451)" << endl;
			cout << "Enter a station abbreviation (e.g Adler)" << endl;
			cout << "Enter a trip id (e.g Tr10426561)" << endl;
			cout << "Enter a bike id (e.g B5218)" << endl;
			cout << "Nearby stations (e.g nearby 41.86 -87.62 0.5)" << endl;
			cout << "Similar trips (e.g similar Tr10424639 0.3)" << endl;
			cout << endl;
}


// Converts the duration of time in the 
// trips file
void convertDuration(string duration)
{
	
	/* Convert seconds minutes and hour */
	int seconds = string2int(duration) % 60;
	int minutes = string2int(duration) / 60;
	int hour = minutes / 60;
	
	if(hour >= 1) // Print hour when minutes is greater than 60
	{
		cout << hour << " hour, " << minutes << " minutes, "  << seconds << " seconds"<< endl;
	}
	else // Print minutes and seconds
	{
		cout << minutes << " minutes, "  << seconds << " seconds"<< endl;
	}
	
}


// Orders the key and value pairs in the vector
bool orderingDist (const pair<string, double> &p1, const pair<string, double> &p2)
{
	return p1.second < p2.second;
}


// Prints "nearby" stations by calculating the 
// latitude, longitude and # of miles entered by the user
void printNearby(string userInput, hashmap<string, Station> &hmapIDStation)
{
		// Cuts off input after "nearby"
		string input = userInput.substr(7);
		
		stringstream s(input);
		string latitude, longitude, mile;
		
	  /* Seperate the input by spaces */
		getline(s, latitude, ' ');
		getline(s, longitude, ' ');
		getline(s, mile, ' ');
		
	  /* Convert latitude, longitude and mile to double */
		double latitudes = (double)atof(latitude.c_str());
		double longitudes = (double)atof(longitude.c_str());
		double miles = (double)atof(mile.c_str());
	
		/* Declare doubles for lat and longs found
		 * in the hashmap, as well as the amount of
		 * distance to be calculated */
		double latitudes2, longitudes2, distance;
	
		/* Declare station struct, vector 
		 * of station keys and distance vector */
		Station sd;
		vector<string> skeys = hmapIDStation.getKeys();
		vector<pair <string, double>> distanceVect;
	
		/* Loop through the station keys vector */
		for(size_t i = 0; i < skeys.size(); i++)
		{
			/* Searches the hash table for each key in the
			 * vector and gets lat2 and long2 coordinates */
			hmapIDStation.search(skeys[i], sd, HashStationID);
			latitudes2 = (double) atof(sd.Latitude.c_str());
			longitudes2 = (double) atof(sd.Longitude.c_str());
			
			/* Calculate distance */
			distance = distBetween2Points(latitudes, longitudes, latitudes2, longitudes2);
			
			/* Push back into keys and distance
			 * into a new vector */
			if(distance <= miles)
			{
				distanceVect.push_back(make_pair(skeys[i], distance));
			}
		}
		
		cout << "Stations within " << mile << " miles of "
			   << "(" << latitude << ", " << longitude << ")" << endl;
		
		if(distanceVect.empty()) // Distance not found
		{
			cout << " none found" << endl;
			cout << endl;
			return;
		}
		else // Distances found, sort vector and print the pairs
		{
			sort(distanceVect.begin(), distanceVect.end(), orderingDist);
			for(auto &pair: distanceVect)
			{
				cout << " station " << pair.first << ": " 
					   << pair.second  << " miles"<< endl;
			}
			cout << endl;
		}
		
}


// Prints "similar" stations by calculating the 
// latitude, longitude and # of miles entered by the user
void printSimilarPath(string userInput,hashmap<string, Station> &hmapIDStation,
											hashmap<string, Trip> &hmapTrip)
{
	// Cuts input after similar
	string input = userInput.substr(8);
	
	// Declare sets for from and to stations
	set<int> fromStation;
	set<int> toStation;
	
	stringstream s(input);
	
	string tripid, mile;
	
	getline(s, tripid, ' ');
	getline(s, mile, ' ');
	
	// Convert mile to double
	double miles = (double)atof(mile.c_str());
	
	cout << "Trips that follow a similar path (+/-" << mile
		   << " miles"<< ")" << " as " << tripid << endl;
	
	/* Declare vector of keys for Stations and trips */
	vector<string> skeys = hmapIDStation.getKeys();
	vector<string> tkeys = hmapTrip.getKeys();
	
	/* Declare Station and Trip Structs */
	Station sd;
	Trip td;
	
	int tripCounter = 0; // counts number of trips
	double lat1 = 0;     // latitude from the trip file
	double long1 = 0;    // longitude from the trip file
	double lat2 = 0;     // latitude from the station file
	double long2 = 0;    // longitude from the station file
	double distance = 0; // distance from stations
	
	// Find the tripID in the hash map
	hmapTrip.search(tripid, td, HashTripID);
	
	// Look for all stations that are near the starting station
	if(hmapIDStation.search(td.FromStation, sd, HashStationID))
	{
		  /* Get latitude and longitude */
			lat1 = (double) atof(sd.Latitude.c_str());   
			long1 = (double) atof(sd.Longitude.c_str());
	}
	else // No trips were found
	{
		cout << " no such trip" << endl;
		cout << endl;
		return;
	}
	
	/* Loop through the station keys vector */
	for(size_t i = 0; i < skeys.size(); i++) 
	{
			/* Searches the hash table for each key in the
			 * vector and gets lat2 and long2 coordinates */
			hmapIDStation.search(skeys[i], sd, HashStationID);
			lat2 = (double) atof(sd.Latitude.c_str());
			long2 = (double) atof(sd.Longitude.c_str());
		
			/* Calculate distance */
			distance = distBetween2Points(lat1, long1, lat2, long2);
			
			if(distance <= miles) // insert in fromStation set
			{
				fromStation.insert(string2int(skeys[i])); 
			}
	}
	
	// Look for all stations that are near the ending station
	if(hmapIDStation.search(td.ToStation, sd, HashStationID))
	{
		lat1 = (double) atof(sd.Latitude.c_str());
		long1 = (double) atof(sd.Longitude.c_str());
	}
	else // No trips were found
	{
		cout << " no such trip" << endl;
		cout << endl;
		return;
	}
	
	/* Loop through the station keys vector*/
	for(size_t i = 0; i < skeys.size(); i++) 
	{
			hmapIDStation.search(skeys[i], sd, HashStationID);
			lat2 = (double) atof(sd.Latitude.c_str());
			long2 = (double) atof(sd.Longitude.c_str());
			distance = distBetween2Points(lat1, long1, lat2, long2);
			
			if(distance <= miles) // insert in toStation set
			{
				toStation.insert(string2int(skeys[i])); // insert in toStation set
			}
	}
	
	
	cout << " nearby starting points: "; // print the fromStation set
	for(auto it : fromStation)
	{
		cout << it << " ";
	}
	cout << endl;
	
	cout << " nearby ending points: "; // print the toStation set
	for(auto it : toStation)
	{
		cout << it << " ";
	}
	cout << endl;
	
	/* Loop through the trip keys vector */
	for(auto& it : tkeys)
	{
		hmapTrip.search(it, td, HashTripID); // Update trips struct
		
		// Get the trip count
		if(fromStation.count(string2int(td.FromStation)) && toStation.count(string2int(td.ToStation)))
			tripCounter++;
	}
	
	// Print the trip count
	cout << " trip count: " << tripCounter << endl << endl;
	
}


// Prints out the station data
void printStationData(Station &sd)
{
		cout << "Station: " << endl;
		cout << " ID: " << sd.ID << endl;
		cout << " Abbrev: " << sd.Abbrev << endl;
		cout << " Fullname: " << sd.Fullname << endl;
		cout << " Location: (" << (double) atof(sd.Latitude.c_str()) << ", " 
				 << (double) atof(sd.Longitude.c_str()) << ")" << endl;
		cout << " Capacity: " << sd.Capacity << endl;
		cout << " Online date: " << sd.OnlineDate << endl;
		cout << endl;
}


// Prints out the trip data
void printTripData(Trip &td, Station &sd ,hashmap<string, Station> &hmapIDStation)
{
		cout << "Trip: " << endl;
		cout << " ID: " << td.TripID << endl;
		cout << " Starttime: " << td.StartTime << endl;
		cout << " Bikeid: " << td.BikeID << endl;
		cout << " Duration: ";
		convertDuration(td.Duration);
	
		/* Search the station data using the trips struct
		 * as the key to update abbrev */
		hmapIDStation.search(td.FromStation, sd, HashStationID);
	
		cout << " From station: " << sd.Abbrev 
			   << " (" << td.FromStation << ")" << endl;
	
		/* Search the station data using the trips struct
		 * as the key to update abbrev */
		hmapIDStation.search(td.ToStation, sd, HashStationID);
	
		cout << " To station: " << sd.Abbrev 
		     << " (" << td.ToStation << ")" << endl;
		cout << " Rider identifies as: " << td.Gender << endl;
		cout << " Birthyear: " << td.BirthYear << endl;
		cout << endl;
}


// Prints out the bike data
void printBikeData(Bike &bd)
{
		cout << "Bike: " << endl;
		cout << " ID: " << bd.ID << endl;
		cout << " Usage: " << bd.Usage << endl;
		cout << endl;
}


int main()
{
		/* Input for two files
		 * and user commands */
		string stationsFilename;
		string tripsFilename;
		string userInput; 
			
		const int N  =   10000;   // stations size
		const int N2 = 2500000;   // trips size
		const int N3 =   50000;   // bikes size
	
		hashmap<string, Station> hmapIDStation(N); // Station hashmap
		hashmap<string, Station> hmapABStation(N); // Abbrev hashmap
		hashmap<string, Trip> hmapTrip(N2);        // Trip hashmap
		hashmap<string, Bike> hmapBike(N3);        // Bike hashmap

	  int numStations = 0;
		int numTrips = 0;
		int numBikes = 0;
	
		cout << "** DIVVY Analysis Program **" << endl << endl;
		cout << "Enter stations file> ";
		getline(cin, stationsFilename);   // prompt user for station file
		cout << "Enter trips file> ";
		getline(cin, tripsFilename);      // prompt user for trip file
		cout << endl;
	
		/* Read through the files */
		bool success1 = inputStationData(stationsFilename, hmapIDStation, hmapABStation, numStations);
		bool success2 = inputTripData(tripsFilename, hmapTrip, hmapBike, numTrips, numBikes);
		
		/* Print # stations, trips and bikes */
		cout << endl;
		cout << "# of stations: " << numStations << endl;
		cout << "# of trips: " << numTrips << endl;
		cout << "# of bikes: " << numBikes << endl << endl;
	
		/* File not found, close program */
		if(!success1 || !success2)
		{
			cout << "No data, file not found?" << endl;
			return 0;
		}
		
		cout << "Please enter a command, help, or #> ";
		getline(cin, userInput); // prompt user command
		
	  // Command loop
		while(userInput != "#")
		{
			Station sd; // Station struct
			Trip td;    // Trip struct
			Bike bd;    // Bike struct
			
			if(userInput == "help") // Execute help command
			{
				helpCommand();
		
			}
			else if(userInput.substr(0,6) == "nearby") // Execute nearby command
			{
				bool stationIDFound = hmapIDStation.search(userInput, sd, HashStationID);
				
				printNearby(userInput, hmapIDStation); // print nearby
			}
			else if(userInput.substr(0,7) == "similar") // Execute similar command
			{
				bool stationIDFound = hmapIDStation.search(userInput, sd, HashStationID);
				bool tripFound = hmapTrip.search(userInput, td, HashTripID);
				
				printSimilarPath(userInput, hmapIDStation, hmapTrip); // print similar
			}
			else if(isNumeric(userInput)) // Execute station id command
			{
				bool stationIDFound = hmapIDStation.search(userInput, sd, HashStationID);
				
				if(stationIDFound) // Station found, print
				{ 
					printStationData(sd);
				}
				else // Error
				{
					cout << "Station not found" << endl << endl;
				}
				
			}
			else if(userInput[0] == 'T' && userInput[1] == 'r') // Execute trip id command
			{

				bool tripFound = hmapTrip.search(userInput, td, HashTripID);
				
				if(tripFound) // Trip found, print
				{
					printTripData(td, sd, hmapIDStation);
				}
				else // Error
				{
					cout << "Trip not found" << endl << endl;
				}
				
			}
			else if(userInput[0] == 'B') // Execute bike id command
			{
				bool bikeFound = hmapBike.search(userInput, bd, HashBikeID);
				
				if(bikeFound) // Bike found, print
				{
					printBikeData(bd);
				}
				else // Error
				{
					cout << "Bike not found" << endl << endl;
				}
			}
			else // Execute station abbrev command
			{
				bool stationAbbrevFound = hmapABStation.search(userInput, sd, HashStationAbbrev);
				
				if(stationAbbrevFound) // Station found, print
				{
					printStationData(sd);
				}
				else
				{
					cout << "Station not found" << endl << endl;
				}
				
			}
			
			cout << "Please enter a command, help, or #> ";
			getline(cin, userInput);
		}
		
	return 0;
}
