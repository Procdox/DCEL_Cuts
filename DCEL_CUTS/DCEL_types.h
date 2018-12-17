#pragma once

#include "DCEL_fll.h"
#include "DCEL_point.h"

class Point;
class Edge;
class Face;
class DCEL;

// Represents a point in space, the ends of edges, and corners of faces
class Point {
	//friend Face;
	friend DCEL;
	friend Edge;

	// The associated DCEL system
	DCEL* universe;

	// An edge leaving this, nullptr if no edges reference this point
	Edge* root;

	// The position of this point of course!
	_P position;

	// This can only be created through DCEL system functions
	Point(DCEL* uni);
	Point(Point&&) = delete;
	Point(const Point&) = delete;
public:
	void setPosition(_P);
	_P getPosition() const;

	//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	//         Traversal Methods

	Edge * getRoot();

	Edge const * getRoot() const;
};

enum EdgeModType { face_destroyed, faces_preserved, face_created};

class EdgeModResult {
	friend Edge;
	//this can only be created by modifications
	EdgeModResult(EdgeModType t, Face* f) {
		type = t;
		relevant = f;
	}
public:

	EdgeModType type;
	Face* relevant;
};

// Represents a one way connection between two points
class Edge {
	//friend Face;
	friend DCEL;
	friend Face;

	// The associated DCEL system
	DCEL* universe;

	// The point this edge originates from
	Point* root;

	// The next edge on the boundary this edge forms
	Edge* next;
	// The previous edge on the boundary this edge forms
	Edge* last;
	// The inverse edge of this one.
	Edge* inv;

	// The face this edge forms a boundary for
	Face* loop;

	//this can only be created through DCEL system functions
	Edge(DCEL* uni);
	Edge(Edge&&) = delete;
	Edge(const Edge&) = delete;
public:
	//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	//         Traversal Methods

	Point * getStart();
	Point * getEnd();

	Edge * getNext();
	Edge * getLast();
	Edge * getInv();

	Face * getFace();

	//get the next edge cw around the root
	Edge * getCW();
	//get the next edge ccw around the root
	Edge * getCCW();

	Point const * getStart() const;
	Point const * getEnd() const;

	Edge const * getNext() const;
	Edge const * getLast() const;
	Edge const * getInv() const;

	Face const * getFace() const;

	//get the next edge cw around the root
	Edge const * getCW() const;
	//get the next edge ccw around the root
	Edge const * getCCW() const;

	//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	//         Modification

	//subdivides this point, maintains this edges root
	void subdivide(_P);

	//detach from the current root point in favor of a novel point
	//just moves root if it is isolated
	//returns the face left at og root if it exists
	EdgeModResult moveRoot(_P);

	//detaches from the current root and inserts to the end of target
	//deletes og root if thereafter isolated
	//returns the face left at og root if it exists
	EdgeModResult insertAfter(Edge*);

	//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	//         Removal

	//remove this edge, its inverse, and either end points that are thereafter isolated
	//returns novel face if created
	EdgeModResult remove();
	//contracts this edge, the resulting point position is this edges root position
	void contract();
};

//the relational types between a query point and a face
enum FaceRelationType { point_exterior, point_on_boundary, point_interior };
//represents the relation a query point has to a face
//returned by getPointRelation
class FaceRelation {
	friend Face;
	//this can only be created by queris / copying 
	FaceRelation(FaceRelationType t, Edge const * e) {
		type = t;
		relevant = e;
	}
public:
	FaceRelationType type;
	//if type is point_on_boundary, this is the edge that contains the point
	//root inclusive, end exclusive
	Edge const * relevant;
};

//represents a loop formed by a series of edges
class Face {
	friend DCEL;
	friend Edge;

	DCEL* universe;

	Edge* root;

	//this can only be created through DCEL system functions
	Face(DCEL* uni);
	Face(Face&&) = delete;
	Face(const Face&) = delete;

	//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	//         Modifiers

	//sets all edges of this loop to reference this
	void reFace();

public:
	//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	//         Traversal Methods

	Edge * getRoot();

	Edge const * getRoot() const;

	//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	//         Queries

	//get the count of edges in the boundary
	int getLoopSize() const;

	//return a list of the points in the loop
	FLL<_P> getLoopPoints() const;

	//return a list of the points in the loop (write-to)
	void getLoopPoints(FLL<_P> &) const;

	//return a list of the edges in the loop
	FLL<Edge const *> getLoopEdges() const;

	//return a list of the edges in the loop (write-to)
	void getLoopEdges(FLL<Edge const *> &);

	//return a list of the faces that share a boundary in the loop
	FLL<Face const *> getNeighbors() const;

	//return a list of the faces that share a boundary in the loop (write-to)
	void getNeighbors(FLL<Face const *> &) const;

	//returns the relation of a point to a planar loop
	//interior is defined by right-bound to edges, this means clockwise loops are inverted containment
	FaceRelation const getPointRelation(_P const &) const;

	//returns the area of the loop
	double loopArea() const;

	/*struct interact_point {
		interaction_state state;
		int region;
		_P point;
		interact_point* next;
		int mid_state;
		interact_point(const _P p) {
			region = -1;
			state = unknown_region;
			point = p;
			next = nullptr;
			mid_state = -1;
		}
		interact_point(const _P p, interact_point* n) {
			region = -1;
			state = unknown_region;
			point = p;
			next = n;
			mid_state = -1;
		}
	};

	struct strand {
		Edge* interior_edge;
		Edge* exterior_edge;
		bool unique_interior;
		bool unique_exterior;
		strand() {
			unique_interior = true;
			unique_exterior = true;
		}
	};

	int getPointState(const _P &test_point) const;

	int getFirstIntersect(const _P &start, const _P &end, _P &intersect) const;*/

	//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	//         Regioning

	//takes in a simple region boundary and divides this face into faces interior to, and faces exterior to that boundary
	void subAllocateFace(FLL<_P> const &boundary, FLL<Face*> &interiors, FLL<Face*> &exteriors);

	void mergeWithFace(Face*, FLL<Face*> &);

	//void cleanBorder();
};

class DCEL {
	FLL<Point*> points;
	FLL<Edge*> edges;
	FLL<Face*> faces;

	friend Edge;

	//creates a point
	//no parameters are initialized
	Point* createPoint();
	//creates an edge and its inverse
	//no parameters are initialized
	Edge* createEdge();
	//creates a face
	//no parameters are initialized
	Face* createFace();

	//removes a point
	//does NOT check to see if references elsewhere
	void removePoint(Point*);
	//removes an edge and its inverse
	//does NOT check to see if references elsewhere
	void removeEdge(Edge*);
	//removes a face
	//does NOT check to see if references elsewhere
	void removeFace(Face*);

public:
	~DCEL();

	//creates an edge and its inverse connecting two novel points
	Edge* addEdge(_P, _P);
	//creates an edge and its inverse connecting after an edge to a novel point
	Edge* addEdge(Edge*, _P);
	//creates an edge and its inverse connecting after an two edges
	Edge* addEdge(Edge*, Edge*);

	//creates a circular chain of edges forming a loop with the given boundary
	//returns a pointer to the clock-wise oriented interior of the boundary
	Face* draw(FLL<_P> const &boundary);

	//void sanityCheck(); TODO
};