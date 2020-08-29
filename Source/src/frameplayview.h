#ifndef VIDEORENDERITEM_H
#define VIDEORENDERITEM_H

/*
 *func: 视频播放器yuv视频渲染组件
 *auth: df
 *time: 2020/06/16
 *
 */

#include <QQuickItem>
#include <QSharedPointer>
#include <QQuickWindow>
#include <QQuickFramebufferObject>
#include "framelrender.h"
#include <memory>
#include <QMutex>


class FramePlayView : public QQuickFramebufferObject
{
    Q_OBJECT
signals:
    void frameGetted();
public:

    FramePlayView(QQuickItem* parent = nullptr);
    ~FramePlayView() override;

    void OnFrameGetted(unsigned char *data,int w,int h);
    char *getData();

    int getWidth(){return m_w;}
    int getHeight(){return m_h;}

protected:
    virtual Renderer *createRenderer() const override;

private:
    QByteArray m_data;
    int m_w = 0;
    int m_h = 0;
};



#endif // VIDEORENDERITEM_H
