#include "twitgui.h"
#include <sstream>

TwitGui::TwitGui(TwitEng* twit, QWidget * parent) : QWidget( parent ){
	twiteng = twit;

	loginWindow = new QWidget;
	//mainWindow = new QWidget;


	userbox = new QLabel("User:");
	userLabel = new QLabel;
	userLabel->setFrameStyle(QFrame::Panel | QFrame::Sunken);
	userLabel->setStyleSheet("QLabel { background-color : white; color : black}");
	
	userlist = new QComboBox;
	options = new QComboBox;
	options->addItem("Home");
	options->addItem("Mentions");
	options->addItem("Following");
	options->addItem("Followers");
	toolButtons = new Toolsbar;
	newTweet = new QLineEdit;
	postButton = new QPushButton("&Post");
	displayOptionLabel = new QLabel("Options:");
	quitButton = new QPushButton("&Quit");
	sscButton = new QPushButton("Output &SSC");
	refTagButton = new QPushButton("&Refresh");
	trendingLabel = new QLabel("   Trending");


	nameEdit = new QLineEdit;
	passwordEdit = new QLineEdit;
	loginButton = new QPushButton("&Log In");
	quitLoginButton = new QPushButton("&Quit");
	registerButton = new QPushButton("&Register");
	loginLayout = new QGridLayout;


	mainFeeds = new QListWidget;

	mentionFeeds = new QListWidget;
	followingList = new QListWidget;
	followerList = new QListWidget;


	// Top hashtags
	topHashtags = new QListWidget;
	topTagsArea = new QScrollArea;

	topHashtags->setWordWrap(true);
	topTagsArea->setWidget(topHashtags);
	topTagsArea->setBackgroundRole(QPalette::Window);
	topTagsArea->setWidgetResizable(true);
	topTagsArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);


	mainFeeds->setWordWrap(true);
	mentionFeeds->setWordWrap(true);
	followingList->setWordWrap(true);
	followerList->setWordWrap(true);

	//mainDisplay = new QScrollArea;
	mainLayout = new QGridLayout;
	


	mainFeedArea = new QScrollArea;
	mainFeedArea->setWidget(mainFeeds);

	mentionFeedArea = new QScrollArea;
	mentionFeedArea->setWidget(mentionFeeds);
	followingArea = new QScrollArea;
	followingArea->setWidget(followingList);
	followerArea = new QScrollArea;
	followerArea->setWidget(followerList);


	mainFeedArea->setBackgroundRole(QPalette::Window);
	mainFeedArea->setWidgetResizable(true);
	//mainFeedArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	mainFeedArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

	mentionFeedArea->setBackgroundRole(QPalette::Window);
	mentionFeedArea->setWidgetResizable(true);
	//mentionFeedArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	mentionFeedArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	mentionFeedArea->hide();

	followingArea->setBackgroundRole(QPalette::Window);
	followingArea->setWidgetResizable(true);
	//followingArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	followingArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	followingArea->hide();

	followerArea->setBackgroundRole(QPalette::Window);
	followerArea->setWidgetResizable(true);
	//followerArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	followerArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	followerArea->hide();

	hashWin = new SearchWin(0);
	userWin = new SearchWin(1);

	//Initialize Userlist
	std::map<std::string, User*> users = twiteng->users();
	std::map<std::string, User*>::iterator it = users.begin();
	for(; it!= users.end(); ++it){
		userlist->addItem(QString::fromStdString(it->first));
	}

	// Initialize main feeds of current user
	// Find current user
	//std::string username = userlist->currentText().toStdString();
	
	// set up current feeds display
	//currentFeeds(username);
	//getTrend();

	QHBoxLayout* userLayout = new QHBoxLayout;
	userLayout->addWidget(userbox);
	userLayout->addWidget(userLabel);
	//userLayout->addWidget(userlist);
	userlist->hide();


	QHBoxLayout* optionLayout = new QHBoxLayout;
	optionLayout->addWidget(displayOptionLabel);
	optionLayout->addWidget(options);

	QFormLayout* addTweetLayout = new QFormLayout;
	addTweetLayout->addRow("&New Tweet:", newTweet);
	
	QGridLayout* tweetLayout = new QGridLayout;
	tweetLayout->addLayout(addTweetLayout,0,0,1,-1);
	tweetLayout->addWidget(postButton,1,0);

	QGridLayout* barLayout = new QGridLayout;

	// Tool bar
	//barLayout->addLayout(userLayout, 0, 0);
	barLayout->addLayout(optionLayout,1,0);
	barLayout->addWidget(toolButtons,2, 0);
	barLayout->addWidget(sscButton,3,0);
	//barLayout->addWidget(quitButton,5,0);
	

	QFormLayout* loginNameLayout = new QFormLayout;
	loginNameLayout->addRow("&Username:", nameEdit);
	QFormLayout* loginPasswordLayout = new QFormLayout;
	loginPasswordLayout->addRow("&Password:", passwordEdit);

	loginLayout->addLayout(loginNameLayout, 0, 0, 1, -1);
	loginLayout->addLayout(loginPasswordLayout, 1, 0, 1, -1);
	
	QHBoxLayout* loginButtonLayout = new QHBoxLayout;
	loginButtonLayout->addWidget(loginButton);
	loginButtonLayout->addWidget(registerButton);

	loginLayout->addLayout(loginButtonLayout, 2, 0, 1, -1);
	loginLayout->addWidget(quitLoginButton);
	loginLayout->setColumnMinimumWidth(0,220);


	loginWindow->setLayout(loginLayout);
	
	loginWindow->show();

	mainLayout->addLayout(userLayout, 0, 0, 1, 1);
	mainLayout->addLayout(barLayout, 1, 0, 2,1);

	// Tweet edit
	mainLayout->addLayout(tweetLayout,0,1,1,2);

	// Main Feed
	mainLayout->addWidget(mainFeedArea,1,1,6,2);
	mainLayout->addWidget(mentionFeedArea,1,1,6,2);
	mainLayout->addWidget(followingArea,1,1,6,2);
	mainLayout->addWidget(followerArea,1,1,6,2);
	mainLayout->addWidget(quitButton, 6, 0, 1,1);


	// Minimum width for main display area
	mainLayout->setColumnMinimumWidth(1,400);
	mainLayout->setRowMinimumHeight(1,200);
	mainLayout->setRowMinimumHeight(2,200);

	mainLayout->addWidget(trendingLabel, 0,3,1,1);
	mainLayout->addWidget(topTagsArea, 1,3,5,1);
	mainLayout->addWidget(refTagButton, 6,3,5,1);


	setLayout(mainLayout);
	this->hide();

	connect(userlist, SIGNAL(currentIndexChanged(int)), this, SLOT(onUserSwitched()));
	connect(options, SIGNAL(currentIndexChanged(int)), this, SLOT(onOptionChanged()));
	connect(toolButtons, SIGNAL(searchHashtagWin()), this, SLOT(searchHashWindow()));
	connect(toolButtons, SIGNAL(searchUserWin()), this, SLOT(searchUserWindow()));

	connect(hashWin, SIGNAL(closePressed()),this, SLOT(closeHashWindow()));
	connect(userWin, SIGNAL(closePressed()),this, SLOT(closeUserWindow()));

	connect(hashWin, SIGNAL(searchAndHash()), this, SLOT(ANDSearch()));
	connect(hashWin, SIGNAL(searchOrHash()), this, SLOT(ORSearch()));
	connect(userWin, SIGNAL(searchUser()), this, SLOT(UserSearch()));
	connect(userWin, SIGNAL(addToFollowing()), this, SLOT(addFollow()));

	connect(postButton, SIGNAL(clicked()), this, SLOT(tweet()));
	connect(quitButton, SIGNAL(clicked()), this, SLOT(onQuitClicked()));

	connect(sscButton, SIGNAL(clicked()), this, SLOT(onSSCButtonClicked()));

	connect(refTagButton, SIGNAL(clicked()), this, SLOT(onRefreshClicked()));

	connect(loginButton, SIGNAL(clicked()), this, SLOT(onLoginButtonClicked()));
	connect(registerButton, SIGNAL(clicked()), this, SLOT(onRegButtonClicked()));
	connect(quitLoginButton, SIGNAL(clicked()), this, SLOT(onQuitLogButtonClicked()));	
}




