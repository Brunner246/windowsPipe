//
// Created by MichaelBrunner on 17/10/2024.
//

module;

#include <string>

export module namedPipe.pipeIdentifier;

export namespace pipe
{
class Identifier
{
    std::string mIdentifier{};

public:
    explicit Identifier(std::string identifier)
        : mIdentifier(std::move(identifier))
    {
    }

    ~Identifier() = default;

    [[nodiscard]] std::string getIdentifier() const
    {
        return mIdentifier;
    }

    operator std::string() const
    {
        return mIdentifier;
    }
};
}