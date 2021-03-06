#ifndef USEREVENTSHANDLER_H
#define USEREVENTSHANDLER_H

#include <QWidget>
#include <QtGui>
#include "GCSCommunicator.h"

class UserEventsHandler : public QWidget
{
    Q_OBJECT
public:
    QLabel *_labelUp, *_labelDown, *_labelLeft, *_labelRight;

    explicit UserEventsHandler(QWidget *parent = 0);

protected:
    bool _upPressed, _downPressed, _leftPressed, _rightPressed;
    QLabel *_gcsMsg;
    GCSCommunicator* communicator;

    void keyReleaseEvent(QKeyEvent* keyvent);
    void keyPressEvent(QKeyEvent* keyvent);
    
signals:
    
public slots:
    
};

#endif // USEREVENTSHANDLER_H
