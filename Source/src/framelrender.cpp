#include "framelrender.h"
#include <QDebug>

#include <QOpenGLFunctions>
#include <QQuickFramebufferObject>
#include <QOpenGLTexture>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <memory>

#include "frameplayview.h"

#ifndef ATTRIB_VERTEX
#define ATTRIB_VERTEX 0
#endif
#ifndef ATTRIB_TEXTURE
#define ATTRIB_TEXTURE 1
#endif


#include <QOpenGLPixelTransferOptions>
static void safeDeleteTexture(QOpenGLTexture *texture)
{
    if (texture)
    {
        if (texture->isBound())
        {
            texture->release();
        }
        if (texture->isCreated())
        {
            texture->destroy();
        }
        delete texture;
        texture = nullptr;
    }
}


int FrameRender::GetYUV420PSize(int w, int h)
{
    int y = w*h;
    int u = (w*h)/4;
    int v = u;
    return y + u +v;
}

FrameRender::FrameRender()
{

}

FrameRender::~FrameRender()
{
    destroyTexture();
}

void FrameRender::init()
{
    initializeOpenGLFunctions();
    glDepthMask(GL_TRUE);
    glEnable(GL_TEXTURE_2D);
    initShader();
    initTexture();
    initGeometry();
}

void FrameRender::paint()
{
    glDepthMask(true);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if (!mTextureAlloced)
    {
        return;
    }
    mProgram.bind();

    mModelMatHandle = mProgram.uniformLocation("u_modelMatrix");
    mViewMatHandle = mProgram.uniformLocation("u_viewMatrix");
    mProjectMatHandle = mProgram.uniformLocation("u_projectMatrix");
    mVerticesHandle = mProgram.attributeLocation("qt_Vertex");
    mTexCoordHandle = mProgram.attributeLocation("texCoord");
    //顶点
    mProgram.enableAttributeArray(mVerticesHandle);
    mProgram.setAttributeArray(mVerticesHandle, mVertices.constData());

    //纹理坐标
    mProgram.enableAttributeArray(mTexCoordHandle);
    mProgram.setAttributeArray(mTexCoordHandle, mTexcoords.constData());

    // MVP矩阵
    mProgram.setUniformValue(mModelMatHandle, mModelMatrix);
    mProgram.setUniformValue(mViewMatHandle, mViewMatrix);
    mProgram.setUniformValue(mProjectMatHandle, mProjectionMatrix);

    // pixFmt
    mProgram.setUniformValue("pixFmt", mPixFmt);

    //纹理
    // Y
    glActiveTexture(GL_TEXTURE0);
    mTexY->bind();

    // U
    glActiveTexture(GL_TEXTURE1);
    mTexU->bind();

    // V
    glActiveTexture(GL_TEXTURE2);
    mTexV->bind();

    mProgram.setUniformValue("tex_y", 0);
    mProgram.setUniformValue("tex_u", 1);
    mProgram.setUniformValue("tex_v", 2);

    glDrawArrays(GL_TRIANGLE_FAN, 0, mVertices.size());

    mProgram.disableAttributeArray(mVerticesHandle);
    mProgram.disableAttributeArray(mTexCoordHandle);
    mProgram.release();
}

void FrameRender::resize(int width, int height)
{
    glViewport(0, 0, width, height);
    float bottom = -1.0f;
    float top = 1.0f;
    float n = 1.0f;
    float f = 100.0f;
    mProjectionMatrix.setToIdentity();
    mProjectionMatrix.frustum(-1.0, 1.0, bottom, top, n, f);
}

void FrameRender::updateTextureInfo(int width, int height, int format)
{
    m_w = width;
    m_h = height;


    destroyTexture();
    initTexture();

    //yuv420p
    mTexY->setSize(width, height);
    mTexY->allocateStorage(QOpenGLTexture::Luminance, QOpenGLTexture::UInt8);

    mTexU->setSize(width / 2, height / 2);
    mTexU->allocateStorage(QOpenGLTexture::Luminance, QOpenGLTexture::UInt8);

    mTexV->setSize(width / 2, height / 2);
    mTexV->allocateStorage(QOpenGLTexture::Luminance, QOpenGLTexture::UInt8);

    mTextureAlloced = true;
}

void FrameRender::updateTextureData(char *data)
{
    if (data == nullptr)
        return;
    int height = m_h;
    int width = m_w;

    QOpenGLPixelTransferOptions options;
    options.setImageHeight(height);
    options.setRowLength(width);
    mTexY->setData(QOpenGLTexture::Luminance, QOpenGLTexture::UInt8,data, &options);
    options.setImageHeight(height/2);
    options.setRowLength(width/2);
    mTexU->setData(QOpenGLTexture::Luminance, QOpenGLTexture::UInt8,data+width*height, &options);
    mTexV->setData(QOpenGLTexture::Luminance, QOpenGLTexture::UInt8, data+width*height +width*height/4, &options);
}

void FrameRender::initTexture()
{
    // yuv420p
    mTexY = new QOpenGLTexture(QOpenGLTexture::Target2D);
    mTexY->setFormat(QOpenGLTexture::LuminanceFormat);
    //    mTexY->setFixedSamplePositions(false);
    mTexY->setMinificationFilter(QOpenGLTexture::Nearest);
    mTexY->setMagnificationFilter(QOpenGLTexture::Nearest);
    mTexY->setWrapMode(QOpenGLTexture::ClampToEdge);

    mTexU = new QOpenGLTexture(QOpenGLTexture::Target2D);
    mTexU->setFormat(QOpenGLTexture::LuminanceFormat);
    //    mTexU->setFixedSamplePositions(false);
    mTexU->setMinificationFilter(QOpenGLTexture::Nearest);
    mTexU->setMagnificationFilter(QOpenGLTexture::Nearest);
    mTexU->setWrapMode(QOpenGLTexture::ClampToEdge);

    mTexV = new QOpenGLTexture(QOpenGLTexture::Target2D);
    mTexV->setFormat(QOpenGLTexture::LuminanceFormat);
    //    mTexV->setFixedSamplePositions(false);
    mTexV->setMinificationFilter(QOpenGLTexture::Nearest);
    mTexV->setMagnificationFilter(QOpenGLTexture::Nearest);
    mTexV->setWrapMode(QOpenGLTexture::ClampToEdge);
}

void FrameRender::initShader()
{
    if (!mProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/vertex.vsh"))
    {
        qWarning() << " add vertex shader file failed.";
        return;
    }
    if (!mProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/fragment.fsh"))
    {
        qWarning() << " add fragment shader file failed.";
        return;
    }
    mProgram.bindAttributeLocation("qt_Vertex", 0);
    mProgram.bindAttributeLocation("texCoord", 1);
    mProgram.link();
    mProgram.bind();
}

void FrameRender::initGeometry()
{
    mVertices << QVector3D(-1, 1, 0.0f)
              << QVector3D(1, 1, 0.0f)
              << QVector3D(1, -1, 0.0f)
              << QVector3D(-1, -1, 0.0f);
    mTexcoords<< QVector2D(0, 1)
               << QVector2D(1, 1)
               << QVector2D(1, 0)
               << QVector2D(0, 0);

    mViewMatrix.setToIdentity();
    mViewMatrix.lookAt(QVector3D(0.0f, 0.0f, 1.001f), QVector3D(0.0f, 0.0f, -5.0f), QVector3D(0.0f, 1.0f, 0.0f));
    mModelMatrix.setToIdentity();
}

void FrameRender::destroyTexture()
{
    safeDeleteTexture(mTexY);
    safeDeleteTexture(mTexU);
    safeDeleteTexture(mTexV);
}
