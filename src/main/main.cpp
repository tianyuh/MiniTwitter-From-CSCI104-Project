#include <qapplication.h>
#include "../Qt/twitgui.h"
#include <QObject>

int main(int argc, char* argv[])
{

	QApplication a(argc, argv);

  if(argc < 2){
    std::cerr << "Please provide the twitter data file" << std::endl;
    return 1;
  }
  TwitEng twit;

  if ( twit.parse(argv[1]) ){
   	std::cerr << "Unable to parse " << argv[1] << std::endl;
    return 1;
  }

  TwitGui twitgui(&twit);
  QObject::connect(&twitgui,SIGNAL(quitProgram()), &a, SLOT(quit()));
  //twitgui.show();
  return a.exec();
}
