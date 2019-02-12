#ifndef CONNECT_BUTTON_H
#define CONNECT_BUTTON_H

#include <QWidget>

namespace Ui
{
    class connect_button;
}

class connect_button : public QWidget
{
    Q_OBJECT

public:
    connect_button ( QWidget *parent = nullptr );
    ~connect_button ( );

private:
    Ui::connect_button *ui;

};

#endif // CONNECT_BUTTON_H
