#include <iostream>

#include "constants.h"
#include "matrix.h"
#include "abstract_partitioner.h"
#include "ppmatrix.h"
#include "branchandbound.h"
#include "resettable_array.h"


int main(int argc, char *argv[]) {
	
	assert_correctness_of_constants();
	
	if (argc > 1) {
		std::cerr << "Usage:\n\t" << argv[0];
		std::cerr << " < infile > outfile\n\n";
		std::cerr << "\tinfile: Input matrix in Matrix Market format.\n";
		std::cerr << "\toutfile: Output location." << std::endl;
	} else {
		std::cerr << argv[0] << std::endl;
		
		matrix m;
		std::cin >> m;
		std::cerr << m.R << 'x' << m.C << " matrix has " << m.NZ << " nonzeros (Cmax = " << m.Cmax << ")." << std::endl;
		std::cerr << m << std::flush;
		
		branchandbound bb(m);
		std::vector<status> r, c;
		std::cerr << "Volume: " << bb.partition(0.0, r, c) << std::endl;
		print_colored(m, r, c);
	}
	
	return 0;
}
