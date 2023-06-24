#include <lib/CCircularBuffer/CCircularBuffer.h>
#include <lib/CCircularBufferExt/CCircularBufferExt.h>

#include <gtest/gtest.h>

TEST(CCircularBufferTest, EmptyValid) {
  CCircularBuffer<int> c_buffer;
  GTEST_ASSERT_TRUE(c_buffer.Empty());
}

TEST(CCircularBufferTest, EmptyValid2) {
  CCircularBuffer<int> c_buffer(6);
  GTEST_ASSERT_TRUE(c_buffer.Empty());
}

TEST(CCircularBufferTest, EmptyNoValid) {
  CCircularBuffer<int> c_buffer(6, 1);
  GTEST_ASSERT_FALSE(c_buffer.Empty());
}


TEST(CCircularBufferTest, DifferentConstructorTest) {
  CCircularBuffer<int> c_circular_buffer1(3, 2);

  GTEST_ASSERT_EQ(CCircularBuffer<int>({2, 2, 2}), c_circular_buffer1);
}


TEST(CCircularBufferTest, AssignTest) {
  CCircularBuffer<int> c_buffer1(2, 1);
  c_buffer1.Assign(3, 2);
  GTEST_ASSERT_EQ(CCircularBuffer<int>({2, 2, 2}), c_buffer1);
}


TEST(CCircularBufferTest, AssignTest2) {
  CCircularBuffer<int> c_buffer1(2, 1);
  c_buffer1.Assign(1, 2);
  GTEST_ASSERT_EQ(CCircularBuffer<int>({2}), c_buffer1);
}


TEST(CCircularBufferTest, PushTest) {
  CCircularBuffer<int> c_buffer(6);
  c_buffer.PushFront(3);
  c_buffer.PushBack(4);
  GTEST_ASSERT_EQ(c_buffer.Front(), 3);
  GTEST_ASSERT_EQ(c_buffer.Back(), 4);
  GTEST_ASSERT_FALSE(c_buffer.Empty());
}

TEST(CCircularBufferTest, PositionTest) {
  CCircularBuffer<int> c_buffer(6);
  c_buffer.PushFront(1);
  c_buffer.PushBack(2);
  GTEST_ASSERT_EQ(c_buffer[0], 1);
  GTEST_ASSERT_EQ(c_buffer[1], 2);
  GTEST_ASSERT_FALSE(c_buffer.Empty());
}


TEST(CCircularBufferTest, EraseTest) {
  CCircularBuffer<int> c_buffer(10);
  c_buffer.PushBack(1);
  c_buffer.PushBack(2);
  c_buffer.PushBack(3);
  c_buffer.PushBack(4);
  auto x = std::find(c_buffer.begin(), c_buffer.end(), 3);
  c_buffer.Erase(x);

  GTEST_ASSERT_EQ(CCircularBuffer<int>({1, 2, 4}), c_buffer);
}

TEST(CCircularBufferTest, EraseRangeTest) {
  CCircularBuffer<int> c_buffer(10);
  c_buffer.PushBack(1);
  c_buffer.PushBack(2);
  c_buffer.PushBack(3);
  c_buffer.PushBack(4);
  c_buffer.PushBack(5);
  c_buffer.PushBack(6);
  auto x1 = std::find(c_buffer.begin(), c_buffer.end(), 2);
  auto x2 = std::find(c_buffer.begin(), c_buffer.end(), 5);
  c_buffer.Erase(x1, x2);

  GTEST_ASSERT_EQ(CCircularBuffer<int>({1, 5, 6}), c_buffer);
}


TEST(CCircularBufferTest, InsertTest) {
  CCircularBuffer<int> c_buffer(6);
  c_buffer.PushBack(2);
  c_buffer.PushBack(3);
  c_buffer.PushBack(4);
  c_buffer.PushBack(5);
  auto x = std::find(c_buffer.begin(), c_buffer.end(), 3);
  c_buffer.Insert(x, 3, 1);

  GTEST_ASSERT_EQ(CCircularBuffer<int>({1, 1, 1, 3, 4, 5}), c_buffer);
}

TEST(CCircularBufferExtTest, InsertTest2) {
  CCircularBufferExt<int> c_buffer({2, 3, 4, 5});
  auto x = std::find(c_buffer.begin(), c_buffer.end(), 3);
  c_buffer.Insert(x, {101, 102});
  std::string ans;

  GTEST_ASSERT_EQ(CCircularBufferExt<int>({2, 101, 102, 3, 4, 5}), c_buffer);
}


TEST(CCircularBufferExtTest, PushBackResizeTest) {
  CCircularBufferExt<int> c_buffer(1);
  c_buffer.PushBack(2);
  GTEST_ASSERT_EQ(c_buffer.Size(), 1);
  c_buffer.PushBack(3);
  GTEST_ASSERT_EQ(c_buffer.Size(), 2);
}

TEST(CCircularBufferTest, SortTest) {
  CCircularBuffer<std::string> c_buffer{"2", "3", "1", "0"};
  std::string ans;

  std::sort(c_buffer.begin(), c_buffer.end());

  GTEST_ASSERT_EQ(CCircularBufferExt<std::string>({"0", "1", "2", "3"}), c_buffer);
}


