//
// Created by MichaelBrunner on 16/10/2024.
//

#include "namedPipe.namedPipeServer.h"
#include <QJsonObject>

#include <QThread>
#include <QLocalSocket>
#include <QJsonDocument>

namespace pipe
{
NamedPipeServer::NamedPipeServer(CwAPI3D::UtilityController *utilityController,
                                 const Identifier &pipeIdentifier,
                                 QObject *parent)
    : QObject(parent)
      , mUtilityController(utilityController)
      , mPipeIdentifier(pipeIdentifier)
{
    if (!mUtilityController) {
        throw std::runtime_error("UtilityController is nullptr");
    }
}
NamedPipeServer::~NamedPipeServer()
{
    qDebug() << "NamedPipeServer destructor";
}

void NamedPipeServer::start()
{
    server = new QLocalServer(this);
    QLocalServer::removeServer(mPipeIdentifier.getIdentifier().c_str());

    if (!server->listen(mPipeIdentifier.getIdentifier().c_str())) {
        qCritical() << "Unable to start server:" << server->errorString();
        return;
    }

    const auto formattedOutput = std::format("Server is listening on '{}' in thread: {}",
                                             mPipeIdentifier.getIdentifier().c_str(),
                                             QThread::currentThread()->objectName().toStdString());
    qDebug() << formattedOutput.c_str();

    std::ignore = connect(server, &QLocalServer::newConnection, this, &NamedPipeServer::onNewConnection);
}

void NamedPipeServer::onNewConnection()
{
    const auto clientConnection = server->nextPendingConnection();

    std::ignore = connect(clientConnection, &QLocalSocket::readyRead, this, &NamedPipeServer::onReadyRead);
    std::ignore = connect(clientConnection, &QLocalSocket::disconnected, clientConnection, &QLocalSocket::deleteLater);
}

void NamedPipeServer::onReadyRead() const
{
    const auto clientConnection = qobject_cast<QLocalSocket *>(sender());
    const QByteArray data = clientConnection->readAll();
    const QJsonDocument jsonDoc = QJsonDocument::fromJson(data);
    const QJsonObject jsonObject = jsonDoc.object();

    qDebug() << "Received JSON from client:" << jsonObject;

    QJsonObject replyObject;
    replyObject["message"] = "Hello from server!";

    const auto projectDataStr = mUtilityController->getProjectData(L"PIPE")->narrowData();
    const QJsonDocument projectDataDoc = QJsonDocument::fromJson(projectDataStr);
    replyObject["projectData"] = projectDataDoc.object();

    replyObject["fileName"] = mUtilityController->get3DFileName()->narrowData();

    const QJsonDocument replyDoc(replyObject);
    clientConnection->write(replyDoc.toJson());
    clientConnection->flush();
}
}
