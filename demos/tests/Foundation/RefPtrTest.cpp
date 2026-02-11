#include <gtest/gtest.h>

#include <Foundation/RefCounted.h>
#include <Foundation/RefPtr.h>

TEST(RefPtrTest, RefPtrIncrementsAndDecrementsRefCountCorrectly)
{
    class Object : public RefCounted
    {
    };

    Object* object = new Object();
    EXPECT_EQ(object->refCount(), 1U);

    {
        RefPtr<Object> refPtr = object;
        EXPECT_TRUE(refPtr.get() == object);
        EXPECT_EQ(object->refCount(), 2U);
    }
    EXPECT_EQ(object->refCount(), 1U);

    object->unref();
}

TEST(RefPtrTest, AdoptingDoesNotIncrementRefCount)
{
    class Object : public RefCounted
    {
    };

    Object* object = new Object();
    EXPECT_EQ(object->refCount(), 1U);

    {
        RefPtr<Object> refPtr = adoptRef(object);
        EXPECT_TRUE(refPtr.get() == object);
        EXPECT_EQ(object->refCount(), 1U);
    }
}

TEST(RefPtrTest, LeakRefTransfersOwnership)
{
    class Object : public RefCounted
    {
    };

    Object* object = new Object();
    EXPECT_EQ(object->refCount(), 1U);

    {
        RefPtr<Object> refPtr(object);
        EXPECT_TRUE(refPtr.get() == object);
        EXPECT_EQ(object->refCount(), 2U);

        Object* leakedObject = refPtr.leakRef();
        EXPECT_TRUE(leakedObject == object);
        EXPECT_TRUE(!refPtr);
        EXPECT_EQ(object->refCount(), 2U);

        leakedObject->unref();
    }
    EXPECT_EQ(object->refCount(), 1U);

    object->unref();
}

TEST(RefPtrTest, CopyingRefPtrIncrementsRefCount)
{
    class BaseObject : public RefCounted
    {
    };

    class DerivedObject : public BaseObject
    {
    };

    BaseObject* object = new BaseObject();
    EXPECT_EQ(object->refCount(), 1U);

    {
        RefPtr<BaseObject> refPtr1(object);
        EXPECT_TRUE(refPtr1.get() == object);
        EXPECT_EQ(object->refCount(), 2U);

        RefPtr<BaseObject> refPtr2(refPtr1);
        EXPECT_TRUE(refPtr2.get() == object);
        EXPECT_EQ(object->refCount(), 3U);

        RefPtr<DerivedObject> refPtr3(refPtr1);
        EXPECT_TRUE(refPtr3.get() == object);
        EXPECT_EQ(object->refCount(), 4U);
    }
    EXPECT_EQ(object->refCount(), 1U);

    object->unref();
}

TEST(RefPtrTest, MoveConstructingRefPtrTransfersOwnership)
{
    class BaseObject : public RefCounted
    {
    };

    class DerivedObject : public BaseObject
    {
    };

    BaseObject* object = new BaseObject();
    EXPECT_EQ(object->refCount(), 1U);

    {
        RefPtr<BaseObject> refPtr1(object);
        EXPECT_TRUE(refPtr1.get() == object);
        EXPECT_EQ(object->refCount(), 2U);

        RefPtr<BaseObject> refPtr2(std::move(refPtr1));
        EXPECT_TRUE(refPtr2.get() == object);
        EXPECT_TRUE(!refPtr1);
        EXPECT_EQ(object->refCount(), 2U);

        RefPtr<DerivedObject> refPtr3(std::move(refPtr2));
        EXPECT_TRUE(refPtr3.get() == object);
        EXPECT_TRUE(!refPtr2);
        EXPECT_EQ(object->refCount(), 2U);
    }
    EXPECT_EQ(object->refCount(), 1U);

    object->unref();
}

TEST(RefPtrTest, AssigningRefPtrIncrementsAndDecrementsRefCountCorrectly)
{
    class BaseObject : public RefCounted
    {
    };

    class DerivedObject : public BaseObject
    {
    };

    BaseObject* object1 = new BaseObject();
    BaseObject* object2 = new BaseObject();
    BaseObject* object3 = new BaseObject();
    EXPECT_EQ(object1->refCount(), 1U);
    EXPECT_EQ(object2->refCount(), 1U);
    EXPECT_EQ(object3->refCount(), 1U);

    {
        RefPtr<BaseObject> refPtr1 = object1;
        RefPtr<BaseObject> refPtr2 = object2;
        RefPtr<DerivedObject> refPtr3 = object3;

        EXPECT_TRUE(refPtr1.get() == object1);
        EXPECT_TRUE(refPtr2.get() == object2);
        EXPECT_TRUE(refPtr3.get() == object3);
        EXPECT_EQ(object1->refCount(), 2U);
        EXPECT_EQ(object2->refCount(), 2U);
        EXPECT_EQ(object3->refCount(), 2U);

        refPtr1 = refPtr2;
        EXPECT_TRUE(refPtr1.get() == object2);
        EXPECT_TRUE(refPtr2.get() == object2);
        EXPECT_EQ(object1->refCount(), 1U);
        EXPECT_EQ(object2->refCount(), 3U);

        refPtr1 = refPtr3;
        EXPECT_TRUE(refPtr1.get() == object3);
        EXPECT_TRUE(refPtr3.get() == object3);
        EXPECT_EQ(object2->refCount(), 2U);
        EXPECT_EQ(object3->refCount(), 3U);
    }
    EXPECT_EQ(object1->refCount(), 1U);
    EXPECT_EQ(object2->refCount(), 1U);
    EXPECT_EQ(object3->refCount(), 1U);

    object1->unref();
    object2->unref();
    object3->unref();
}
