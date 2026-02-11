#include <gtest/gtest.h>

#include <Foundation/RefCounted.h>

TEST(RefCountedTest, RefCountIncrementsAndDecrementsCorrectly)
{
    class Object : public RefCounted
    {
    };

    Object* object = new Object();
    EXPECT_EQ(object->refCount(), 1U);

    object->ref();
    EXPECT_EQ(object->refCount(), 2U);

    object->unref();
    EXPECT_EQ(object->refCount(), 1U);

    object->unref();
}

TEST(RefCountedTest, ObjectDeletedWhenRefCountReachesZero)
{
    bool deleted = false;

    class Object : public RefCounted
    {
    public:
        Object(bool& deleted) : deleted { deleted } {}
    private:
        ~Object() override { deleted = true; }
        bool& deleted;
    };

    Object* object = new Object(deleted);
    EXPECT_FALSE(deleted);

    object->unref();
    EXPECT_TRUE(deleted);
}
