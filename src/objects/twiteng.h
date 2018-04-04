#ifndef TWITENG_H
#define TWITENG_H
#include <map>
#include <string>
#include <set>
#include <vector>
#include <stack>
#include "user.h"
#include "datetime.h"
#include "../utils/heap.h"
#include "tagdata.h"

class TwitEng
{
 public:
  TwitEng();
  ~TwitEng();
  /**
   * Parses the Twitter database and populates internal structures
   * @param filename of the database file
   * @return true if there is an error, false if successful
   */
  bool parse(char* filename);

  /**
   * Allocates a tweet from its parts and adds it to internal structures
   * @param username of the user who made the tweet
   * @param timestamp of the tweet
   * @param text is the actual text of the tweet as a single string
   */
  void addTweet(std::string& username, DateTime& time, std::string& text);

  /**
   * Searches for tweets with the given words and strategy
   * @param words is the hashtag terms in any case without the '#'
   * @param strategy 0=AND, 1=OR
   * @return the tweets that match the search
   */
  std::vector<Tweet*> search(std::vector<std::string>& terms, int strategy);

  User* getUser(std::string const& name);

  std::vector<User*> findUsers(std::string const& term);

  /**
   * Dump feeds of each user to their own file
   */
  void dumpFeeds();

  /* You may add other member functions */
  std::vector<Tweet*> mentionFeeds(std::string const& name);

  std::map<std::string, User*> users();

  std::vector<Tweet*> getMainFeeds(std::string const& name);

  void save(std::string filename);

  void tarjan(std::ofstream& outfile, std::string name, std::map<std::string, int>& dfn,
    std::map<std::string, int>& low, int& index, std::stack<std::string>& uStack, 
    std::set<std::string>& set, int& sscindex);

  void findSSC(std::string filename);

  void getHashTrend();

  std::vector<std::string> topHashtags();

  unsigned int passwordHasher(std::string password);


  void addUser(std::string& username, unsigned int& encPassword);


 private:
  /* Add any other data members or helper functions here  */
  std::map<std::string, User*> allUsers_;
  std::list<Tweet*> allTweets_;
  std::map<std::string, std::set<Tweet*> > allHashtags_;

  Heap<TagData, TagStringEqual, TagIntGreater, TagStringHasher> trending_;
  
};


#endif
