#ifndef TWITGUI_H
#define TWITGUI_H

#include "../objects/twiteng.h"
#include "searchwin.h"
#include "toolsbar.h"
#include <iostream>
#include <string>
#include <stdlib.h>
#include <QWidget>
#include <QListWidget>
#include <QPushButton>
#include <QLabel>
#include <QScrollArea>
#include <QComboBox>
#include <QGridLayout>
#include <QFormLayout>
#include <QString>
#include <QLineEdit>
#include <QFileDialog>
#include <QMessageBox>

class TwitGui : public QWidget{
	Q_OBJECT
public:

	TwitGui(TwitEng* twit, QWidget * parent = 0);
	~TwitGui();
	void changeUser();
	void searchHashAnd();
	void searchHashOr();
	void searchUser();
	void saveAndQuit(std::string filename);
	void currentFeeds(std::string name);
	void getTrend();

signals:
	void quitProgram();

public slots:
	void onUserSwitched();
	void onOptionChanged();
	void tweet();
	void searchHashWindow();
	void closeHashWindow();
	void searchUserWindow();
	void closeUserWindow();
	void ANDSearch();
	void ORSearch();
	void UserSearch();
	void addFollow();	
	void onQuitClicked();
	void onSSCButtonClicked();
	void onRefreshClicked();
	void onLoginButtonClicked();
	void onRegButtonClicked();
	void onQuitLogButtonClicked();

private:
	std::string currentUser;
	QLabel* userbox;
	QComboBox* userlist;
	QLabel* userLabel;
	QLabel* displayOptionLabel;
	QComboBox* options;
	QPushButton* quitButton;
	Toolsbar* toolButtons;
	QLineEdit* newTweet;
	QPushButton* postButton;
	QPushButton* sscButton;


	QListWidget* mainFeeds;
	QListWidget* mentionFeeds;

	//QScrollArea* mainDisplay;
	
	QScrollArea* mainFeedArea;
	QScrollArea* mentionFeedArea;
	QScrollArea* followingArea;
	QScrollArea* followerArea;

	QListWidget* followingList;
	QListWidget* followerList;

	QListWidget* topHashtags;
	QScrollArea* topTagsArea;

	QPushButton* refTagButton;
	QLabel* trendingLabel;

	SearchWin* hashWin;
	SearchWin* userWin;

	QGridLayout* mainLayout;
	TwitEng* twiteng;

	QLineEdit* nameEdit;
	QLineEdit* passwordEdit;
	QPushButton* loginButton;
	QPushButton* quitLoginButton;
	QPushButton* registerButton;

	QGridLayout* loginLayout;

	QWidget* loginWindow;
	//QWidget* mainWindow;
};

#endif