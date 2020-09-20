#ifndef EFFECTVIEW_H
#define EFFECTVIEW_H

#include <QTableWidget>
#include "effecthorizonheader.h"
#include "QtLib/box.h"
#include "trackitem.h"
#include "EUTractor.h"
#include "QtLib/public.h"
#include "tracksplit.h"

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
    void resizeEvent(QResizeEvent* pEvent);

private:
    void Layout();
    void InitFirstColumn();
    CBox* InitCellItem(QString strIcon, QString strText, QVector<QString> vecResLock, QVector<QString> vecVisible, QVector<QString> vecMute);
    void InitTrackContainer();
    void InitSpliter();

private:
    CTrackItem* AppendClip(int type, QString strText, QImage imgThumb, shared_ptr<Mlt::ClipInfo> clipInfo);
    QString trackItemText(QString strMediaPath);
    void SelectItem(CTrackItem* pCurItem);
    void RefreshTrackItems(int type);
    void DeleteTrackItems(QVector<CTrackItem*>& vecTrackItems);
    void ResetColumnWidth();
    void ResetSpliter(bool bVisible);
    void ResetProducer(int type, int positon);
    double GetPixelPerFrame();

private:
    void addItem2Vector(int type, CTrackItem* pItem);
    CItemShadow* itemShadow(int type);
    CBox* itemContainer(int type);
    int itemWidth(int type);
    QVector<CTrackItem*> getAllTrackItems();
    int getSpliterMaximum();
    int getSpliterCurFrame();
    bool isTrackProducer();

private:
    shared_ptr<CEUMainVideoTrack> mainVideoTrack();
    shared_ptr<CEUSubVideoTrack> subVideoTrack();

signals:
    void sigColumnWidthChanged(int);

private slots:
    void slotAddMedia2Track(int type, const QVariant& media);
    void slotAddPIP2Track(int type,const QVariant& media);
    void slotTrackItemSelect(CTrackItem* pItem);
    void slotScaleValueChanged(int value);
    void slotClipTrim(CTrackItem* pItem, int in,int out);
    void slotClipMove(CTrackItem* pItem, int clipIndex, int position);
    void slotItemDelete(CTrackItem* pItem);
    void slotSpliterMove(int x, int y);
    void slotSpliterBtnClicked(int x);
    void slotVideoPoregressValueChanged(int value);
    void slotSectionClick(int logicalIndex);
    void slotHorizonBarChanged(int value);

private:
    CTrackSplit* m_pSpliter;
    CEffectHorizonHeader* m_pEffectHeader;
    CTrackItem* m_pCurrentItem;

    QVector<QPair<CBox*, CItemShadow*>> m_vecItemContainer;
    QVector<CTrackItem*> m_vecTrackItems4Video;
    QVector<CTrackItem*> m_vecTrackItems4PIP;
    int m_operColumnWidth;
};

#endif // EFFECTVIEW_H
