/*~-------------------------------------------------------------------------~~*
 * Copyright (c) 2016 Los Alamos National Laboratory, LLC
 * All rights reserved
 *~-------------------------------------------------------------------------~~*/
////////////////////////////////////////////////////////////////////////////////
/// \file
/// \brief Define the main types for the hydro solver.
////////////////////////////////////////////////////////////////////////////////

#pragma once

// user includes
#include <flecsale/common/types.h>
#include <flecsale/eqns/euler_eqns.h>
#include <flecsale/eqns/flux.h>
#include <flecsale/eos/ideal_gas.h>
#include <flecsale/math/general.h>
#include <flecsale/utils/trivial_string.h>

#include <flecsale/mesh/burton/burton.h>

#include "../common/utils.h"

namespace apps {
namespace hydro {

// some namespace aliases
namespace common= flecsale::common;
namespace mesh  = flecsale::mesh;
namespace math  = flecsale::math;
namespace utils = flecsale::utils;
namespace geom  = flecsale::geom;
namespace eos   = flecsale::eos;
namespace eqns  = flecsale::eqns;

// the handle type
#if FLECSI_RUNTIME_MODEL == FLECSI_RUNTIME_MODEL_legion
template<typename T>
using dense_handle_w__ =
  flecsi::data::legion::dense_handle_t<T, flecsi::wo, flecsi::wo, flecsi::ro>;

template<typename T>
using dense_handle_rw__ =
  flecsi::data::legion::dense_handle_t<T, flecsi::rw, flecsi::rw, flecsi::ro>;

template<typename T>
using dense_handle_r__ =
  flecsi::data::legion::dense_handle_t<T, flecsi::ro, flecsi::ro, flecsi::ro>;
#elif FLECSI_RUNTIME_MODEL == FLECSI_RUNTIME_MODEL_mpi
template<typename T>
using dense_handle_w__ =
  flecsi::data::mpi::dense_handle_t<T, flecsi::wo, flecsi::wo, flecsi::ro>;

template<typename T>
using dense_handle_rw__ =
  flecsi::data::mpi::dense_handle_t<T, flecsi::rw, flecsi::rw, flecsi::ro>;

template<typename T>
using dense_handle_r__ =
  flecsi::data::mpi::dense_handle_t<T, flecsi::ro, flecsi::ro, flecsi::ro>;
#endif

template<typename DC>
using client_handle_w__ = flecsi::data_client_handle__<DC, flecsi::wo>;

template<typename DC>
using client_handle_r__ = flecsi::data_client_handle__<DC, flecsi::ro>;


// mesh and some underlying data types
template <std::size_t N>
using mesh__ = typename mesh::burton::burton_mesh_t<N>;

using size_t = common::size_t;
using real_t = common::real_t;

using eos_t = eos::ideal_gas_t<real_t>;

template< std::size_t N >
using eqns__ = typename eqns::euler_eqns_t<real_t, N>;

template< std::size_t N >
using flux_data__ = typename eqns__<N>::flux_data_t;

template< std::size_t N >
using state_data__ = typename eqns__<N>::state_data_t;

// explicitly use some other stuff
using std::cout;
using std::cerr;
using std::endl;

//! \brief a class to distinguish between different types of 
//!   update errors.
enum class solution_error_t 
{
  unphysical, variance, ok
};

//! \brief a class to distinguish between different types of 
//!   restarting modes.
enum class mode_t 
{
  normal, retry, restart, quit
};

//! a trivially copyable character array
using char_array_t = utils::trivial_string_t;

////////////////////////////////////////////////////////////////////////////////
//! \brief alias the flux function
//! Change the called function to alter the flux evaluation.
////////////////////////////////////////////////////////////////////////////////
template< typename E, typename UL, typename UR, typename V >
auto flux_function( UL && left_state, UR && right_state, V && norm )
{ 
  return 
    eqns::hlle_flux<E>( std::forward<UL>(left_state), 
                        std::forward<UR>(right_state), 
                        std::forward<V>(norm) ); 
}

////////////////////////////////////////////////////////////////////////////////
//! \brief alias the boundary flux function
//! Change the called function to alter the flux evaluation.
////////////////////////////////////////////////////////////////////////////////
template< typename E, typename U, typename V >
auto boundary_flux( U && state, V && norm )
{ 
  return 
    E::wall_flux( std::forward<U>(state), std::forward<V>(norm) ); 
}

////////////////////////////////////////////////////////////////////////////////
//! \brief Pack data into a tuple
//! Change the called function to alter the flux evaluation.
////////////////////////////////////////////////////////////////////////////////
template< typename T, typename...ARGS >
decltype(auto) pack( T && loc, ARGS&&... args )
{ 
  return 
    std::forward_as_tuple( std::forward<ARGS>(args)(std::forward<T>(loc))... ); 
}

} // namespace hydro
} // namespace apps
