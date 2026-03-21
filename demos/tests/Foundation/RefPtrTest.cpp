#include <gtest/gtest.h>

#include <Foundation/RefCounted.h>
#include <Foundation/RefPtr.h>

TEST(RefPtrTest, DefaultConstructsToNullptr)
{
    class Object : public RefCounted
    {
    };

    RefPtr<Object> refPtr;
    EXPECT_EQ(refPtr.get(), nullptr);
}

TEST(RefPtrTest, ConstructsFromNullptr)
{
    class Object : public RefCounted
    {
    };

    RefPtr<Object> refPtr(nullptr);
    EXPECT_EQ(refPtr.get(), nullptr);
}

TEST(RefPtrTest, ConstructsFromPointerAndIncrementsRefCount)
{
    class Object : public RefCounted
    {
    };

    Object* object = new Object();
    EXPECT_EQ(object->refCount(), 1U);

    RefPtr<Object> refPtr(object);
    EXPECT_EQ(refPtr.get(), object);
    EXPECT_EQ(object->refCount(), 2U);

    object->unref();
}

TEST(RefPtrTest, CopyConstructsFromRefPtrAndIncrementsRefCount)
{
    class Object : public RefCounted
    {
    };

    Object* object = new Object();
    EXPECT_EQ(object->refCount(), 1U);

    RefPtr<Object> refPtr1(object);
    EXPECT_EQ(refPtr1.get(), object);
    EXPECT_EQ(object->refCount(), 2U);

    RefPtr<Object> refPtr2(refPtr1);
    EXPECT_EQ(refPtr2.get(), object);
    EXPECT_EQ(object->refCount(), 3U);

    object->unref();
}

TEST(RefPtrTest, CopyConstructsFromRefPtrOfInheritedTypeAndIncrementsRefCount)
{
    class BaseObject : public RefCounted
    {
    };

    class DerivedObject : public BaseObject
    {
    };

    DerivedObject* object = new DerivedObject();
    EXPECT_EQ(object->refCount(), 1U);

    RefPtr<DerivedObject> refPtr1(object);
    EXPECT_EQ(refPtr1.get(), object);
    EXPECT_EQ(object->refCount(), 2U);

    RefPtr<BaseObject> refPtr2(refPtr1);
    EXPECT_EQ(refPtr2.get(), object);
    EXPECT_EQ(object->refCount(), 3U);

    object->unref();
}

TEST(RefPtrTest, MoveConstructsFromRefPtrAndTransfersOwnership)
{
    class Object : public RefCounted
    {
    };

    Object* object = new Object();
    EXPECT_EQ(object->refCount(), 1U);

    RefPtr<Object> refPtr1(object);
    EXPECT_EQ(refPtr1.get(), object);
    EXPECT_EQ(object->refCount(), 2U);

    RefPtr<Object> refPtr2(std::move(refPtr1));
    EXPECT_EQ(refPtr1.get(), nullptr);
    EXPECT_EQ(refPtr2.get(), object);
    EXPECT_EQ(object->refCount(), 2U);

    object->unref();
}

TEST(RefPtrTest, MoveConstructsFromRefPtrOfInheritedTypeAndTransfersOwnership)
{
    class BaseObject : public RefCounted
    {
    };

    class DerivedObject : public BaseObject
    {
    };

    DerivedObject* object = new DerivedObject();
    EXPECT_EQ(object->refCount(), 1U);

    RefPtr<DerivedObject> refPtr1(object);
    EXPECT_EQ(refPtr1.get(), object);
    EXPECT_EQ(object->refCount(), 2U);

    RefPtr<BaseObject> refPtr2(std::move(refPtr1));
    EXPECT_EQ(refPtr1.get(), nullptr);
    EXPECT_EQ(refPtr2.get(), object);
    EXPECT_EQ(object->refCount(), 2U);

    object->unref();
}

TEST(RefPtrTest, AdoptingDoesNotIncrementRefCount)
{
    class Object : public RefCounted
    {
    };

    Object* object = new Object();
    EXPECT_EQ(object->refCount(), 1U);

    RefPtr<Object> refPtr = RefPtr<Object>::adopt(object);
    EXPECT_EQ(refPtr.get(), object);
    EXPECT_EQ(object->refCount(), 1U);

    object->unref();
}

