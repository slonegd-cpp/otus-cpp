#include "user.h"
#include "screen.h"
#include "figures.h"
#include "document.h"


int main(int argc, char const *argv[])
{
    User user;
    Document doc {user};
    Screen screen {doc};
    screen.clear();

    while (user.getEvent() != User::Event::exit_) {
        switch (user.getEvent()) {
            case User::Event::none : break;
            case User::Event::openDoc :
                doc.initData();
                screen.clear();
                screen.draw();
                break;
            case User::Event::saveDoc :
                doc.saveData();
                break;
            case User::Event::newDoc :
                doc.clearData();
                screen.clear();
                break;
            case User::Event::addFigure :
                doc.addFigure();
                screen.drawLast();
                break;
            case User::Event::removeFigure :
                doc.removeFigure();
                screen.clear();
                screen.draw();
                break;
        }
        user.clearEvent();
    }

}