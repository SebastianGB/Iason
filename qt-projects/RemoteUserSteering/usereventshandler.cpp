#include "usereventshandler.h"

UserEventsHandler::UserEventsHandler(QWidget *parent) :
    QWidget(parent)
{
    _upPressed = _downPressed = _leftPressed = _rightPressed = false;
    QFont f( "Courier New", 10, QFont::Bold);

    _labelUp = new QLabel("Up   ", this);
    _labelUp->setFont(f);
    _labelUp->move(42,0);
    _labelUp->setStyleSheet("QLabel { background-color : red; }");
    _labelUp->show();

    _labelDown = new QLabel("Down ", this);
    _labelDown->setFont(f);
    _labelDown->move(42,20);
    _labelDown->setStyleSheet("QLabel { background-color : red; }");
    _labelDown->show();

    _labelLeft = new QLabel("Left ", this);
    _labelLeft->setFont(f);
    _labelLeft->move(0,20);
    _labelLeft->setStyleSheet("QLabel { background-color : red; }");
    _labelLeft->show();

    _labelRight = new QLabel("Right", this);
    _labelRight->setFont(f);
    _labelRight->move(84,20);
    _labelRight->setStyleSheet("QLabel { background-color : red; }");
    _labelRight->show();

    //the focus of the Widget is set to high, so that the Key events will work
    this->setFocusPolicy(Qt::StrongFocus);
}

void UserEventsHandler::keyReleaseEvent(QKeyEvent* keyvent)
{
    switch(keyvent->key())
    {
    case Qt::Key_Up:
        _upPressed = false;
        _labelUp->setStyleSheet("QLabel { background-color : red; }");
        break;
    case Qt::Key_Down:
        _downPressed = false;
        _labelDown->setStyleSheet("QLabel { background-color : red; }");
        break;
    case Qt::Key_Left:
        _leftPressed = false;
        _labelLeft->setStyleSheet("QLabel { background-color : red; }");
        break;
    case Qt::Key_Right:
        _leftPressed = false;
        _labelRight->setStyleSheet("QLabel { background-color : red; }");
        break;
    }
}

void UserEventsHandler::keyPressEvent(QKeyEvent* keyvent)
{
    switch(keyvent->key())
    {
    case Qt::Key_Up:
        _upPressed = true;
        _labelUp->setStyleSheet("QLabel { background-color : green; }");
        break;
    case Qt::Key_Down:
        _downPressed = true;
        _labelDown->setStyleSheet("QLabel { background-color : green; }");
        break;
    case Qt::Key_Left:
        _leftPressed = true;
        _labelLeft->setStyleSheet("QLabel { background-color : green; }");
        break;
    case Qt::Key_Right:
        _leftPressed = true;
        _labelRight->setStyleSheet("QLabel { background-color : green; }");
        break;
    }
}
