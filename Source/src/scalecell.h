#ifndef CSCALECELL_H
#define CSCALECELL_H

#include <qwidget.h>
#include <qpainter.h>
class CScaleCell : public QWidget
{
    Q_OBJECT
public:
    CScaleCell(QWidget* parent = nullptr);
    void setScaleFactor(int factor);
    double getPixelPerFrame();

protected:
    void paintEvent(QPaintEvent* pEvent);
    void mouseReleaseEvent(QMouseEvent* pEvent);

private:
    QString calcTimeStr(int scale) const;
    int getScaleFactor() const;

signals:
    void sigScaleClick();

private:
    int m_factor;
    mutable int m_scaleCount;
};

#endif // CSCALECELL_H
