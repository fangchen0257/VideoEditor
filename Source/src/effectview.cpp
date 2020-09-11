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
{
    Layout();
}

CEffectView::CEffectView(int row, int column, QWidget *parent)
    :QTableWidget (row, column, parent)
{
    Layout();
}

void CEffectView::Layout()
{
    do
    {
        m_pEffectHeader = new CEffectHorizonHeader;
        if (nullptr == m_pEffectHeader) break;
        setHorizontalHeader(m_pEffectHeader);

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

void CEffectView::AppendClip(int type, QString strText, shared_ptr<CEUProducer> pProducer)
{
    do
    {
        if (nullptr==m_pHboxTrackVideo || nullptr==m_pEffectHeader) break;

        CTrackItem* pTrackItem = new CTrackItem(TRACK_TYPE(type), strText, pProducer, m_pHboxTrackVideo);
        if (nullptr == pTrackItem) break;
        connect(pTrackItem, SIGNAL(sigItemSelect(CTrackItem*)), this, SLOT(slotTrackItemSelect(CTrackItem*)));
        connect(pTrackItem, SIGNAL(sigClipTrim(int,int)), this, SLOT(slotClipTrim(int,int)));

        double pixelPerFrame = m_pEffectHeader->getPixelPerFrame();
        pTrackItem->ResetPixelPerFrame(pixelPerFrame);
        pTrackItem->move(0,5);
        pTrackItem->show();
        m_vecTrackItems.push_back(pTrackItem);
    } while(0);
}

QString CEffectView::trackItemText(QString strMediaPath)
{
    strMediaPath = strMediaPath.right(strMediaPath.length() - strMediaPath.lastIndexOf("/")-1);
    return strMediaPath;
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
        shared_ptr<CEUProducer> pProducer = pMainTrack->clip(clipIndex);

        AppendClip(type, trackItemText(mediaPath), pProducer);
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
    } while(0);
}

void CEffectView::slotScaleValueChanged(int value)
{
    if (nullptr == m_pEffectHeader) return;
    m_pEffectHeader->setScaleFactor(value);

    for (int i=0,iEnd=m_vecTrackItems.size(); i<iEnd; ++i)
    {
        CTrackItem* pItem = m_vecTrackItems[i];
        if (nullptr == pItem) continue;
        pItem->ResetPixelPerFrame(m_pEffectHeader->getPixelPerFrame());
    }
}

void CEffectView::slotClipTrim(int in, int out)
{
    do
    {
#if 0
        GlobalUtinityObject* pGlobalObj = QmlTypesRegister::instance().UtinityObject();
        if (nullptr == pGlobalObj) break;

        shared_ptr<CEUMainVideoTrack> pMainTrack = pGlobalObj->GetTrackor().mainVideoTrack();
        if (nullptr == pMainTrack) break;

        pMainTrack->trimClipIn(0, in);
        pMainTrack->trimClipOut(0, out);

        int clipIndex = pMainTrack->playlist()->count()-1;
        shared_ptr<CEUProducer> pProducer = pMainTrack->clip(clipIndex);

        AppendClip(0, "1234567", pProducer);
#endif
    } while(0);
}
