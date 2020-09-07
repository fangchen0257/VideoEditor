#include <QApplication>
#include <QQmlApplicationEngine>
#include <QMutex>
#include <QByteArray>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QQmlContext>
#include <qtranslator.h>
#include <windows.h>
#include <qdir.h>

#include "src/qmltypesregister.h"
#include "src/mainwin.h"

void QtMessageLogHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    // 加锁
    static QMutex mutex;
    mutex.lock();

    QByteArray localMsg = msg.toLocal8Bit();

    QString message;
    switch(type)
    {
    case QtDebugMsg:
        message = QString("Debug: ");
        break;
    case QtWarningMsg:
        message = QString("Warning: ");
        break;
    case QtCriticalMsg:
        message = QString("Critical: ");
        break;
    case QtFatalMsg:
        message = QString("Fatal: ");
        break;
    case QtInfoMsg:
        message = QString("Info:");
        break;
    }

    // 设置输出信息格式
    QString strDateTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    QString strMessage = QString("[%5] %1 File: %2  Line: %3  Function: %4")
            .arg(localMsg.constData()).arg(context.file).arg(context.line).arg(context.function).arg(strDateTime);

    // 输出信息至文件中（读写、追加形式）
    QFile file("XVideoEdit.log");
    file.open(QIODevice::ReadWrite | QIODevice::Append);
    QTextStream stream(&file);
    stream << message << strMessage << "\r\n";
    file.flush();
    file.close();

    // 解锁
    mutex.unlock();
}

QString GetExecPath()
{
    wchar_t wzPath[MAX_PATH] = {0};
    ::GetModuleFileNameW(nullptr, wzPath, MAX_PATH);
    wchar_t* p = wcsrchr(wzPath, '\\');
    if (nullptr != p)
    {
        *p = '\0';
    }

    QString strExecPath = QString::fromWCharArray(wzPath);
    return strExecPath;
}

void SetCurExecPath()
{
    QString strPath = GetExecPath();

    const wchar_t* pPath = reinterpret_cast<const wchar_t*>(strPath.utf16());
    if (nullptr != pPath)
    {
        ::SetCurrentDirectoryW(pPath);
    }

    QDir::setCurrent(strPath);
    QCoreApplication::addLibraryPath(strPath);
}

void installTranslator(QApplication* pApp)
{
    do
    {
        if (nullptr == pApp) break;

        QTranslator* pTrans = new QTranslator;
        if (nullptr == pTrans) break;
        pTrans->load("../Resource/QtXVideoEdit_zh_CN.qm");
        pApp->installTranslator(pTrans);
    } while(0);
}

void resetStyleSheet(QApplication* pApp)
{
    do
    {
        if (nullptr == pApp) break;
        QString strStyleSheet = QString("*{font-size: 14px; font-family:Segoe UI;}\r\n");

        QFile file(":/res/StyleSheet.css");
        if (file.open(QIODevice::ReadOnly|QIODevice::Text))
        {
            QByteArray strStyle = file.readAll();
            strStyleSheet += strStyle;
        }
        pApp->setStyleSheet(strStyleSheet);
    } while(0);
}

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication app(argc, argv);

    if(app.testAttribute(Qt::AA_UseDesktopOpenGL))
    {
        app.setAttribute(Qt::AA_UseDesktopOpenGL);
    }
    SetCurExecPath();
    installTranslator(&app);
    resetStyleSheet(&app);

    CMainWin mainw;
    mainw.setFixedSize(1280,768);
    mainw.show();

    return app.exec();
}
