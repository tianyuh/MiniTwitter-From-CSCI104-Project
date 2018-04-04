#include "twiteng.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cmath>


TwitEng::TwitEng() : trending_(2){

}

TwitEng::~TwitEng(){
	std::map<std::string, User*>::iterator itUser;
	for(itUser = allUsers_.begin(); itUser != allUsers_.end();++itUser ){
		delete itUser->second;
	}

	std::list<Tweet*>::iterator itTwit;
	for(itTwit = allTweets_.begin(); itTwit != allTweets_.end();++itTwit ){
		delete *itTwit;
	}


}


bool TwitEng::parse(char* filename){

	std::ifstream infile(filename);
	int numUsers;
	std::string userLine;

	if(!infile.is_open()){
		return true;
	}
	infile >> numUsers;

	int i = 0;
	// Read user lines
	while(i < numUsers){
		getline(infile, userLine);

		if(userLine.empty() || userLine.find_first_not_of(' ') == std::string::npos){
			continue;
		}

		// Read current user
		std::stringstream ss(userLine);
		std::string tempName;
		unsigned int tempPassword;
		ss >> tempName;
		ss >> tempPassword;
		User* tempUser;
		std::map<std::string, User*>::iterator it = allUsers_.find(tempName);
		if(it == allUsers_.end()){
			tempUser = new User(tempName, tempPassword);
			allUsers_.insert(std::make_pair(tempName, tempUser));
		}
		else{
			tempUser = it->second;
			tempUser->setPassword(tempPassword);
		}

		// Read user's following
		while(ss >> tempName){
			User* tempFollowing;
			std::map<std::string, User*>::iterator it2 = allUsers_.find(tempName);
			// if the following username doesn't exist, create a user
			if(it2 == allUsers_.end()){
				tempFollowing = new User(tempName);
				allUsers_.insert(std::make_pair(tempName, tempFollowing));
			}
			else{ // if the following username exists, just use it
				tempFollowing = it2->second;
			}
			tempUser->addFollowing(tempFollowing);
			tempFollowing->addFollower(tempUser);
		}
		i++;
	}



	// Read tweet lines
	std::string tweetLine;
	while(getline(infile,tweetLine)){

		if(tweetLine.empty() || tweetLine.find_first_not_of(' ') == std::string::npos){
			continue;
		}

		// Store tweet content
		std::stringstream ts(tweetLine);
		DateTime tempTime;
		std::string username;
		std::string text;
		ts >> tempTime >> username;
		getline(ts, text);
		std::map<std::string, User*>::iterator it = allUsers_.find(username);
		Tweet * tempTweet = new Tweet(it->second, tempTime, text);
		it->second->addTweet(tempTweet);
		allTweets_.push_back(tempTweet);

		// Store hashtags and related tweets
		std::set<std::string> tempTags = tempTweet->hashTags();
		std::set<std::string>::iterator itTag;

		for(itTag = tempTags.begin(); itTag != tempTags.end(); ++itTag){
			std::map<std::string, std::set<Tweet*> >::iterator itMap;
			itMap = allHashtags_.find(*itTag);
			if(itMap != allHashtags_.end()){
				itMap->second.insert(tempTweet);
			}
			else{
				std::set<Tweet*> tempSet;
				tempSet.insert(tempTweet);
				allHashtags_.insert(std::make_pair(*itTag, tempSet));
			}
		}
	}


	getHashTrend();

	return false;
}


void TwitEng::addTweet(std::string& username, DateTime& time, std::string& text){
	std::map<std::string, User*>::iterator it = allUsers_.find(username);
	if(it == allUsers_.end()){
		std::cout << "User does not exist." << std::endl;
		return;
	}

	text = " " + text; 
	Tweet * tempTweet = new Tweet(it->second, time, text);
	// Add to user
	it->second->addTweet(tempTweet);
	// Add to tweets list
	allTweets_.push_back(tempTweet);
	// Add to hashtag map
	std::set<std::string> hash = tempTweet->hashTags();
	std::set<std::string>::iterator it2 = hash.begin();
	for(; it2 != hash.end(); ++it2){
		std::map<std::string, std::set<Tweet*> >::iterator itMap;
		itMap = allHashtags_.find(*it2);
		if(itMap != allHashtags_.end()){

			// Update the hastags trending
			int oldOccurence = itMap->second.size();
			TagData oldTag(*it2,oldOccurence);
			TagData newTag(*it2,oldOccurence + 1);
			trending_.decreaseKey(oldTag, newTag);

			itMap->second.insert(tempTweet);
		}
		else{
			std::set<Tweet*> tempSet;
			tempSet.insert(tempTweet);
			allHashtags_.insert(std::make_pair(*it2, tempSet));

			TagData newTag(*it2,1);
			trending_.push(newTag);
		}

	}
}


