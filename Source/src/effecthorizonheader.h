#ifndef EFFECTHORIZONHEADER_H
#define EFFECTHORIZONHEADER_H

#include <qheaderview.h>
class CEffectHorizonHeader : public QHeaderView
{
public:
    CEffectHorizonHeader(QWidget* parent = nullptr);

protected:
    virtual void paintSection(QPainter *painter, const QRect &rect, int logicalIndex) const;

private:
    void paintScale(QPainter *painter, const QRect &rect, int factor) const;
    QString calcTimeStr(int scale) const;

private:
    int m_factor;
};

#endif // EFFECTHORIZONHEADER_H
