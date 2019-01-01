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

#ifndef POOLED_OBJECT_HXX
#define POOLED_OBJECT_HXX 1

/*******************************************************************************
* INTERFACE
*******************************************************************************/

#include "PooledObject.hpp"

/*******************************************************************************
* SYSTEM INCLUDES
*******************************************************************************/

#include <cassert>
#include <cstddef>
#include <cstdlib>
#include <mutex>

/*******************************************************************************
* STATIC FIELDS
*******************************************************************************/

template <typename Obj, size_t initial_pool_size>
ObjectPool<Obj, initial_pool_size>* PooledObject<Obj, initial_pool_size>::object_pool;

template <typename Obj, size_t initial_pool_size>
std::once_flag PooledObject<Obj, initial_pool_size>::init_flag;

/*******************************************************************************
* OPERATORS
*******************************************************************************/

template <typename Obj, size_t initial_pool_size>
void* PooledObject<Obj, initial_pool_size>::operator new (std::size_t sz) {
  std::call_once(PooledObject::init_flag, &PooledObject::init_and_scope_pool);
  return PooledObject::object_pool->allocate();
}

template <typename Obj, size_t initial_pool_size>
static void PooledObject<Obj, initial_pool_size>::operator delete (void* ptr) {
  assert(ptr != nullptr);
  PooledObject::object_pool->deallocate(ptr);
}

/*******************************************************************************
* CONSTRUCTORS
*******************************************************************************/

template <typename Obj, size_t initial_pool_size>
PooledObject<Obj, initial_pool_size>::PooledObject () {
  std::call_once(PooledObject::init_flag, &PooledObject::init_and_scope_pool);
}

/*******************************************************************************
* STATIC SPECIALIZED METHODS
*******************************************************************************/

template <typename Obj, size_t initial_pool_size>
void PooledObject<Obj, initial_pool_size>::manual_allocate_object_pool () {
  std::call_once(PooledObject::init_flag, &PooledObject::init_and_scope_pool);
}

template <typename Obj, size_t initial_pool_size>
size_t PooledObject<Obj, initial_pool_size>::get_peak_size () {
  return PooledObject::object_pool->get_peak_size();
}

/*******************************************************************************
* STATIC HELPER METHODS
*******************************************************************************/

template <typename Obj, size_t initial_pool_size>
void PooledObject<Obj, initial_pool_size>::init_and_scope_pool () {
  PooledObject::object_pool = new ObjectPool<Obj, initial_pool_size>{};
  std::atexit(&PooledObject::delete_pool);
}

template <typename Obj, size_t initial_pool_size>
void PooledObject<Obj, initial_pool_size>::delete_pool () {
  delete PooledObject::object_pool;
}

/*******************************************************************************
* END
*******************************************************************************/

#endif // POOLED_OBJECT_HXX

