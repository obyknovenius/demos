#include <gtest/gtest.h>

#include <Foundation/WeakPtr.h>
#include <Foundation/RefCounted.h>

class A : public Weakable<A>
{
};

class B : public RefCounted, public Weakable<B>, public A
{
};


TEST(WeakPtr, Basics)
{
    WeakPtr<A> weakA;
    WeakPtr<B> weakB;
    EXPECT_TRUE(weakA.get() == nullptr);
    EXPECT_TRUE(weakB.get() == nullptr);

    {
        RefPtr<B> b = adoptRef(new B);
        weakA = b;
        weakB = b;
        EXPECT_TRUE(weakA.get() == b.get());
        EXPECT_TRUE(weakB.get() == b.get());
    }

    EXPECT_TRUE(weakA.get() == nullptr);
    EXPECT_TRUE(weakB.get() == nullptr);
}
