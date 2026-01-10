#include "Window.h"

#include "Event.h"
#include "WindowDecorationView.h"
#include "WindowTitleBar.h"
#include "Wayland/Window.h"
#include <Gfx/Color.h>
#include <Gfx/Context.h>
#include <Gfx/Rect.h>
#include <memory>

namespace GUI
{
    NonNull<RefPtr<Window>> Window::make()
    {
        return Wayland::Window::make();
    }

    Window::Window(Gfx::Size size) :
        _size { size },
        _decorationView { DecorationView::make(this) }
    {
    }

    Window::~Window() = default;

    void Window::dispatchEvent(std::unique_ptr<const Event> event)
    {
        switch (event->type)
        {
            case Event::Type::pointerButtonPressed:
            {
                auto view = _decorationView->hitTest(*event->position);
                view->onPointerButtonPressed(*event);
                break;
            }

            case Event::Type::pointerButtonReleased:
            {
                auto view = _decorationView->hitTest(*event->position);
                view->onPointerButtonReleased(*event);
                break;
            }

            case Event::Type::pointerEntered:
            {
                auto view = _decorationView->hitTest(*event->position);
                view->onPointerEntered(*event);
                _viewUnderPointer = view;
                break;
            }

            case Event::Type::pointerMoved:
            {
                auto view = _decorationView->hitTest(*event->position);

                if (auto viewUnderPointer = _viewUnderPointer.strong())
                {
                    if (viewUnderPointer != view)
                    {
                        viewUnderPointer->onPointerLeft(*event);
                        view->onPointerEntered(*event);
                        _viewUnderPointer = view;
                    }
                    else
                    {
                        view->onPointerMoved(*event);
                    }
                }
                break;
            }

            case Event::Type::pointerLeft:
            {
                if (auto viewUnderPointer = _viewUnderPointer.strong())
                {
                    viewUnderPointer->onPointerLeft(*event);
                    _viewUnderPointer = nullptr;
                }
                break;
            }

            default:
                break;
        }
    }

    void Window::close()
    {
        if (onClose)
            onClose();
    }

    void Window::layout()
    {
        _decorationView->setFrame({ 0, 0, _size.width, _size.height });
        _decorationView->layout();
    }

    void Window::redraw(NonNull<RefPtr<Gfx::Context>> context)
    {
        context->fillRect({ 0, 0, _size.width, _size.height }, Gfx::Color::white);
        _decorationView->redraw(context);
    }

    void Window::pushCursor(Cursor cursor)
    {
        _cursorStack.push_back(_currentCursor);
        setCursor(cursor);
    }

    void Window::popCursor()
    {
        if (!_cursorStack.empty())
        {
            setCursor(_cursorStack.back());
            _cursorStack.pop_back();
        }
    }
}
