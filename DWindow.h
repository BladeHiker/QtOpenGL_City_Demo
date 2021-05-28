#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_DWindow.h"

class DWindow : public QMainWindow
{
    Q_OBJECT

public:
    DWindow(QWidget *parent = Q_NULLPTR);

private:
    Ui::DWindowClass ui;
};
