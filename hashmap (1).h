/*hashmap.h*/

//
// << Niko Castellana >>
// U. of Illinois, Chicago
// CS 251: Spring 2020
// Project #06: hashing
//
// References: original non-collision code written by Prof. Hummel (UIC)
// 
// hashmap implements a hash table and does not assume a perfect hash function;
// the class is designed to handle collisions.  The class provides
// the underlying hash table along with insert() and search() functions;
// probing is used in both the insert() and search() to handle collisions;
// the user of this class must provided a Hash(key, N) function that 
// hashes the given key and returns an integer in the range 0..N-1, 
// inclusive, if the key is valid.  If the key is invalid, the Hash() 
// function must return a negative value.
//
// The hash function is provided in the calls to insert and search.
// Pass the name of your hash function as the 3rd parameter.  Example:
//
//   int myHash(string key, int N)
//   { ... }
//
//   int N = 10000;
//   hashmap<string, double>  hmap(N);  
//
//   if (hmap.insert("deep-dish pizza", 19.99, myHash))
//     cout << "inserted" << endl;
//   else
//     cout << "not inserted?!" << endl;
// 

#pragma once

#include <iostream>
#include <functional>
#include <vector>

using namespace std;

template<typename KeyT, typename ValueT>
class hashmap
{
private:
  struct HashEntry
  {
    bool   ContainsData;  // false => empty, true => contains data
    KeyT   Key;           // (key, value)
    ValueT Value;

    HashEntry()
    {
      ContainsData = false;
    }
  };

  HashEntry* HT;  // hash table array of structs
  int        N;   // capacity of hash table (# of locations or array size)
	
public:
  //
  // constructor: N is the size of the underlying hash table
  //
  hashmap(int N)
  {
    this->N = N;
    this->HT = new HashEntry[N];
  }
  
  //
  // destructor
  //
  virtual ~hashmap()
  {
    delete[] this->HT;
  }
	
	//
	// copy constructor
	// 
	hashmap(const hashmap &other)
	{
		/* Copy the size of other array
		 * and allocate a new array */
		this->N = other.N;
		HashEntry* HT2 = new HashEntry[other.N];
		
		/* Copy from other array to new array */
		for(int i = 0; i < N; ++i)
		{
			HT2[i].Key = other.HT[i].Key;
			HT2[i].Value = other.HT[i].Value;
			HT2[i].ContainsData = other.HT[i].ContainsData;
		}
		
		this->HT = HT2; // Complete the copy
	}
	
	//
	// Equals operator
	// 
	hashmap& operator=(const hashmap&other)
	{ 
		/* Clear this array */
		delete[] this->HT;
		
		/* Copy the size of other array
		 * and allocate a new array */
		this->N = other.N;
		HashEntry* HT2 = new HashEntry[other.N];
		
		/* Copy from other array to new array */
		for(int i = 0; i < N; ++i)
		{
			HT2[i].Key = other.HT[i].Key;
			HT2[i].Value = other.HT[i].Value;
			HT2[i].ContainsData = other.HT[i].ContainsData;
		}
		
		this->HT = HT2; // Complete the copy
		
		return *this;
	}

  //
  // getN
  //
  // Returns the capacity of the hash table, i.e. the value N that
  // was passed to the constructor.
  //
  int getN() const
  {
    return this->N;
  }
	
	// Takes all the keys from the hash table
	// and pushes them into a vector
	vector<KeyT> getKeys()
	{
			vector<KeyT> ID;
			int index = 0;
			
			while(index < N)
			{
				if(HT[index].ContainsData)
				{
					ID.push_back(HT[index].Key);
				}
				
				index++;
			}
			
			return ID;
	}

