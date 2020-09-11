#ifndef QMLTYPESREGISTER_H
#define QMLTYPESREGISTER_H

#include <QObject>
#include <QQmlContext>
#include "globalutinityobject.h"

class QmlTypesRegister : public QObject
{
    Q_OBJECT
public:

    static QmlTypesRegister& instance();
    ~QmlTypesRegister();
    void RegisterCommonType();

    /*
     * func: 将C++类型设置到QML中作为全局变量
     *
     */
    void setGlobalCariable(QQmlContext* context);
    GlobalUtinityObject *UtinityObject(){return m_UtinityObject;}

private:
    explicit QmlTypesRegister(QObject *parent = nullptr);
    GlobalUtinityObject *m_UtinityObject = nullptr;
};

#endif // QMLTYPESREGISTER_H
