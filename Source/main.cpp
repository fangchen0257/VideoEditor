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
#include "src/EUType.h"
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

class Application : public QApplication
{
public:

    Application(int &argc, char **argv)
        : QApplication(argc, argv)
    {
        QDir dir(applicationDirPath());
        dir.cd("lib");
        dir.cd("qt5");
        addLibraryPath(dir.absolutePath());
        setOrganizationName("Easeus");
        setApplicationName("EuVideoEdit");
        setApplicationVersion("1.0");
        setAttribute(Qt::AA_UseHighDpiPixmaps);
        setAttribute(Qt::AA_DontCreateNativeWidgetSiblings);

        QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

        dir = applicationDirPath();
        if (!playerGPU && drawMethod == Qt::AA_UseSoftwareOpenGL)
        {
            if (QFile::exists(dir.filePath("opengl32sw.dll")))
            {
                QFile::rename(dir.filePath("opengl32sw.dll"), dir.filePath("opengl32.dll"));
            }
        }
        else if (QFile::exists(dir.filePath("opengl32.dll")))
        {
            QFile::remove(dir.filePath("opengl32sw.dll"));
            QFile::rename(dir.filePath("opengl32.dll"), dir.filePath("opengl32sw.dll"));
        }

        if (playerGPU)
        {
            QCoreApplication::setAttribute(Qt::AA_UseDesktopOpenGL);
        }
        else if (drawMethod >= Qt::AA_UseDesktopOpenGL &&
                 drawMethod <= Qt::AA_UseSoftwareOpenGL)
        {
            QCoreApplication::setAttribute(Qt::ApplicationAttribute(drawMethod));
        }

        QTranslator* pTrans = new QTranslator;
        if (pTrans)
        {
            pTrans->load("../Resource/QtXVideoEdit_zh_CN.qm");
            installTranslator(pTrans);
        }

        QString strStyleSheet = QString("*{font-size: 14px; font-family:Segoe UI;}\r\n");

        QFile file(":/res/StyleSheet.css");
        if (file.open(QIODevice::ReadOnly|QIODevice::Text))
        {
            QByteArray strStyle = file.readAll();
            strStyleSheet += strStyle;
        }
        setStyleSheet(strStyleSheet);

        Mlt::Factory::init();
        setProfile(profile, kDefaultMltProfile);
    }

    ~Application()
    {
    }
};

int main(int argc, char *argv[])
{
    Application app(argc, argv);

    CMainWin mainw;
    mainw.setFixedSize(1280,768);
    mainw.show();

    return app.exec();
}
