#include <gtest/gtest.h>

#include <Foundation/WeakPtr.h>

TEST(WeakPtrTest, DefaultConstructsToNullptr)
{
    class Object : public EnableWeakPtr<Object>
    {
    };

    WeakPtr<Object> weakPtr;
    EXPECT_EQ(weakPtr.get(), nullptr);
}

TEST(WeakPtrTest, ConstructsFromNullptr)
{
    class Object : public EnableWeakPtr<Object>
    {
    };

    WeakPtr<Object> weakPtr(nullptr);
    EXPECT_EQ(weakPtr.get(), nullptr);
}

TEST(WeakPtrTest, ConstructsFromPointer)
{
    class Object : public EnableWeakPtr<Object>
    {
    };

    Object* object = new Object();
    WeakPtr<Object> weakPtr(object);

    EXPECT_EQ(weakPtr.get(), object);

    delete object;
}

TEST(WeakPtrTest, CopyConstructsFromAnotherWeakPtr)
{
    class Object : public EnableWeakPtr<Object>
    {
    };

    Object* object = new Object();
    WeakPtr<Object> weakPtr1(object);
    EXPECT_EQ(weakPtr1.get(), object);

    WeakPtr<Object> weakPtr2(weakPtr1);
    EXPECT_EQ(weakPtr2.get(), object);

    delete object;
}

TEST(WeakPtrTest, MoveConstructsFromAnotherWeakPtr)
{
    class Object : public EnableWeakPtr<Object>
    {
    };

    Object* object = new Object();
    WeakPtr<Object> weakPtr1(object);
    EXPECT_EQ(weakPtr1.get(), object);

    WeakPtr<Object> weakPtr2(std::move(weakPtr1));
    EXPECT_EQ(weakPtr1.get(), nullptr);
    EXPECT_EQ(weakPtr2.get(), object);

    delete object;
}

TEST(WeakPtrTest, AssignsFromNullptr)
{
    class Object : public EnableWeakPtr<Object>
    {
    };

    Object* object = new Object();
    WeakPtr<Object> weakPtr(object);
    EXPECT_EQ(weakPtr.get(), object);

    weakPtr = nullptr;
    EXPECT_EQ(weakPtr.get(), nullptr);

    delete object;
}

TEST(WeakPtrTest, AssignsFromPointer)
{
    class Object : public EnableWeakPtr<Object>
    {
    };

    WeakPtr<Object> weakPtr;
    EXPECT_EQ(weakPtr.get(), nullptr);

    Object* object = new Object();
    weakPtr = object;
    EXPECT_EQ(weakPtr.get(), object);

    delete object;
}

TEST(WeakPtrTest, CopyAssignsFromWeakPtr)
{
    class Object : public EnableWeakPtr<Object>
    {
    };

    Object* object = new Object();
    WeakPtr<Object> weakPtr1(object);
    EXPECT_EQ(weakPtr1.get(), object);

    WeakPtr<Object> weakPtr2;
    weakPtr2 = weakPtr1;
    EXPECT_EQ(weakPtr2.get(), object);

    delete object;
}

TEST(WeakPtrTest, MoveAssignsFromWeakPtr)
{
    class Object : public EnableWeakPtr<Object>
    {
    };

    Object* object = new Object();
    WeakPtr<Object> weakPtr1(object);
    EXPECT_EQ(weakPtr1.get(), object);

    WeakPtr<Object> weakPtr2;
    weakPtr2 = std::move(weakPtr1);
    EXPECT_EQ(weakPtr1.get(), nullptr);
    EXPECT_EQ(weakPtr2.get(), object);

    delete object;
}

TEST(WeakPtrTest, OperatorArrowAccessesMember)
{
    class Object : public EnableWeakPtr<Object>
    {
    public:
        int value { 42 };
    };

    Object* object = new Object();
    WeakPtr<Object> weakPtr(object);

    EXPECT_EQ(weakPtr->value, 42);

    delete object;
}

TEST(WeakPtrTest, OperatorStarDereferencesPointer)
{
    class Object : public EnableWeakPtr<Object>
    {
    public:
        int value { 42 };
    };

    Object* object = new Object();
    WeakPtr<Object> weakPtr(object);

    EXPECT_EQ((*weakPtr).value, 42);

    delete object;
}

