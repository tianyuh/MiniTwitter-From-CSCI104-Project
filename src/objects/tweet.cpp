#include "tweet.h"
#include "user.h"
#include <sstream>

Tweet::Tweet(){
	user_ = NULL;
	startWithMention_ = false;
}

Tweet::Tweet(User* user, DateTime& time, std::string& text){
	user_ = user;
	time_.hour = time.hour;
	time_.min = time.min;
	time_.sec = time.sec;
	time_.year = time.year;
	time_.month = time.month;
	time_.day = time.day;
	text_ = text;
	startWithMention_ = false;
	// find hastags
	std::stringstream ss(text);
	std::string temp; 
	while(ss>>temp){
		if(temp[0] == '#'){
			temp.erase(temp.begin());
			for(unsigned int i = 0; i < temp.size(); i++){
				temp[i] = tolower(temp[i]);
			}
			hashtags_.insert(temp);
		}
	}

	// Find mentions
	std::stringstream ss2(text);

	// Check the first word
	ss2 >> temp;
	if(temp[0] == '@'){
		startWithMention_ = true;
		temp.erase(temp.begin());
		mentions_.insert(temp);
		startMention_ = temp;
	}

	// Check the rest of the text
	while(ss2 >> temp){
		if(temp[0] == '@'){
			temp.erase(temp.begin());
			mentions_.insert(temp);
		}
	}
}


DateTime const & Tweet::time() const{
	return time_;
}

std::string const & Tweet::text() const{
	return text_;
}

std::set<std::string> Tweet::hashTags() const{
	return hashtags_;
}


bool Tweet::operator<(const Tweet& other) const{
	return time_ < other.time();
}


std::ostream& operator<<(std::ostream& os, const Tweet & t){
	os << t.time_ << " " << t.user_->name() << t.text_;
	return os;
}


User* Tweet::user() const{
	return user_;
}

std::set<std::string> Tweet::mentions() const{
	return mentions_;
}

bool Tweet::startWithMention() const{
	return startWithMention_;
}


std::string const Tweet::startMention() const{
	return startMention_;
}
