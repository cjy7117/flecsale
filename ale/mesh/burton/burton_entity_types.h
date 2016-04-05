/*~--------------------------------------------------------------------------~*
 *  @@@@@@@@  @@           @@@@@@   @@@@@@@@ @@
 * /@@/////  /@@          @@////@@ @@////// /@@
 * /@@       /@@  @@@@@  @@    // /@@       /@@
 * /@@@@@@@  /@@ @@///@@/@@       /@@@@@@@@@/@@
 * /@@////   /@@/@@@@@@@/@@       ////////@@/@@
 * /@@       /@@/@@//// //@@    @@       /@@/@@
 * /@@       @@@//@@@@@@ //@@@@@@  @@@@@@@@ /@@
 * //       ///  //////   //////  ////////  //
 *
 * Copyright (c) 2016 Los Alamos National Laboratory, LLC
 * All rights reserved
 *~--------------------------------------------------------------------------~*/
/*!
 * \file burton_entity_types.h
 * \authors bergen
 * \date Initial file creation: Nov 15, 2015
 ******************************************************************************/

#pragma once

//! user includes
#include "flecsi/mesh/mesh_types.h"

#include "../../geom/normal.h"
#include "../../math/math.h"
#include "../../mesh/burton/burton_mesh_traits.h"


namespace ale {
namespace mesh {

////////////////////////////////////////////////////////////////////////////////
// Expose some types
////////////////////////////////////////////////////////////////////////////////

//! the flecsi mesh topology type
using flecsi::mesh_topology_t;
//! the flecsi mesh topology type
using mesh_topology_base_t = flecsi::mesh_topology_base_t;

//! the flexi domain type
using flecsi::domain_;
//! the flexi domain entity type
using flecsi::domain_entity;
//! the flecsi entity group
using flecsi::entity_group;
//! the flecsi mesh enity
using flecsi::mesh_entity_t;


//! some flexi flags
using flecsi::persistent;
using flecsi::flecsi_internal;
using flecsi::flecsi_user_space;

// some flecsi types
using flecsi::id_t;
using flecsi::id_vector_t;

// forward declarations
class burton_corner_t;
class burton_wedge_t;

////////////////////////////////////////////////////////////////////////////////
//! \class burton_vertex_t burton_entity_types.h
//! \brief The burton_vertex_t type provides an interface for managing
//!   geometry and state associated with mesh vertices.
//!
//! \tparam N The domain of the vertex.
////////////////////////////////////////////////////////////////////////////////
class burton_vertex_t
  : public mesh_entity_t<0, burton_mesh_traits_t::num_domains>
{
public:

  //============================================================================
  // Typedefs
  //============================================================================

  //! Type containing coordinates of the vertex.
  using point_t = burton_mesh_traits_t::point_t;

  //! Handle for accessing state at vertex.
  using data_t = burton_mesh_traits_t::data_t;

  //! the bitfield type
  using bitfield_t = burton_mesh_traits_t::bitfield_t;

  //! Number of domains in the burton mesh.
  static constexpr size_t num_domains = burton_mesh_traits_t::num_domains;

  //============================================================================
  // Constructors
  //============================================================================

  //! Constructor
  burton_vertex_t(mesh_topology_base_t & mesh) : mesh_(mesh) {}

  //============================================================================
  // Accessors / Modifiers
  //============================================================================

  //! \brief Set the coordinates for a vertex.
  //! \param[in] coordinates Coordinates value to set for vertex.
  void set_coordinates(const point_t & coordinates)
  {
    auto c = data_t::instance().dense_accessor<point_t, flecsi_internal>(
      "coordinates", mesh_.runtime_id() );
    c[mesh_entity_base_t<num_domains>::template id<0>()] = coordinates;
  } // set_coordinates

  //! \brief Get the coordinates at a vertex from the state handle.
  //! \return coordinates of vertex.
  const point_t & coordinates() const
  {
    const auto c = data_t::instance().dense_accessor<point_t, flecsi_internal>(
      "coordinates", mesh_.runtime_id() );
    return c[mesh_entity_base_t<num_domains>::template id<0>()];
  } // coordinates

  //! \brief Get the coordinates at a vertex from the state handle.
  //! \return coordinates of vertex.
  //! \remark this is the non const version
  point_t & coordinates()
  {
    auto c = data_t::instance().dense_accessor<point_t, flecsi_internal>(
      "coordinates", mesh_.runtime_id() );
    return c[mesh_entity_base_t<num_domains>::template id<0>()];
  } // coordinates

  //! is this a boundary
  bool is_boundary() const
  {
    auto flag =
      data_t::instance().dense_accessor<bitfield_t, flecsi_internal>(
        "point_flags", mesh_.runtime_id() );
    return flag[mesh_entity_base_t<num_domains>::template id<0>()].anybitset();
  }

  //============================================================================
  // Private Data
  //============================================================================

 private:

  //! a reference to the mesh topology
  const mesh_topology_base_t & mesh_;

}; // class burton_vertex_t

////////////////////////////////////////////////////////////////////////////////
//! \class burton_edge_t burton_entity_types.h
//! \brief The burton_edge_t type provides an interface for managing
//!   geometry and state associated with mesh edges.
//!
//! \tparam N The domain of the edge.
////////////////////////////////////////////////////////////////////////////////
struct burton_edge_t
    : public mesh_entity_t<1, burton_mesh_traits_t::num_domains> {

  //============================================================================
  // Typedefs
  //============================================================================

  //! Type of floating point.
  using real_t = burton_mesh_traits_t::real_t;

  //! Type containing coordinates of the vertex.
  using point_t = burton_mesh_traits_t::point_t;

  //! Type vector type.
  using vector_t = burton_mesh_traits_t::vector_t;

  //! the bitfield type
  using bitfield_t = burton_mesh_traits_t::bitfield_t;

  //! Number of domains in the burton mesh.
  static constexpr size_t num_domains = burton_mesh_traits_t::num_domains;

  //! Handle for accessing state at vertex.
  using data_t = burton_mesh_traits_t::data_t;

  //============================================================================
  // Constructors
  //============================================================================

  //! the constructor
  burton_edge_t(mesh_topology_base_t & mesh) : mesh_(mesh) {}

  //============================================================================
  // Accessors / Modifiers
  //============================================================================
  
  //! the edge midpoint
  point_t midpoint() const;

  //! the edge length
  real_t  length() const;

  //! the edge normal
  vector_t normal() const;

  //! is this a boundary
  bool is_boundary() const
  {
    auto flag =
      data_t::instance().dense_accessor<bitfield_t, flecsi_internal>(
        "edge_flags", mesh_.runtime_id() );
    return flag[mesh_entity_base_t<num_domains>::template id<0>()].anybitset();
  }

  //============================================================================
  // Private Data
  //============================================================================

 private:

  //! a reference to the mesh topology
  const mesh_topology_base_t & mesh_;

}; // struct burton_edge_t

////////////////////////////////////////////////////////////////////////////////
//! \class burton_cell_t burton_entity_types.h
//! \brief The burton_cell_t type provides an interface for managing and
//!   geometry and state associated with mesh cells.
////////////////////////////////////////////////////////////////////////////////
struct burton_cell_t
    : public mesh_entity_t<2, burton_mesh_traits_t::num_domains> {

  //============================================================================
  // Typedefs
  //============================================================================

  //! Type containing coordinates of the vertex.
  using point_t = burton_mesh_traits_t::point_t;

  //! Type of floating point.
  using real_t = burton_mesh_traits_t::real_t;

  //! Handle for accessing state at vertex.
  using data_t = burton_mesh_traits_t::data_t;

  //! Number of domains in the burton mesh.
  static constexpr size_t num_domains = burton_mesh_traits_t::num_domains;

  //! The different cell types
  enum class cell_type_t {
    triangle,
    quadrilateral,
    polygon
  };

  //============================================================================
  // Constructors
  //============================================================================

  //! Constructor
  burton_cell_t(const mesh_topology_base_t & mesh) : mesh_(mesh) 
  {};
  //! Destructor
  virtual ~burton_cell_t() {}


  //============================================================================
  // Accessors / Modifiers
  //============================================================================

  //! get the mesh
  const mesh_topology_base_t & mesh() const
  { return mesh_; }

  //! the centroid
  virtual point_t centroid() const {}; // = 0; FIXME

  //! the area of the cell
  virtual real_t area() const {}; // = 0; FIXME

  //! the minimum length in the cell
  virtual real_t min_length() const;

  //! the cell type
  virtual cell_type_t type() const {}; // = 0; FIXME

  //! get the region id
  auto & region()
  {
    auto cell_regions =
      data_t::instance().dense_accessor<size_t, flecsi_internal>(
        "cell_region", mesh_.runtime_id() );
    return cell_regions[mesh_entity_base_t<num_domains>::template id<0>()];
  }

  //! get the region id
  auto region() const
  {
    auto cell_regions =
      data_t::instance().dense_accessor<size_t, flecsi_internal>(
        "cell_region", mesh_.runtime_id() );
    return cell_regions[mesh_entity_base_t<num_domains>::template id<0>()];
  }

  //----------------------------------------------------------------------------
  //! \brief create_entities is a function that creates entities
  //!   of topological dimension dim, using vertices v, and puts the vertices
  //!   in e. See, e.g., burton_quadrilateral_cell_t for an implementation of
  //!   this pure virtual function.
  //!
  //! \param[in] dim The topological dimension of the entity to create.
  //! \param[out] e Vector to fill with ids of the vertices making the entity.
  //! \param[in] v Vertex ids for the cell.
  //! \param[in] vertex_count The number of vertices making up the entity.
  //!
  //! \return A pair with a) the number of vertex collections making up the
  //!   entity and b) the number of vertices per collection.
  //----------------------------------------------------------------------------
  virtual std::vector<id_t> create_entities(
      size_t dim, id_t * e, id_t * v, size_t vertex_count) 
  {}

  //----------------------------------------------------------------------------
  //! \brief create_bound_entities binds mesh entities across domains.
  //!   See, e.g., burton_quadrilateral_cell_t for an implementation of
  //!   this pure virtual function.
  //!
  //! \param[in] dim The topological dimension of the entity being bound.
  //! \param[in] ent_ids The entity ids of the entities making up the binding.
  //! \param[out] c The collection of the ids making up the bound entity.
  //!
  //! \return A pair with a) the number of entity collections making up the
  //!   binding and b) the number of entities per collection.
  //----------------------------------------------------------------------------
  virtual std::vector<id_t> create_bound_entities(
    size_t from_domain, size_t to_domain, size_t dim, id_t ** ent_ids, 
    size_t * ent_counts, id_t * c ) 
  {}

  //============================================================================
  // Private Data
  //============================================================================

private:

  //! a reference to the mesh topology
  const mesh_topology_base_t & mesh_;

}; // class burton_cell_t

////////////////////////////////////////////////////////////////////////////////
//! \class burton_triangle_t burton_entity_types.h
//!
//! \brief The burton_triangle_t type provides a derived instance of
//!   burton_cell_t for 2D triangle cells.
////////////////////////////////////////////////////////////////////////////////
class burton_triangle_cell_t : public burton_cell_t
{
public:

  //============================================================================
  // Constructors
  //============================================================================

  burton_triangle_cell_t(mesh_topology_base_t & mesh) : burton_cell_t(mesh)
  { }

  //============================================================================
  // Accessors / Modifiers
  //============================================================================

  //! the centroid
  point_t centroid() const override;

  //! the area of the cell
  real_t area() const override;

  //! the minimum length in the cell
  real_t min_length() const override;

  //! the cell type
  burton_cell_t::cell_type_t type() const override 
  { return burton_cell_t::cell_type_t::triangle; };

  //! \brief create_entities function for burton_triangle_cell_t.
  inline std::vector<id_t> create_entities(
      size_t dim, id_t * e, id_t * v, size_t vertex_count )
  {
    assert( vertex_count == 3 );
    e[0] = v[0];
    e[1] = v[1];

    e[2] = v[1];
    e[3] = v[2];

    e[4] = v[2];
    e[5] = v[0];

    return {2, 2, 2};
  } // create_entities

  //! \brief create_bound_entities function for burton_triangle_cell_t.
  inline std::vector<id_t> create_bound_entities(
    size_t from_domain, size_t to_domain, size_t dim, id_t ** ent_ids, 
    size_t * ent_counts, id_t * c )
  {
    assert( ent_counts[0] == 3 );
    switch (dim) {
      // Corners
      // The right edge is always first
      case 1:
        // corner 0
        c[0] = ent_ids[0][0]; // vertex 0
        c[1] = ent_ids[1][0]; // edge 0, abuts vertex 0
        c[2] = ent_ids[1][2]; // edge 3, abuts vertex 0

        // corner 1
        c[3] = ent_ids[0][1]; // vertex 1
        c[4] = ent_ids[1][1]; // edge 1, abuts vertex 1
        c[5] = ent_ids[1][0]; // edge 0, abuts vertex 1

        // corner 2
        c[6] = ent_ids[0][2]; // vertex 2
        c[7] = ent_ids[1][2]; // edge 2, abuts vertex 2
        c[8] = ent_ids[1][1]; // edge 1, abuts vertex 2

        return {3, 3, 3};

      // Wedges
      case 2:

        // wedge 0
        c[0] = ent_ids[0][0]; // vertex 0
        c[1] = ent_ids[1][2]; // edge 2

        // wedge 1
        c[2] = ent_ids[0][0]; // vertex 0
        c[3] = ent_ids[1][0]; // edge 0

        // wedge 2
        c[4] = ent_ids[0][1]; // vertex 1
        c[5] = ent_ids[1][0]; // edge 0

        // wedge 3
        c[6] = ent_ids[0][1]; // vertex 1
        c[7] = ent_ids[1][1]; // edge 1

        // wedge 4
        c[8] = ent_ids[0][2]; // vertex 2
        c[9] = ent_ids[1][1]; // edge 1

        // wedge 5
        c[10] = ent_ids[0][2]; // vertex 2
        c[11] = ent_ids[1][2]; // edge 2

        return {2, 2, 2, 2, 2, 2};

      default:
        assert(false && "Unknown bound entity type");
    } // switch
  } // create_bound_entities


}; // class burton_triangle_cell_t


////////////////////////////////////////////////////////////////////////////////
//! \class burton_quadrilateral_t burton_entity_types.h
//!
//! \brief The burton_quadrilateral_t type provides a derived instance of
//!   burton_cell_t for 2D quadrilateral cells.
////////////////////////////////////////////////////////////////////////////////
class burton_quadrilateral_cell_t : public burton_cell_t
{
public:

