#ifndef OPENGLRENDER_H
#define OPENGLRENDER_H


/*
 *class: opengl渲染类 yuv视频
 *auth: df
 *time: 2020/06/16
 *
 */

#include <QQuickItem>
#include <QOpenGLFunctions>
#include <QOpenGLWindow>
#include <QOpenGLShaderProgram>
#include <QSharedPointer>
#include <QOpenGLTexture>
#include <QVector>
#include <QMatrix4x4>
#include <QQuickFramebufferObject>
#include <QQuickWindow>
#include <QOpenGLFramebufferObjectFormat>
#include <QQuickWindow>
#include <atomic>


class FrameRender : public QOpenGLFunctions
{

public:
    static int GetYUV420PSize(int w,int h);
    FrameRender();
    ~FrameRender();
    void init();
    void paint();
    void resize(int width, int height);
    void updateTextureInfo(int width, int height, int format);
    void updateTextureData(char *data);
protected:
    void initTexture();
    void initShader();
    void initGeometry();
private:

    QOpenGLShaderProgram mProgram;
    QOpenGLTexture *mTexY = nullptr;
    QOpenGLTexture *mTexU = nullptr;
    QOpenGLTexture *mTexV = nullptr;
    QVector<QVector3D> mVertices;
    QVector<QVector2D> mTexcoords;
    int mModelMatHandle, mViewMatHandle, mProjectMatHandle;
    int mVerticesHandle;
    int mTexCoordHandle;

    QMatrix4x4 mModelMatrix;
    QMatrix4x4 mViewMatrix;
    QMatrix4x4 mProjectionMatrix;
    GLint mPixFmt = 0;
    bool mTextureAlloced = false;

    int m_w = 0;
    int m_h = 0;
};

#endif // OPENGLRENDER_H
