//
// Created by MichaelBrunner on 16/10/2024.
//

#include "windowsPipe.namedPipeServer.h"
#include <QJsonObject>
#include <ranges>

#include <QThread>
#include <QLocalSocket>
#include <QJsonDocument>

namespace pipe
{
NamedPipeServer::NamedPipeServer(CwAPI3D::UtilityController *utilityController, QObject *parent)
    : QObject(parent)
      , mUtilityController(utilityController)
{
    if (!mUtilityController) {
        throw std::runtime_error("UtilityController is nullptr");
    }
}

void NamedPipeServer::start()
{
    server = new QLocalServer(this);
    QLocalServer::removeServer("CadworkPipe");

    if (!server->listen("CadworkPipe")) {
        qCritical() << "Unable to start server:" << server->errorString();
        return;
    }

    std::ignore = connect(server, &QLocalServer::newConnection, this, &NamedPipeServer::onNewConnection);
    qDebug() << "Server is listening on 'CadworkPipe' in thread:" << QThread::currentThread();
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
    replyObject["projectData"] = mUtilityController->getProjectData(L"PIPE")->narrowData();
    replyObject["fileName"] = mUtilityController->get3DFileName()->narrowData();

    const QJsonDocument replyDoc(replyObject);
    clientConnection->write(replyDoc.toJson());
    clientConnection->flush();
}
}
