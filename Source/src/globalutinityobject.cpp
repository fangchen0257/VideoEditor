#include "globalutinityobject.h"

#include "frameplayview.h"

#include <QDebug>
#include <QFileDialog>
#include <QDir>
#include <QUrl>

static FramePlayView *g_player;
static int g_maxFrame;

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
    GlobalUtinityObject *obj = static_cast<GlobalUtinityObject *>(szUserInfo);

    mlt_position pos = timePos - 1;

    if (pos < 0)
        pos = 0;

    obj->UpdateDuration(frame.pos, pos);
    obj->ReleaseFrame(frame);
}

GlobalUtinityObject::GlobalUtinityObject(QObject *parent)
    : QObject(parent)
{
    m_MltCtrl.EUInit(this, FrameRecvFun);
    m_EUTractor.Init();
}

void GlobalUtinityObject::setTimeCtrlRect(const int x, const int y, const int width, const int height)
{
    time_ctrl_rect_.setRect(x, y, width, height);
}

void GlobalUtinityObject::focusInTimeEdit(const int x, const int y)
{
    if(!time_ctrl_rect_.contains(x, y))
    {
        emit timeEditFocusOut();
    }
}

void GlobalUtinityObject::addVideoToPlayView(const QVariant &fileUrl)
{

  auto videoPath = getRealVideoPath(fileUrl);
  m_MltCtrl.EUOpen(videoPath.toUtf8().data());
  ResetDuration();
  m_bPlaying = true;
  m_bStop = false;
  m_MltCtrl.EUPlay();
  QMetaObject::invokeMethod(g_player, "setPause", Q_ARG(QVariant, false));
}

void GlobalUtinityObject::addVideoToTrack(const QVariant &fileUrl)
{
    emit sigAddMedia2Track(MEDIA_VIDEO, fileUrl);
}

void GlobalUtinityObject::addPIPToTrack(const QVariant &fileUrl)
{
    emit sigAddPIP2Track(MEDIA_PIP, fileUrl);
}

void GlobalUtinityObject::scaleSliderValueChanged(int value)
{
    emit sigScaleSliderValueChanged(value);
}

void GlobalUtinityObject::videoProgressValueChanged(int value)
{
    emit sigVideoProgressValueChanged(value);
}

QString GlobalUtinityObject::getRealVideoPath(const QVariant &url)
{
    auto videoPath = url.toUrl().path();
    auto pos = videoPath.indexOf(":");
    if(-1 != pos){
        videoPath = videoPath.mid(pos - 1);
    }
    return videoPath;
}

QVariant GlobalUtinityObject::getVideoCoverImage(const QVariant& url)
{
    auto videoRealPath = getRealVideoPath(url);
    qDebug() << videoRealPath.toUtf8().data();
    return  "file:///C:/Users/dfei/Desktop/timg.jpg";

}

void GlobalUtinityObject::ResetDuration()
{
    int duration = m_MltCtrl.EUGetLength() - 1;
    double fps = m_MltCtrl.EUGetFps();
    int second = static_cast<int>(duration / fps);
    int frames = static_cast<int>(second * fps);
    g_maxFrame = duration;
    emit initVideoDuration(duration,second,duration - frames);
}

void GlobalUtinityObject::IniViewDuration()
{
      m_bStop = false;
      m_bPlaying = false;
      ResetDuration();
}

void GlobalUtinityObject::SwitchPlayState()
{
    if (!m_bStop)
    {
        setPlay(!m_bPlaying);
    }
}

void GlobalUtinityObject::SetCurFrame(const int frame)
{
    double fps = m_MltCtrl.EUGetFps();
    double frames = 0;
    int second = static_cast<int>(frame / fps);


    if (second >= 0)
    {
        frames = fps * second;
    }

    int f = static_cast<int>(frame - frames);

    emit setVideoDuration(frame);
    emit setVideoSecond(second, f);
}

void GlobalUtinityObject::UpdateDuration(int pos, int second)
{
    if (pos >= g_maxFrame)
    {
        QMetaObject::invokeMethod(g_player, "setPause", Q_ARG(QVariant, true));
        m_bPlaying = false;
        m_bStop = true;
    }

    SetCurFrame(pos);
}

void GlobalUtinityObject::ReleaseFrame(FrameData data)
{
    m_MltCtrl.ReleaseFrame(data);
}

CMltCtl &GlobalUtinityObject::GetMltCtrl()
{
    return m_MltCtrl;
}

CEUTractor &GlobalUtinityObject::GetTrackor()
{
    return m_EUTractor;
}

void GlobalUtinityObject::openFile()
{
    QString file = QFileDialog::getOpenFileName(nullptr,QObject::tr("select file"),QDir::currentPath(),"*.*");

    m_MltCtrl.EUOpen(file.toStdString().c_str());

    int duration = m_MltCtrl.EUGetLength() - 1;
    g_maxFrame = duration;
    emit initVideoDuration(duration,0,0);

    m_bPlaying = true;
    m_bStop = false;

    m_MltCtrl.EUPlay();
}

void GlobalUtinityObject::stopPlay()
{
    m_bPlaying = false;
    m_bStop = true;
    m_MltCtrl.EUStop();
}

void GlobalUtinityObject::seekToPos(int duration)
{
    m_MltCtrl.EUPause();
    QMetaObject::invokeMethod(g_player, "setPause", Q_ARG(QVariant, true));
    m_bPlaying = false;
    m_MltCtrl.EUSeekToPos(duration);
}

void GlobalUtinityObject::setPlay(bool play)
{
    if (m_bStop && play)
    {
        m_bStop = false;
        m_bPlaying = true;
        m_MltCtrl.EUSeekToPos(0);
        m_MltCtrl.EUPlay();
        QMetaObject::invokeMethod(g_player, "setPause", Q_ARG(QVariant, false));
    }
    else
    {
        m_bPlaying = play;
        if (m_bPlaying)
            m_MltCtrl.EUPlay();
        else
            m_MltCtrl.EUPause();
        QMetaObject::invokeMethod(g_player, "setPause", Q_ARG(QVariant, !m_bPlaying));
    }
}

void GlobalUtinityObject::reset()
{
    seekToPos(0);
}

void GlobalUtinityObject::setVolume(int volume)
{
    double v = volume/100.0;
    m_MltCtrl.EUSetVolume(v);
}

void GlobalUtinityObject::setPlayObject(QObject *obj)
{
    g_player = qobject_cast<FramePlayView *>(obj);
}
