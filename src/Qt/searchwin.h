#ifndef SEARCHWIN_H
#define SEARCHWIN_H

#include "../objects/twiteng.h"

#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <string>
#include <QWidget>
#include <QListWidget>
#include <QPushButton>
#include <QLabel>
#include <QScrollArea>
#include <QComboBox>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QString>
#include <QPalette>
#include <QLineEdit>

class SearchWin: public QWidget {
	Q_OBJECT
public:
	SearchWin(int type, QWidget *parent = 0);
	~SearchWin();
	void hashSearchResults(std::vector<Tweet*>& tweets);
	void userSearchResults(std::vector<User*>& users);
	void searchHash(TwitEng* twiteng, int strategy);
	void searchUser(TwitEng* twiteng);
	void follow(TwitEng* twit, std::string currentUsername);

signals:
	void searchAndHash();
	void searchOrHash();
	void searchUser();
	void closePressed();
	void addToFollowing();


public slots:
	void searchAndHashClicked();
	void searchOrHashClicked();
	void searchUserClicked();
	//void searchClear(); // clean the list
	void closeButtonClicked();
	void onFollowButtonClicked();

private:
	QLabel* searchType;
	QLineEdit* searchKey;
	QScrollArea* resultArea;
	QListWidget* results;
	QPushButton* ANDSearch;
	QPushButton* ORSearch;
	QPushButton* enterButton;

	QPushButton* closeButton;

	QPushButton* followButton;

	QGridLayout* mainLayout;
	QFormLayout* form;
	QHBoxLayout* buttonLayout;
};

#endif