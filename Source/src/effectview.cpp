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
    ,m_pSpliter(nullptr)
    ,m_pEffectHeader(nullptr)
    ,m_pCurrentItem(nullptr)
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
    if (nullptr != m_pSpliter)
    {
        int posX = FIRST_COLUMN_WIDTH-(m_pSpliter->width()/2);
        m_pSpliter->SetPosX(posX);
        m_pSpliter->move(posX, 0);
        m_pSpliter->setFixedHeight(height());
    }

    QTableWidget::showEvent(pEvent);
}

void CEffectView::resizeEvent(QResizeEvent *pEvent)
{
    if (nullptr != m_pSpliter)
    {
        bool bVisible = m_pSpliter->isVisible();
        ResetSpliter(bVisible);
    }

    QTableWidget::resizeEvent(pEvent);
}

void CEffectView::Layout()
{
    do
    {
        m_pEffectHeader = new CEffectHorizonHeader(this);
        if (nullptr == m_pEffectHeader) break;
        connect(m_pEffectHeader, &CEffectHorizonHeader::sectionClicked, this, &CEffectView::slotSectionClick);
        setHorizontalHeader(m_pEffectHeader);
        setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
        setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);

        verticalHeader()->setVisible(false);
        horizontalHeader()->setStretchLastSection(true);
        setColumnWidth(COL_EFFECT_NAME, FIRST_COLUMN_WIDTH);

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
        InitSpliter();
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
    for (int i; i<(ROW_CNT-1); i++)
    {
        CBox* pHboxContainer = new CBox(false);
        if (nullptr == pHboxContainer) break;

        pHboxContainer->SetMargins(0,5,0,0);
        setCellWidget(i,1,pHboxContainer);

        CItemShadow* pItemShadow = new CItemShadow(QColor(0,0,0,127), pHboxContainer);
        if (nullptr != pItemShadow)
        {
            pItemShadow->setVisible(false);
        }

        QPair<CBox*, CItemShadow*> pairContainer = qMakePair(pHboxContainer,pItemShadow);
        m_vecItemContainer.push_back(pairContainer);
    }
}

void CEffectView::InitSpliter()
{
    m_pSpliter = new CTrackSplit(this);
    if (nullptr != m_pSpliter)
    {
        connect(m_pSpliter, SIGNAL(sigSpliterMove(int,int)), this, SLOT(slotSpliterMove(int,int)));
        connect(m_pSpliter, SIGNAL(sigSpliterBtnClicked(int)), this, SLOT(slotSpliterBtnClicked(int)));
        m_pSpliter->setVisible(false);
    }
}

void CEffectView::AppendClip(int type, QString strText, QImage imgThumb, shared_ptr<Mlt::ClipInfo> clipInfo)
{
    do
    {
        CTrackItem* pTrackItem = new CTrackItem(TRACK_TYPE(type), strText, imgThumb, clipInfo, itemContainer(type));
        if (nullptr == pTrackItem) break;
        pTrackItem->SetShadowItem(itemShadow(type));
        connect(pTrackItem, SIGNAL(sigItemSelect(CTrackItem*)), this, SLOT(slotTrackItemSelect(CTrackItem*)));
        connect(pTrackItem, SIGNAL(sigClipTrim(CTrackItem*,int,int)), this, SLOT(slotClipTrim(CTrackItem*,int,int)));
        connect(pTrackItem, SIGNAL(sigMoveClip(CTrackItem*,int,int)), this, SLOT(slotClipMove(CTrackItem*,int,int)));
        connect(pTrackItem, SIGNAL(sigItemDelete(CTrackItem*)), this, SLOT(slotItemDelete(CTrackItem*)));

        double pixelPerFrame = GetPixelPerFrame();
        pTrackItem->ResetPixelPerFrame(pixelPerFrame);
        int positon = static_cast<int>(clipInfo->start*pixelPerFrame);
        pTrackItem->move(positon, 5);
        pTrackItem->show();

        addItem2Vector(type, pTrackItem);
        slotTrackItemSelect(pTrackItem);
    } while(0);
}