TEST(CCircularBufferTest, CopyConstructorTest) {
  CCircularBuffer<int> c_buffer1(2, 1);
  CCircularBuffer<int> c_buffer2(c_buffer1);
  GTEST_ASSERT_EQ(c_buffer1, c_buffer2);
}

TEST(CCircularBufferTest, SwapTest) {
  std::vector<int> ans1({1, 2});
  std::vector<int> ans2({10, 11});
  CCircularBuffer<int> c_buffer1({1, 2});
  CCircularBuffer<int> c_buffer2({10, 11});
  swap(c_buffer1, c_buffer2);
  for (int i = 0; i < ans1.size(); ++i) {
    ASSERT_EQ(ans1[i], c_buffer2[i]);
  }

  c_buffer1.swap(c_buffer2);
  for (int i = 0; i < ans1.size(); ++i) {
    ASSERT_EQ(ans1[i], c_buffer1[i]);
  }
}

TEST(CCircularBufferTest, CIteratorTest) {
  CCircularBuffer<int> c_buffer{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  int pos = 0;
  for (int value : c_buffer) {
    ASSERT_EQ(value, pos++);
  }
}


TEST(CCircularBufferTest, EqualTests) {
  CCircularBuffer<int> c_buffer1({1, 2});
  CCircularBuffer<int> c_buffer2({1, 2});
  CCircularBuffer<int> c_buffer3({2, 1});
  CCircularBuffer<int> c_buffer4({2, 1, 2});

  ASSERT_TRUE(c_buffer1 == c_buffer2);
  ASSERT_TRUE(c_buffer1 != c_buffer3);
  ASSERT_TRUE(c_buffer1 != c_buffer4);
}

TEST(IteratorTest, EqualTests) {
  CCircularBuffer<int> c_buffer1({0, 1, 2, 3});
  CCircularBuffer<int>::iterator my_iterator = c_buffer1.begin();
  CCircularBuffer<int>::const_iterator const_my_iterator = c_buffer1.cbegin();

  my_iterator += 3;
  const_my_iterator += 3;
  ASSERT_TRUE(my_iterator == c_buffer1.begin() + 3);
  ASSERT_TRUE(const_my_iterator == c_buffer1.cbegin() + 3);
}


TEST(IteratorTest, EqualTests2) {
  CCircularBuffer<int> circular_buffer1({0, 1, 2, 3});
  CCircularBuffer<int>::iterator my_iterator = circular_buffer1.begin();
  CCircularBuffer<int>::const_iterator const_my_iterator = circular_buffer1.cbegin();

  my_iterator += 3;
  const_my_iterator += 3;

  ASSERT_TRUE(my_iterator > circular_buffer1.begin());
  ASSERT_TRUE(circular_buffer1.begin() < my_iterator);
  ASSERT_TRUE(circular_buffer1.begin() != my_iterator);
  ASSERT_FALSE(circular_buffer1.begin() == my_iterator);

  ASSERT_TRUE(const_my_iterator > circular_buffer1.cbegin());
  ASSERT_TRUE(circular_buffer1.cbegin() < const_my_iterator);
  ASSERT_TRUE(circular_buffer1.cbegin() != const_my_iterator);
  ASSERT_FALSE(circular_buffer1.cbegin() == const_my_iterator);

  my_iterator -= 3;
  const_my_iterator -= 3;

  ASSERT_TRUE(circular_buffer1.begin() <= my_iterator);
  ASSERT_TRUE(circular_buffer1.begin() >= my_iterator);

  ASSERT_TRUE(circular_buffer1.cbegin() <= const_my_iterator);
  ASSERT_TRUE(circular_buffer1.cbegin() >= const_my_iterator);
}

TEST(CCircularBufferExtTest, MaxElementTest) {
  CCircularBufferExt<int> buffer_ext{1, 4, 3, 2};
  ASSERT_EQ(*std::max_element(buffer_ext.begin(), buffer_ext.end()), 4);
  ASSERT_EQ(*std::max_element(buffer_ext.begin() + 2, buffer_ext.end()), 3);
}


TEST(CCircularBufferExtTest, PopBackTests) {
  CCircularBufferExt<int> buffer_ext;

  for (int i = 0; i < 10; ++i) buffer_ext.PushBack(i);
  for (int i = 0; i < 10; ++i) buffer_ext.PopBack();

  ASSERT_TRUE(buffer_ext.Empty());
}

TEST(CCircularBufferExtTest, PopFrontTests) {
  CCircularBufferExt<int> buffer_ext;

  for (int i = 0; i < 10; ++i) buffer_ext.PushBack(i);
  for (int i = 0; i < 10; ++i) buffer_ext.PopFront();

  ASSERT_TRUE(buffer_ext.Empty());
}

TEST(CCircularBufferExtTest, MaxSize) {
  CCircularBufferExt<int> buffer_ext_i;
  CCircularBufferExt<char> buffer_ext_c;

  ASSERT_EQ(buffer_ext_i.MaxSize(), UINT64_MAX / sizeof(int));
  ASSERT_EQ(buffer_ext_c.MaxSize(), UINT64_MAX / sizeof(char));
}
