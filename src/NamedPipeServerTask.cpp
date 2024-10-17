//
// Created by MichaelBrunner on 17/10/2024.
//

#include "NamedPipeServerTask.h"
#include <QThread>

namespace pipe
{
NamedPipeServerTask::NamedPipeServerTask(CwAPI3D::UtilityController *utilityController, const pipe::Identifier &pipeIdentifier)
    : utilityController(utilityController), pipeIdentifier(pipeIdentifier)
{
}

void NamedPipeServerTask::run()
{
    server = QSharedPointer<NamedPipeServer>::create(utilityController, pipeIdentifier);
    server->moveToThread(QThread::currentThread());
    server->start();
}
}