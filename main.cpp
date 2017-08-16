#include <iostream>
#include <algorithm>

#include "src/corpus.hpp"

int main()
{
	std::vector< std::vector< std::string > > ds = {{"头伸","头伸","头伸头伸战场"},{"1","5","6"},{"你好","中国"}};
	gibbs_lda::Corpus corpus(ds.begin(), ds.end());

	/*
	for (int i=0; i<3; i++)
	{
		for (int j=0; j<3; j++)
			std::cout<<corpus.at(i, j)<<" ";
		std::cout<<"\n";
	}
	*/

	for (auto& doc : corpus)
	{
		for (auto& word_id : doc)
		{
			std::cout<< corpus.id_to_word(word_id)<<" ";
		}
		std::cout<<"\n";
	}
	return 0;
}
