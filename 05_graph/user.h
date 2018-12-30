#pragma once

#include <fstream>
#include "figures.h"

struct User
{
    /// список действий пользователя
    enum Event {
        none,
        newDoc,
        saveDoc,
        openDoc,
        addFigure,
        removeFigure,
        exit_
    } event = Event::none;

    /// возвращает текущее действие пользователя
    Event getEvent() {}

    /// сбрасывает теущее действие пользователя
    void clearEvent()
    {
        event = Event::none;
    }

    /// возарвщает ссылку на файл для чтения сохранения 
    std::fstream& getFile() {}

    /// возвращает указатель на вновь созданную или удаляемую фигуру
    FigureBase* getFigure() {}

};