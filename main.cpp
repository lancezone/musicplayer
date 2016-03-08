
#include <QApplication>
#include "musicplayer.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //创建主界面
    musicplayer w;
    w.show();
    return a.exec();
}
