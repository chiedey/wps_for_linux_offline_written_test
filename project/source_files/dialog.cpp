/**
 * 头文件包含
 */
#include "./dialog.h"

/**
 * 对话窗口构造函数
 */
Dialog::Dialog( QWidget *parent ) : QDialog(parent)
{

    /*创建所需窗口部件*/
    lineEditOfFile1Path = new QLineEdit; //文件1地址显示框
    lineEditOfFile2Path = new QLineEdit; //文件2地址显示框

    buttonOfChooseFile1 = new QPushButton(tr("&choose file1")); //选择文件按钮1
    buttonOfChooseFile2 = new QPushButton(tr("&choose file2")); //选择文件按钮2
    buttonOfProcess     = new QPushButton(tr("&process")); //执行操作按钮

    /*设置信号和槽的连接*/
    connect( buttonOfChooseFile1, SIGNAL(clicked()), this, SLOT(choose_file1()) );
    connect( buttonOfChooseFile2, SIGNAL(clicked()), this, SLOT(choose_file2()) );
    connect( buttonOfProcess,     SIGNAL(clicked()), this, SLOT(process())      );

    /*创建布局*/
    QHBoxLayout *upLayout   = new QHBoxLayout; //第一行布局
    QHBoxLayout *midLayout  = new QHBoxLayout; //第二行布局
    QHBoxLayout *downLayout = new QHBoxLayout; //第三行布局
    QVBoxLayout *mainLayout = new QVBoxLayout; //总体布局

    /*把各种窗口部件添加到布局当中*/
    upLayout->addWidget(lineEditOfFile1Path);
    upLayout->addWidget(buttonOfChooseFile1);

    midLayout->addWidget(lineEditOfFile2Path);
    midLayout->addWidget(buttonOfChooseFile2);

    downLayout->addWidget(buttonOfProcess);

    /*把各部分布局添加到总体布局当中*/
    mainLayout->addLayout(upLayout);
    mainLayout->addLayout(midLayout);
    mainLayout->addLayout(downLayout);

    setLayout(mainLayout);

}

/**
 * 对话窗口析构函数
 */
Dialog::~Dialog()
{

    /*释放内存*/
    delete lineEditOfFile1Path;
    delete lineEditOfFile2Path;

    delete buttonOfChooseFile1;
    delete buttonOfChooseFile2;
    delete buttonOfProcess;

}

/**
 * 选择文件按钮1对应插槽
 */
void Dialog::choose_file1()
{

    QString initName = lineEditOfFile1Path->text();
    if( initName.isEmpty() ) {
        initName = QDir::homePath();
    }
    QString fileName = QFileDialog::getOpenFileName( this, tr("Choose File1"), initName );
    fileName = QDir::toNativeSeparators(fileName);
    if( !fileName.isEmpty() ) {
        lineEditOfFile1Path->setText(fileName);
    }

}

/**
 * 选择文件按钮2对应插槽
 */
void Dialog::choose_file2()
{

    QString initName = lineEditOfFile2Path->text();
    if( initName.isEmpty() ) {
        initName = QDir::homePath();
    }
    QString fileName = QFileDialog::getOpenFileName( this, tr("Choose File2"), initName );
    fileName = QDir::toNativeSeparators(fileName);
    if( !fileName.isEmpty() ) {
        lineEditOfFile2Path->setText(fileName);
    }

}

/**
 * 执行操作按钮对应插槽
 */
void Dialog::process()
{
    QList<QString> dataOfFile1, dataOfFile2;

    QFile file1(lineEditOfFile1Path->text());
    QFile file2(lineEditOfFile2Path->text());

    _get_data ( &file1, &file2, &dataOfFile1, &dataOfFile2 ); //获取文件数据
    _backup   ( &file1, &file2 );                             //备份原文件
    _delt_dupl( &dataOfFile1, &dataOfFile2 );                 //数据处理
    _update   ( &file1, &file2, &dataOfFile1, &dataOfFile2 ); //数据更新导入文件

}

/**
 * 获取数据内部函数
 */
void Dialog::_get_data( QFile *file1, QFile *file2, QList<QString> *dataOfFile1, QList<QString> *dataOfFile2 )
{

    if( !(*file1).open(QIODevice::ReadOnly) || !(*file2).open(QIODevice::ReadOnly) ) {
        return;
    }

    while(!(*file1).atEnd()) {
        *dataOfFile1 << (*file1).readLine();
    }
    while(!(*file2).atEnd()) {
        *dataOfFile2 << (*file2).readLine();
    }

    (*file1).close();
    (*file2).close();
}

/**
 * 备份数据内部函数
 */
void Dialog::_backup( QFile *file1, QFile *file2 )
{

    /*原文件重命名备份*/
    (*file1).rename(lineEditOfFile1Path->text(),lineEditOfFile1Path->text()+".bak");
    (*file2).rename(lineEditOfFile2Path->text(),lineEditOfFile2Path->text()+".bak");

}

/**
 * 删除两文件重复行内部函数
 */
void Dialog::_delt_dupl( QList<QString> *dataOfFile1, QList<QString> *dataOfFile2 )
{

    for( int i = 0; i < (*dataOfFile1).length(); i++ ) {
        for( int j = 0; j < (*dataOfFile2).length(); j++ ) {
            if( !(*dataOfFile1)[i].compare((*dataOfFile2)[j]) ) {
                (*dataOfFile1).removeAt(i);
                (*dataOfFile2).removeAt(j);
            }
        }
    }

}

/**
 * 文件内容更新内部函数
 */
void Dialog::_update( QFile *file1, QFile *file2, QList<QString> *dataOfFile1, QList<QString> *dataOfFile2 )
{

    /*以只写方式打开文件并检查打开是否成功*/
    if( !(*file1).open(QIODevice::WriteOnly) || !(*file2).open(QIODevice::WriteOnly) ) {
        return;
    }

    QTextStream updateFile1(file1);
    QTextStream updateFile2(file2);

    /*写入处理后的数据*/
    for( int i = 0; i < (*dataOfFile1).length(); i++ ) {
        updateFile1 << (*dataOfFile1)[i];
    }
    for( int j = 0; j < (*dataOfFile2).length(); j++ ) {
        updateFile2 << (*dataOfFile2)[j];
    }

    /*关闭文件*/
    (*file1).close();
    (*file2).close();
}
