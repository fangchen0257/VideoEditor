
#include "frameplayview.h"
#include "framelrender.h"
#include <QTimer>

 class FrameViewRender:public QQuickFramebufferObject::Renderer
 {
 public:
     FrameViewRender();
     void render() override;
     QOpenGLFramebufferObject *createFramebufferObject(const QSize &size) override;
     void synchronize(QQuickFramebufferObject *) override;
 private:
     FrameRender m_render;
     QQuickWindow *m_window = nullptr;
     int m_w = 0;
     int m_h = 0;
 };


 FrameViewRender::FrameViewRender()
 {
     m_render.init();
 }

 void FrameViewRender::render()
 {
     m_render.paint();
     if (m_window)
        m_window->resetOpenGLState();
 }

 QOpenGLFramebufferObject *FrameViewRender::createFramebufferObject(const QSize &size)
 {
     QOpenGLFramebufferObjectFormat format;
     format.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
     format.setSamples(4);
     m_render.resize(size.width(), size.height());
     return new QOpenGLFramebufferObject(size, format);
 }

 void FrameViewRender::synchronize(QQuickFramebufferObject *item)
 {
    FramePlayView *pv = qobject_cast<FramePlayView *>(item);
    if (!pv)
        return;

    int w = pv->getWidth();
    int h = pv->getHeight();

    if (w != m_w || h != m_h)
    {
        m_w = w;
        m_h = h;
        m_render.updateTextureInfo(w,h,0);
    }

    m_window = pv->window();
    m_render.updateTextureData(pv->getData());
 }

FramePlayView::FramePlayView(QQuickItem* parent)
    : QQuickFramebufferObject(parent)
{
    connect(this,&FramePlayView::frameGetted,this,&FramePlayView::update);
}

FramePlayView::~FramePlayView()
{

}

void FramePlayView::OnFrameGetted(unsigned char *data,int w,int h)
{
    m_w = w;
    m_h = h;
    int size = FrameRender::GetYUV420PSize(w,h);
    m_data.resize(size);
    memcpy(m_data.data(),data,size);
    emit frameGetted();
}

char *FramePlayView::getData()
{
    return m_data.data();
}

FramePlayView::Renderer *FramePlayView::createRenderer() const
{
    return new FrameViewRender();
}


