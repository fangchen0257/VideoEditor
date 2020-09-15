#include "effectview.h"
#include <qheaderview.h>
#include <qfile.h>
#include <qscrollbar.h>
#include "QtLib/ImgButton.h"
#include <qlabel.h>
#include <qvariant.h>
#include <qurl.h>
#include <qimage.h>
#include "qmltypesregister.h"

CEffectView::CEffectView(QWidget* parent)
    :QTableWidget(parent)
    ,m_pEffectHeader(nullptr)
    ,m_pHboxTrackVideo(nullptr)
    ,m_pItemShadow(nullptr)
    ,m_operColumnWidth(0)
{
    Layout();
}

CEffectView::CEffectView(int row, int column, QWidget *parent)
    :QTableWidget (row, column, parent)
{
    Layout();
}

void CEffectView::showEvent(QShowEvent *pEvent)
{
    m_operColumnWidth = columnWidth(COL_OPERA_REGION);
    qDebug() << "m_operColumnWidth : " << m_operColumnWidth;

    QTableWidget::showEvent(pEvent);
}

void CEffectView::Layout()
{
    do
    {
        m_pEffectHeader = new CEffectHorizonHeader(this);
        if (nullptr == m_pEffectHeader) break;
        setHorizontalHeader(m_pEffectHeader);
        setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
        setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);

        verticalHeader()->setVisible(false);
        horizontalHeader()->setStretchLastSection(true);
        setColumnWidth(COL_EFFECT_NAME, 150);

        QVector<int> vecRowHeight;
        vecRowHeight<<65<<65<<35<<35<<35<<35<<35;
        for (int i=0; i<ROW_CNT; i++)
        {
            setRowHeight(i,vecRowHeight[i]);
        }

        setFocusPolicy(Qt::NoFocus);
        setSelectionMode(QAbstractItemView::NoSelection);
        setShowGrid(false);
        setAlternatingRowColors(true);
        setStyleSheet("QTableWidget {border:0px; background-color: #212735; alternate-background-color: #252b39;}");

        InitFirstColumn();
        InitTrackContainer();
        InitShadowItem();
    } while(0);
}

void CEffectView::InitFirstColumn()
{
    do
    {
        QString strIcon = ":/res/video_icon.png";
        QString strText = QObject::tr("Video");
        QVector<QString> vecResLock;
        vecResLock << ":/res/unlock.png";
        vecResLock << ":/res/unlock_hover.png";
        vecResLock << ":/res/unlock_clicked.png";
        vecResLock << ":/res/unlock_disable.png";

        QVector<QString> vecVisible;
        vecVisible<<":/res/layer_visible_on.png";
        vecVisible<<":/res/layer_visible_on_hover.png";
        vecVisible<<":/res/layer_visible_on_pressed.png";
        vecVisible<<":/res/layer_visible_on.png_disable";

        QVector<QString> vecMute;
        vecMute<<":/res/audio_volume_high.png";
        vecMute<<":/res/audio_volume_high_hover.png";
        vecMute<<":/res/audio_volume_high_pressed.png";
        vecMute<<":/res/audio_volume_high_disable.png";
        CBox* pBoxVideo = InitCellItem(strIcon, strText, vecResLock, vecVisible, vecMute);
        if (nullptr == pBoxVideo) break;
        pBoxVideo->SetMargins(18,12,8,12);
        setCellWidget(0,0, pBoxVideo);

        strIcon=":/res/pip_icon.png"; strText=QObject::tr("PIP");
        CBox* pBoxPicInPic = InitCellItem(strIcon, strText, vecResLock, vecVisible, vecMute);
        if (nullptr == pBoxPicInPic) break;
        pBoxPicInPic->SetMargins(18,12,8,12);
        setCellWidget(1,0,pBoxPicInPic);

        strIcon=":/res/filter_icon.png"; strText=QObject::tr("Filter");
        CBox* pBoxFilter = InitCellItem(strIcon, strText, vecResLock, vecVisible, QVector<QString>());
        if (nullptr == pBoxFilter) break;
        pBoxFilter->SetMargins(18,8,8,8);
        setCellWidget(2,0,pBoxFilter);

        strIcon=":/res/text_icon.png"; strText=QObject::tr("Text");
        CBox* pBoxText = InitCellItem(strIcon, strText, vecResLock, vecVisible, QVector<QString>());
        if (nullptr == pBoxText) break;
        pBoxText->SetMargins(18,8,8,8);
        setCellWidget(3,0,pBoxText);

        strIcon=":/res/music_icon.png"; strText=QObject::tr("Music");
        CBox* pBoxMusic = InitCellItem(strIcon, strText, vecResLock, QVector<QString>(), vecMute);
        if (nullptr == pBoxMusic) break;
        pBoxMusic->SetMargins(18,8,8,8);
        setCellWidget(4,0,pBoxMusic);
    } while(0);
}

