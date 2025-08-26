#include <QApplication>
#include <QQmlApplicationEngine>
#include <QFontDatabase>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //QML
    QQmlApplicationEngine engine;
    QUrl url("qrc:/Main.qml");

    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &a, [url](QObject *obj, const QUrl &objUrl) {
                         if (!obj && url == objUrl)
                             QCoreApplication::exit(-1);
                     }, Qt::QueuedConnection);
    engine.load(url);

    return a.exec();
}