  //
  // insert
  //
  // Inserts the given (key, value) pair into the hash table, overwriting
  // the previous value if already inserted.  If the insert was successful
  // then true is returned, otherwise false is returned (meaning the key
  // was not hashed successfully, e.g. due to improper formatting).
  //
  // NOTE: the caller must provide a function Hash(key, N) that returns
  // an integer in the the range 0..N-1, inclusive, if the key is valid.
  // If the key is invalid, the hash function must return a negative
  // integer. The hash function is provided by simply calling insert
  // with the name of your hash function.  Example:
  //
  //   int myHash(string key, int N)
  //   { ... }
  //
  //   int N = 10000;
  //   hashmap<string, double>  hmap(N);  
  //
  //   if (hmap.insert("deep-dish pizza", 19.99, myHash))
  //     cout << "inserted" << endl;
  //   else
  //     cout << "not inserted?!" << endl;
  //
  bool insert(KeyT key, ValueT value, function<int(KeyT,int)> Hash)
  {
    //
    // Call the provided hash function with the key, and N, the size
    // of our hash table:
    //
  
	   int index = Hash(key, N); // Hash the key to find index

    //
    // insert:
    //

		if (index < 0 || index >= N)  // invalid key or hash function:
		{
			return false;
		}
		else // Valid key and hash:
		{
			/* Loop while the index contains data */
			while(HT[index].ContainsData == true)
			{
				if(HT[index].Key == key) // Key already exists:
					break;
				
				// Move to next array index:
				index++;
				
				// Move to first array index when
				// end of the array is reached: 
				if(index == N)
					index = 0;
			}
		
			/* Insert Key and Value */
			HT[index].Key = key;
			HT[index].Value = value;
			HT[index].ContainsData = true;  // array location now contains data
			
			return true;
		}
			
  }

	//
  // Inserts into the hash function without probing, used for bike usage
	bool insert2(KeyT key, ValueT value, function<int(KeyT,int)> Hash)
  {
    //
    // Call the provided hash function with the key, and N, the size
    // of our hash table:
    //
  
	   int index = Hash(key, N); // Hash the key to find index

    //
    // insert:
    //

		if (index < 0 || index >= N)  // invalid key or hash function:
		{
			return false;
		}
		else // Valid key and hash:
		{
			/* Loop while the index contains data */
			if(HT[index].ContainsData == true)
			{
				HT[index].Value.Usage++; // increment bike usage
			}
			else
			{
				/* Insert Key and Value */
				HT[index].Key = key;
				HT[index].Value = value;
				HT[index].ContainsData = true;  // array location now contains data
			  HT[index].Value.Usage = 1; // bike found, set usage to 1
			}
		
			return true;
		}
			
  }
	
  //
  // search
  //
  // Searches the hash table for the matching key, and if found then
  // (1) the value is returned via the reference parameter and (2) true
  // is returned.  If the key could not be found, or if the hash 
  // function returned an invalid value (e.g. due to an improperly 
  // formatted key), then false is returned.
  //
  // NOTE: the caller must provide a function Hash(key, N) that returns
  // an integer in the the range 0..N-1, inclusive, if the key is valid.
  // If the key is invalid, the hash function must return a negative
  // integer. The hash function is provided by simply calling search
  // with the name of your hash function.  Example:
  //
  //   int myHash(string key, int N)
  //   { ... }
  //
  //   int N = 10000;
  //   hashmap<string, double>  hmap(N);
  //   ...
  //   double price;
  //   if (hmap.search("deep-dish pizza", price, myHash))
  //     cout << "deep-dish piazza costs " << price << endl;
  //   else
  //     cout << "not found?!" << endl;
  //
  bool search(KeyT key, ValueT& value, function<int(KeyT,int)> Hash) const
  {
    //
    // Call the provided hash function with the key, and N, the size
    // of our hash table:
    //
	  int index = Hash(key, N); // Hash the key to find index

    //
    // search:
    //
		if (index < 0 || index >= N)  // invalid key or hash function:
		{
			return false;
		}
		
		/* Loop while the index contains data */
		while(HT[index].ContainsData == true)
		{
			if(HT[index].ContainsData == false) // Data not found:
			{
				return false;
			}
		
			if(HT[index].Key == key) // Data found, return value:
			{
				value = HT[index].Value;
				return true;
			}
			else // Move to next array index:
			{
				index++;
				
				// Move to first array index when
				// end of the array is reached: 
				if(index == N)
					index = 0;
			}

		}
		
		return false; 
  }

};
