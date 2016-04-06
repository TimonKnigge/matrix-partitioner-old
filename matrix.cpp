#include <iostream>
#include <sstream>
#include <memory>
#include <string>
#include <algorithm>

#include "matrix.h"


std::istream &operator>>(std::istream &stream, matrix &m) {

	m.R = m.C = 0;
	
	// Extract the first line (header)
	std::string header;
	getline(stream, header);
	std::stringstream header_stream;
	header_stream << header;
	
	// Read '%%MatrixMarket
	std::string file_type_indicator;
	header_stream >> file_type_indicator;
	if (file_type_indicator != "%%MatrixMarket") {
		std::cerr << "Could not read input file: invalid header." << std::endl;
		return stream;
	}
	
	std::string type;
	header_stream >> type;
	if (type != "matrix") {
		std::cerr << "Could not read input file: invalid type." << std::endl;
		return stream;
	}
	
	// Information about how the matrix is stored
	int values = 1;		// How many values are used to store an entry (besides its position)? E.g. 2 for complex, 0 for pattern.
	bool symmetric = false;	// Is only the lower triangular form stored?
	bool coordinate = true;	// Coordinate or array format?
	std::string indicator;
	while (header_stream >> indicator) {
		std::transform(indicator.begin(), indicator.end(), indicator.begin(), ::tolower);
		if (indicator == "array")
			coordinate = false;
		else if (indicator == "pattern")
			values = 0;
		else if (indicator == "complex")
			values = 2;
		else if (indicator == "symmetric" || indicator == "skew-symmetric" || indicator == "hermitian")
			symmetric = true;
	}
	
	// Read comments
	std::string discard;
	while (stream.peek() == '%')
		getline(stream, discard);
	discard.clear();
	
	if (coordinate) {
		size_t R, C, NZ;
		stream >> R >> C >> NZ;
		
		m.R = R;
		m.C = C;
		m.NZ = NZ;
		m.rows.assign(R, std::vector< std::shared_ptr<entry> >());
		m.cols.assign(C, std::vector< std::shared_ptr<entry> >());
		
		for (; NZ > 0; --NZ) {
			size_t r, c;
			stream >> r >> c;
			
			--r;
			--c;
			
			for (int i = 0; i < values; ++i) {
				float tmp;
				stream >> tmp;
			}
			
			m.add_entry(r, c);
			if (symmetric && r != c)
				m.add_entry(c, r);
		}
		
	} else {
		std::cerr << "Array matrix market format is not currently supported" << std::endl;
		return stream;
	}

	m.sort_adjacency_lists();
	
	return stream;
}

std::ostream &operator<<(std::ostream &stream, const matrix &m) {
	
	for (size_t r = 0; r < m.R; ++r) {
		size_t j = 0;
		for (size_t i = 0; i < m.C; ++i) {
			if (j < m.rows[r].size() && m.rows[r][j]->c == i) {
				stream << '#';
				++j;
			} else	stream << '.';
		}
		stream << '\n';
	}
	
	return stream;
}

void matrix::add_entry(size_t r, size_t c) {
	auto e = std::make_shared<entry>();
	e->r = r;
	e->c = c;
	e->ri = rows[r].size();
	e->ci = cols[c].size();
	
	rows[r].push_back(e);
	cols[c].push_back(e); 
}

void matrix::sort_adjacency_lists() {
	
	// Sort the entries in each row by their columns
	for (size_t r = 0; r < R; ++r) {
		std::sort(rows[r].begin(), rows[r].end(),
			[](const std::shared_ptr<entry> &l, const std::shared_ptr<entry> &r) {
			return l->c < r->c;
		});
		for (size_t i = 0; i < rows[r].size(); ++i)
			rows[r][i]->ri = i;
	}
	
	// Sort the entries in each column by their rows
	for (size_t c = 0; c < C; ++c) {
		std::sort(cols[c].begin(), cols[c].end(),
			[](const std::shared_ptr<entry> &l, const std::shared_ptr<entry> &r) {
			return l->r < r->r;
		});
		for (size_t i = 0; i < cols[c].size(); ++i)
			cols[c][i]->ci = i;
	}
}
