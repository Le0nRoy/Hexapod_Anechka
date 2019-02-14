#include "connect_button.h"
#include "ui_connect_button.h"
#include "QPushButton"

connect_button::connect_button ( QWidget *parent ) :
    QWidget ( parent ),
    ui ( new Ui::connect_button )
{
    ui -> setupUi ( this );
    // TODO
    // кнопка посередине экрана
    // при нажатии на кнопку должен открываться другой виджет
    QPushButton *conBut = new QPushButton;

}
