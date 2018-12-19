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

class Region {
	FLL<Face<Pint> *> Boundaries;

	//if non-trivially connected, will absorb the target region into this one via face merging
	void merge(Region*);

	double area();

	//restricts a region to another, returns a list of regions inside and outside the restricted region.
	friend void allocate_from(Region*, Region*, FLL<Region*>, FLL<Region*>);

	//returns the relation of a point to a planar loop
	//interior is defined by right-bound to edges, this means clockwise loops are inverted containment
	//FaceRelation<_P> const getPointRelation(_P const &) const;

};


