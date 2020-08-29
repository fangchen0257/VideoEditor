#include "qmltypesregister.h"
#include "frameplayview.h"
#include "globalutinityobject.h"

QmlTypesRegister::QmlTypesRegister(QObject *parent) : QObject(parent)
{

}

QmlTypesRegister &QmlTypesRegister::instance()
{
    static QmlTypesRegister qmlInstance;
    return qmlInstance;
}

QmlTypesRegister::~QmlTypesRegister()
{

}

void QmlTypesRegister::RegisterCommonType()
{
    qmlRegisterType<FramePlayView>("Play.FramePlayView", 1, 0, "FramePlayView");
}

void QmlTypesRegister::setGlobalCariable(QQmlContext *context)
{
    do
    {
        if(nullptr == context) {
            break;
        }

        //全局辅助类
        context->setContextProperty("_global_utinity_obj", new GolbalUtinityObject(this));
    }while(false);
}
