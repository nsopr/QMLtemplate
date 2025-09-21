#include <QApplication>
#include <QQmlApplicationEngine>
#include <QFontDatabase>
#include <QQmlContext>

#include "firebase.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //c++ source
    firebase fb;

    //QML
    QQmlApplicationEngine engine;
    QUrl url("qrc:/Main.qml");

    //c++ <-> QML
    engine.rootContext()->setContextProperty("firebase", &fb);
    engine.rootContext()->setContextProperty("group_type", &fb.group_type);
    engine.rootContext()->setContextProperty("user_list", &fb.user_list);

    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &a, [url](QObject *obj, const QUrl &objUrl) {
                         if (!obj && url == objUrl)
                             QCoreApplication::exit(-1);
                     }, Qt::QueuedConnection);
    engine.load(url);

    return a.exec();
}
