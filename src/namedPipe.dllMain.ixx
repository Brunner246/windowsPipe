//
// Created by MichaelBrunner on 16/10/2024.
//

module;

#include <cwapi3d/CwAPI3D.h>
#include <QSharedPointer>
#include <QThread>
#include <QThreadPool>
#include <QDebug>
#include <QJsonDocument>

#include "NamedPipeServerTask.h"
#include "namedPipe.namedPipeServer.h"

export module namedPipe.dllMian;
import std;

static auto serverThreads = new QVector<QThread *>();

CWAPI3D_PLUGIN bool plugin_x64_init(CwAPI3D::ControllerFactory *aFactory)
{
    aFactory->getUtilityController()->printToConsole(L"init plugin_x64_init");
    // set some random project data for testing purposes
    QJsonDocument jsonDocument;
    jsonDocument.setObject(QJsonObject({{"superImportantData", 42}}));
    aFactory->getUtilityController()->setProjectData(L"PIPE", QString(jsonDocument.toJson()).toStdWString().c_str());

    // QSharedPointer<pipe::NamedPipeServer>::create();
    const auto pipeNameIdentifierArray = std::array<pipe::Identifier, 2>{
        pipe::Identifier("CadworkPipe"),
        pipe::Identifier("AnotherCadworkPipe")
    };
    serverThreads->reserve(pipeNameIdentifierArray.size());

    std::ranges::for_each(pipeNameIdentifierArray,
                          [&](const auto &pipeNameIdentifier)
                          {
                              const auto serverPtr = new pipe::NamedPipeServer(
                                  aFactory->getUtilityController(),
                                  pipeNameIdentifier);

                              auto thread = new QThread();
                              serverThreads->push_back(thread);
                              serverPtr->moveToThread(thread);

                              std::ignore = QObject::connect(thread,
                                                             &QThread::started,
                                                             serverPtr,
                                                             &pipe::NamedPipeServer::start);
                              std::ignore = QObject::connect(thread,
                                                             &QThread::finished,
                                                             thread,
                                                             &QThread::deleteLater);
                              std::ignore = QObject::connect(thread,
                                                             &QThread::finished,
                                                             serverPtr,
                                                             &QObject::deleteLater);

                              thread->start();

                              qDebug() << "Server is now running in a detached QThread";
                          });

    return false;
}