std::vector<Tweet*> TwitEng::search(std::vector<std::string>& terms, int strategy){
	std::vector<Tweet*> matches;

	// Find sets of tweets that match the terms
	std::list< std::set<Tweet*> > tempTwitSets;
	std::vector<std::string>::iterator itTerm;
	for(itTerm = terms.begin(); itTerm !=terms.end();++itTerm){
		std::string termLower = *itTerm;
		for(unsigned int i = 0; i < termLower.size(); i++){
			termLower[i] = tolower(termLower[i]);
		}
		std::map<std::string, std::set<Tweet*> >::iterator itHash;
		itHash = allHashtags_.find(termLower);
		if(itHash != allHashtags_.end()){
			tempTwitSets.push_back(itHash->second);
		}
	}

	// Merge the sets 
	std::list<std::set<Tweet*> >::iterator itTwitSet = tempTwitSets.begin();
	std::set<Tweet*> tempTweets = *itTwitSet;

	for(; itTwitSet != tempTwitSets.end(); ++itTwitSet){

		if(strategy == 1){ // Merge by OR
			std::set<Tweet*>::iterator itTweet;
			for(itTweet = (*itTwitSet).begin() ; itTweet !=(*itTwitSet).end(); ++itTweet ){
				tempTweets.insert(*itTweet);
			}
		}
		else if(strategy == 0){ // Merge by AND
			std::set<Tweet*>::iterator itTweet2;
			std::set<Tweet*> temp;
			for(itTweet2 = (*itTwitSet).begin() ; itTweet2 !=(*itTwitSet).end(); ++itTweet2 ){
				if(tempTweets.find(*itTweet2) != tempTweets.end()){
					temp.insert(*itTweet2);
				}
			}
			tempTweets = temp;
		}
	}

	// Copy from set to vector
	std::set<Tweet*>::iterator itMatchSet;
	for(itMatchSet = tempTweets.begin(); itMatchSet != tempTweets.end(); ++itMatchSet){
		matches.push_back(*itMatchSet);
	}

	return matches;
}



void TwitEng::dumpFeeds(){
	std::map<std::string, User*>::iterator it;

	for(it = allUsers_.begin();it != allUsers_.end(); ++it){
		std::string username = it->second->name();

		std::string filename = username + ".feed";
		std::ofstream outfile(filename.c_str());
		outfile << username << std::endl;

		std::vector<Tweet*> userFeed(it->second->getFeed());
		std::vector<Tweet*>::iterator itTweet;
		for(itTweet = userFeed.begin(); itTweet != userFeed.end(); ++itTweet){
			outfile << *(*itTweet) << std::endl;
		}
		outfile.close();
	}
}



 std::vector<Tweet*> TwitEng::mentionFeeds(std::string const& username){
 	std::vector<Tweet*> mentionFeed;
 	
 	std::list<Tweet*>::iterator it;

 	for(it = allTweets_.begin() ; it != allTweets_.end(); ++it){
 		if((*it)->startWithMention() ){
			if((*it)->startMention() == username){
				mentionFeed.push_back(*it);
			}
		}
		else{
			std::set<std::string> tempMent = (*it)->mentions();
			if(tempMent.find(username) != tempMent.end()){
				mentionFeed.push_back(*it);
			}
		}

 	}

 	Msort<Tweet*, TweetComp> tweetSort;
	tweetSort.mergeSort(mentionFeed, TweetComp());
     
 	return mentionFeed;
 }



std::map<std::string, User*> TwitEng::users(){
	return allUsers_;
}


std::vector<Tweet*> TwitEng::getMainFeeds(std::string const& username){
	std::map<std::string, User*>::iterator it = allUsers_.find(username);
	std::vector<Tweet*> mainFeeds;
	if(it != allUsers_.end()){
		mainFeeds = it->second->getFeed();
	}
	return mainFeeds;
}



std::vector<User*> TwitEng::findUsers(std::string const& term){
 	std::vector<User*> matches;
 	if(term.empty()){
 		return matches;
 	}

 	std::map<std::string, User*>::iterator it;
 	for(it = allUsers_.begin(); it != allUsers_.end(); ++it){
 		if(it->first.find(term) == 0){
 			matches.push_back(it->second);
 		}
 	}
 	return matches;
 }


 User* TwitEng::getUser(std::string const& name){
 	User* match;

 	std::map<std::string, User*>::iterator it;
 	it = allUsers_.find(name);
 	
 	if(it != allUsers_.end()){
 		match  = it->second;
 		return match;
 	}

 	return NULL;
 }


