#pragma once
#include "DCEL_point.h"
#include "DCEL_types.h"

/*

Contains a definition for a specific DCEL representation that 
allows for continuous regional allocations in the discrete 2d plane

continuous implies that any boundary is "inside" every other boundary
therefor there can only be one clockwise boundary and any number of discrete counter
clockwise boundaries contained within the clockwise boundary.

*/

//the relational types between a query point and a face
enum FaceRelationType { point_exterior, point_on_boundary, point_interior };
//represents the relation a query point has to a face
//returned by getPointRelation
struct FaceRelation {
	//this can only be created by queris / copying 
	FaceRelation(FaceRelationType t, Edge<Pint> const * e) {
		type = t;
		relevant = e;
	}
	FaceRelationType type;
	//if type is point_on_boundary, this is the edge that contains the point
	//root inclusive, end exclusive
	Edge<Pint> const * relevant;
};

class Region {
	FLL<Face<Pint> *> Boundaries;

	FaceRelation contains(Pint const &test_point);

	//if non-trivially connected, will absorb the target region into this one via face merging
	bool merge(Region*);

	//restricts a region to another, returns a list of regions inside and outside the restricted region.
	friend void allocate_from(Region*, Region*, FLL<Region*>, FLL<Region*>);

	//returns the relation of a point to a planar loop
	//interior is defined by right-bound to edges, this means clockwise loops are inverted containment
	//FaceRelation<_P> const getPointRelation(_P const &) const;

};


