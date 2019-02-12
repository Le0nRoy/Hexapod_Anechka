#include "anechka.h"
#include <QApplication>

int main ( int argc, char *argv[ ] )
{
    QApplication a ( argc, argv );
    anechka w;
    w.show ( );

    return a.exec ( );
}