void TwitEng::save(std::string filename){
	std::map<std::string, User*>::iterator it = allUsers_.begin();

	std::ofstream outfile(filename.c_str());
	outfile << allUsers_.size() << std::endl; 
	
	for(; it != allUsers_.end(); ++it){
		outfile << it->second->name() << " " << it->second->password();
		std::set<User*> followings = it->second->following();
		std::set<User*>::iterator it2 = followings.begin();

		for(; it2 != followings.end(); ++it2){
			outfile << " " << (*it2)->name();
		}
		outfile << std::endl;
	}

	std::list<Tweet*>::iterator it3 = allTweets_.begin();
	for(;it3 != allTweets_.end(); ++it3){
		outfile << *(*it3) << std::endl;
	}
}

// Tarjan's algorithm used to find strongly connected users
void TwitEng::tarjan(std::ofstream& outfile, std::string name, 
	std::map<std::string, int>& dfn, std::map<std::string, int>& low, 
	int& index, std::stack<std::string>& uStack, std::set<std::string>& uSet, 
	int& sscindex){

	dfn[name] = ++index;
	low[name] = dfn[name];
	uStack.push(name);
	uSet.insert(name);

	std::set<User*> tempFollowing = allUsers_[name]->following();
	std::set<User*>::iterator it = tempFollowing.begin();
	for(; it != tempFollowing.end(); ++it){
		std::string v = (*it)->name();
		if(dfn.find(v) == dfn.end()){
			tarjan( outfile, v , dfn, low, index, uStack, uSet, sscindex);
			low[name] = std::min(low[name],low[v]); 
		}
		else if(uSet.find(v) != uSet.end()){
			low[name] = std::min(low[name],dfn[v]);
		}
	}

	if(low[name] == dfn[name]){
		std::vector<std::string> ssc;
		while(1){
			std::string top = uStack.top();
			uStack.pop();
			std::set<std::string>::iterator ittop = uSet.find(top);
			uSet.erase(ittop);

			ssc.push_back(top);
			if(top == name ){
				break;
			}
		}

		if(!ssc.empty()){
			outfile << "Component " << sscindex++ << std::endl; 
			std::vector<std::string>::iterator itssc;
			for(itssc = ssc.begin(); itssc != ssc.end(); ++itssc){
				outfile << *itssc << std::endl;
			}
			outfile << "\n";
		}
	}

}


// Find strong connected users by Tarjan's algorithm
void TwitEng::findSSC(std::string filename){
	std::ofstream outfile(filename.c_str());
	std::stack<std::string> userStack;
	std::set<std::string> userSet;
	std::map<std::string, int> dfn;
	std::map<std::string, int> low;
	int index = 0;
	int sscindex = 1;

	std::map<std::string, User*>::iterator it;
	it = allUsers_.begin();

	int i = 0;
	for(; it != allUsers_.end(); ++it){
		std::string name = it->first;
		
		if(dfn.find(name) == dfn.end()){
			i++;
			tarjan(outfile, name, dfn, low, index, userStack, userSet, sscindex);
		}
	}
	
	outfile.close();
}




void TwitEng::getHashTrend(){
	std::map<std::string, std::set<Tweet*>>::iterator it = allHashtags_.begin();

	for(; it != allHashtags_.end(); ++it){
		TagData temp( it->first, int((it->second).size()) );
		trending_.push(temp);
	}
}

std::vector<std::string> TwitEng::topHashtags(){
	std::vector<TagData> tagVec;
	std::vector<std::string> pureTags;

	int count = 0;
	while(count < 5 && !(trending_.empty())){
		tagVec.push_back(trending_.top());
		trending_.pop();
		count++;
	}

	for(unsigned int i = 0; i < tagVec.size(); i++){
		trending_.push(tagVec[i]);
		pureTags.push_back(tagVec[i].tag);
	}

	return pureTags;
}



unsigned int TwitEng::passwordHasher(std::string password){
	unsigned long long passInt = 0;
	int n = password.length();
	unsigned int passArr[4];
	int encryptedCode;

	int idx = 0;
	while(password[idx]){
		passInt += (unsigned long long)(pow(128, n-1-idx))*int(password[idx]);
		idx++;
	}

	for(int i = 3; i >=0; i--){
		passArr[i] = passInt % 65521;
		passInt = passInt / 65521;
	}

	encryptedCode = (45912*passArr[0] + 35511*passArr[1] 
		+ 65169*passArr[2] + 4625*passArr[3]) % 65521;

	return encryptedCode;
}



void TwitEng::addUser(std::string& username, unsigned int& encPassword){

	std::map<std::string, User*>::iterator it = allUsers_.find(username);

	if(it != allUsers_.end()){
		return;
	}

	User* newUser = new User(username, encPassword);
	allUsers_.insert(std::make_pair(username, newUser));
}


