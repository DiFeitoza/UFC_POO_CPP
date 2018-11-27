#ifndef TWEETGENERATOR_H_
#define TWEETGENERATOR_H_

#include "repository.h"
#include "tweet.h"

class TweetGenerator{
    int nextId = {0};
    Repository<Tweet> * p_r_tweet;
public:    
    TweetGenerator(Repository<Tweet>* r_tweet){
        this->p_r_tweet = r_tweet;
    }
    Tweet* createtwt(string username, string text){
        p_r_tweet->add(to_string(nextId), Tweet(nextId, username, text));
        nextId++;
        return p_r_tweet->getT(to_string(nextId-1));
    }
};

#endif