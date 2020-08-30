#ifndef _IMAGE_BUTTON_H_
#define _IMAGE_BUTTON_H_

#include <qpushbutton.h>
typedef enum _BTN_STATUS_
{
    BTN_NORMAL,
    BTN_HOVER,
    BTN_PRESS,
    BTN_DISABLE,
    BTN_CNT
}BTN_STATUS;

class CImagButton : public QPushButton
{
    Q_OBJECT
public:
    CImagButton(QVector<QString> vecRes, int btnId = -1);
    void SetImgRes(QVector<QString> vecRes);

protected:
    void paintEvent(QPaintEvent* pEvent);
    void enterEvent(QEvent* pEvent);
    void leaveEvent(QEvent* pEvent);
    void mousePressEvent(QMouseEvent* pEvent);
    void mouseReleaseEvent(QMouseEvent* pEvent);

private:
    bool ptInRect(QPoint pt, QRect rect);

signals:
    void sigClick(int);

private:
    QVector<QString> m_vecImgRes;
    BTN_STATUS m_btnStatus;
    int m_bindId;
};

#endif
