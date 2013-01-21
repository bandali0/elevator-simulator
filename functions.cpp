#include "mainwindow.h"
#include <sstream>

// a function to convert an instance of c++ string object to QString (which is used in Qt)
QString toQString(std::string const &s)
{
    return QString::fromUtf8(s.c_str());
}

// a function to convert an instance of QString object (which is used in Qt) to c++ string
std::string fromQString(QString const &s)
{
    return std::string(s.toUtf8().data());
}

// a function to convert c++ string to an int
std::string intToString (int num)
{
    std::stringstream ss;
    ss << num;
    return ss.str();
}
