#pragma once

namespace Foundation
{
    class Object;
    template <typename T> class WeakPtr;

    class WeakLink final
    {
        friend class Object;
        template <typename T> friend class WeakPtr;

    public:
        Object* get() const { return _ptr; }
    
    private:
        explicit WeakLink(Object* ptr) : _ptr(ptr) {}
        ~WeakLink() = default;

        void retain() { ++_retainCount; }

        void release()
        {
            if (--_retainCount == 0)
                delete this;
        }

        void revoke() { _ptr = nullptr; }

        Object* _ptr = nullptr;
        unsigned _retainCount = 1;
    };

    class Object
    {
    public:
        void retain() { ++_retainCount; }

        void release()
        {
            if (--_retainCount == 0)
                delete this;
        }

        unsigned retainCount() const { return _retainCount; }

        WeakLink* weakLink()
        {
            if (!_weakLink)
                _weakLink = new WeakLink(this);
            return _weakLink;
        }

    protected:
        virtual ~Object()
        {
            if (_weakLink)
            {
                _weakLink->revoke();
                _weakLink->release();
                _weakLink = nullptr;
            }
        }

    private:
        unsigned _retainCount = 1;
        WeakLink* _weakLink = nullptr;
    };
}

using Foundation::Object;
