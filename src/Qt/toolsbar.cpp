#include "toolsbar.h"

Toolsbar::Toolsbar(QWidget *parent) : QWidget( parent ){

	vertical = new QVBoxLayout( this );
	//userlist = new QComboBox;
	//showMentionFeed = new QCheckBox("Show &Mentions");

	searchHash = new QPushButton("Find &Hashtags");
	searchUser = new QPushButton("Find &User");


	//mentionFeed->setToolButtonStyle(Qt::ToolButtonTextOnly);
	//mentionFeed->setText("See Mentions");

	//vertical->addWidget(userlist);
	//vertical->addWidget(showMentionFeed);
	vertical->addWidget(searchHash);
	vertical->addWidget(searchUser);	
	setLayout(vertical);

	connect(searchHash, SIGNAL(clicked()), this, SLOT(onSearchHashButtonClicked()));
	connect(searchUser, SIGNAL(clicked()), this, SLOT(onSearchUserButtonClicked()));

	//connect(showMentionFeed, SIGNAL(toggled(bool)), this, SLOT(onMentionButtonToggled));
}

Toolsbar::~Toolsbar(){
	delete searchHash;
	delete searchUser;
	delete vertical;
}



void Toolsbar::onSearchHashButtonClicked(){
	emit searchHashtagWin();
}

void Toolsbar::onSearchUserButtonClicked(){
	emit searchUserWin();
}

/*void Toolsbar::onMentionButtonToggled(){
	if(showMentionFeed->isChecked()){
		emit showMentions();
	}
	else{
		emit hideMentions();
	}
}*/
