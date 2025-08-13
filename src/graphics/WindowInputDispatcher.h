#pragma once
#include <memory>

#include "MainWindow.h"

class IKeyStrategy
{
protected:
    std::unique_ptr<IKeyStrategy> next;
public:
    virtual void onKey(int key, int action) = 0;
    virtual void setNext(std::unique_ptr<IKeyStrategy>&& next_);
    virtual ~IKeyStrategy() = default;
};

class WKeyStrategy : public IKeyStrategy
{
    void onKey(int key, int action) override;
};
class SKeyStrategy : public IKeyStrategy
{
    void onKey(int key, int action) override;
};
class DKeyStrategy : public IKeyStrategy
{
    void onKey(int key, int action) override;
};
class AKeyStrategy : public IKeyStrategy
{
    void onKey(int key, int action) override;
};


class WindowInputDispatcher
{
    friend class MainWindow;

};