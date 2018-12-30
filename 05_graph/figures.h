#pragma once

#include <iostream>
#include <list>

/// перечень возможных фигур
enum class Figures {
    point, circle, rectangle
};

/// для вывода в поток типа Figures
std::ostream& operator << (std::ostream&, Figures) {}


/// Базовая структура графического примитива
struct FigureBase
{
    /// вывод фигуры на экран
    virtual void draw() = 0; 
    /// для экспорта фигуры в файл
    virtual void export_(std::fstream&) = 0;
};

struct Point : FigureBase
{
    /// начальные точки фигуры
    int x,y;
    void draw() override {}
    void export_(std::fstream& stream) override
    {
        stream << Figures::point << ' ' << x << ' ' << y << std::endl;
    }
    Point (int x, int y) : x(x), y(y) {}
};


struct Circle : Point
{
    int radius;
    void draw() override {}
    void export_(std::fstream& stream) override
    {
        stream << Figures::circle << ' ' << x << ' ' << y << ' ' << radius << std::endl;
    }
    Circle (int x, int y, int radius) : Point{x,y}, radius(radius) {}
};

struct Rectangle : Point
{
    int weight;
    int hight;
    void draw() override {}
        void export_(std::fstream& stream) override
    {
        stream << Figures::rectangle << ' ' << x << ' ' << y << ' ' << weight << ' ' << hight << std::endl;
    }
    Rectangle (int x, int y, int weight, int hight) : Point{x,y}, weight(weight), hight(hight) {}
};

/// ну и ещё куча фигур