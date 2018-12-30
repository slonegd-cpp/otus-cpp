#pragma once

#include "user.h"
#include "document.h"

struct Screen
{
    Document& doc;

    Screen (Document& doc) : doc(doc) {}

    /// очищает поле экрана
    void clear() {}

    /// отрисовывает все фигуры из doc
    void draw()
    {
        for (const auto& figure : doc.data)
            figure->draw();
    }

    /// отрисовывает последнюю фигуру из doc
    void drawLast() {}
};