#include <gtest/gtest.h>

#include <Foundation/WeakPtr.h>

TEST(WeakPtrTest, WeakPtrReturnsValidPointerWhileObjectIsAlive)
{
    class Object : public EnableWeakPtr<Object>
    {
    };

    Object* object = new Object();
    WeakPtr<Object> weakPtr = object;

    EXPECT_TRUE(weakPtr.get() == object);

    delete object;
}

TEST(WeakPtrTest, WeakPtrReturnsNullptrAfterObjectIsDeleted)
{
    class Object : public EnableWeakPtr<Object>
    {
    };

    Object* object = new Object();
    WeakPtr<Object> weakPtr = object;

    delete object;

    EXPECT_TRUE(weakPtr.get() == nullptr);
}

TEST(WeakPtrTest, WeakPtrCanBeReassignedToAnotherObject)
{
    class Object : public EnableWeakPtr<Object>
    {
    };

    Object* object1 = new Object();
    Object* object2 = new Object();
    WeakPtr<Object> weakPtr = object1;

    EXPECT_TRUE(weakPtr.get() == object1);

    weakPtr = object2;

    EXPECT_TRUE(weakPtr.get() == object2);

    delete object1;
    delete object2;
}

TEST(WeakPtrTest, WeakPtrCanBeResetToNullptr)
{
    class Object : public EnableWeakPtr<Object>
    {
    };

    Object* object = new Object();
    WeakPtr<Object> weakPtr = object;

    EXPECT_TRUE(weakPtr.get() == object);

    weakPtr = nullptr;

    EXPECT_TRUE(weakPtr.get() == nullptr);

    delete object;
}

TEST(WeakPtrTest, WeakPtrWorksWithMultipleInheritance)
{
    class BaseObject : public EnableWeakPtr<BaseObject>
    {
    };

    class DerivedObject : public EnableWeakPtr<DerivedObject>, public BaseObject
    {
    };

    WeakPtr<BaseObject> weakBasePtr;
    WeakPtr<DerivedObject> weakDerivedPtr;
    EXPECT_TRUE(weakBasePtr.get() == nullptr);
    EXPECT_TRUE(weakDerivedPtr.get() == nullptr);

    {
        auto* derivedObject = new DerivedObject;
        weakBasePtr = derivedObject;
        weakDerivedPtr = derivedObject;
        EXPECT_TRUE(weakBasePtr.get() == derivedObject);
        EXPECT_TRUE(weakDerivedPtr.get() == derivedObject);
        delete derivedObject;
    }

    EXPECT_TRUE(weakBasePtr.get() == nullptr);
    EXPECT_TRUE(weakDerivedPtr.get() == nullptr);
}
