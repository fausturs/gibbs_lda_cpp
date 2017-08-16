#ifndef CORPUS_HPP
#define CORPUS_HPP

#include <string>
#include <vector>
#include <map>
#include <unordered_map>

namespace gibbs_lda {

class Corpus{

	Corpus(const Corpus&) = delete;
	Corpus& operator=(const Corpus&) = delete;
    
public:
    std::vector< std::vector<int> > documents;
    
    std::unordered_map<std::string, int> word_to_id;
    std::unordered_map<int, std::string> id_to_word;

	Corpus();
	template<class Iterator_tp>
	Corpus(Iterator_tp begin, Iterator_tp end);

	~Corpus();

	//moveable
	Corpus(Corpus&&);
	Corpus& operator=(Corpus&&);

	//return the word id of document i word j
	//return -1 when out of range
    int at(size_t i, size_t j) const;
	//
	size_t documents_num() const;
	size_t words_num(size_t i) const;
	//
	void clear();
	//
    template<class Iterator_tp>
	void append(Iterator_tp begin, Iterator_tp end);
};
    
//some template member function
    
    template<class Iterator_tp>
    void Corpus::append(Iterator_tp begin, Iterator_tp end)
    {
    	for (auto it = begin; it!=end; it++)
    	{
    		//a temp value for the new document
    		std::vector<int> doc;
    		for (const auto& word : *it)
    		{
    			if (word_to_id.find(word) == word_to_id.end())
    			{
    				//a new word
    				int id = word_to_id.size();
    				word_to_id[word] = id;
    				id_to_word[id] = word;
    			}
    			doc.push_back(word_to_id[word]);
    		}
    		if (doc.size()>0) documents.push_back( std::move(doc) );
    	}
    }
}
#endif
