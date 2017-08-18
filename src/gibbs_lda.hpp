#ifndef GIBBS_LDA_HPP
#define GIBBS_LDA_HPP

#include <iostream>
#include <sstream>
#include <vector>
#include <list>
#include <random>

#include "corpus.hpp"

namespace gibbs_lda{

class Gibbs_LDA{
public:
    using id_prob_tuple_list = std::vector< std::pair<int, double> >;
    
private:
    Gibbs_LDA(const Gibbs_LDA&) = delete;
    Gibbs_LDA& operator=(const Gibbs_LDA&) = delete;
protected:
    std::mt19937 mt;
    
	size_t topic_num;
	size_t documents_num;
	size_t all_words_num;
    
    //the multinomial distribution's parameter for each document
    double alpha;
    double beta;

    //total number of words assigned to topic j, size is topic_num*1
    std::vector< int > words_num_of_topic;
    //number of instances of word i assigned to topic j, size is all_words_num*topic_num
    std::vector< std::vector<int> > nw;
    //number of words in document i assigned to topic j, size is documents_num*topic_num
    std::vector< std::vector<int> > nd;
    //total number of words assigned to topic j, size topic_num*1
    //sum_w_nw[topic] = \sum_w nw[w][topic]
    std::vector< int > sum_w_nw;
    
	//\theta_d|\alpha ~ Dirichlet(\alpha)  size is documents_num*topic_num
	//std::vector< std::vector<double> > theta;
	//\phi_k|\beta ~ Dirichlet(\beta)      size is topic_num*all_words_num
	//std::vector< std::vector<double> > phi;
    
    //topic
    std::vector< std::vector<int> > z;

    
    void initialize_model(const gibbs_lda::Corpus& corpus, size_t topic_num,int rand_seed);
    
    //sampling
    int gibbs_sampling(int, int, int);
    //
    void sort_by_probability(id_prob_tuple_list&) const;
public:
    
    //the model output the log to this ostringstream
    static std::ostringstream lda_log;
    
	//
	Gibbs_LDA() = default;
    ~Gibbs_LDA() = default;

    Gibbs_LDA(Gibbs_LDA&&);
    Gibbs_LDA& operator=(Gibbs_LDA&&);

	//clear all parameter 
	void clear();
		
    /*
     corpus
        training data
     topic_num
        default 20
     random_seed
        if rand_seed==-1, code use std::random_device as the seed; else use this value
     max_iterate_num
        max number of Gibbs sampling iterations
     mylog
        a ostream object, for record training information. default lda_log is a static srtingstream object defind in this clas.
     */
    void train(const gibbs_lda::Corpus& corpus, size_t topic_num = 20, int rand_seed=1, size_t max_iterate_num=200, std::ostream& mylog = lda_log);
    
    //return a list of tuple, the first element is topic and the second is it's probability
    id_prob_tuple_list get_document_topics(int doc_index, double minimum_probability = 0.05, bool need_sort = false) const;
    //given a word, return a list of tuple (topic id, probability)
    id_prob_tuple_list get_term_topics(int word, double minimum_probability = 0.05, bool need_sort = false) const;
    //return a list of (word, probability) 2-tuples for the most probable words in topic topicid.
    id_prob_tuple_list get_topic(int topic, double minimum_probability = 0.05, bool need_sort = false) const;

    void save(const std::string& file_path);
    void load(const std::string& file_path);
    //for test;
    void print();
};
    
    
}

#endif
