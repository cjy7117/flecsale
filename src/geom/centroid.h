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
 * \file centroid.h
 * 
 * \brief Evaluate the centroid of a list of points.
 *
 ******************************************************************************/
#pragma once


// user includes
#include "../std/type_traits.h"
#include "../utils/check_types.h"
#include "detail/centroid.h"


namespace ale {
namespace geom {




//! \brief compute centroid for 2d
//! \remark all arguments must be of the same type
//! \remark this one is used for three or more arguments
template< class T, class... Types >
constexpr
std::enable_if_t< 
  (sizeof...(Types) > 2 && utils::are_type_t<T,Types...>::value && 
   std::decay_t<T>::size() == 2), 
  std::decay_t<T> >
centroid( T && t, Types&&... args )
{ 
  // initialize centroid and volume
  std::decay_t<T> cx( 0 );
  typename std::decay_t<T>::value_type vol(0);
  // call main implementation, sticking first point back on end
  detail::centroid_2d( cx, vol, 
                       std::forward<T>(t), 
                       std::forward<Types>(args)..., 
                       std::forward<T>(t) );
  cx /=  3 * vol; // divide by volume
  return cx;
}


//! \brief compute centroid for 2d or 3d
//! \remark this is a runtime computation
template< 
  typename T, typename... Args, 
  template <typename, typename...> class V 
>
auto centroid_2d( const V<T, Args...> & points )
{ 
  assert( points.size() > 2 && "not enough points for 2d" );

  T cx(0);
  typename T::value_type a(0);

  auto n = points.size();

  auto po = &points.back();
  for ( auto i=0; i<n; i++ ) {
    auto pn = &points[i];
    auto tmp = (*po)[0]*(*pn)[1] - (*pn)[0]*(*po)[1];
    a += tmp;
    cx[0] += tmp * ( (*po)[0] + (*pn)[0] );
    cx[1] += tmp * ( (*po)[1] + (*pn)[1] );
    po = pn;
  }

  cx /= 3 * a;

  return cx;
}


//! \brief compute centroid for 2d or 3d
//! \remark this is a runtime computation
template< 
  typename T, typename... Args, 
  template <typename, typename...> class V 
>
auto centroid( const V<T, Args...> & points )
{ 
  assert( points.size() > 0 && "not enough points" );
  auto dim = points[0].size();
  if ( dim == 2 )
    return centroid_2d( points );
  else
    assert( false && "unsuported dimemsion" );
}




} // namespace geom
} // namespace ale
