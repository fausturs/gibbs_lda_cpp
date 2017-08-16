#ifndef GIBBS_LDA_HPP
#define GIBBS_LDA_HPP

#include <iostream>
#include <vector>
#include <list>

class Gibbs_LDA{
protected:
	int topic_num;
public:

	using id_prob_tuple = std::vector< std::pair<int, double> >;

	//
	Gibbs_LDA();
	~Gibbs_LDA();


	//clear all parameter 
	void clear();
	//return a list of tuple, the first element is topic and the second is it's probability
	id_prob_tuple get_document_topics() const;
	//given a word's id, return a list of tuple (topic id, probability)
	id_prob_tuple get_term_topics() const;
	//return a list of (word id, probability) 2-tuples for the most probable words in topic topicid.
	id_prob_tuple get_topic() const;
	//update the model with new documents
	void update();
}

#endif
