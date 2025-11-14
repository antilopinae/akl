#pragma once

#include "__sync_definitions.hpp"
#include "definitions.hpp"

namespace akl {

/*!
* \ingroup util atomic instruction that is equivalent to the following:
 \code
 if (a==oldval) {
   a = newval;
   return true;
 }
 else {
   return false;
}
\endcode
*/
template <typename T>
bool atomic_compare_and_swap(T& a, T oldval, T newval) {
    return bool_compare_and_swap(&a, oldval, newval);
}

/*!
* \ingroup util
 atomic instruction that is equivalent to the following:
 \code
 if (a==oldval) {
   a = newval;
   return true;
 }
 else {
   return false;
}
\endcode
*/
template <typename T>
bool atomic_compare_and_swap(volatile T& a, T oldval, T newval) {
    return bool_compare_and_swap(&a, oldval, newval);
}

/*!
* \ingroup util
 atomic instruction that is equivalent to the following:
 \code
 if (a==oldval) {
   a = newval;
   return true;
 }
 else {
   return false;
}
\endcode
*/
template <>
inline bool atomic_compare_and_swap(volatile double& a, double oldval, double newval) {
    volatile uint64_t* a_ptr = reinterpret_cast<volatile uint64_t*>(&a);
    const uint64_t* oldval_ptr = reinterpret_cast<const uint64_t*>(&oldval);
    const uint64_t* newval_ptr = reinterpret_cast<const uint64_t*>(&newval);
    return bool_compare_and_swap(a_ptr, *oldval_ptr, *newval_ptr);
}

/*!
 * \ingroup util
   atomic instruction that is equivalent to the following:
   \code
   if (a==oldval) {
     a = newval;
     return true;
   }
   else {
     return false;
  }
  \endcode
*/
template <>
inline bool atomic_compare_and_swap(volatile float& a, float oldval, float newval) {
    volatile uint32_t* a_ptr = reinterpret_cast<volatile uint32_t*>(&a);
    const uint32_t* oldval_ptr = reinterpret_cast<const uint32_t*>(&oldval);
    const uint32_t* newval_ptr = reinterpret_cast<const uint32_t*>(&newval);
    return bool_compare_and_swap(a_ptr, *oldval_ptr, *newval_ptr);
}

/*!
 * \ingroup util
 * \brief Atomically exchanges the values of a and b.
 * \warning This is not a full atomic exchange. Read of a,
 * and the write of b into a is atomic. But the write into b is not.
 */
template <typename T>
void atomic_exchange(T& a, T& b) {
    b = lock_test_and_set(&a, b);
}

/**
 * \ingroup util
 * \brief Atomically exchanges the values of a and b.
 * \warning This is not a full atomic exchange. Read of a,
 * and the write of b into a is atomic. But the write into b is not.
 */
template <typename T>
void atomic_exchange(volatile T& a, T& b) {
    b = lock_test_and_set(&a, b);
}

/**
 * \ingroup util
 * \brief Atomically sets a to the newval, returning the old value
 */
template <typename T>
T fetch_and_store(T& a, const T& newval) {
    return lock_test_and_set(&a, newval);
}
}  // namespace akl
