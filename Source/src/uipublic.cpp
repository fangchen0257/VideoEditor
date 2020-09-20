#include "uipublic.h"

CUIPublic &CUIPublic::instance()
{
    static CUIPublic uipublic;
    return uipublic;
}

CUIPublic::~CUIPublic()
{

}

void CUIPublic::SetMainWin(QWidget *pMainWin)
{
    m_pMainWin = pMainWin;
}

QWidget *CUIPublic::GetMainWin()
{
    return m_pMainWin;
}

CUIPublic::CUIPublic()
{

}
