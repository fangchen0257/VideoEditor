#ifndef EFFECTVIEW_H
#define EFFECTVIEW_H

#include <QTableWidget>
#include "effecthorizonheader.h"
#include "QtLib/box.h"
#include "trackitem.h"
#include "EUTractor.h"
#include "QtLib/public.h"

enum _TRACK_OPER_
{
    OPER_VISIBLE,
    OPER_MUTE,
    OPER_LOCK
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

protected:
    void showEvent(QShowEvent* pEvent);

private:
    void Layout();
    void InitFirstColumn();
    CBox* InitCellItem(QString strIcon, QString strText, QVector<QString> vecResLock, QVector<QString> vecVisible, QVector<QString> vecMute);
    void InitTrackContainer();
    void InitShadowItem();

private:
    void AppendClip(int type, QString strText, QImage imgThumb, shared_ptr<Mlt::ClipInfo> clipInfo);
    QString trackItemText(QString strMediaPath);
    void SelectItem(CTrackItem* pCurItem);
    void RefreshTrackItems(int type, shared_ptr<CEUMainVideoTrack> pMainTrack);
    void DeleteTrackItems(QVector<CTrackItem*>& vecTrackItems);
    void ResetColumnWidth();

signals:
    void sigColumnWidthChanged(int);

private slots:
    void slotAddMedia2Track(int type, const QVariant& media);
    void slotTrackItemSelect(CTrackItem* pItem);
    void slotScaleValueChanged(int value);
    void slotClipTrim(CTrackItem* pItem, int in,int out);
    void slotClipMove(CTrackItem* pItem, int clipIndex, int position);
    void slotItemDelete(CTrackItem* pItem);

private:
    CEffectHorizonHeader* m_pEffectHeader;
    CBox* m_pHboxTrackVideo;
    CItemShadow* m_pItemShadow;
    QVector<CTrackItem*> m_vecTrackItems;
    int m_operColumnWidth;
};

#endif // EFFECTVIEW_H