TwitGui::~TwitGui(){
	delete userbox;
	delete userlist;
	delete options;
	delete displayOptionLabel;
	delete quitButton;
	delete newTweet;
	delete postButton;
	delete mainFeeds;
	delete mentionFeeds;
	delete followingList;
	delete followerList;
	delete mainLayout;
	delete mainFeedArea;
	delete mentionFeedArea;
	delete followingArea;
	delete followerArea;
	delete sscButton;
	delete topHashtags;
	delete topTagsArea;
	delete refTagButton;
	delete trendingLabel;

	delete nameEdit;
	delete passwordEdit;
	delete loginButton;
	delete quitLoginButton;
	delete registerButton;

	delete loginLayout;
	delete loginWindow;
}





void TwitGui::ANDSearch(){
	hashWin->searchHash(twiteng, 0);
}

void TwitGui::ORSearch(){
	hashWin->searchHash(twiteng, 1);
}

void TwitGui::UserSearch(){
	userWin->searchUser(twiteng);
}




void TwitGui::onUserSwitched(){
	std::string name = userlist->currentText().toStdString();
	currentFeeds(name);
}


void TwitGui::currentFeeds(std::string name){
	mainFeeds->clear();
	mentionFeeds->clear();
	followerList->clear();
	followingList->clear();

	User* currentUser = twiteng->getUser(name);

	// Feeds
	std::vector<Tweet*> mainf; // main feed in Tweet format
	std::vector<Tweet*> mentf; // mention feed in Tweet format

	// Followings and followers
	std::set<User*> followingUsers; 
	std::set<User*> followerUsers;

	// get the feeds
	if(currentUser != NULL){
		mainf = twiteng->getMainFeeds(name);
		mentf = twiteng->mentionFeeds(name);
		followingUsers = currentUser->following();
		followerUsers = currentUser->followers();
	}

	// Convert the tweets to strings
	// convert main feed
	std::vector<Tweet*>::iterator ittweet;
	if(!mainf.empty()){
		for(ittweet = mainf.begin(); ittweet != mainf.end(); ++ittweet){
			std::stringstream ss;
			ss << *(*ittweet) << std::endl;
			std::string tweetStr; 
			getline(ss, tweetStr);

			mainFeeds->addItem(QString::fromStdString(tweetStr));
		}
	}
	// convert mention feed
	if(!mentf.empty()){
		for(ittweet = mentf.begin(); ittweet != mentf.end(); ++ittweet){
			std::stringstream ss;
			ss << *(*ittweet) << std::endl;
			std::string tweetStr; 
			getline(ss, tweetStr);

			mentionFeeds->addItem(QString::fromStdString(tweetStr));
		}
	}	

	std::set<User*>::iterator ituser;
	if(!followingUsers.empty()){
		for (ituser = followingUsers.begin(); ituser != followingUsers.end(); ++ituser)
		{
			std::string tempName = (*ituser)->name();
			followingList->addItem(QString::fromStdString(tempName));
		}
	}

	if(!followerUsers.empty()){
		for (ituser = followerUsers.begin(); ituser != followerUsers.end(); ++ituser)
		{
			std::string tempName = (*ituser)->name();
			followerList->addItem(QString::fromStdString(tempName));
		}
	}
}

