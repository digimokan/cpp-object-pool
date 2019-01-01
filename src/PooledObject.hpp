/*******************************************************************************
module:   PooledObject
author:   digimokan
date:     19 NOV 2018 (created)
purpose:  make new/delete calls utilize an object pool
usage:    class MyObject : PooledObject<MyObject, 10> { ...
          --> for an object pool of 10 objects
          --> object pool created with the first call of "new MyObject"
          --> object pool destroyed on program termination
*******************************************************************************/

#ifndef POOLED_OBJECT_HPP
#define POOLED_OBJECT_HPP 1

/*******************************************************************************
* SYSTEM INCLUDES
*******************************************************************************/

#include <cstddef>
#include <mutex>

/*******************************************************************************
* USER INCLUDES
*******************************************************************************/

#include "ObjectPool.hpp"

/*******************************************************************************
* INTERFACE
*******************************************************************************/

template <typename Obj, size_t initial_pool_size>
class PooledObject {

public:

  // constructors
  PooledObject ();

  // destructor
  virtual ~PooledObject () = default;

  // operators
  PooledObject (const PooledObject& in) = default;
  PooledObject& operator= (const PooledObject& rh) = default;
  PooledObject (PooledObject&& in) noexcept = default;
  PooledObject& operator= (PooledObject&& rh) noexcept = default;
  void* operator new (std::size_t sz);
  void operator delete (void* ptr);

  // static specialized methods
  static void manual_allocate_object_pool ();
  static size_t get_peak_size ();

private:

  // static fields
  static ObjectPool<Obj, initial_pool_size>* object_pool;
  static std::once_flag init_flag;

  // static helper methods
  static void init_and_scope_pool ();
  static void delete_pool ();

};

/*******************************************************************************
* IMPLEMENTATION
*******************************************************************************/

#include "PooledObject.hxx"

/*******************************************************************************
* END
*******************************************************************************/

#endif // POOLED_OBJECT_HPP

