/**
 * 头文件包含
 */
#include <QApplication>
#include "dialog.h"

/**
 * 主函数
 */
int main( int argc, char *argv[] )
{
    QApplication app(argc,argv);

    Dialog *dialog = new Dialog;

    dialog->show();

    return app.exec();
}
