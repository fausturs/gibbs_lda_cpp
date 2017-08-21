#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>

#include <assert.h>

#include "src/corpus.hpp"
#include "src/gibbs_lda.hpp"

std::vector<std::string> split(const std::string& st, char spliter=' ')
{
    std::vector<std::string> ans;
	size_t position=-1;
	for (size_t i = 0; i<st.size(); i++ )
		if (st[i] == spliter) 
		{
			ans.push_back(st.substr(position+1, i-position-1));
			position = i;
		}
	ans.push_back(st.substr(position+1));
	return ans;
}

int main(int args, char* argv[])
{
	std::vector< std::vector<std::string> > documents;
	
	std::ifstream myin("./trndocs.txt");
	assert(myin);
	//reading
	std::string line;
	while (getline(myin, line))
	{
		auto temp = split(line);
		documents.push_back( std::move(temp) );
	}
	//traning lda model
	gibbs_lda::Corpus corpus(documents.begin(), documents.end());

	gibbs_lda::Gibbs_LDA lda;
	lda.train(corpus, 20, 1, 200, std::clog);

	std::cerr<<"saveing...\n";
	lda.save("model.txt");
	std::cerr<<"loading...\n";
	lda.load("model.txt");

	int doc_id = std::atoi(argv[1]);	
	auto a = lda.get_document_topics(doc_id, 0, true);
	for (auto &x : a)
		std::cout<<x.first<<"\t"<<x.second<<"\n";

	return 0;
}
