#ifndef ANECHKA_H
#define ANECHKA_H

#include <QMainWindow>

namespace Ui
{
    class anechka;
}

class anechka : public QMainWindow
{
    Q_OBJECT

public:
    explicit anechka ( QWidget *parent = nullptr );
    ~anechka ( );

private slots:


private:
    Ui::anechka *ui;

};

#endif // ANECHKA_H
