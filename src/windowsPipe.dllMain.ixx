//
// Created by MichaelBrunner on 16/10/2024.
//

module;

#include <cwapi3d/CwAPI3D.h>
#include <QSharedPointer>
#include <QThread>
#include <QDebug>
#include <QJsonDocument>

#include "windowsPipe.namedPipeServer.h"

export module windowsPipe.dllMian;

static QThread *serverThread = nullptr;

CWAPI3D_PLUGIN bool plugin_x64_init(CwAPI3D::ControllerFactory *aFactory)
{
    aFactory->getUtilityController()->printToConsole(L"init plugin_x64_init");
    // set some random project data for testing purposes
    QJsonDocument jsonDocument;
    jsonDocument.setObject(QJsonObject({{"superImportantData", 42}}));
    aFactory->getUtilityController()->setProjectData(L"PIPE", QString(jsonDocument.toJson()).toStdWString().c_str());

    // QSharedPointer<pipe::NamedPipeServer>::create();
    const auto serverPtr = new pipe::NamedPipeServer(aFactory->getUtilityController());
    serverThread = new QThread();
    serverPtr->moveToThread(serverThread);

    std::ignore = QObject::connect(serverThread, &QThread::started, serverPtr, &pipe::NamedPipeServer::start);
    std::ignore = QObject::connect(serverThread, &QThread::finished, serverThread, &QThread::deleteLater);
    std::ignore = QObject::connect(serverThread, &QThread::finished, serverPtr, &QObject::deleteLater);

    serverThread->start();

    qDebug() << "Server is now running in a detached QThread";
    return false;
}
