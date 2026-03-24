#include <gtest/gtest.h>

#include <Foundation/NonNull.h>
#include <Foundation/RefCounted.h>
#include <Foundation/RefPtr.h>

TEST(NonNullTest, ConstructsWithNonNullValue)
{
    int value = 42;
    NonNull<int*> nonNull(&value);
    EXPECT_EQ(nonNull.get(), &value);
}

TEST(NonNullTest, ConstructsFromRefPtr)
{
    struct Object : RefCounted {};

    Object* object = new Object();

    NonNull<RefPtr<Object>> nonNullRefPtr(object);
    EXPECT_EQ(nonNullRefPtr.get(), object);

    object->unref();
}

TEST(NonNullTest, CopyConstructsNonNull)
{
    int value = 42;
    NonNull<int*> nonNull1(&value);
    NonNull<int*> nonNull2(nonNull1);
    EXPECT_EQ(nonNull2.get(), &value);
}

TEST(NonNullTest, CopyConstructsFromNonNullOfDerivedType)
{
    struct Base { int x; };
    struct Derived : Base { int y; };

    Derived object;
    NonNull<Derived*> nonNull1(&object);
    NonNull<Base*> nonNull2(nonNull1);
    EXPECT_EQ(nonNull2.get(), &object);
}
