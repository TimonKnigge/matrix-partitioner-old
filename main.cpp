#include <iostream>

#include "matrix.h"
#include "abstract_partitioner.h"
#include "branchandbound.h"
#include "resettable_array.h"


int main(int argc, char *argv[]) {
	
	if (argc > 1) {
		std::cerr << "Usage:\n\t" << argv[0];
		std::cerr << " < infile > outfile\n\n";
		std::cerr << "\tinfile: Input matrix in Matrix Market format.\n";
		std::cerr << "\toutfile: Output location." << std::endl;
	} else {
		std::cerr << argv[0] << std::endl;
		
		matrix m;
		std::cin >> m;
		std::cout << m << std::flush;
		
		branchandbound bb(m);
		std::vector<status> r, c;
		std::cerr << "Result: " << bb.partition(0.0, r, c) << std::endl;
	}
	
	return 0;
}
