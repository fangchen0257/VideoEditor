#ifndef EFFECTHORIZONHEADER_H
#define EFFECTHORIZONHEADER_H

#include <qheaderview.h>

enum _COLUMN_ITEMS_
{
    COL_EFFECT_NAME,
    COL_OPERA_REGION,
    COL_CNT
};

#define FIRST_COLUMN_WIDTH 150
class CEffectHorizonHeader : public QHeaderView
{
public:
    CEffectHorizonHeader(QWidget* parent = nullptr);
    void setScaleFactor(int factor);
    double getPixelPerFrame();

protected:
    virtual void paintSection(QPainter *painter, const QRect &rect, int logicalIndex) const;

private:
    void paintScale(QPainter *painter, const QRect &rect, int factor) const;
    QString calcTimeStr(int scale) const;
    int getScaleFactor() const;

private:
    QWidget* m_pParent;
    int m_factor;
    mutable int m_scaleCount;
};

#endif // EFFECTHORIZONHEADER_H
