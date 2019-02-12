#include "anechka.h"
#include "ui_anechka.h"

// Можно через стакед виджет.
// Можно динамически создавать/удалять нужные виджеты.

anechka::anechka ( QWidget *parent ) :
    QMainWindow ( parent ),
    ui ( new Ui::anechka )
{
    ui -> setupUi ( this );

    // 16:9
    this -> setGeometry ( QRect ( 200, 200, 1280, 720 ) );

    // TODO
    // connect_button
    // список доступных bluetooth устройств
    // ввод пароля ( при необходимости )
    // окно робота ( датчики, камера, управление )

}

anechka::~anechka ( )
{
    delete ui;
}
