#include "user.h"

User::User(std::string name){
	name_ = name;
	password_ = 0;	
}

User::User(std::string name, unsigned int password){
	name_ = name;
	password_ = password;
}

User::~User(){

}

std::string User::name() const{
	return name_;
}

std::set<User*> User::followers() const{
	return followers_;
}

std::set<User*> User::following() const{
	return following_;
}

std::list<Tweet*> User::tweets() const{
	return tweets_;
}

void User::addFollower(User* u){
	followers_.insert(u);
}

void User::addFollowing(User* u){
	following_.insert(u);
}

void User::addTweet(Tweet* t){
	tweets_.push_back(t);
}

std::vector<Tweet*> User::getFeed(){
	std::vector<Tweet*> feed(tweets_.begin(),tweets_.end());
	std::set<User*>::iterator it1;
	for(it1 = following_.begin(); it1 != following_.end(); ++it1){
		std::list<Tweet*>::iterator it2;
		std::list<Tweet*> temp = (*it1)->tweets();
		for(it2 = temp.begin(); it2 != temp.end(); ++it2){
			if((*it2)->startWithMention() && (*it2)->startMention() != name_){
				continue;
			}
			feed.push_back(*it2);
		}
	}

	Msort<Tweet*, TweetComp> tweetSort;
	tweetSort.mergeSort(feed, TweetComp());

	return feed;
}


unsigned int User::password(){
	return password_;
}

void User::setPassword(unsigned int password){
	password_ = password;
}