CBox *CEffectView::InitCellItem(QString strIcon, QString strText, QVector<QString> vecResLock, QVector<QString> vecVisible, QVector<QString> vecMute)
{
    CBox* pCell = nullptr;
    do
    {
        pCell = new CBox(false);
        if (nullptr == pCell) break;
        pCell->SetSpacing(8);

        CImagButton* pImgVideo = new CImagButton(strIcon);
        if (nullptr == pImgVideo) break;
        pCell->PackStart(pImgVideo,false,0,Qt::AlignLeft|Qt::AlignVCenter);

        QLabel* pLabText = new QLabel(strText);
        if (nullptr == pLabText) break;
        pLabText->setStyleSheet("QLabel{font-size:16px; color:#aeb9d2;font-family:Segoe UI Semibold;font-weight:bold;}");
        pCell->PackStart(pLabText,false,0,Qt::AlignLeft|Qt::AlignVCenter);
        pCell->AddSpacerItem(true);

        CImagButton* pImgLock = new CImagButton(vecResLock, OPER_LOCK);
        if (nullptr == pImgLock) break;
        pCell->PackStart(pImgLock,false,4,Qt::AlignLeft|Qt::AlignVCenter);

        CBox* pVboxVisibleMute = new CBox(true);
        if (nullptr == pVboxVisibleMute) break;
        if (!vecVisible.isEmpty())
        {
            CImagButton* pImgVisible = new CImagButton(vecVisible, OPER_VISIBLE);
            if (nullptr == pImgVisible) break;
            pVboxVisibleMute->PackStart(pImgVisible);
            pVboxVisibleMute->AddSpacing(!vecMute.isEmpty()?16:0);
        }

        if (!vecMute.isEmpty())
        {
            CImagButton* pImgMute = new CImagButton(vecMute, OPER_MUTE);
            if (nullptr == pImgMute) break;
            pVboxVisibleMute->PackStart(pImgMute);
        }

        pCell->PackStart(pVboxVisibleMute);
    } while(0);

    return pCell;
}

void CEffectView::InitTrackContainer()
{
    do
    {
        m_pHboxTrackVideo = new CBox(false);
        if (nullptr == m_pHboxTrackVideo) break;
        m_pHboxTrackVideo->SetMargins(0,5,0,0);
        setCellWidget(ROW_VIDEO,1,m_pHboxTrackVideo);
    } while(0);
}

void CEffectView::InitShadowItem()
{
    m_pItemShadow = new CItemShadow(QColor(0,0,0,127), m_pHboxTrackVideo);
    if (nullptr != m_pItemShadow)
    {
        m_pItemShadow->setVisible(false);
    }
}

void CEffectView::AppendClip(int type, QString strText, QImage imgThumb, shared_ptr<Mlt::ClipInfo> clipInfo)
{
    do
    {
        if (nullptr==m_pHboxTrackVideo || nullptr==m_pEffectHeader) break;

        CTrackItem* pTrackItem = new CTrackItem(TRACK_TYPE(type), strText, imgThumb, clipInfo, m_pHboxTrackVideo);
        if (nullptr == pTrackItem) break;
        pTrackItem->SetShadowItem(m_pItemShadow);
        connect(pTrackItem, SIGNAL(sigItemSelect(CTrackItem*)), this, SLOT(slotTrackItemSelect(CTrackItem*)));
        connect(pTrackItem, SIGNAL(sigClipTrim(CTrackItem*,int,int)), this, SLOT(slotClipTrim(CTrackItem*,int,int)));
        connect(pTrackItem, SIGNAL(sigMoveClip(CTrackItem*,int,int)), this, SLOT(slotClipMove(CTrackItem*,int,int)));
        connect(pTrackItem, SIGNAL(sigItemDelete(CTrackItem*)), this, SLOT(slotItemDelete(CTrackItem*)));

        double pixelPerFrame = m_pEffectHeader->getPixelPerFrame();
        pTrackItem->ResetPixelPerFrame(pixelPerFrame);
        int positon = clipInfo->start*pixelPerFrame;
        pTrackItem->move(positon, 5);
        pTrackItem->show();

        m_vecTrackItems.push_back(pTrackItem);
        SelectItem(pTrackItem);
    } while(0);
}