TEST(WeakPtrTest, OperatorEqualComparesUnderlyingPointer)
{
    class Object : public EnableWeakPtr<Object>
    {
    };

    Object* object1 = new Object();
    Object* object2 = new Object();

    WeakPtr<Object> weakPtr1;
    WeakPtr<Object> weakPtr2(object1);
    WeakPtr<Object> weakPtr3(object1);
    WeakPtr<Object> weakPtr4(object2);

    EXPECT_TRUE(weakPtr1 == nullptr);
    EXPECT_TRUE(nullptr == weakPtr1);
    EXPECT_FALSE(weakPtr1 != nullptr);
    EXPECT_FALSE(nullptr != weakPtr1);

    EXPECT_TRUE(weakPtr2 != nullptr);
    EXPECT_TRUE(nullptr != weakPtr2);
    EXPECT_FALSE(weakPtr2 == nullptr);
    EXPECT_FALSE(nullptr == weakPtr2);

    EXPECT_TRUE(weakPtr2 == weakPtr3);
    EXPECT_TRUE(weakPtr1 != weakPtr3);
    EXPECT_FALSE(weakPtr2 != weakPtr3);
    EXPECT_FALSE(weakPtr1 == weakPtr3);

    EXPECT_TRUE(weakPtr2 != weakPtr4);
    EXPECT_TRUE(weakPtr4 != weakPtr2);
    EXPECT_FALSE(weakPtr2 == weakPtr4);
    EXPECT_FALSE(weakPtr4 == weakPtr2);

    delete object1;
    delete object2;
}

TEST(WeakPtrTest, OperatorBoolReturnsTrueWhenValidAndFalseWhenNullptr)
{
    class Object : public EnableWeakPtr<Object>
    {
    };

    Object* object = new Object();
    WeakPtr<Object> weakPtr(object);
    EXPECT_TRUE(weakPtr);

    delete object;
    EXPECT_FALSE(weakPtr);
}

TEST(WeakPtrTest, ResetsToNullptrAfterObjectIsDeleted)
{
    class Object : public EnableWeakPtr<Object>
    {
    };

    Object* object = new Object();
    WeakPtr<Object> weakPtr(object);

    delete object;
    EXPECT_EQ(weakPtr.get(), nullptr);
}

TEST(WeakPtrTest, MultipleWeakPtrsResetToNullptrAfterObjectIsDeleted)
{
    class Object : public EnableWeakPtr<Object>
    {
    };

    Object* object = new Object();
    WeakPtr<Object> weakPtr1(object);
    WeakPtr<Object> weakPtr2(object);
    WeakPtr<Object> weakPtr3(object);
    EXPECT_EQ(weakPtr1.get(), object);
    EXPECT_EQ(weakPtr2.get(), object);
    EXPECT_EQ(weakPtr3.get(), object);

    delete object;
    EXPECT_EQ(weakPtr1.get(), nullptr);
    EXPECT_EQ(weakPtr2.get(), nullptr);
    EXPECT_EQ(weakPtr3.get(), nullptr);
}

TEST(WeakPtrTest, WorksWithMultipleInheritance)
{
    class BaseObject : public EnableWeakPtr<BaseObject>
    {
    };

    class DerivedObject : public EnableWeakPtr<DerivedObject>, public BaseObject
    {
    };

    WeakPtr<BaseObject> weakBasePtr;
    WeakPtr<DerivedObject> weakDerivedPtr;
    EXPECT_EQ(weakBasePtr.get(), nullptr);
    EXPECT_EQ(weakDerivedPtr.get(), nullptr);

    {
        auto* derivedObject = new DerivedObject;
        weakBasePtr = derivedObject;
        weakDerivedPtr = derivedObject;
        EXPECT_EQ(weakBasePtr.get(), derivedObject);
        EXPECT_EQ(weakDerivedPtr.get(), derivedObject);

        delete derivedObject;
    }

    EXPECT_EQ(weakBasePtr.get(), nullptr);
    EXPECT_EQ(weakDerivedPtr.get(), nullptr);
}
