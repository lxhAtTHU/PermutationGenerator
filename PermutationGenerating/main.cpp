#include <iostream>
#include <mpi.h>
#include <assert.h>
#include <cstdlib>
#include "PermutationGenerator.h"
#include "ChangeCarryNumber.h"
using namespace std;



int main(int argc, char**argv) {
	
	int n = 5;
	if(argc > 1){
		n = atoi(argv[1]);
	}
 
	int required_level = MPI_THREAD_SERIALIZED;
	int provided_level;
	MPI_Init_thread(NULL, NULL, required_level, &provided_level);
	assert(provided_level >= required_level);
	int rank, nprocs;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &nprocs);

	
	PermutationGenerator pg(n);

  	pg.generate_permutations(rank, nprocs, PermutationGenerator::RECURSION, "output/recursion.txt");
  	pg.generate_permutations(rank, nprocs, PermutationGenerator::CLASSIC_LEXICOGRAPHIC, "output/class_lexi.txt");
  	pg.generate_permutations(rank, nprocs, PermutationGenerator::SJT, "output/SJT.txt");

 	pg.generate_permutations(rank, nprocs, PermutationGenerator::INCREMENTAL_CARRYING, "output/incremental.txt");
	pg.generate_permutations(rank, nprocs, PermutationGenerator::DECREMENTAL_CARRYING, "output/decremental.txt");
	pg.generate_permutations(rank, nprocs, PermutationGenerator::MEDIATOR_LEXICOHRAPHIC, "output/mediator_lexi.txt");
	pg.generate_permutations(rank, nprocs, PermutationGenerator::SWAPPING, "output/swapping.txt");
 
	pg.generate_permutations(rank, nprocs, PermutationGenerator::HEAP, "output/heap.txt");
	pg.generate_permutations(rank, nprocs, PermutationGenerator::INTEGRATED, "output/integrated.txt");

	MPI_Finalize();  
    return 0;
}