void TwitGui::onOptionChanged(){
	int index = options->currentIndex();
	switch (index) {
		case 0:
			mainFeedArea->show();
			mentionFeedArea->hide();
			followingArea->hide();
			followerArea->hide();

			break;
		case 1:
			mainFeedArea->hide();
			mentionFeedArea->show();
			followingArea->hide();
			followerArea->hide();			

			break;
		case 2:
			mainFeedArea->hide();
			mentionFeedArea->hide();
			followingArea->show();
			followerArea->hide();

			break;
		case 3:
			mainFeedArea->hide();
			mentionFeedArea->hide();
			followingArea->hide();
			followerArea->show();
		default:
			break;
	}

}

void TwitGui::tweet(){
	//std::string username = userlist->currentText().toStdString();
	std::string username = currentUser;
	std::string tweetText = newTweet->text().toStdString();
	if(!tweetText.empty()){
		DateTime now;
		twiteng->addTweet(username, now, tweetText);
		currentFeeds(username);
		newTweet->clear();	
	}
	else{
		QMessageBox::StandardButton answer;
		answer = QMessageBox::question(this, "", "The tweet text is empty. Are you sure that you want to post?"
			, QMessageBox::Yes | QMessageBox::No);
		if(answer == QMessageBox::Yes){
			DateTime now;
			twiteng->addTweet(username, now, tweetText);
			currentFeeds(username);
			newTweet->clear();
		}
	}
	
}


