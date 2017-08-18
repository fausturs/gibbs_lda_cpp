#include <iostream>
#include <sstream>
#include <algorithm>
#include <numeric>

#include "gibbs_lda.hpp"
#include "Timer.hpp"

using namespace gibbs_lda;

//define lda_log, it's a static member
std::ostringstream Gibbs_LDA::lda_log;

void Gibbs_LDA::clear()
{
    lda_log.clear();
    //
    topic_num = documents_num = all_words_num = 0;
    
    //
    alpha = beta = 0;
    //move
    words_num_of_topic= std::vector<int>();
    
    nw = std::vector< std::vector<int> >();
    nd = std::vector< std::vector<int> >();
    sum_w_nw = std::vector<int>();
    
    //theta = std::vector< std::vector<double> >();
    //phi = std::vector< std::vector<double> >();
    
    z = std::vector< std::vector<int> >();
}

void Gibbs_LDA::initialize_model(const gibbs_lda::Corpus& corpus, size_t topic_num, int rand_seed)
{
    //clear the old data
    clear();
    
    //when rand_seed != -1, use pseudo-random
    if (rand_seed == -1)
        mt = std::mt19937( std::random_device()() ) ;
    else
        mt = std::mt19937( rand_seed );
    
    this->topic_num = topic_num;
    this->documents_num = corpus.documents_num();
    this->all_words_num = corpus.all_words_num();
    
    //copy this from https://github.com/mrquincle/gibbs-lda
    alpha   = 50.0 / topic_num;
    beta    = 0.1;
    
    words_num_of_topic.resize(this->topic_num, 0);
    
    //use words_num_of_topic as a const parameter for init other value
    nw.resize(all_words_num, words_num_of_topic);
    nd.resize(documents_num, words_num_of_topic);
    
    //same size
    sum_w_nw = words_num_of_topic;
    
    //theta has the same size with nd
    //std::vector<double> temp1(topic_num, 0.0);
    //theta.resize(documents_num, temp1);
    //
    //std::vector<double> temp2(all_words_num, 0.0);
    //phi.resize(topic_num, temp2);
    
    //z is the same size with corpus
    std::uniform_int_distribution<int> uniform_int_dis(0, topic_num-1);
    int doc_index = 0;
    for (auto & doc : corpus)
    {
        std::vector<int> temp(doc.size());
        int word_index = 0;
        for (auto & word : doc)
        {
            //random a topic for the word
            int topic = uniform_int_dis(mt);
            temp[word_index++] = topic;
            
            nw[word][topic]         += 1;
            nd[doc_index][topic]    += 1;
            sum_w_nw[topic]         += 1;
        }
        ++doc_index;
        z.push_back( std::move(temp) );
    }
}

int Gibbs_LDA::gibbs_sampling(int word, int doc_index, int word_index)
{
    double v_beta   = all_words_num*beta;
    double k_alpha  = topic_num*alpha;
    
    std::vector<double> p(topic_num);
    // do multinomial sampling via cumulative method
    for (int topic = 0; topic<topic_num; topic++)
    {
        p[topic] =
        (nw[word][topic] + beta) / (sum_w_nw[topic] + v_beta) *
        (nd[doc_index][topic] + alpha) / (z[doc_index].size() + k_alpha);
    }
    // cumulate multinomial parameters, sum will be in p[K-1]
    std::partial_sum(p.begin(), p.end(), p.begin());
    // scaled sample because of unnormalized p[]
    std::uniform_real_distribution<double> uniform_real_dis(0, 1);
    double threshold = uniform_real_dis(mt) * p.back();
    //sampling
    auto ans = std::find_if(p.begin(), p.end(), [threshold](double x){return x>threshold;});
    //
    return std::distance(p.begin(), ans);
}

void Gibbs_LDA::sort_by_probability(id_prob_tuple_list& array) const
{
    std::sort(array.begin(), array.end(),
              [](std::pair<int, double>& lhs, std::pair<int, double>& rhs){return lhs.second>rhs.second;}
              );
}


void Gibbs_LDA::train(const gibbs_lda::Corpus& corpus, size_t topic_num, int rand_seed, size_t max_iterate_num, std::ostream& mylog)
{
    //init
    initialize_model(corpus, topic_num, rand_seed);
    
    wjy::Timer timer;
    timer.start();
    mylog<<"Training start\n";
    //sampling
    for (int it_num=0; it_num<max_iterate_num; ++it_num)
    {
        //write log information, output once every 50 iterations
        if (it_num%50 == 49)
        {
            timer.end();
            mylog<<"epoch:\t"<<it_num/50<<"\tusing time:\t"<<timer.get_duration_s()<<"s\n";
        }
        int doc_index = 0;
        for (auto & doc : corpus)
        {
            int word_index = 0;
            for (auto & word : doc)
            {
                //get the old topic
                int topic = z[doc_index][word_index];
                //remove the old topic
                nw[word][topic]         -= 1;
                nd[doc_index][topic]    -= 1;
                sum_w_nw[topic]         -= 1;
                //sampling a new topic
                topic = gibbs_sampling(word, doc_index, word_index);
                //update the model
                nw[word][topic]         += 1;
                nd[doc_index][topic]    += 1;
                sum_w_nw[topic]         += 1;
                
                z[doc_index][word_index] = topic;
                
                ++word_index;
            }
            ++doc_index;
        }
    }
    timer.end();
    mylog<<"Training finish!"<<"\tusing time:\t"<<timer.get_duration_s()<<"s\n";
}


Gibbs_LDA::id_prob_tuple_list Gibbs_LDA::get_document_topics(int doc_index, double minimum_probability, bool need_sort) const
{
    //theta, get topic distribution
    id_prob_tuple_list ans;
    const auto & counter = nd[doc_index];
    double sum = z[doc_index].size();
    for (int i=0; i<topic_num; ++i)
    {
        double p = (counter[i] + alpha) / (sum + topic_num * alpha);
        if (p<minimum_probability) continue;
        ans.emplace_back( i, p );
    }
    
    //need sort?
    if (need_sort) sort_by_probability(ans);
    
    return ans;
}

Gibbs_LDA::id_prob_tuple_list Gibbs_LDA::get_term_topics(int word, double minimum_probability, bool need_sort) const
{
    id_prob_tuple_list ans;
    const auto & counter = nw[word];
    double sum = std::accumulate(counter.begin(), counter.end(), 0.0);
    for (int i=0; i<topic_num; ++i)
    {
        double p = counter[i]/sum;
        if (p<minimum_probability) continue;
        ans.emplace_back( i, p );
    }
    
    //need sort?
    if (need_sort) sort_by_probability(ans);
    
    return ans;
}

Gibbs_LDA::id_prob_tuple_list Gibbs_LDA::get_topic(int topic, double minimum_probability, bool need_sort) const
{
    //beta, word distribution
    id_prob_tuple_list ans;
    double sum = sum_w_nw[topic];
    for (int i=0; i<all_words_num; ++i)
    {
        double p = (nw[i][topic] + beta) / (sum + all_words_num * beta);
        if (p<minimum_probability) continue;
        ans.emplace_back( i, p );
    }
    
    //need sort?
    if (need_sort) sort_by_probability(ans);
    
    return ans;
}

void Gibbs_LDA::print()
{
    std::cout<<"z\n";
    for (auto & line : z)
    {
        for (auto topic : line) std::cout<<topic<<" ";
        std::cout<<"\n";
    }
    std::cout<<"nd\n";
    for (auto & line : nd)
    {
        for (auto topic : line) std::cout<<topic<<" ";
        std::cout<<"\n";
    }
    
}
