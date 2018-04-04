#include "searchwin.h"


SearchWin::SearchWin(int type, QWidget *parent) 
: QWidget( parent ){
	searchType = new QLabel;
	searchKey = new QLineEdit;
	resultArea = new QScrollArea;
	results = new QListWidget;
	ANDSearch = new QPushButton("Search by &AND");
	ORSearch = new QPushButton("Search by &OR");
	enterButton = new QPushButton("Search &User");
	closeButton = new QPushButton("&Close");
	followButton = new QPushButton("&Follow");

	mainLayout = new QGridLayout;
	form = new QFormLayout;
	buttonLayout = new QHBoxLayout;

	results->setWordWrap(true);
	resultArea->setBackgroundRole(QPalette::Window);
	resultArea->setWidget(results);
	resultArea->setWidgetResizable(true);
	resultArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);


	if(type == 1){
		searchType->setText("Username");
		form->addRow(searchType, searchKey);
		mainLayout->addLayout(form, 0, 0);
		mainLayout->addWidget(enterButton, 1 ,0);
		mainLayout->addWidget(resultArea,2, 0);	
		QGridLayout* buttonLayout = new QGridLayout;
		buttonLayout->addWidget(closeButton,0,0);
		buttonLayout->addWidget(followButton,0,1);
		mainLayout->addLayout(buttonLayout,3,0);
		mainLayout->setRowMinimumHeight(2,200);
	}
	else{
		searchType->setText("Hashtags");
		form->addRow(searchType, searchKey);
		buttonLayout->addWidget(ANDSearch);
		buttonLayout->addWidget(ORSearch);	
		mainLayout->addLayout(form, 0, 0);
		mainLayout->addLayout(buttonLayout, 1, 0);
		mainLayout->addWidget(resultArea,2,0);
		mainLayout->addWidget(closeButton,3,0);
		mainLayout->setRowMinimumHeight(2,200);
	}

	setLayout(mainLayout);

	connect(ANDSearch, SIGNAL(clicked()), this, SLOT(searchAndHashClicked()));
	connect(ORSearch, SIGNAL(clicked()), this, SLOT(searchOrHashClicked()));
	connect(enterButton, SIGNAL(clicked()), this, SLOT(searchUserClicked()));
	connect(searchKey, SIGNAL(textChanged(const QString &)), results, SLOT(clear()));
	connect(closeButton, SIGNAL(clicked()), this, SLOT(closeButtonClicked()));
	connect(followButton, SIGNAL(clicked()), this, SLOT(onFollowButtonClicked()));
}

SearchWin::~SearchWin(){
	delete searchType;
	delete searchKey;
	delete resultArea;
	delete results;
	delete ANDSearch;
	delete ORSearch;
	delete enterButton;
	delete mainLayout;
	delete form;
	delete buttonLayout;
	delete closeButton;
	delete followButton;

}

void SearchWin::searchAndHashClicked(){
	emit searchAndHash();
}

void SearchWin::searchOrHashClicked(){
	emit searchOrHash();

}

void SearchWin::searchUserClicked(){
	emit searchUser();
}


void SearchWin::hashSearchResults(std::vector<Tweet*>& tweets){
	results->clear();
	if(tweets.empty()){
		results->addItem("No Matches.");
	}
	else{
		std::vector<Tweet*>::iterator it = tweets.begin();
		for(; it != tweets.end(); it++){
			std::stringstream ss;
			std::string temp;
			ss << *(*it) << std::endl;
			getline(ss, temp);
			results->addItem(QString::fromStdString(temp));
		}
	}
}


void SearchWin::userSearchResults(std::vector<User*>& users){
	results->clear();
	if(users.empty()){
		results->addItem("No Matches.");
	}
	else{
		std::vector<User*>::iterator it = users.begin();
		for(; it != users.end(); it++){
			std::string temp = (*it)->name();
			results->addItem(QString::fromStdString(temp));
		}
	}
}


void SearchWin::searchHash(TwitEng* twiteng, int strategy){

	std::string line = searchKey->text().toStdString();
	std::stringstream ss(line);
	std::vector<std::string> hashtags;
	std::string temp;
	while(ss >> temp){
		hashtags.push_back(temp);
	}
	std::vector<Tweet*> matches = twiteng->search(hashtags, strategy);
	hashSearchResults(matches);
}


void SearchWin::searchUser(TwitEng* twiteng){
	std::string name = searchKey->text().toStdString();
	std::vector<User*> matches = twiteng->findUsers(name);
	userSearchResults(matches);
}

void SearchWin::closeButtonClicked(){
	results->clear();
	searchKey->clear();
	emit closePressed();
}

void SearchWin::onFollowButtonClicked(){
	emit addToFollowing();
}

void SearchWin::follow(TwitEng* twit, std::string currentUsername){
	//std::string currentUsername = userlist->currentText().toStdString();
	std::string name2Follow = results->currentItem()->text().toStdString();
	User* currentUser = twit->getUser(currentUsername);
	User* user2Follow = twit->getUser(name2Follow);

	currentUser->addFollowing(user2Follow);
	user2Follow->addFollower(currentUser);
}
