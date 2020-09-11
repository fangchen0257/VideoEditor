#ifndef GOLBALUTINITY_H
#define GOLBALUTINITY_H

/**
  全局的辅助类， 便于数据交互
 **/
#include <QObject>
#include <QRect>
#include <QPoint>

#include "EUMltCtl.h"
#include "EUTractor.h"

typedef enum _MEDIA_ITEM_TYPE_
{
    MEDIA_VIDEO,
    MEDIA_TRANSITION,
    MEDIA_PIP,
    MEDIA_FILTER,
    MEDIA_TEXT,
    MEDIA_MUSIC
};
class GlobalUtinityObject : public QObject
{
    Q_OBJECT

public:
    explicit GlobalUtinityObject(QObject *parent = nullptr);
    Q_INVOKABLE void setTimeCtrlRect(const int x, const int y, const int width, const int height);
    Q_INVOKABLE void openFile();
    Q_INVOKABLE void stopPlay();
    Q_INVOKABLE void seekToPos(int duration);
    Q_INVOKABLE void setPlay(bool play);
    Q_INVOKABLE void reset();
    Q_INVOKABLE void setVolume(int volume);
    Q_INVOKABLE void setPlayObject(QObject *obj);
    Q_INVOKABLE void focusInTimeEdit(const int x, const int y);
    Q_INVOKABLE void addVideoToPlayView(const QVariant& fileUrl);
    Q_INVOKABLE void addVideoToTrack(const QVariant& fileUrl);
    Q_INVOKABLE void scaleSliderValueChanged(int value);
    void UpdateDuration(int pos, int second);
    void ReleaseFrame(FrameData data);
    CMltCtl &GetMltCtrl();
    CEUTractor &GetTrackor();

signals:
    void timeEditFocusOut();
    void initVideoDuration(const int duration);
    void setVideoDuration(const int duration);
    void setVideoSecond(const int duration, int framenumber);
    void sigAddMedia2Track(int type, const QVariant& fileUrl);
    void sigScaleSliderValueChanged(int value);

private:
    QRect time_ctrl_rect_;
    CMltCtl m_MltCtrl;
    CEUTractor m_EUTractor;
    bool m_bPlaying = false;
    bool m_bStop = true;
};

#endif // GOLBALUTINITY_H
