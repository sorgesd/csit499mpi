 // required MPI include file
#include "mpi.h"
#include <stdlib.h>
#include <stdio.h>
#include <list>
#include <map>
#include <unordered_map>
#include <iostream>
#include <string>
#include <cstddef>
#include <unistd.h>
#include <fstream>

using namespace std;
void permute(list<int> current, list<int> left);
list<list<int>> permutations;
int factorial(int number);
void genLists(int number, int * size);
int number = 6;
string filePath = "/work/csit499unk/sorgesd/csit499mpi/latinsquare6final.txt";

int printLatinSquares(list<list<list<int>>> returnableLatinSquares);
list<list<list<int>>> getLatinSquares(list<int> startingLine); 
list<list<list<int>>> addLines(list<list<int>> setLines, list<list<int>> possibilities);
list<list<int>> filterPossibilities(list<list<int>> setLines, list<list<int>> possibilites);
int main(int argc, char *argv[])
	{
	char hostname[MPI_MAX_PROCESSOR_NAME];
   	// Find out rank, size
	int  numtasks, len, rc;
	int world_rank;
	   // initialize MPI
   	MPI_Init(&argc,&argv);
	MPI_Get_processor_name(hostname, &len);
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
	int world_size;
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);
		
	int init;
	int * array;
	int size;
	int returnSize;
	int lines;
	if (world_rank == 0)
		{
			// Create the output file
			ofstream myfile;
                        myfile.open (filePath);
			myfile << "";
			myfile.close();

			init = 1; //send the size first
			int workers = world_size - 1;
			if (workers == 0) {
				printf("WARNING: No additional workers");
				workers = 1;
			}
			
			int factInt = factorial(number);
			for(int i= 1; i < world_size; i++)
			{
				// Send size of latin square
				MPI_Send(&number, 1, MPI_INT, i, 0, MPI_COMM_WORLD);	
			}
			array = (int *) malloc(sizeof(int) * number * number);
			int totalLatinSquares = 0;
			for(int i= 1; i < world_size; i++)
			{
			MPI_Recv(&returnSize,1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
				totalLatinSquares += returnSize;
			}
			printf("Total latin squares: %d", totalLatinSquares);
		} else if (world_rank != 0) 
		{
			
			//float workers = world_size-1.0;
			float rank = world_rank -1.0;
			// Generate list of lines, permutations, for each world
			genLists(number, &size);
			
    			MPI_Recv(&number, 1, MPI_INT, 0, 0, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
	
			int workers = world_size - 1;
                        if (workers == 0) {
                                printf("WARNING: No additional workers");
                                workers = 1;
                        }
                        // Calculate the total number of lines and the portion this world_rank is responsible for
                        int factInt = factorial(number);
                        lines = (factInt / workers);
                        // If there are more than can be evenly distributed, just add one to each world
			if (factInt % workers > 0) {
                                lines += 1;
                        }
			
			// Determine lines this world is responsible
			int startIndex = (world_rank - 1) * lines;	
			int endIndex = world_rank * lines;
			
			if (endIndex > factorial(number)) {
				endIndex = factorial(number);
			}
			printf("\nStart Index: %d, End Index: %d for World Rank %d\n", startIndex, endIndex, world_rank);
			
			list<list<int>> worldLines (permutations);

			returnSize = 0; 
			// Create a list of latin squares representing all combinations beginning with the lines assigned to the world
			for (int i = 0; i < factorial(number); i++) {
				if (i < startIndex) {
					worldLines.pop_front();
				} else if (i < endIndex) {
					returnSize += printLatinSquares(getLatinSquares(worldLines.front()));
					cout << world_rank  << ":" << returnSize << "\n";
					worldLines.pop_front();
				}		
			}
			
			MPI_Send(&returnSize, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
		}
	printf ("Number of tasks= %d My rank= %d Running on %s\n", world_size,world_rank,hostname);

   // done with MPI
   MPI_Finalize();
   }

int printLatinSquares(list<list<list<int>>> returnableLatinSquares) {
	 
	cout << "print latin squares\n";
			string printable ("");
                        for (list<list<int>> x : returnableLatinSquares) {
                                for (list<int> i : x) {
                                        for (int i2 : i) {
                                                printable += to_string(i2) + " ";
                                        }
                                        printable += "\n";
                                }
                                printable += "\n";
                        }
			ofstream myfile;
			myfile.open(filePath, ios::app);
			if (!myfile.is_open()) {
                                sleep(100);
                                myfile.open (filePath, ios::app);
                        }
			myfile << printable;
                        myfile.close();
	return returnableLatinSquares.size();
}
    
void genLists(int number, int * size)
	{
		list<int> l;
		// Generate a list of numbers, 0 - n
		for(int i = 0; i < number; i++)
		{
			l.push_back(i);
		}
		list<int> empty;
		// Get all line permutations of numbers
		permute(empty, l);  
		
		cout <<"Size = "<< permutations.size()<< endl;
	} 


/* Recursively get all line permustations for a given set of numbers, left */
void permute(list<int> current, list<int> left)
{
	if(left.empty())
	{
		permutations.push_front(current);
	}
	else
	{
		list<int> left2(left);
		for(int i : left2)
		{
			list<int> newCurrent (current);
			list<int> newleft (left);
			newleft.remove(i);
			newCurrent.push_back(i);
			permute(newCurrent,newleft);
		}
	}
}

// Get all latin square combinations for a set starting line
list<list<list<int>>> getLatinSquares(list<int> startingLine) {
	list<list<int>> possibleLines (permutations);
	list<list<int>> setLines;
	setLines.push_back(startingLine);
	return addLines(setLines, possibleLines);
}

// Recursively retreive all remaining combinations for a partially generated list, where setLines are the currently generated list and possibilities are the remaining line combinations
list<list<list<int>>> addLines(list<list<int>> setLines, list<list<int>> possibilities) {
	list<list<int>> filteredPossibilities = filterPossibilities(setLines, possibilities);
	
	list<list<list<int>>> latinSquares;

	if (filteredPossibilities.size() == 1) {
		// If only one possible line is left, complete the latin square
		setLines.push_back(filteredPossibilities.front());
		latinSquares.push_back(setLines);
	} else {
		// Pass each possible next line to generate "more complete" latin squares
		for(list<int> line: filteredPossibilities) {
			list<list<int>> newSetLines (setLines);
			newSetLines.push_back(line);
			list<list<list<int>>> newSets = addLines(newSetLines, filteredPossibilities);
			latinSquares.splice(latinSquares.end(), newSets);
		}
	}

	return latinSquares;
}

// Remove all lines that conflict with any of the currently set lines
list<list<int>> filterPossibilities(list<list<int>> setLines, list<list<int>> possibilities) {
	list<list<int>> fList;
	for(list<int> pLine: possibilities) {
		bool conflict = false;
		for(list<int> sLine: setLines) {
			// Check every possible row in each set line against the possible line
			for(int i =0; i < sLine.size(); i++) {
				// If the front value is equal, mark the line as a conflict that will not be returned
				if (sLine.front() == pLine.front()) {
					conflict = true;
				}
				// Rotate through each character to compare all values and maintain line integrity
				int temp = sLine.front();
				sLine.pop_front();
				sLine.push_back(temp);
				temp = pLine.front();
				pLine.pop_front();
				pLine.push_back(temp);
			}
			if (conflict) {
				break;
			}
		}
		if (!conflict) {
			fList.push_back(pLine);
		}
	}
	return fList;
}

int factorial (int n) {
        if (n > 1)
                return n * factorial(n -1);
        else
                return 1;
}