QString CEffectView::trackItemText(QString strMediaPath)
{
    strMediaPath = strMediaPath.right(strMediaPath.length() - strMediaPath.lastIndexOf("/")-1);
    return strMediaPath;
}

void CEffectView::SelectItem(CTrackItem *pCurItem)
{
    QVector<CTrackItem*> vecAllItems = getAllTrackItems();
    for (int i=0,iEnd=vecAllItems.size(); i<iEnd; ++i)
    {
        CTrackItem* pItem = vecAllItems[i];
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

void CEffectView::RefreshTrackItems(int type)
{
    switch (type)
    {
    case ROW_VIDEO:
        {
            DeleteTrackItems(m_vecTrackItems4Video);
            shared_ptr<Mlt::Playlist> playList = mainVideoTrack()->playlist();
            if (nullptr == playList) break;

            for (int i=0,iEnd=playList->count(); i<iEnd; ++i)
            {
                if (playList->is_blank(i)) continue;
                shared_ptr<Mlt::ClipInfo> clipInfo(playList->clip_info(i));

                qDebug() << clipInfo->clip;
                qDebug() << clipInfo->start;
                qDebug() << clipInfo->frame_in;
                qDebug() << clipInfo->frame_out;
                qDebug() << clipInfo->frame_count;
                qDebug() << clipInfo->length;

                QImage imgThumb = mainVideoTrack()->clip(i)->image(IMG_THUMB_W, IMG_THUMB_H);
                std::string strXml = mainVideoTrack()->clip(i)->xml();
                qDebug() << "media path" << strXml.c_str();

                AppendClip(type, trackItemText(strXml.c_str()), imgThumb, clipInfo);
            }
        }
        break;
    case ROW_PICINPIC:
        {
            DeleteTrackItems(m_vecTrackItems4PIP);
            shared_ptr<Mlt::Playlist> playList = subVideoTrack()->playlist();
            if (nullptr == playList) break;

            for (int i=0,iEnd=playList->count(); i<iEnd; ++i)
            {
                if (playList->is_blank(i)) continue;
                shared_ptr<Mlt::ClipInfo> clipInfo(playList->clip_info(i));

                qDebug() << clipInfo->clip;
                qDebug() << clipInfo->start;
                qDebug() << clipInfo->frame_in;
                qDebug() << clipInfo->frame_out;
                qDebug() << clipInfo->frame_count;
                qDebug() << clipInfo->length;

                QImage imgThumb = subVideoTrack()->clip(i)->image(IMG_THUMB_W, IMG_THUMB_H);
                std::string strXml = subVideoTrack()->clip(i)->xml();
                qDebug() << "media path" << strXml.c_str();

                AppendClip(type, trackItemText(strXml.c_str()), imgThumb, clipInfo);
            }
        }
        break;
    case ROW_FILTERS:
        break;
    case ROW_TEXT:
        break;
    case ROW_MUSIC:
        break;
    }
}

void CEffectView::DeleteTrackItems(QVector<CTrackItem *> &vecTrackItems)
{
    for (int i=0, iEnd=vecTrackItems.size(); i<iEnd; ++i)
    {
        CTrackItem* pItem = vecTrackItems[i];
        if (nullptr != pItem)
        {
            pItem->deleteLater();
            pItem = nullptr;
        }
    }

    m_pCurrentItem = nullptr;
    vecTrackItems.clear();
}

void CEffectView::ResetColumnWidth()
{
    int totalWidth = 0;
    int videoW = itemWidth(ROW_VIDEO);
    if (videoW > totalWidth) { totalWidth = videoW; }
    int pipW = itemWidth(ROW_PICINPIC);
    if (pipW > totalWidth) { totalWidth = pipW; }

    if (totalWidth <= m_operColumnWidth) {
        totalWidth = m_operColumnWidth;
    }
    setColumnWidth(COL_OPERA_REGION, totalWidth);
    emit sigColumnWidthChanged(totalWidth);
}

void CEffectView::ResetSpliter(bool bVisible)
{
    if (nullptr == m_pSpliter) return;

    m_pSpliter->setFixedHeight(height());
    m_pSpliter->setVisible(bVisible);
}

void CEffectView::ResetProducer(int type, int positon)
{
    do
    {
        GlobalUtinityObject* pGlobalObj = QmlTypesRegister::instance().UtinityObject();
        if (nullptr == pGlobalObj) break;

        CEUTractor trackor =  pGlobalObj->GetTrackor();
        shared_ptr<Mlt::Producer> pProducer = trackor.producer();
        if (nullptr == pProducer) break;

        if (ROW_VIDEO == type)
        {
            pGlobalObj->GetMltCtrl().EUSetProducer(pProducer);
        }

        pGlobalObj->IniViewDuration();
        pGlobalObj->seekToPos(positon);
    } while(0);
}

double CEffectView::GetPixelPerFrame()
{
    if (nullptr == m_pEffectHeader) return 0.0;
    return m_pEffectHeader->getPixelPerFrame();
}

void CEffectView::addItem2Vector(int type, CTrackItem *pItem)
{
    switch (type)
    {
    case ROW_VIDEO:
        m_vecTrackItems4Video.push_back(pItem);
        break;
    case ROW_PICINPIC:
        m_vecTrackItems4PIP.push_back(pItem);
        break;
    case ROW_FILTERS:
        break;
    case ROW_TEXT:
        break;
    case ROW_MUSIC:
        break;
    }
}

CBox *CEffectView::itemContainer(int type)
{
    if (m_vecItemContainer.size() != (ROW_CNT-1)) return nullptr;
    return m_vecItemContainer[type].first;
}

int CEffectView::itemWidth(int type)
{
    QVector<CTrackItem*> vecTrackItem;
    switch (type)
    {
    case ROW_VIDEO:
        vecTrackItem = m_vecTrackItems4Video;
        break;
    case ROW_PICINPIC:
        vecTrackItem = m_vecTrackItems4PIP;
        break;
    case ROW_FILTERS:
        break;
    case ROW_TEXT:
        break;
    case ROW_MUSIC:
        break;
    }

    int totalWidth = 0;
    for (int i=0,iEnd=vecTrackItem.size(); i<iEnd; ++i)
    {
        CTrackItem* pItem = vecTrackItem[i];
        if (nullptr != pItem)
        {
            totalWidth += pItem->width();
        }
    }

    return totalWidth;
}

QVector<CTrackItem *> CEffectView::getAllTrackItems()
{
    QVector<CTrackItem* > vecAllItems;
    vecAllItems += m_vecTrackItems4Video;
    vecAllItems += m_vecTrackItems4PIP;

    return vecAllItems;
}

int CEffectView::getSpliterMaximum()
{
    int spliterMaximum = 0;
    QVector<CTrackItem*> vecTracktems = getAllTrackItems();
    for (int i=0,iEnd=vecTracktems.size(); i<iEnd; ++i)
    {
        CTrackItem* pItem = vecTracktems[i];
        if (nullptr == pItem) continue;

        shared_ptr<Mlt::ClipInfo> clipInfo = pItem->GetClipInfo();

        int itemOutPosition = clipInfo->start+clipInfo->frame_out;
        if (spliterMaximum <= itemOutPosition)
        {
            spliterMaximum = itemOutPosition;
        }
    }

    spliterMaximum = static_cast<int>(spliterMaximum*GetPixelPerFrame());
    return spliterMaximum;
}

int CEffectView::getSpliterCurFrame()
{
    int curFrame = 0;
    do
    {
        if (nullptr == m_pSpliter) break;

        int curX = m_pSpliter->pos().x()-m_pSpliter->InitializePos();
        double pixPerFrame = GetPixelPerFrame();
        if (pixPerFrame > 0.0)
        {
            curFrame = static_cast<int>(curX/pixPerFrame);
        }
    } while(0);

    return curFrame;
}

bool CEffectView::isTrackProducer()
{
    bool bIsTrackProducer = false;
    do
    {
        GlobalUtinityObject* pGlobalObj = QmlTypesRegister::instance().UtinityObject();
        if (nullptr == pGlobalObj) break;

        CEUTractor trackor =  pGlobalObj->GetTrackor();
        shared_ptr<Mlt::Producer> pProducer = trackor.producer();
        bIsTrackProducer = (pProducer==pGlobalObj->GetMltCtrl().EuGetCurrentProducer());
    }while(0);

    return bIsTrackProducer;
}

shared_ptr<CEUMainVideoTrack> CEffectView::mainVideoTrack()
{
    GlobalUtinityObject* pGlobalObj = QmlTypesRegister::instance().UtinityObject();
    return pGlobalObj->GetTrackor().mainVideoTrack();
}

shared_ptr<CEUSubVideoTrack> CEffectView::subVideoTrack()
{
    GlobalUtinityObject* pGlobalObj = QmlTypesRegister::instance().UtinityObject();
    return pGlobalObj->GetTrackor().subVideoTrack();
}

CItemShadow *CEffectView::itemShadow(int type)
{
    if (m_vecItemContainer.size() != (ROW_CNT-1)) return  nullptr;
    return m_vecItemContainer[type].second;
}

void CEffectView::slotAddMedia2Track(int type, const QVariant &media)
{
    do
    {
        ResetSpliter(true);
        GlobalUtinityObject* pGlobalObj = QmlTypesRegister::instance().UtinityObject();
        if (nullptr == pGlobalObj) break;

        shared_ptr<CEUMainVideoTrack> pMainTrack = pGlobalObj->GetTrackor().mainVideoTrack();
        if (nullptr == pMainTrack) break;

        QString mediaPath = media.toUrl().path();
        int pos = mediaPath.indexOf(":");
        if(-1 != pos){
            mediaPath = mediaPath.mid(pos - 1);
        }

        qDebug() << "slotAddMedia2Track enter";
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

        qDebug() << "slotAddMedia2Track leave";
    } while(0);
}

void CEffectView::slotAddPIP2Track(int type, const QVariant &media)
{
    do
    {
        ResetSpliter(true);
        GlobalUtinityObject* pGlobalObj = QmlTypesRegister::instance().UtinityObject();
        if (nullptr == pGlobalObj) break;

        shared_ptr<CEUSubVideoTrack> pSubTrack = pGlobalObj->GetTrackor().subVideoTrack();
        if (nullptr == pSubTrack) break;

        QString mediaPath = media.toUrl().path();
        int pos = mediaPath.indexOf(":");
        if(-1 != pos){
            mediaPath = mediaPath.mid(pos - 1);
        }

        qDebug() << "slotAddPIP2Track enter";
        qDebug() << "media path:" << mediaPath;
        if (!pSubTrack->appendClip(mediaPath.toUtf8().constData())) break;

        int clipIndex = pSubTrack->playlist()->count()-1;
        shared_ptr<Mlt::ClipInfo> clipInfo(pSubTrack->playlist()->clip_info(clipIndex));

        qDebug() << "clip index:" << clipInfo->clip;
        qDebug() << "start:" << clipInfo->start;
        qDebug() << "frame_in:" << clipInfo->frame_in;
        qDebug() << "frame_out:" << clipInfo->frame_out;
        qDebug() << "frame_count:" << clipInfo->frame_count;
        qDebug() << "frame_length:" << clipInfo->length;

        QImage imgThumb = pSubTrack->clip(clipIndex)->image(IMG_THUMB_W, IMG_THUMB_H);
        AppendClip(type, trackItemText(mediaPath), imgThumb, clipInfo);
        ResetColumnWidth();

        qDebug() << "slotAddPIP2Track leave";
    } while(0);
}

void CEffectView::slotTrackItemSelect(CTrackItem *pItem)
{
    do
    {
        if (nullptr == pItem) break;

        ResetProducer(pItem->type(), pItem->GetClipInfo()->start);
        SelectItem(pItem);
        m_pCurrentItem = pItem;
    } while(0);
}

void CEffectView::slotScaleValueChanged(int value)
{
    if (nullptr == m_pEffectHeader) return;

    m_pEffectHeader->setScaleFactor(value);
    double pixePerFrame = m_pEffectHeader->getPixelPerFrame();
    qDebug() << "value" << value << "pixePerFrame : " << pixePerFrame;

    QVector<CTrackItem*> vecAllItems = getAllTrackItems();
    for (int i=0,iEnd=vecAllItems.size(); i<iEnd; ++i)
    {
        CTrackItem* pItem = vecAllItems[i];
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

        switch (type)
        {
        case ROW_VIDEO:
            {
                int clipIndex = clipInfo->clip;
                mainVideoTrack()->trimClipIn(clipIndex, in);
                mainVideoTrack()->trimClipOut(clipIndex, out);
            }
            break;
        case ROW_PICINPIC:
            {
                int clipIndex = clipInfo->clip;
                qDebug() << "clipindex:" << clipIndex << "type :" << type << "in:" << in << "out:" <<out;

                subVideoTrack()->trimClipIn(clipIndex, in);
                subVideoTrack()->trimClipOut(clipIndex, out);
            }
            break;
        }

        //重新初始化所有剪辑
        RefreshTrackItems(type);
    } while(0);
}

void CEffectView::slotClipMove(CTrackItem* pItem, int clipIndex, int position)
{
    do
    {
        if (nullptr == pItem) break;

        int type = pItem->type();
        switch (type)
        {
        case ROW_VIDEO:
            {
                mainVideoTrack()->moveClip(clipIndex, position);
                qDebug()<<"ClipIndex:"<<clipIndex<<"pos:"<<position;
            }
            break;
        case ROW_PICINPIC:
            {
                subVideoTrack()->moveClip(clipIndex, position);
                qDebug()<<"ClipIndex:"<<clipIndex<<"pos:"<<position;
            }
            break;
        }

        RefreshTrackItems(type);
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
        RefreshTrackItems(pItem->type());

        ResetColumnWidth();
    } while(0);
}

void CEffectView::slotSpliterMove(int x, int y)
{
    do
    {
        if (nullptr == m_pSpliter) break;

        int frame = 0;
        int totalW = getSpliterMaximum();
        if (x <= (totalW+m_pSpliter->InitializePos()))
        {
            m_pSpliter->move(x, y);

            double pixPerFrame = GetPixelPerFrame();
            GlobalUtinityObject* pGlobalObj = QmlTypesRegister::instance().UtinityObject();
            if (nullptr!=pGlobalObj && (pixPerFrame>0.0))
            {
                frame = static_cast<int>((x-m_pSpliter->InitializePos())/pixPerFrame);
                pGlobalObj->SetCurFrame(frame);
            }
        }

        if (nullptr != m_pCurrentItem)
        {
            ResetProducer(m_pCurrentItem->type(), frame);
        }
    } while(0);
}

void CEffectView::slotSpliterBtnClicked(int x)
{
    Q_UNUSED(x);
}

void CEffectView::slotVideoPoregressValueChanged(int value)
{
    if (nullptr == m_pSpliter) return;

    if (isTrackProducer())
    {
        double pixelPerFrame = GetPixelPerFrame();
        int pixel = static_cast<int>(pixelPerFrame*value);
        m_pSpliter->move(pixel+m_pSpliter->InitializePos(), 0);
    }
}

void CEffectView::slotSectionClick(int logicalIndex)
{
    if (COL_OPERA_REGION == logicalIndex)
    {
        QPoint ptCursor = QCursor::pos();
        QPoint ptCur = m_pEffectHeader->mapFromGlobal(ptCursor);
        if (nullptr != m_pSpliter)
        {
            slotSpliterMove(ptCur.x()-m_pSpliter->width()/2, 0);
        }
    }
}
