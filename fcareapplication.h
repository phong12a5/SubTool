#ifndef FCAREAPPLICATION_H
#define FCAREAPPLICATION_H

#include <QApplication>

class FCareApplication: public QApplication
{
public:
    explicit FCareApplication(int &argc, char **argv);
    ~FCareApplication();

    void startApplication();
};

#endif // FCAREAPPLICATION_H
