#include <gtest/gtest.h>

#include <Foundation/Object.h>
#include <Foundation/StrongPtr.h>
#include <Foundation/WeakPtr.h>

class Window : public Object
{
public:
    class Delegate
    {
    public:
        virtual void draw() = 0;
    };

    Window() = default;

    void setDelegate(WeakPtr<Delegate> delegate)
    {
        _delegate = delegate;
    }

    void draw()
    {
        if (StrongPtr delegate = _delegate)
            delegate->draw();
    }

protected:
    virtual ~Window() = default;

private:
    WeakPtr<Delegate> _delegate;
};

class WindowDelegate : public Object, public Window::Delegate
{
public:
    WindowDelegate() = default;
    
    void draw() override
    {
    }

private:
    ~WindowDelegate() override = default;
};


TEST(ObjectAndProtocolTest, ItWorks)
{
    auto window = makeStrong<Window>();
    auto delegate = makeStrong<WindowDelegate>();
    window->setDelegate(delegate);
    window->draw();
}
