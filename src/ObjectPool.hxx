/*******************************************************************************
module:   ObjectPool
author:   digimokan
date:     19 NOV 2018 (created)
purpose:  pool of objects for new/delete heap allocations
*******************************************************************************/

#ifndef OBJECT_POOL_HXX
#define OBJECT_POOL_HXX 1

/*******************************************************************************
* SYSTEM INCLUDES
*******************************************************************************/

#include <cassert>
#include <cstddef>
#include <mutex>
#include <new>

/*******************************************************************************
* INTERFACE
*******************************************************************************/

#include "ObjectPool.hpp"

/*******************************************************************************
* CONSTRUCTORS
*******************************************************************************/

template <typename T, size_t init_num_obj>
ObjectPool<T, init_num_obj>::ObjectPool ()
    : pool_blocks{new Storage[init_num_obj]},
      size{0}, peak_size{0} {
  assert(init_num_obj > 0);
  this->init_blocks();
}

/*******************************************************************************
* DESTRUCTOR
*******************************************************************************/

template <typename T, size_t init_num_obj>
ObjectPool<T, init_num_obj>::~ObjectPool () {
  delete[] this->pool_blocks;
}

/*******************************************************************************
* SPECIALIZED METHODS
*******************************************************************************/

template <typename T, size_t init_num_obj>
void* ObjectPool<T, init_num_obj>::allocate () {
  std::lock_guard<std::mutex> kg{this->mtx};
  assert(this->free_list_head != nullptr);
  auto block{ this->free_list_head };
  this->free_list_head = this->free_list_head->next;
  this->inc_sizes();
  return block;
}

template <typename T, size_t init_num_obj>
void ObjectPool<T, init_num_obj>::deallocate (void* ptr) {
  std::lock_guard<std::mutex> kg{this->mtx};
  assert(ptr != nullptr);
  auto block{ reinterpret_cast<StoredObj<T, init_num_obj>*>(ptr) };
  block->next = this->free_list_head;
  this->free_list_head = block;
  this->dec_sizes();
}

template <typename T, size_t init_num_obj>
size_t ObjectPool<T, init_num_obj>::get_peak_size () const {
  return this->peak_size;
}

/*******************************************************************************
* HELPER METHODS
*******************************************************************************/

template <typename T, size_t init_num_obj>
void ObjectPool<T, init_num_obj>::init_blocks () {
  auto prev {this->get_block_addr(0)};
  this->set_block_next(prev, nullptr);
  this->free_list_head = prev;
  for (size_t i = 1; i < init_num_obj; i++) {
    auto curr {this->get_block_addr(i)};
    this->set_block_next(prev, curr);
    prev = curr;
  }
  this->set_block_next(prev, nullptr);
}

template <typename T, size_t init_num_obj>
StoredObj<T, init_num_obj>* ObjectPool<T, init_num_obj>::get_block_addr (size_t index) {
  return reinterpret_cast<StoredObj<T, init_num_obj>*>(std::addressof(this->pool_blocks[index]));
}

template <typename T, size_t init_num_obj>
void ObjectPool<T, init_num_obj>::set_block_next (StoredObj<T, init_num_obj>* block,
    StoredObj<T, init_num_obj>* next_link) {
  block->next = next_link;
}

template <typename T, size_t init_num_obj>
void ObjectPool<T, init_num_obj>::inc_sizes () {
  this->size++;
  if (this->size > this->peak_size)
    this->peak_size = size;
}

template <typename T, size_t init_num_obj>
void ObjectPool<T, init_num_obj>::dec_sizes () {
  this->size--;
}

/*******************************************************************************
* END
*******************************************************************************/

#endif // OBJECT_POOL_HXX

