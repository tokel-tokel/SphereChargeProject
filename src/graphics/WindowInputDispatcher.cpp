#include "WindowInputDispatcher.h"

void IKeyStrategy::setNext(std::unique_ptr<IKeyStrategy>&& next_)
{
    next = std::move(next_);
}

void WKeyStrategy::onKey(int key, int action)
{
    if (next) next->onKey(key, action);
}

void SKeyStrategy::onKey(int key, int action)
{
    if (next) next->onKey(key, action);
}

void DKeyStrategy::onKey(int key, int action)
{
    if (next) next->onKey(key, action);
}

void AKeyStrategy::onKey(int key, int action)
{
    if (next) next->onKey(key, action);
}