  //============================================================================
  // Constructors
  //============================================================================

  burton_quadrilateral_cell_t(mesh_topology_base_t & mesh) : burton_cell_t(mesh)
  { }

  //============================================================================
  // Accessors / Modifiers
  //============================================================================

  //! the centroid
  point_t centroid() const override;

  //! the area of the cell
  real_t area() const override;

  //! the minimum length in the cell
  real_t min_length() const override;

  //! the cell type
  burton_cell_t::cell_type_t type() const override 
  { return burton_cell_t::cell_type_t::quadrilateral; };

  //! \brief create_entities function for burton_quadrilateral_cell_t.
  inline std::vector<id_t> create_entities(
      size_t dim, id_t * e, id_t * v, size_t vertex_count)
  {
    assert( vertex_count == 4 );
    e[0] = v[0];
    e[1] = v[1];

    e[2] = v[1];
    e[3] = v[2];

    e[4] = v[2];
    e[5] = v[3];

    e[6] = v[3];
    e[7] = v[0];

    return {2, 2, 2, 2};
  } // create_entities

  //----------------------------------------------------------------------------
  /*!
    \brief create_bound_entities function for burton_quadrilateral_cell_t.

    \verbatim

    The following shows the labeling of the primitives making up a cell. Given
    vertices v*, edges e*, and center vertex cv.

    v3------e2-------v2
    |                 |
    |                 |
    |                 |
    |                 |
    e3      cv       e1
    |                 |
    |                 |
    |                 |
    |                 |
    v0------e0-------v1

    A wedge is defined by a vertex, an edge, and the cell itself. The wedge
    indexing is shown below.

    v3------e2-------v2
    | \      |      / |
    |   \  w6|w5  /   |
    |  w7 \  |  / w4  |
    |       \|/       |
    e3------cv-------e1
    |       /|\       |
    |  w0 /  |  \ w3  |
    |   /  w1|w2  \   |
    | /      |      \ |
    v0------e0-------v1

    A corner is defined by a vertex and two edges.

    c0 = {v0, e0, e3}
    c1 = {v1, e0, e1}
    c2 = {v2, e1, e2}
    c3 = {v3, e2, e3}

    \endverbatim
   */
  //----------------------------------------------------------------------------
  inline std::vector<id_t> create_bound_entities(
    size_t from_domain, size_t to_domain, size_t dim, id_t ** ent_ids, 
    size_t * ent_counts, id_t * c )
  {
    assert( ent_counts[0] == 4 );
    switch (dim) {
      // Corners
      // The right edge is always first
      case 1:
        // corner 0
        c[0] = ent_ids[0][0]; // vertex 0
        c[1] = ent_ids[1][0]; // edge 0, abuts vertex 0
        c[2] = ent_ids[1][3]; // edge 3, abuts vertex 0

        // corner 1
        c[3] = ent_ids[0][1]; // vertex 1
        c[4] = ent_ids[1][1]; // edge 1, abuts vertex 1
        c[5] = ent_ids[1][0]; // edge 0, abuts vertex 1

        // corner 2
        c[6] = ent_ids[0][2]; // vertex 2
        c[7] = ent_ids[1][2]; // edge 2, abuts vertex 2
        c[8] = ent_ids[1][1]; // edge 1, abuts vertex 2

        // corner 3
        c[9] = ent_ids[0][3]; // vertex 3
        c[10] = ent_ids[1][3]; // edge 3, abuts vertex 3
        c[11] = ent_ids[1][2]; // edge 2, abuts vertex 3

        return {3, 3, 3, 3};

      // Wedges
      case 2:

        // wedge 0
        c[0] = ent_ids[0][0]; // vertex 0
        c[1] = ent_ids[1][3]; // edge 3

        // wedge 1
        c[2] = ent_ids[0][0]; // vertex 0
        c[3] = ent_ids[1][0]; // edge 0

        // wedge 2
        c[4] = ent_ids[0][1]; // vertex 1
        c[5] = ent_ids[1][0]; // edge 0

        // wedge 3
        c[6] = ent_ids[0][1]; // vertex 1
        c[7] = ent_ids[1][1]; // edge 1

        // wedge 4
        c[8] = ent_ids[0][2]; // vertex 2
        c[9] = ent_ids[1][1]; // edge 1

        // wedge 5
        c[10] = ent_ids[0][2]; // vertex 2
        c[11] = ent_ids[1][2]; // edge 2

        // wedge 6
        c[12] = ent_ids[0][3]; // vertex 3
        c[13] = ent_ids[1][2]; // edge 2

        // wedge 7
        c[14] = ent_ids[0][3]; // vertex 3
        c[15] = ent_ids[1][3]; // edge 3

        return {2, 2, 2, 2, 2, 2, 2, 2};

      default:
        assert(false && "Unknown bound entity type");
    } // switch
  } // create_bound_entities


}; // class burton_quadrilateral_cell_t

////////////////////////////////////////////////////////////////////////////////
//! \class burton_polygonal_t burton_entity_types.h
//!
//! \brief The burton_polygonal_t type provides a derived instance of
//!   burton_cell_t for 2D polygonal cells.
////////////////////////////////////////////////////////////////////////////////
class burton_polygonal_cell_t : public burton_cell_t
{
public:

  //============================================================================
  // Constructors
  //============================================================================

  burton_polygonal_cell_t(mesh_topology_base_t & mesh) : burton_cell_t(mesh)
  { }

  //============================================================================
  // Accessors / Modifiers
  //============================================================================

  //! the centroid
  point_t centroid() const override;

  //! the area of the cell
  real_t area() const override;

  //! the cell type
  burton_cell_t::cell_type_t type() const override 
  { return burton_cell_t::cell_type_t::polygon; };

  //! \brief create_entities function for burton_polygonal_cell_t.
  inline std::vector<id_t> create_entities(
      size_t dim, id_t * e, id_t * v, size_t vertex_count)
  {
    auto vp = v[vertex_count - 1];
    for ( auto i=0, ind=0; i<vertex_count; i++ ) {
      auto vn = v[i];
      e[ ind++ ] = vp;
      e[ ind++ ] = vn;
      vp = vn;
    }
    return std::vector<id_t>(vertex_count, 2);
  } // create_entities

  //!  \brief create_bound_entities function for burton_polygonal_cell_t.
  inline std::vector<id_t> create_bound_entities(
    size_t from_domain, size_t to_domain, size_t dim, id_t ** ent_ids, 
    size_t * ent_counts, id_t * c )
  {
    auto vertex_count = ent_counts[0];

    switch (dim) {
    // Corners
    // The right edge is always first
    case 1: {

        auto vp = vertex_count - 1;
        for ( auto i=0, ind=0; i<vertex_count; i++ ) {
          auto vn = i;
          c[ ind++ ] = ent_ids[0][vn]; // vertex 0
          c[ ind++ ] = ent_ids[1][vn]; // edge 0, abuts vertex 0
          c[ ind++ ] = ent_ids[1][vp]; // edge 3, abuts vertex 0
          vp = vn;
        }
        return std::vector<id_t>(vertex_count, 3);
    }
    // Wedges
    case 2: {
      
      auto vp = vertex_count - 1;
      for ( auto i=0, ind=0; i<vertex_count; i++ ) {
        auto vn = i;
        // wedge 0
        c[ ind++ ] = ent_ids[0][vn]; // vertex 0
        c[ ind++ ] = ent_ids[1][vp]; // edge 3
        // wedge 1
        c[ ind++ ] = ent_ids[0][vn]; // vertex 0
        c[ ind++ ] = ent_ids[1][vn]; // edge 0
        vp = vn;
      }
      return std::vector<id_t>(2*vertex_count, 2);
    }
    default:
      assert(false && "Unknown bound entity type");
    } // switch
  } // create_bound_entities


}; // class burton_polygonal_cell_t

////////////////////////////////////////////////////////////////////////////////
//! \class burton_wedge_t burton_entity_types.h
//! \brief The burton_wedge_t type provides an interface for managing and
//!   geometry and state associated with mesh wedges.
//!
//! \tparam N The domain of the wedge.
////////////////////////////////////////////////////////////////////////////////
class burton_wedge_t
    : public mesh_entity_t<2, burton_mesh_traits_t::num_domains>
{
public:

  //============================================================================
  // Typedefs
  //============================================================================

  //! Physics vector type.
  using vector_t = burton_mesh_traits_t::vector_t;

  //============================================================================
  // Constructors
  //============================================================================

  burton_wedge_t(const mesh_topology_base_t & mesh) : mesh_(mesh)
  {}


  //============================================================================
  // Accessors / Modifiers
  //============================================================================

  //! Set the cell that a wedge is in.
  void set_cell(burton_cell_t * cell) { cell_ = cell; }

  //! Set the edge that a wedge has.
  void set_edge(burton_edge_t * edge) { edge_ = edge; }

  //! Set the vertex that a wedge has.
  void set_vertex(burton_vertex_t * vertex) { vertex_ = vertex; }

  //! Set the corner that a wedge is in.
  void set_corner(burton_corner_t * corner) { corner_ = corner; }

  //! Get the cell that a wedge is in.
  const burton_cell_t * cell() const { return cell_; }

  //! Get the edge that a wedge has.
  const burton_edge_t * edge() const { return edge_; }

  //! Get the vertex that a wedge has.
  const burton_vertex_t * vertex() const { return vertex_; }

  //! Get the corner that a wedge is in.
  const burton_corner_t * corner() const { return corner_; }

  //! \brief Get the cell facet normal for the wedge.
  //! \return Cell facet normal vector.
  vector_t facet_normal_left() const
  {
    auto e = edge()->midpoint();
    auto v = vertex()->coordinates();
    return { v[1] - e[1], e[0] - v[0] };
  }
  vector_t facet_normal_right() const
  {
    auto e = edge()->midpoint();
    auto v = vertex()->coordinates();
    return { e[1] - v[1], v[0] - e[0] };
  }

  //============================================================================
  // Private Data
  //============================================================================

 private:

  burton_cell_t * cell_;
  burton_edge_t * edge_;
  burton_vertex_t * vertex_;
  burton_corner_t * corner_;

  //! a reference to the mesh topology
  const mesh_topology_base_t & mesh_;

}; // struct burton_wedge_t

////////////////////////////////////////////////////////////////////////////////
//! \class burton_corner_t burton_entity_types.h
//! \brief The burton_corner_t type provides an interface for managing and
//!   geometry and state associated with mesh corners.
//!
//! \tparam N The domain of the corner.
////////////////////////////////////////////////////////////////////////////////
class burton_corner_t
    : public mesh_entity_t<1, burton_mesh_traits_t::num_domains>
{
public:

  //============================================================================
  // Typedefs
  //============================================================================

  //! Type of floating point.
  using real_t = burton_mesh_traits_t::real_t;

  //! Type containing coordinates of a vertex.
  using point_t = burton_mesh_traits_t::point_t;

  //! Type vector type.
  using vector_t = burton_mesh_traits_t::vector_t;

  //============================================================================
  // Constructors
  //============================================================================

  burton_corner_t(const mesh_topology_base_t & mesh) : mesh_(mesh) {}

  //============================================================================
  // Accessors / Modifiers
  //============================================================================

  //! \brief Add a wedge to the mesh.
  //!
  //! \param[in] w The wedge to add to the mesh.
  void add_wedge(burton_wedge_t * w)
  {
    wedges_.add(w);
    w->set_corner(this);
  }

  //! Set the cell that a corner is in.
  void set_cell(burton_cell_t * cell) { cell_ = cell; }

  //! Set the first edge that a corner has.
  void set_edge1(burton_edge_t * edge) { edge1_ = edge; }

  //! Set the second edge that a corner has.
  void set_edge2(burton_edge_t * edge) { edge2_ = edge; }

  //! Set the vertex that a corner has.
  void set_vertex(burton_vertex_t * vertex) { vertex_ = vertex; }

  //! Get the cell that a corner is in.
  const burton_cell_t * cell() const { return cell_; }

  //! Get edge1 that a corner has.
  const burton_edge_t * edge1() const { return edge1_; }

  //! Get edge2 that a corner has.
  const burton_edge_t * edge2() const { return edge2_; }

  //! Get the vertex that a corner has.
  const burton_vertex_t * vertex() const { return vertex_; }

  //! the area of the corner
  real_t area() {
    /*
      Area of a quadrilateral. No sides parallel.

                 D
          /-------------\
         /                \
      B /                   \  C
       /                      \
      /-------------------------\
                 A

      area = 1/2 mag(A X B) + 1/2 mag(C X D)
     */

    auto p = edge1()->midpoint() - edge2()->midpoint();
    auto q = vertex()->coordinates() - cell()->centroid();
    
    using math::cross_product;

    auto det = cross_product( p, q );

    return 0.5 * std::abs( det );
  }

   //! \brief Get the wedges for the mesh.
   //! \return The wedges in the mesh.
  entity_group<burton_wedge_t> & wedges() { return wedges_; } // wedges

  //============================================================================
  // Private Data
  //============================================================================

private:

  entity_group<burton_wedge_t> wedges_;
  burton_cell_t * cell_;
  burton_edge_t * edge1_;
  burton_edge_t * edge2_;
  burton_vertex_t * vertex_;

  //! a reference to the mesh topology
  const mesh_topology_base_t & mesh_;

}; // class burton_corner_t

} // namespace mesh
} // namespace ale


/*~-------------------------------------------------------------------------~-*
 * Formatting options
 * vim: set tabstop=2 shiftwidth=2 expandtab :
 *~-------------------------------------------------------------------------~-*/