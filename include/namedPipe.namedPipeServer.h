//
// Created by MichaelBrunner on 16/10/2024.
//
#pragma once

#include <QObject>
#include <QLocalServer>
#include <QJsonObject>
#include <cwapi3d/CwAPI3D.h>

import namedPipe.pipeIdentifier;

namespace pipe
{
class NamedPipeServer final: public QObject
{
    Q_OBJECT

public:
    explicit NamedPipeServer(CwAPI3D::UtilityController *utilityController,
                             const pipe::Identifier &pipeIdentifier,
                             QObject *parent = nullptr);
    ~NamedPipeServer() override;

    void start();

private slots:
    void onNewConnection();
    void onReadyRead() const;

private:
    QLocalServer *server{};
    CwAPI3D::UtilityController *mUtilityController{nullptr};
    Identifier mPipeIdentifier;
};
}

// #include "NamedPipeServer.moc"
