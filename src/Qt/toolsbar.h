#ifndef TOOLSBAR_H
#define TOOLSBAR_H

#include "../objects/twiteng.h"
#include <stdlib.h>
#include <QWidget>
#include <QPushButton>
#include <QComboBox>
#include <QVBoxLayout>
#include <QString>
#include <QCheckBox>


class Toolsbar : public QWidget {
	Q_OBJECT

public:

	Toolsbar(QWidget *parent = 0);
	~Toolsbar();

signals:
	void searchHashtagWin();
	void searchUserWin();
	//void showMentions();
	//void hideMentions();

public slots:
	void onSearchHashButtonClicked();
	void onSearchUserButtonClicked();
	//void onMentionButtonToggled();

private:

	//QCheckBox* showMentionFeed;
	QPushButton* searchHash;
	QPushButton* searchUser;
	QVBoxLayout* vertical;
};

#endif
