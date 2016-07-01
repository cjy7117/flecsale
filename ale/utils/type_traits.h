/*~-------------------------------------------------------------------------~~*
 *     _   ______________     ___    __    ______
 *    / | / / ____/ ____/    /   |  / /   / ____/
 *   /  |/ / / __/ /  ______/ /| | / /   / __/   
 *  / /|  / /_/ / /__/_____/ ___ |/ /___/ /___   
 * /_/ |_/\____/\____/    /_/  |_/_____/_____/   
 * 
 * Copyright (c) 2016 Los Alamos National Laboratory, LLC
 * All rights reserved
 *~-------------------------------------------------------------------------~~*/
/*!
 *
 * \file check_types.h
 * 
 * \brief Statically check if all arguments are of the same type.
 *
 ******************************************************************************/
#pragma once

// system inculdes
#include <type_traits>

// user includes
#include "detail/type_traits.h"


namespace ale {
namespace utils {


////////////////////////////////////////////////////////////////////////////////
//! \brief Test to see if all variadic template arguments are of type Target
////////////////////////////////////////////////////////////////////////////////
template<typename Target, typename... Ts>
using are_type_t = detail::and_< 
  std::is_same< std::decay_t<Ts>, std::decay_t<Target> >... >;



////////////////////////////////////////////////////////////////////////////////
//! \brief check if a particular type is an iterator
////////////////////////////////////////////////////////////////////////////////

template <typename T>
struct is_iterator {
  static char test(...);

  template <typename U,
            typename=typename std::iterator_traits<U>::difference_type,
            typename=typename std::iterator_traits<U>::pointer,
            typename=typename std::iterator_traits<U>::reference,
            typename=typename std::iterator_traits<U>::value_type,
            typename=typename std::iterator_traits<U>::iterator_category
  > static long test(U&&);

  constexpr static bool value = std::is_same<decltype(test(std::declval<T>())),long>::value;
};

////////////////////////////////////////////////////////////////////////////////
//! \brief helper function for checking if iterator
////////////////////////////////////////////////////////////////////////////////
template< typename T >
constexpr bool is_iterator_v = is_iterator<T>::value;

////////////////////////////////////////////////////////////////////////////////
//! \brief check if a particular type is callable
////////////////////////////////////////////////////////////////////////////////

template<typename T>
struct is_callable_impl {
private:
  typedef char(&yes)[1];
  typedef char(&no)[2];

  struct Fallback { void operator()(); };
  struct Derived : T, Fallback { };

  template<typename U, U> struct Check;

  template<typename>
  static yes test(...);

  template<typename C>
  static no test(Check<void (Fallback::*)(), &C::operator()>*);

public:
  static const bool value = sizeof(test<Derived>(0)) == sizeof(yes);
};

template<typename T>
struct is_callable : std::conditional<
  std::is_class<T>::value,
  is_callable_impl<T>,
  std::false_type 
>::type
{ };

////////////////////////////////////////////////////////////////////////////////
//! \brief helper function for checking if callable
////////////////////////////////////////////////////////////////////////////////
template< typename T >
constexpr bool is_callable_v = is_callable<T>::value;

////////////////////////////////////////////////////////////////////////////////
//! \brief check if a particular type is a container
////////////////////////////////////////////////////////////////////////////////
template<typename T, typename _ = void>
struct is_container : std::false_type {};

template<typename... Ts>
struct is_container_helper {};

template<typename T>
struct is_container<
  T,
  std::conditional_t<
    false,
    is_container_helper<
      typename T::value_type,
      typename T::size_type,
      typename T::allocator_type,
      typename T::iterator,
      typename T::const_iterator,
      decltype(std::declval<T>().size()),
      decltype(std::declval<T>().begin()),
      decltype(std::declval<T>().end()),
      decltype(std::declval<T>().cbegin()),
      decltype(std::declval<T>().cend()),
      decltype(std::declval<T>().data())
    >,
    void
  >
> : public std::true_type {};

////////////////////////////////////////////////////////////////////////////////
//! \brief helper function for checking if container
////////////////////////////////////////////////////////////////////////////////
template< typename T >
constexpr bool is_container_v = is_container<T>::value;

////////////////////////////////////////////////////////////////////////////////
//! \brief extract the extents from an array
////////////////////////////////////////////////////////////////////////////////
template< typename T, std::size_t... I >
constexpr auto extract_extents_helper( std::index_sequence<I...> )
{
  constexpr std::size_t extents[ std::rank<T>::value ] = 
    { std::extent<T, I>::value... };
  return  extents;
};


template< typename T >
constexpr auto & extract_extents() 
{
  using Indices = std::make_index_sequence< std::rank<T>::value >;
  return extract_extents_helper< T >( Indices{} );
}

} // namespace
} // namespace



/*~------------------------------------------------------------------------~--*
 * Formatting options
 * vim: set tabstop=2 shiftwidth=2 expandtab :
 *~------------------------------------------------------------------------~--*/