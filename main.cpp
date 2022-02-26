#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext >
#include <QtQml/QtQml>
#include <QSystemTrayIcon>
#include "Repack.h"
int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QGuiApplication app(argc, argv);

    //注册打包类型
    qmlRegisterType<Repack>("an.qt.Repack", 1, 0, "Reapck");

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(
        &engine, &QQmlApplicationEngine::objectCreated, &app,
        [url](QObject *obj, const QUrl &objUrl)
        {
            if (!obj && url == objUrl) QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);

    //设置一个属性
    QQmlContext *ctx = engine.rootContext();
    ctx->setContextProperty("repack", new Repack);
    engine.load(url);

    //任务托盘

    return app.exec();
}
