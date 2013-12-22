#include <QtGui>
#include "usereventshandler.h"

 int main(int argc, char *argv[])
 {
     QApplication app(argc, argv);
     QWidget window;
     window.resize(320, 240);
     window.setWindowTitle(QApplication::translate("RemoteUserSteering", "Remote User Steering"));
     window.show();

     UserEventsHandler* usrEventHandler = new UserEventsHandler(&window);
     usrEventHandler->move(20,20);
     usrEventHandler->setSizeIncrement(100,100);
     usrEventHandler->show();

     return app.exec();
 }
