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
		this->word_2_id = std::move( rhs.word_2_id );
		this->id_2_word = std::move( rhs.id_2_word );
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

Document& Corpus::operator[](size_t i)
{
	//Do not consider crossing the border
	return documents[i];
}

const Document& Corpus::operator[](size_t i) const
{
	//Do not consider crossing the border
	return documents[i];
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

int Corpus::word_to_id(const std::string& word) const
{
	auto it = word_2_id.find(word);
	if (it != word_2_id.end()) return it->second;
	return -1;
}

std::string Corpus::id_to_word(int id) const
{
	if (id<0 || id_2_word.size()<=id) return "";
	return id_2_word[id];
}

bool Corpus::has_word(const std::string& word) const
{
	return word_2_id.find(word) != word_2_id.end();
}

bool Corpus::has_id(int id) const
{
	return  (0<=id && id<id_2_word.size());
}

std::vector< std::string > Corpus::all_words() const
{
	return id_2_word;
}

size_t Corpus::all_words_num() const
{
    return id_2_word.size();
}

void Corpus::clear()
{
	documents.clear();
	word_2_id.clear();
	id_2_word.clear();
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
//			if (word_2_id.find(word) == word_2_id.end()) 
//			{
//				//a new word
//				int id = word_2_id.size();
//				word_2_id[word] = id;
//				id_2_word.push_bask(word);
//			}
//			doc.push_back(word_2_id[word]);
//		}	
//		if (doc.size()>0) documents.push_back( std::move(doc) );
//	}
//}
//

Corpus::const_iterator Corpus::begin() const
{
	return documents.begin();
}
Corpus::const_reverse_iterator Corpus::rbegin() const
{
	return documents.rbegin();
}
Corpus::const_iterator Corpus::cbegin() const
{
	return documents.cbegin();
}
Corpus::const_reverse_iterator Corpus::crbegin() const
{
	return documents.crbegin();
}

Corpus::const_iterator Corpus::end() const
{
	return documents.end();
}
Corpus::const_reverse_iterator Corpus::rend() const
{
	return documents.rend();
}
Corpus::const_iterator Corpus::cend() const
{
	return documents.cend();
}
Corpus::const_reverse_iterator Corpus::crend() const
{
	return documents.crend();
}
