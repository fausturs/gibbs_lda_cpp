#include <utility>

#include "corpus.hpp"

using namespace gibbs_lda;

//do nothing
Corpus::Corpus(){}

//template function should be defined in hpp file
//
//template<class Iterator_tp>
//Corpus::Corpus(Iterator_tp begin, Iterator_tp end)
//{
//	this->append<Iterator_tp>(begin, end);
//}

//do nothing
Corpus::~Corpus(){}

Corpus::Corpus(Corpus&& rhs)
{
	//if (*rhs == this) return;
	*this = std::move(rhs);
}

Corpus& Corpus::operator=(Corpus&& rhs)
{
	if (&rhs != this)
	{	
		this->documents = std::move( rhs.documents );
		this->word_to_id = std::move( rhs.word_to_id );
		this->id_to_word = std::move( rhs.id_to_word );
	}
	return *this;
}

int Corpus::at(size_t i, size_t j) const
{
	//if (i<0 || j<0) return -1; size_t is always greater than 0
	if (i<documents.size() && j<documents[i].size())
		return documents[i][j];
	else 
		return -1;
}

size_t Corpus::documents_num() const
{
	return documents.size();
}

size_t Corpus::words_num(size_t i) const
{
	//if (i<0) return -1;
	if (i<documents.size())
		return documents[i].size();
	else
		return -1;
}

void Corpus::clear()
{
	documents.clear();
	word_to_id.clear();
	id_to_word.clear();
}

//this function should be defined in hpp file
//
//template<class Iterator_tp>
//void Corpus::append(Iterator_tp begin, Iterator_tp end)
//{
//	for (auto it = begin; it!=end; it++)
//	{
//		//a temp value for the new document
//		std::vector<int> doc;
//		for (const auto& word : *it)
//		{
//			if (word_to_id.find(word) == word_to_id.end()) 
//			{
//				//a new word
//				int id = word_to_id.size();
//				word_to_id[word] = id;
//				id_to_word[id] = word;
//			}
//			doc.push_back(word_to_id[word]);
//		}	
//		if (doc.size()>0) documents.push_back( std::move(doc) );
//	}
//}
