//
// Created by MichaelBrunner on 17/10/2024.
//

#pragma once

#include <QRunnable>
#include <QSharedPointer>
#include "namedPipe.namedPipeServer.h"
#include <QEventLoop>

namespace pipe
{
class NamedPipeServerTask final: public QRunnable
{
public:
    explicit NamedPipeServerTask(CwAPI3D::UtilityController *utilityController, const pipe::Identifier &pipeIdentifier);
    ~NamedPipeServerTask() override = default;

    void run() override;

private:
    CwAPI3D::UtilityController *utilityController;
    pipe::Identifier pipeIdentifier;
    QSharedPointer<NamedPipeServer> server;
};
}