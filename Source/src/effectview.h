#ifndef EFFECTVIEW_H
#define EFFECTVIEW_H

#include <QTableWidget>
#include "effecthorizonheader.h"
#include "QtLib/box.h"
#include "trackitem.h"

enum _TRACK_OPER_
{
    OPER_VISIBLE,
    OPER_MUTE,
    OPER_LOCK
};

enum _COLUMN_ITEMS_
{
    COL_EFFECT_NAME,
    COL_OPERA_REGION,
    COL_CNT
};

enum _ROW_ITEMS_
{
    ROW_VIDEO,
    ROW_PICINPIC,
    ROW_FILTERS,
    ROW_TEXT,
    ROW_MUSIC,
    ROW_TRANSITIONS,
    ROW_CNT
};

class CEffectView: public QTableWidget
{
    Q_OBJECT
public:
    CEffectView(QWidget* parent = nullptr);
    CEffectView(int row, int column, QWidget* parent = nullptr);

private:
    void Layout();
    void InitFirstColumn();
    CBox* InitCellItem(QString strIcon, QString strText, QVector<QString> vecResLock, QVector<QString> vecVisible, QVector<QString> vecMute);
    void InitTrackContainer();

private:
    void ResetTrack();

private:
    CEffectHorizonHeader* m_pEffectHeader;
    CBox* m_pHboxTrackVideo;
};

#endif // EFFECTVIEW_H
