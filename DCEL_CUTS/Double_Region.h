#pragma once
#include "Double_Point.h"
#include "DCEL.h"

/*

Contains a definition for a specific DCEL representation that
allows for continuous regional allocations in the discrete 2d plane

continuous implies that any boundary is "inside" every other boundary
therefor there can only be one clockwise boundary and any number of discrete counter
clockwise boundaries contained within the clockwise boundary.

represented by doubles, with (dist(a,b) < 1) => (a == b)
*/

//the relational types between a query point and a face
enum FaceRelationType { point_exterior, point_on_boundary, point_interior };

struct interact {
	Pdbl location;
	FaceRelationType type;
	Edge<Pdbl> * mark;
	bool mid_interior;
	Pdbl mid;

	interact() {
		mark = nullptr;
	}
};

//represents the relation a query point has to a face
//returned by getPointRelation
struct FaceRelation {
	//this can only be created by queris / copying 
	FaceRelation(FaceRelationType t, Edge<Pdbl> * e) {
		type = t;
		relevant = e;
	}
	FaceRelationType type;
	//if type is point_on_boundary, this is the edge that contains the point
	//root inclusive, end exclusive
	Edge<Pdbl> * relevant;
};

FaceRelation const getPointRelation(Face<Pdbl> &rel, Pdbl const &test_point);

bool merge(Region<Pdbl> * a, Region<Pdbl> * b);

void determineInteriors(Region<Pdbl> *, FLL<interact *> &, FLL<Face<Pdbl> *> &,
	FLL<Face<Pdbl> *> &);

bool markRegion(Region<Pdbl> *, FLL<Pdbl> const &, FLL<interact *> &);

//type dependent
void subAllocate(Region<Pdbl> * target, FLL<Pdbl> const & boundary,
	FLL<Region<Pdbl> *> &exteriors, FLL<Region<Pdbl> *> & interiors);

FaceRelation contains(Region<Pdbl> * target, Pdbl const & test_point);

FLL<Region<Pdbl> *> getNeighbors(Region<Pdbl> * target);

void cleanRegion(Region<Pdbl> * target);