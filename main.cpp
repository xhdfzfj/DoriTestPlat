#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include <QLocale>
#include <QTranslator>

#include "./Control/GuiDrawControl.h"
#include "Model/MainModelClass.h"
#include "Class/SimFlash/SimFlashDrawControl.h"

MainModelClass * gMainModelObjP;

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QGuiApplication app(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "TestPlatProject_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            app.installTranslator(&translator);
            break;
        }
    }

    qmlRegisterType< GuiDrawControl >( "xhd.controls.guidrawcontrol", 1, 0, "GuiDrawControl" );
    qmlRegisterType< SimFlashDrawControl >( "xhd.controls.simflashddrawcontrol", 1, 0, "SimFlashDrawControl" );

    gMainModelObjP = new MainModelClass();

    QQmlApplicationEngine engine;

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.rootContext()->setContextProperty( "MainModelObj", gMainModelObjP );
    engine.rootContext()->setContextProperty( "logViewModelInstance", gMainModelObjP->mLogViewModelObjP );
    engine.rootContext()->setContextProperty( "spiCmdViewModelInstance", gMainModelObjP->mSpiModelObjP );
    engine.rootContext()->setContextProperty( "serialConfigModelInstance", gMainModelObjP->mSerialConfigModelObjP);
    engine.load(url);

    return app.exec();
}
