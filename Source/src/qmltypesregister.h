#ifndef QMLTYPESREGISTER_H
#define QMLTYPESREGISTER_H

#include <QObject>
#include <QQmlContext>

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

private:
    explicit QmlTypesRegister(QObject *parent = nullptr);

};

#endif // QMLTYPESREGISTER_H