QString CEffectView::trackItemText(QString strMediaPath)
{
    strMediaPath = strMediaPath.right(strMediaPath.length() - strMediaPath.lastIndexOf("/")-1);
    return strMediaPath;
}

void CEffectView::SelectItem(CTrackItem *pCurItem)
{
    for (int i=0,iEnd=m_vecTrackItems.size(); i<iEnd; ++i)
    {
        CTrackItem* pItem = m_vecTrackItems[i];
        if (nullptr == pItem) continue;

        if (pItem == pCurItem)
        {
            pItem->SetSelect(true);
        }
        else
        {
            pItem->stackUnder(pCurItem);
            pItem->SetSelect(false);
        }
    }
}

void CEffectView::RefreshTrackItems(int type, shared_ptr<CEUMainVideoTrack> pMainTrack)
{
    do
    {
        DeleteTrackItems(m_vecTrackItems);
        shared_ptr<Mlt::Playlist> playList = pMainTrack->playlist();
        if (nullptr == playList) break;

        for (int i=0,iEnd=playList->count(); i<iEnd; ++i)
        {
            shared_ptr<Mlt::ClipInfo> clipInfo(playList->clip_info(i));

            qDebug() << clipInfo->clip;
            qDebug() << clipInfo->start;
            qDebug() << clipInfo->frame_in;
            qDebug() << clipInfo->frame_out;
            qDebug() << clipInfo->frame_count;
            qDebug() << clipInfo->length;

            QImage imgThumb = pMainTrack->clip(i)->image(IMG_THUMB_W, IMG_THUMB_H);
            std::string strXml = pMainTrack->clip(i)->xml();
            qDebug() << "media path" << strXml.c_str();

            AppendClip(type, trackItemText(strXml.c_str()), imgThumb, clipInfo);
        }
    } while(0);
}

void CEffectView::DeleteTrackItems(QVector<CTrackItem *> &vecTrackItems)
{
    for (int i=0, iEnd=vecTrackItems.size(); i<iEnd; ++i)
    {
        CTrackItem* pItem = m_vecTrackItems[i];
        if (nullptr != pItem)
        {
            pItem->deleteLater();
            pItem = nullptr;
        }
    }

    m_vecTrackItems.clear();
}

void CEffectView::ResetColumnWidth()
{
    int itemTotalWidth = 0;
    for (int i=0,iEnd=m_vecTrackItems.size(); i<iEnd; ++i)
    {
        CTrackItem* pItem = m_vecTrackItems[i];
        if (nullptr != pItem)
        {
            itemTotalWidth += pItem->width();
        }
    }
    qDebug() << "clip total width:" << itemTotalWidth;

    if (itemTotalWidth <= m_operColumnWidth)
    {
        itemTotalWidth = m_operColumnWidth;
    }
    setColumnWidth(COL_OPERA_REGION, itemTotalWidth);
    emit sigColumnWidthChanged(itemTotalWidth);
}

void CEffectView::slotAddMedia2Track(int type, const QVariant &media)
{
    do
    {
        GlobalUtinityObject* pGlobalObj = QmlTypesRegister::instance().UtinityObject();
        if (nullptr == pGlobalObj) break;

        shared_ptr<CEUMainVideoTrack> pMainTrack = pGlobalObj->GetTrackor().mainVideoTrack();
        if (nullptr == pMainTrack) break;

        QString mediaPath = media.toUrl().path();
        int pos = mediaPath.indexOf(":");
        if(-1 != pos){
            mediaPath = mediaPath.mid(pos - 1);
        }

        qDebug() << "media path:" << mediaPath;
        if (!pMainTrack->appendClip(mediaPath.toUtf8().constData())) break;

        int clipIndex = pMainTrack->playlist()->count()-1;
        shared_ptr<Mlt::ClipInfo> clipInfo(pMainTrack->playlist()->clip_info(clipIndex));

        qDebug() << "clip index:" << clipInfo->clip;
        qDebug() << "start:" << clipInfo->start;
        qDebug() << "frame_in:" << clipInfo->frame_in;
        qDebug() << "frame_out:" << clipInfo->frame_out;
        qDebug() << "frame_count:" << clipInfo->frame_count;
        qDebug() << "frame_length:" << clipInfo->length;

        QImage imgThumb = pMainTrack->clip(clipIndex)->image(IMG_THUMB_W, IMG_THUMB_H);
        AppendClip(type, trackItemText(mediaPath), imgThumb, clipInfo);
        ResetColumnWidth();
    } while(0);
}

