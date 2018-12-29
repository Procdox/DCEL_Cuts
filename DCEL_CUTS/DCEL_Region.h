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

struct interact {
	Pint location;
	FaceRelationType type;
	Edge<Pint> * mark;

	interact() {
		mark = nullptr;
	}
};

//represents the relation a query point has to a face
//returned by getPointRelation
struct FaceRelation {
	//this can only be created by queris / copying 
	FaceRelation(FaceRelationType t, Edge<Pint> * e) {
		type = t;
		relevant = e;
	}
	FaceRelationType type;
	//if type is point_on_boundary, this is the edge that contains the point
	//root inclusive, end exclusive
	Edge<Pint> * relevant;
};

FaceRelation const getPointRelation(Face<Pint> &rel, Pint const &test_point);

class Region {
	FLL<Face<Pint> *> Boundaries;
	DCEL<Pint> * universe;

	friend void determineInteriors(Region *, FLL<interact *> &,
		FLL<Face<Pint> *> &, FLL<Face<Pint> *> &);

	friend FLL<interact *> markRegion(Region *, FLL<Pint> const &);


	friend void subAllocate(Region*, FLL<Pint> const &, FLL<Region*> &, FLL<Region*> &);

public:
	Region(DCEL<Pint> *);
	Region(DCEL<Pint> *, FLL<Pint> const &);

	FLL< Face<Pint> *> const * getBounds() {
		return &Boundaries;
	}

	FaceRelation contains(Pint const &test_point);

	//if non-trivially connected, will absorb the target region into this one via face merging
	bool merge(Region*);

};


