#include <iostream>
#include <algorithm>

#include "src/corpus.hpp"

int main()
{
	std::vector< std::vector< std::string > > ds = {{"1","2","3"},{"1","5","6"},{"6","8"}};
	gibbs_lda::Corpus corpus(ds.begin(), ds.end());

	for (int i=0; i<3; i++)
	{
		for (int j=0; j<3; j++)
			std::cout<<corpus.at(i, j)<<" ";
		std::cout<<"\n";
	}

	return 0;
}
