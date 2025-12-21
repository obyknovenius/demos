#include <gtest/gtest.h>

#include <Core/RefCounted.h>

TEST(RefCountedTest, ObjectDeletedWhenRefCountReachesZero)
{
    bool deleted = false;

    class TestRefCounted : public RefCounted
    {
    public:
        TestRefCounted(bool& deleted) : deleted { deleted } {}
    protected:
        ~TestRefCounted() override { deleted = true; }
    private:
        bool& deleted;
    };

    auto* obj = new TestRefCounted(deleted);
    EXPECT_FALSE(deleted);

    obj->unref();
    EXPECT_TRUE(deleted);
}
