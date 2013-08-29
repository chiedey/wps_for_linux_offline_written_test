#ifndef DIALOG_H
#define DIALOG_H

/**
 * 头文件包含
 */
#include <QFile>
#include <QDialog>
#include <QString>
#include <QWidget>
#include <QLineEdit>
#include <QFormLayout>
#include <QFileDialog>
#include <QPushButton>
#include <QTextStream>


/**
 * 对话窗口类定义
 */
class Dialog : public QDialog
{
    Q_OBJECT

  public:
    Dialog( QWidget *parent = NULL );
   ~Dialog();

  private slots:
    void choose_file1();
    void choose_file2();
    void process();

  private:
    void _get_data ( QFile *file1, QFile *file2, QList<QString> *dataOfFile1, QList<QString> *dataOfFile2 );
    void _backup   ( QFile *file1, QFile *file2                                                           );
    void _delt_dupl(                             QList<QString> *dataOfFile1, QList<QString> *dataOfFile2 );
    void _update   ( QFile *file1, QFile *file2, QList<QString> *dataOfFile1, QList<QString> *dataOfFile2 );

  private:
    QLineEdit   *lineEditOfFile1Path;
    QLineEdit   *lineEditOfFile2Path;

    QPushButton *buttonOfChooseFile1;
    QPushButton *buttonOfChooseFile2;
    QPushButton *buttonOfProcess;

};

#endif
