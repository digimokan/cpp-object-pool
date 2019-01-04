/*******************************************************************************
module:   ObjectPool
author:   digimokan
date:     19 NOV 2018 (created)
purpose:  pool of objects for new/delete heap allocations
*******************************************************************************/

#ifndef OBJECT_POOL_HPP
#define OBJECT_POOL_HPP 1

/*******************************************************************************
* SYSTEM INCLUDES
*******************************************************************************/

#include <cstddef>
#include <mutex>
#include <type_traits>

/*******************************************************************************
* Types
*******************************************************************************/

template<typename T, size_t init_num_obj>
union StoredObj {
  T obj;
  StoredObj* next;
};

/*******************************************************************************
* INTERFACE
*******************************************************************************/

template <typename T, size_t init_num_obj>
class ObjectPool {

public:

  // constructors
  ObjectPool ();

  // destructor
  ~ObjectPool ();

  // operators
  ObjectPool (const ObjectPool& in) = default;
  ObjectPool& operator= (const ObjectPool& rh) = default;
  ObjectPool (ObjectPool&& in) = default;
  ObjectPool& operator= (ObjectPool&& rh) = default;

  // specialized
  void* allocate ();
  void deallocate (void* ptr);
  size_t get_peak_size () const;

private:

  // types
  using Storage = typename std::aligned_storage<sizeof(T), std::alignment_of<T>::value>::type;

  // fields
  Storage* pool_blocks;
  StoredObj<T, init_num_obj>* free_list_head;
  std::mutex mtx;
  size_t size;
  size_t peak_size;

  // helper methods
  void init_blocks ();
  StoredObj<T, init_num_obj>* get_block_addr (size_t index);
  void set_block_next (StoredObj<T, init_num_obj>* block, StoredObj<T, init_num_obj>* next_link);
  void inc_sizes ();
  void dec_sizes ();

};

/*******************************************************************************
* IMPLEMENTATION
*******************************************************************************/

#include "ObjectPool.hxx"

/*******************************************************************************
* END
*******************************************************************************/

#endif // OBJECT_POOL_HPP

