//
// Created by MichaelBrunner on 16/10/2024.
//
#pragma once

#pragma once

#include <QObject>
#include <QLocalServer>
#include <QJsonObject>
#include <vector>
#include <cwapi3d/CwAPI3D.h>

namespace pipe
{
class NamedPipeServer final: public QObject
{
    Q_OBJECT

public:
    explicit NamedPipeServer(CwAPI3D::UtilityController *utilityController, QObject *parent = nullptr);

    // Start the server
    void start();

private slots:
    void onNewConnection();
    void onReadyRead() const;

private:
    QLocalServer *server{};

    CwAPI3D::UtilityController *mUtilityController{nullptr};
};
}

// #include "NamedPipeServer.moc"