TEST(RefPtrTest, DestructingDecrementsRefCount)
{
    class Object : public RefCounted
    {
    };

    Object* object = new Object();
    EXPECT_EQ(object->refCount(), 1U);

    {
        RefPtr<Object> refPtr = object;
        EXPECT_EQ(refPtr.get(), object);
        EXPECT_EQ(object->refCount(), 2U);
    }

    EXPECT_EQ(object->refCount(), 1U);

    object->unref();
}

TEST(RefPtrTest, AssignsFromNullptrAndDecrementsRefCount)
{
    class Object : public RefCounted
    {
    };

    Object* object = new Object();
    EXPECT_EQ(object->refCount(), 1U);

    RefPtr<Object> refPtr(object);
    EXPECT_EQ(refPtr.get(), object);
    EXPECT_EQ(object->refCount(), 2U);

    refPtr = nullptr;
    EXPECT_EQ(refPtr.get(), nullptr);
    EXPECT_EQ(object->refCount(), 1U);
}

TEST(RefPtrTest, AssignsFromPointerAndDecrementsAndIncrementsRefCount)
{
    class Object : public RefCounted
    {
    };

    Object* object1 = new Object();
    EXPECT_EQ(object1->refCount(), 1U);

    Object* object2 = new Object();
    EXPECT_EQ(object2->refCount(), 1U);

    RefPtr<Object> refPtr(object1);
    EXPECT_EQ(refPtr.get(), object1);
    EXPECT_EQ(object1->refCount(), 2U);

    refPtr = object2;
    EXPECT_EQ(object1->refCount(), 1U);
    EXPECT_EQ(refPtr.get(), object2);
    EXPECT_EQ(object2->refCount(), 2U);

    object1->unref();
    object2->unref();
}

TEST(RefPtrTest, CopyAssignsFromRefPtrAndDecrementsAndIncrementsRefCount)
{
    class Object : public RefCounted
    {
    };

    Object* object1 = new Object();
    EXPECT_EQ(object1->refCount(), 1U);

    Object* object2 = new Object();
    EXPECT_EQ(object2->refCount(), 1U);

    RefPtr<Object> refPtr1(object1);
    EXPECT_EQ(refPtr1.get(), object1);
    EXPECT_EQ(object1->refCount(), 2U);

    RefPtr<Object> refPtr2(object2);
    EXPECT_EQ(refPtr2.get(), object2);
    EXPECT_EQ(object2->refCount(), 2U);

    refPtr2 = refPtr1;
    EXPECT_EQ(object2->refCount(), 1U);
    EXPECT_EQ(refPtr2.get(), object1);
    EXPECT_EQ(object1->refCount(), 3U);

    object1->unref();
    object2->unref();
}

TEST(RefPtrTest, CopyAssignsFromRefPtrOfInheritedTypeAndDecrementsAndIncrementsRefCount)
{
    class BaseObject : public RefCounted
    {
    };

    class DerivedObject : public BaseObject
    {
    };

    DerivedObject* object1 = new DerivedObject();
    EXPECT_EQ(object1->refCount(), 1U);

    DerivedObject* object2 = new DerivedObject();
    EXPECT_EQ(object2->refCount(), 1U);

    RefPtr<DerivedObject> refPtr1(object1);
    EXPECT_EQ(refPtr1.get(), object1);
    EXPECT_EQ(object1->refCount(), 2U);

    RefPtr<BaseObject> refPtr2(object2);
    EXPECT_EQ(refPtr2.get(), object2);
    EXPECT_EQ(object2->refCount(), 2U);

    refPtr2 = refPtr1;
    EXPECT_EQ(object2->refCount(), 1U);
    EXPECT_EQ(refPtr2.get(), object1);
    EXPECT_EQ(object1->refCount(), 3U);

    object1->unref();
    object2->unref();
}

TEST(RefPtrTest, MoveAssignsFromRefPtrAndTransfersOwnership)
{
    class Object : public RefCounted
    {
    };

    Object* object1 = new Object();
    EXPECT_EQ(object1->refCount(), 1U);

    Object* object2 = new Object();
    EXPECT_EQ(object2->refCount(), 1U);

    RefPtr<Object> refPtr1(object1);
    EXPECT_EQ(refPtr1.get(), object1);
    EXPECT_EQ(object1->refCount(), 2U);

    RefPtr<Object> refPtr2(object2);
    EXPECT_EQ(refPtr2.get(), object2);
    EXPECT_EQ(object2->refCount(), 2U);

    refPtr2 = std::move(refPtr1);
    EXPECT_EQ(object2->refCount(), 1U);
    EXPECT_EQ(refPtr1.get(), nullptr);
    EXPECT_EQ(refPtr2.get(), object1);
    EXPECT_EQ(object1->refCount(), 2U);

    object1->unref();
    object2->unref();
}

