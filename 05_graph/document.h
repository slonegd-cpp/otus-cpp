#pragma once

#include <list>
#include <fstream>
#include "user.h"

struct Document {
    User& user;

    Document (User& user) : user(user) {}

    /// список всех фигур
    std::list<FigureBase*> data;

    /// инициализация data из файла
    void initData()
    {
        // файл берём у пользователя
        std::fstream& file = user.getFile();
    }

    /// запись данных в файл
    void saveData()
    {
        // файл берём у пользователя
        std::fstream& file = user.getFile();
        for (const auto& figure : data)
            figure->export_(file);
    }

    /// очищает data
    void clearData()
    {
        data.clear();
    }

    /// добавляет новую фигуру в data
    void addFigure()
    {
        // указатель на фигуру берём у пользователя
        FigureBase* p = user.getFigure();
        data.push_back (p);
    }

    /// удаляет фигуру из data
    void removeFigure()
    {
        // номер фигуры из списка указывает пользователь
        data.remove (user.getFigure());
    }
};