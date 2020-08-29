#include "globalutinityobject.h"

#include "frameplayview.h"

#include <QDebug>
#include <QFileDialog>
#include <QDir>
#include <QUrl>

static FramePlayView *g_player;

void FrameRecvFun( void *szUserInfo, FrameData frame, mlt_position timePos )
{
    static int w = 0;
    static int h = 0;

    if (g_player)
    {
        if (w != frame.width || h != frame.height)
        {
            w = frame.width;
            h = frame.height;
            QMetaObject::invokeMethod(g_player, "calcRenderSize", Q_ARG(QVariant, w), Q_ARG(QVariant, h));
        }

        g_player->OnFrameGetted(frame.image,frame.width,frame.height);
    }
    GolbalUtinityObject *obj = static_cast<GolbalUtinityObject *>(szUserInfo);
    emit obj->setVideoDuration(timePos);
    obj->ReleaseFrame(frame);
}

GolbalUtinityObject::GolbalUtinityObject(QObject *parent)
    : QObject(parent)
{
    m_MltCtrl.EUInit(this,FrameRecvFun,"");
}

void GolbalUtinityObject::setTimeCtrlRect(const int x, const int y, const int width, const int height)
{
    time_ctrl_rect_.setRect(x, y, width, height);
}

void GolbalUtinityObject::focusInTimeEdit(const int x, const int y)
{
    if(!time_ctrl_rect_.contains(x, y))
    {
        emit timeEditFocusOut();
    }
}

void GolbalUtinityObject::addVideoToPlayView(const QVariant &fileUrl)
{
  auto videoPath = fileUrl.toUrl().path();
  auto pos = videoPath.indexOf(":");
  if(-1 != pos){
      videoPath = videoPath.mid(pos - 1);
  }

  m_MltCtrl.EUOpen(videoPath.toUtf8().data());
  int duration = m_MltCtrl.EUGetDuration();
  emit initVideoDuration(duration);
  m_bPlaying = true;
  m_MltCtrl.EUPlay();
}

void GolbalUtinityObject::ReleaseFrame(FrameData data)
{
    m_MltCtrl.ReleaseFrame(data);
}

void GolbalUtinityObject::openFile()
{
    QString file = QFileDialog::getOpenFileName(nullptr,QObject::tr("select file"),QDir::currentPath(),"*.*");

    m_MltCtrl.EUOpen(file.toStdString().c_str());

    int duration = m_MltCtrl.EUGetDuration();
    emit initVideoDuration(duration);

    m_bPlaying = true;

    m_MltCtrl.EUPlay();
}

void GolbalUtinityObject::stopPlay()
{
    m_bPlaying = false;
    m_MltCtrl.EUStop();
}

void GolbalUtinityObject::seekToPos(int duration)
{
    m_MltCtrl.EUSeek(duration);
}

void GolbalUtinityObject::setPlay(bool play)
{

    if (m_bPlaying != play)
    {
        m_bPlaying = play;
        if (m_bPlaying)
            m_MltCtrl.EUPlay();
        else
            m_MltCtrl.EUPause();
    }
}

void GolbalUtinityObject::setPlayObject(QObject *obj)
{
    g_player = qobject_cast<FramePlayView *>(obj);
}