TEST(RefPtrTest, MoveAssignsFromRefPtrOfInheritedTypeAndTransfersOwnership)
{
    class BaseObject : public RefCounted
    {
    };

    class DerivedObject : public BaseObject
    {
    };

    DerivedObject* object1 = new DerivedObject();
    EXPECT_EQ(object1->refCount(), 1U);

    DerivedObject* object2 = new DerivedObject();
    EXPECT_EQ(object2->refCount(), 1U);

    RefPtr<DerivedObject> refPtr1(object1);
    EXPECT_EQ(refPtr1.get(), object1);
    EXPECT_EQ(object1->refCount(), 2U);

    RefPtr<BaseObject> refPtr2(object2);
    EXPECT_EQ(refPtr2.get(), object2);
    EXPECT_EQ(object2->refCount(), 2U);

    refPtr2 = std::move(refPtr1);
    EXPECT_EQ(object2->refCount(), 1U);
    EXPECT_EQ(refPtr1.get(), nullptr);
    EXPECT_EQ(refPtr2.get(), object1);
    EXPECT_EQ(object1->refCount(), 2U);

    object1->unref();
    object2->unref();
}

TEST(RefPtrTest, OperatorArrowAccessesMember)
{
    class Object : public RefCounted
    {
    public:
        int value { 42 };
    };

    Object* object = new Object();
    RefPtr<Object> refPtr(object);
    EXPECT_EQ(refPtr->value, 42);

    object->unref();
}

TEST(RefPtrTest, OperatorStarDereferencesPointer)
{
    class Object : public RefCounted
    {
    public:
        int value { 42 };
    };

    Object* object = new Object();
    RefPtr<Object> refPtr(object);
    EXPECT_EQ((*refPtr).value, 42);

    object->unref();
}

TEST(RefPtrTest, OperatorEqualComparesUnderlyingPointer)
{
    class Object : public RefCounted
    {
    };

    Object* object1 = new Object();
    Object* object2 = new Object();

    RefPtr<Object> refPtr1;
    RefPtr<Object> refPtr2(object1);
    RefPtr<Object> refPtr3(object1);
    RefPtr<Object> refPtr4(object2);

    EXPECT_TRUE(refPtr1 == nullptr);
    EXPECT_TRUE(nullptr == refPtr1);
    EXPECT_FALSE(refPtr1 != nullptr);
    EXPECT_FALSE(nullptr != refPtr1);

    EXPECT_TRUE(refPtr2 != nullptr);
    EXPECT_TRUE(nullptr != refPtr2);
    EXPECT_FALSE(refPtr2 == nullptr);
    EXPECT_FALSE(nullptr == refPtr2);

    EXPECT_TRUE(refPtr2 == refPtr3);
    EXPECT_TRUE(refPtr1 != refPtr3);
    EXPECT_FALSE(refPtr2 != refPtr3);
    EXPECT_FALSE(refPtr1 == refPtr3);

    EXPECT_TRUE(refPtr2 != refPtr4);
    EXPECT_TRUE(refPtr4 != refPtr2);
    EXPECT_FALSE(refPtr2 == refPtr4);
    EXPECT_FALSE(refPtr4 == refPtr2);

    object1->unref();
    object2->unref();
}

TEST(RefPtrTest, OperatorBoolReturnsTrueWhenValidAndFalseWhenNullptr)
{
    class Object : public RefCounted
    {
    };

    Object* object = new Object();
    RefPtr<Object> refPtr(object);
    EXPECT_TRUE(refPtr);

    refPtr = nullptr;
    EXPECT_FALSE(refPtr);

    object->unref();
}

TEST(RefPtrTest, LeakTransfersOwnership)
{
    class Object : public RefCounted
    {
    };

    Object* object = new Object();
    EXPECT_EQ(object->refCount(), 1U);

    RefPtr<Object> refPtr(object);
    EXPECT_EQ(refPtr.get(), object);
    EXPECT_EQ(object->refCount(), 2U);

    Object* leakedObject = refPtr.leak();
    EXPECT_EQ(leakedObject, object);
    EXPECT_EQ(refPtr.get(), nullptr);
    EXPECT_EQ(object->refCount(), 2U);

    leakedObject->unref();
    EXPECT_EQ(object->refCount(), 1U);

    object->unref();
}
