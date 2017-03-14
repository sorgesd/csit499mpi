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
void permute(list<list<int>> *result, list<int> current, list<int> left);
list<list<int>> permutations;
int factorial(int number);
void genLists(int number, int * size);
int number = 6;
string filePath = "/work/csit499unk/sorgesd/csit499mpi/latinsquare6x128.txt";

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
	// cell * cellArray;
	int returnSize;
/**
	if (world_rank == 0)
		{
		
		//	printf("Enter the number:");
		//	scanf("%d", &number);

		//	cellArray = genLists(number, &size);
		
		//	list<int> line = permutations.front();  
			
		// list<list<list<int>>> latinSquares = getLatinSquares(line);
		
		for (list<list<int>> ls : latinSquares) {
			for (list<int> line : ls) {
				for (int i : line) {
					printf("%d,", i);	
				}			
				cout << endl;
			}
			cout << endl;
		}	
	}
*/		
	//MPI_Bcast(&size,  1,    MPI_INT,0,MPI_COMM_WORLD);
	//MPI_Bcast(cellArray, size, mpi_cell_type, 0, MPI_COMM_WORLD);
	int lines;
	if (world_rank == 0)
		{
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
			//printf("size %d", sizeof(permutations)/sizeof(permutations.front()));
			int factInt = factorial(number);
			lines = (factInt / workers);
			if (factInt % workers > 0) {
				lines += 1;
			}
			for(int i= 1; i < world_size; i++)
			{
			//	int arraySize = number * lines;
			//	cell *lineSet = (cell *) malloc(sizeof(cell) * arraySize);
    				//lineArray = (int *) malloc(sizeof(int) * arraySize
				MPI_Send(&number, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
				
			}
			array = (int *) malloc(sizeof(int) * number * number);
			int totalLatinSquares = 0;
			for(int i= 1; i < world_size; i++)
			{
			MPI_Recv(&returnSize,1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
//				cout << i;
//				cout << endl;
//				cout << returnSize;
				totalLatinSquares += returnSize;
				/* char del = ' ';
				for(int i = 0; i < number*number; i++)
				{
					del = ',';
				}*/
			}
			printf("Total latin squares: %d", totalLatinSquares);
		} else if (world_rank != 0) 
		{
			
			//float workers = world_size-1.0;
			float rank = world_rank -1.0;
			
			genLists(number, &size);
			
    			MPI_Recv(&number, 1, MPI_INT, 0, 0, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
	
			int workers = world_size - 1;
                        if (workers == 0) {
                                printf("WARNING: No additional workers");
                                workers = 1;
                        }
                        //printf("size %d", sizeof(permutations)/sizeof(permutations.front()));
                        int factInt = factorial(number);
                        lines = (factInt / workers);
                        if (factInt % workers > 0) {
                                lines += 1;
                        }

			int startIndex = (world_rank - 1) * lines;	
			int endIndex = world_rank * lines;
			
			if (endIndex > factorial(number)) {
				endIndex = factorial(number);
			}
			printf("\nStart Index: %d, End Index: %d for World Rank %d\n", startIndex, endIndex, world_rank);
			
			list<list<int>> worldLines (permutations);

			returnSize = 0; 
			//list<list<list<int>>> returnableLatinSquares;
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
		ofstream myfile;
                        myfile.open (filePath, ios::app);
                        if (!myfile.is_open()) {
                                sleep(100);
                                myfile.open (filePath, ios::app);
                        }

                        for (list<list<int>> x : returnableLatinSquares) {
                                for (list<int> i : x) {
                                        for (int i2 : i) {
                                                myfile << i2 << " ";
                                        }
                                        myfile << "\n";
                                }
                                myfile << "\n";
                        }
                        myfile.close();
	return returnableLatinSquares.size();
}
    
void genLists(int number, int * size)
	{
		//cell * cellArray;
		//int gridsize = number * number;
		//int *array = (int *)malloc(sizeof(int) * gridsize);
		list<int> l;
		for(int i = 0; i < number; i++)
		{
			l.push_back(i);
		}
		//for(int i = 1; i < number * number;i++)
		//{
		//	array[i] = -1;
		//}
		//an algorithm goes here!!
		//algorithm(number);
		list<list<int>> result;
		list<int> empty;
		permute(&result,empty, l);  
		//*size = result.size() * number;
		/*cellArray = (cell *)malloc(sizeof(cell) * number * result.size());
		int index = 0;
		for(list<int> l : result)
		{
			for(int lvalue: l)
			{
					cellArray[index].value = lvalue;	
					index++;
					//cout<<index << " " << lvalue << endl;
			}
		}**/
		
		cout <<"Size = "<< result.size()<< endl;
	} 


/* print permutations of string */
void permute(list<list<int>> *result, list<int> current, list<int> left)
{
	if(left.empty())
	{
//		cout <<"Size = "<< result->size()<< endl;
		result->push_front(current);	
		permutations.push_front(current);
//		cout <<"Size = "<< result->size()<< endl;
		for(int j: current)
		{
			//cout << j << ",";
		}
		//cout << endl;
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
			permute(result,newCurrent,newleft);
		}
	}
}

int getIndex(int row, int col, int size)
{
	int result = (row * size)+ col;
	return result;
}

list<list<list<int>>> getLatinSquares(list<int> startingLine) {
	//list<list<list<int>>> latinSquares;
	list<list<int>> possibleLines (permutations);
	list<list<int>> setLines;
	setLines.push_back(startingLine);
	return addLines(setLines, possibleLines);
}

list<list<list<int>>> addLines(list<list<int>> setLines, list<list<int>> possibilities) {
	list<list<int>> filteredPossibilities = filterPossibilities(setLines, possibilities);
	
	list<list<list<int>>> latinSquares;

	if (filteredPossibilities.size() == 1) {
		setLines.push_back(filteredPossibilities.front());
		latinSquares.push_back(setLines);
	} else {
		for(list<int> line: filteredPossibilities) {
			list<list<int>> newSetLines (setLines);
			newSetLines.push_back(line);
			list<list<list<int>>> newSets = addLines(newSetLines, filteredPossibilities);
			latinSquares.splice(latinSquares.end(), newSets);
		}
	}

	return latinSquares;
}

list<list<int>> filterPossibilities(list<list<int>> setLines, list<list<int>> possibilities) {
	list<list<int>> fList;
	for(list<int> pLine: possibilities) {
		bool conflict = false;
		for(list<int> sLine: setLines) {
			for(int i =0; i < sLine.size(); i++) {
				if (sLine.front() == pLine.front()) {
					conflict = true;
				}
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