void TwitGui::searchHashWindow(){
	hashWin->show();
}

void TwitGui::closeHashWindow(){
	hashWin->hide();
}

void TwitGui::searchUserWindow(){
	userWin->show();
}

void TwitGui::closeUserWindow(){
	userWin->hide();
}

void TwitGui::addFollow(){
	//std::string currentUsername = userlist->currentText().toStdString();
	userWin->follow(twiteng, currentUser);
	currentFeeds(currentUser);
}


void TwitGui::saveAndQuit(std::string filename){
	twiteng->save(filename);
	emit quitProgram();
}

void TwitGui::onQuitClicked(){
	QMessageBox::StandardButton answer;
	answer = QMessageBox::question(this, "", "Are you sure that you want to quit?"
		, QMessageBox::Yes | QMessageBox::No);
	if(answer == QMessageBox::Yes){
		QMessageBox::StandardButton answer2;
		answer2 = QMessageBox::question(this, "", "Would you like to save your changes?"
			, QMessageBox::Yes | QMessageBox::No);

		if(answer2 == QMessageBox::Yes){
			QString filename = QFileDialog::getSaveFileName();
			saveAndQuit(filename.toStdString());
		}
		else{
			emit quitProgram();
		}
	}
}

void TwitGui::onSSCButtonClicked(){
	QString filename = QFileDialog::getSaveFileName();
	twiteng->findSSC(filename.toStdString());
}



void TwitGui::getTrend(){
	std::vector<std::string> tags = twiteng->topHashtags();
	std::vector<std::string>::iterator it = tags.begin();
	topHashtags->clear();
	for(; it != tags.end(); ++it){
		topHashtags->addItem(QString::fromStdString(*it));
	}
}


void TwitGui::onRefreshClicked(){
	getTrend();
}


void TwitGui::onLoginButtonClicked(){

	std::string name = nameEdit->text().toStdString();
	std::string password = passwordEdit->text().toStdString();

	if(!name.empty() && !password.empty()){
		User* user = twiteng->getUser(name);
		if(user == NULL){
			QMessageBox::warning(this, "Warning", "User doesn't exist!");
			
		}
		else{
			unsigned int encoded = twiteng->passwordHasher(password);
			if(encoded == user->password()){
				nameEdit->clear();
				passwordEdit->clear();
				loginWindow->hide();
				currentUser = name;
				userLabel->setText(QString::fromStdString(name));
				currentFeeds(name);
				getTrend();
				this->show();
			}
			else{
				passwordEdit->clear();
				QMessageBox::warning(this, "Warning", "Incorrect password!");
			}
		}
	}
	else{
		
		if(name.empty()){
			QMessageBox::warning(this, "Warning", "Please provide your username!");

		}
		else if(password.empty()){
			QMessageBox::warning(this, "Warning", "Please provide your password!");
		}
	}
}


void TwitGui::onRegButtonClicked(){
	std::string name = nameEdit->text().toStdString();
	std::string password = passwordEdit->text().toStdString();

	if(!name.empty() && !password.empty()){
		User* user = twiteng->getUser(name);
		if(user != NULL){
			QMessageBox::warning(this, "Warning", "Username already used!");
		}
		else{
			unsigned int encoded = twiteng->passwordHasher(password);
			twiteng->addUser(name, encoded);

			nameEdit->clear();
			passwordEdit->clear();
			loginWindow->hide();
			currentUser = name;
			userLabel->setText(QString::fromStdString(name));
			currentFeeds(name);
			getTrend();
			this->show();
		}
	}
	else{
		
		if(name.empty()){
			QMessageBox::warning(this, "Warning", "Please provide your username!");
			
		}
		else if(password.empty()){
			QMessageBox::warning(this, "Warning", "Please provide your password!");
		}
	}

}

void TwitGui::onQuitLogButtonClicked(){
	QMessageBox::StandardButton answer;
	answer = QMessageBox::question(this, "", "Are you sure that you want to quit?"
		, QMessageBox::Yes | QMessageBox::No);
	if(answer == QMessageBox::Yes){
		emit quitProgram();
	}
}


