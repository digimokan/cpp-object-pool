/*******************************************************************************
* DOCTEST CONFIG
*******************************************************************************/

#include "doctest.h"

/*******************************************************************************
* USER INCLUDES
*******************************************************************************/

#include "PooledObject.hpp"

/*******************************************************************************
* TEST CASES
*******************************************************************************/

TEST_CASE("create non-pooled objects on stack") {

  class Derived : public PooledObject<Derived, 10> {
  public:
    explicit Derived (int x) : x{x} { }
    int get_x () const { return x; }
  private:
    int x;
  };

  Derived d1{7};
  Derived d2{8};

  CHECK_EQ(d1.get_x(), 7);
  CHECK_EQ(d2.get_x(), 8);

}

TEST_CASE("allocate max pooled objects on heap") {

  class Derived : public PooledObject<Derived, 10> {
  public:
    explicit Derived (int x) : x{x} { }
    int get_x () const { return x; }
  private:
    int x;
  };

  Derived* d1 = new Derived{1};
  Derived* d2 = new Derived{2};
  Derived* d3 = new Derived{3};
  Derived* d4 = new Derived{4};
  Derived* d5 = new Derived{5};
  Derived* d6 = new Derived{6};
  Derived* d7 = new Derived{7};
  Derived* d8 = new Derived{8};
  Derived* d9 = new Derived{9};
  Derived* d10 = new Derived{10};

  CHECK_EQ(d1->get_x(), 1);
  CHECK_EQ(d2->get_x(), 2);
  CHECK_EQ(d3->get_x(), 3);
  CHECK_EQ(d4->get_x(), 4);
  CHECK_EQ(d5->get_x(), 5);
  CHECK_EQ(d6->get_x(), 6);
  CHECK_EQ(d7->get_x(), 7);
  CHECK_EQ(d8->get_x(), 8);
  CHECK_EQ(d9->get_x(), 9);
  CHECK_EQ(d10->get_x(), 10);

}

TEST_CASE("allocate/deallocate batches of objects") {

  class Derived : public PooledObject<Derived, 4> {
  public:
    explicit Derived (int x) : x{x} { }
    int get_x () const { return x; }
  private:
    int x;
  };

  Derived* d1 = new Derived{1};
  Derived* d2 = new Derived{2};
  Derived* d3 = new Derived{3};
  Derived* d4 = new Derived{4};
  CHECK_EQ(d1->get_x(), 1);
  CHECK_EQ(d2->get_x(), 2);
  CHECK_EQ(d3->get_x(), 3);
  CHECK_EQ(d4->get_x(), 4);

  delete d1;
  delete d2;
  delete d3;
  delete d4;

  d1 = new Derived{10};
  d2 = new Derived{20};
  d3 = new Derived{30};
  d4 = new Derived{40};
  CHECK_EQ(d1->get_x(), 10);
  CHECK_EQ(d2->get_x(), 20);
  CHECK_EQ(d3->get_x(), 30);
  CHECK_EQ(d4->get_x(), 40);

  delete d1;
  delete d3;
  delete d4;
  delete d2;

  d1 = new Derived{100};
  d2 = new Derived{200};
  d3 = new Derived{300};
  d4 = new Derived{400};
  CHECK_EQ(d1->get_x(), 100);
  CHECK_EQ(d2->get_x(), 200);
  CHECK_EQ(d3->get_x(), 300);
  CHECK_EQ(d4->get_x(), 400);

}

TEST_CASE("check correct peak_size") {

  class Derived : public PooledObject<Derived, 3> {
  public:
    explicit Derived (int x) : x{x} { }
    int get_x () const { return x; }
  private:
    int x;
  };

  Derived* d1;
  Derived* d2;
  Derived* d3;
  d1 = new Derived{1};
  delete d1;
  d1 = new Derived{1};
  d2 = new Derived{2};
  delete d1;
  delete d2;
  d1 = new Derived{1};
  d2 = new Derived{2};
  d3 = new Derived{3};
  delete d1;
  delete d2;
  delete d3;

  CHECK_EQ(Derived::get_peak_size(), 3);

}

TEST_CASE("verify manual pool allocation") {

  class Derived : public PooledObject<Derived, 3> {
  public:
    explicit Derived (int x) : x{x} { }
    int get_x () const { return x; }
  private:
    int x;
  };

  Derived::manual_allocate_object_pool();
  Derived* d1{nullptr};
  d1 = new Derived{1};
  CHECK_NE(d1, nullptr);

  class AltDerived : public PooledObject<Derived, 3> {
  public:
    explicit AltDerived (int x) : x{x} { }
    int get_x () const { return x; }
  private:
    int x;
  };

  AltDerived* ad1{nullptr};
  ad1 = new AltDerived{1};
  AltDerived::manual_allocate_object_pool();
  CHECK_NE(ad1, nullptr);


}

TEST_CASE("over-allocate by one object"
          * doctest::skip(true)) {

  class Derived : public PooledObject<Derived, 3> {
  public:
    explicit Derived (int x) : x{x} { }
    int get_x () const { return x; }
  private:
    int x;
  };

  Derived* d1 = new Derived{1};
  Derived* d2 = new Derived{2};
  Derived* d3 = new Derived{3};
  CHECK_THROWS(new Derived{4});
  delete d1;
  delete d2;
  delete d3;

}

TEST_CASE("create simple objects on heap") {

  class Simple : public PooledObject<Simple, 10> {
    int x;
  };

  Simple* s1 = new Simple{};
  Simple* s2 = new Simple{};
  Simple* s3 = new Simple{};
  delete s1;
  delete s2;
  delete s3;

}

