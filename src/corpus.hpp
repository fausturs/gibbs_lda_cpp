#ifndef CORPUS_HPP
#define CORPUS_HPP

#include <string>
#include <vector>
#include <unordered_map>

namespace gibbs_lda {


using Document = std::vector<int>;
    
class Corpus{

	Corpus(const Corpus&) = delete;
	Corpus& operator=(const Corpus&) = delete;
    
    using document_type = std::vector< Document >;
    
    document_type documents;
    
    std::unordered_map<std::string, int>    word_2_id;
    std::vector< std::string >              id_2_word;
    
public:
    
    //using iterator                  = document_type::iterator;
    //using reverse_iterator          = document_type::reverse_iterator;
    using const_iterator            = document_type::const_iterator;
    using const_reverse_iterator    = document_type::const_reverse_iterator;
    
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
	//get document by index
	Document& operator[](size_t i);
	const Document& operator[](size_t i) const;
	//how many documents in corpus and how many words in each document
	size_t documents_num() const;
	size_t words_num(size_t i) const;
    //
    int word_to_id(const std::string&) const;
    std::string id_to_word(int) const;
    //
    bool has_word(const std::string&) const;
    bool has_id(int) const;
	//return a copy of id_2_word
    std::vector< std::string > all_words() const;
    //return the total number of different words, it's id_2_word.size()
    size_t all_words_num() const;
    //
	void clear();
	//
    template<class Iterator_tp>
	void append(Iterator_tp begin, Iterator_tp end);
    
    //
    const_iterator begin() const;
    const_reverse_iterator rbegin() const;
    const_iterator cbegin() const;
    const_reverse_iterator crbegin() const;
    //
    const_iterator end() const;
    const_reverse_iterator rend() const;
    const_iterator cend() const;
    const_reverse_iterator crend() const;

};
    
//some template member function
    
template<class Iterator_tp>
Corpus::Corpus(Iterator_tp begin, Iterator_tp end)
{
    this->append<Iterator_tp>(begin, end);
}

template<class Iterator_tp>
void Corpus::append(Iterator_tp begin, Iterator_tp end)
{
    for (auto it = begin; it!=end; it++)
    {
        //a temp value for the new document
        std::vector<int> doc;
        for (const auto& word : *it)
        {
            if (word_2_id.find(word) == word_2_id.end())
            {
                //a new word
                int id = word_2_id.size();
                word_2_id[word] = id;
                id_2_word.push_back(word);
            }
            doc.push_back(word_2_id[word]);
        }
        if (doc.size()>0) documents.push_back( std::move(doc) );
    }
}
}
#endif
