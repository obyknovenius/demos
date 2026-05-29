#include <gtest/gtest.h>

#include <Foundation/NonNull.h>
#include <Foundation/Object.h>
#include <Foundation/Protocol.h>
#include <Foundation/WeakPtr.h>

class Window : public Object
{
public:
    static NonNull<RefPtr<Window>> create()
    {
        return RefPtr<Window>::adopt(new Window());
    }

    class Delegate : public Protocol
    {
    public:
        virtual void draw() = 0;
    };

    void setDelegate(WeakPtr<Delegate> delegate)
    {
        _delegate = delegate;
    }

    void draw()
    {
        if (RefPtr delegate = _delegate)
            delegate->draw();
    }

protected:
    Window() = default;

private:
    WeakPtr<Delegate> _delegate;
};

class WindowDelegate : public Object, public Window::Delegate
{
public:
    static NonNull<RefPtr<WindowDelegate>> create()
    {
        return RefPtr<WindowDelegate>::adopt(new WindowDelegate());
    }

    void draw() override
    {
    }

protected:
    WindowDelegate() = default;
};


TEST(ObjectAndProtocolTest, ItWorks)
{
    auto window = Window::create();
    auto delegate = WindowDelegate::create();
    window->setDelegate(delegate);
    window->draw();
}
