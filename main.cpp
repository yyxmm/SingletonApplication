#include "MainWindow.h"
#include <QApplication>
#include <QSharedMemory>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QSharedMemory *shareMem = new QSharedMemory(QString("SingleInstanceIdentify"));

    /* if the sharedmemory has not been created, it returns false, otherwise true.
         * But if the application exit unexpectedly, the sharedmemory will not detach.
         * So, we try twice.
         */
    volatile short i = 2;
    while (i--)
    {
        if (shareMem->attach(QSharedMemory::ReadOnly)) /* no need to lock, bcs it's read only */
        {
            shareMem->detach();
        }
    }

    int result = 0;
    if (shareMem->create(1))
    {
        MainWindow w;
        w.show();
        result = a.exec();

        if (shareMem->isAttached())
            shareMem->detach();
        delete shareMem;
    }
    else
    {
        QMessageBox::information(nullptr, "提示", "程序已经在运行。");
    }

    return result;
}
