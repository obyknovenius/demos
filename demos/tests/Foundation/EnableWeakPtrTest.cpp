#include <gtest/gtest.h>

#include <Foundation/RefCounted.h>
#include <Foundation/EnableWeakPtr.h>
#include <Foundation/WeakPtr.h>

class A : public EnableWeakPtr<A>
{
};

class B : public EnableWeakPtr<B>, public A
{
};


TEST(WeakPtr, Basics)
{
    WeakPtr<A> weakA;
    WeakPtr<B> weakB;
    EXPECT_TRUE(weakA.get() == nullptr);
    EXPECT_TRUE(weakB.get() == nullptr);

    {
        auto* b = new B;
        weakA = b;
        weakB = b;
        EXPECT_TRUE(weakA.get() == b);
        EXPECT_TRUE(weakB.get() == b);
        delete b;
    }

    EXPECT_TRUE(weakA.get() == nullptr);
    EXPECT_TRUE(weakB.get() == nullptr);
}