void CEffectView::slotTrackItemSelect(CTrackItem *pItem)
{
    do
    {
        Q_UNUSED(pItem);

        GlobalUtinityObject* pGlobalObj = QmlTypesRegister::instance().UtinityObject();
        if (nullptr == pGlobalObj) break;

        CEUTractor trackor =  pGlobalObj->GetTrackor();
        shared_ptr<Mlt::Producer> pProducer = trackor.producer();
        if (nullptr == pProducer) break;

        pGlobalObj->GetMltCtrl().EUSetProducer(pProducer);
        pGlobalObj->GetMltCtrl().EUSeekToPos(0);
        pGlobalObj->GetMltCtrl().EUPause();
        SelectItem(pItem);
    } while(0);
}

void CEffectView::slotScaleValueChanged(int value)
{
    if (nullptr == m_pEffectHeader) return;

    m_pEffectHeader->setScaleFactor(value);
    double pixePerFrame = m_pEffectHeader->getPixelPerFrame();
    qDebug() << "value" << value << "pixePerFrame : " << pixePerFrame;

    for (int i=0,iEnd=m_vecTrackItems.size(); i<iEnd; ++i)
    {
        CTrackItem* pItem = m_vecTrackItems[i];
        if (nullptr == pItem) continue;

        pItem->ResetPixelPerFrame(pixePerFrame);
    }

    ResetColumnWidth();
}

void CEffectView::slotClipTrim(CTrackItem* pItem, int in, int out)
{
    do
    {
        if (nullptr == pItem) break;

        int type = pItem->type();
        shared_ptr<Mlt::ClipInfo> clipInfo = pItem->GetClipInfo();
        if (nullptr == clipInfo) break;

        GlobalUtinityObject* pGlobalObj = QmlTypesRegister::instance().UtinityObject();
        if (nullptr == pGlobalObj) break;

        shared_ptr<CEUMainVideoTrack> pMainTrack = pGlobalObj->GetTrackor().mainVideoTrack();
        if (nullptr == pMainTrack) break;

        int clipIndex = clipInfo->clip;
        pMainTrack->trimClipIn(clipIndex, in);
        pMainTrack->trimClipOut(clipIndex, out);

        //重新初始化所有剪辑
        RefreshTrackItems(type, pMainTrack);
    } while(0);
}

void CEffectView::slotClipMove(CTrackItem* pItem, int clipIndex, int position)
{
    do
    {
        if (nullptr == pItem) break;

        GlobalUtinityObject* pGlobalObj = QmlTypesRegister::instance().UtinityObject();
        if (nullptr == pGlobalObj) break;

        shared_ptr<CEUMainVideoTrack> pMainTrack = pGlobalObj->GetTrackor().mainVideoTrack();
        if (nullptr == pMainTrack) break;

        pMainTrack->moveClip(clipIndex, position);
        qDebug()<<"ClipIndex:"<<clipIndex<<"pos:"<<position;

        RefreshTrackItems(pItem->type(), pMainTrack);
    } while(0);
}

void CEffectView::slotItemDelete(CTrackItem *pItem)
{
    do
    {
        if (nullptr == pItem) break;

        shared_ptr<Mlt::ClipInfo> clipInfo = pItem->GetClipInfo();
        if (nullptr == clipInfo) break;

        GlobalUtinityObject* pGlobalObj = QmlTypesRegister::instance().UtinityObject();
        if (nullptr == pGlobalObj) break;

        shared_ptr<CEUMainVideoTrack> pMainTrack = pGlobalObj->GetTrackor().mainVideoTrack();
        if (nullptr == pMainTrack) break;

        pMainTrack->removeClip(clipInfo->clip);
        RefreshTrackItems(pItem->type(), pMainTrack);

        ResetColumnWidth();
    } while(0);
}
