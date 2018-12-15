// DCEL.cpp : Defines the exported functions for the DLL application.
//
#include "stdafx.h"
#include "DCEL_types.h"

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//         Point Definitions
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

Point::Point(DCEL* uni) {
	universe = uni;
}

void Point::setPosition(_P p) {
	position = p;
}
_P Point::getPosition() const {
	return position;
}

Edge const * Point::getRoot() const {
	return root;
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//         Edge Definitions
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

Edge::Edge(DCEL* uni) {
	universe = uni;
}

Point const * Edge::getStart() const {
	return root;
}
Point const * Edge::getEnd() const {
	return inv->root;
}

Edge const * Edge::getNext() const {
	return next;
}
Edge const * Edge::getLast() const {
	return last;
}
Edge const * Edge::getInv() const {
	return inv;
}

Face const * Edge::getFace() const {
	return loop;
}

Edge const * Edge::getCW() const {
	return last->inv;
}
Edge const * Edge::getCCW() const {
	return inv->next;
}

void Edge::subdivide(_P mid_point) {
	Edge* adjoint = universe->createEdge();
	Point* mid = universe->createPoint();

	//we need to connect mid - root, position

	//20 variables internal to our 4 edges
	//4 of these are inverses, can be ignored
	//5: root, last, inv->next, loop, inv->loop don't change
	//- adjoint->(next, last, loop, root)
	//- adoint->inv->(next, last, loop, root)
	//- next
	//- inv->(last, root)

	//11 internal changes

	//6 possible external references
	//root->root doesnt change
	//last->next doesnt change
	//inv->next->last doesnt change
	//- inv->root->root
	//- last->next
	//- inv->next->last

	//3 external changes

	mid->position = mid_point;
	mid->root = adjoint;

	if (next != inv) {
		inv->root->root = next;

		adjoint->next = next;
		adjoint->last = this;
		adjoint->loop = loop;
		adjoint->root = mid;

		adjoint->inv->next = inv;
		adjoint->inv->last = inv->last;
		adjoint->inv->loop = inv->loop;
		adjoint->inv->root = inv->root;

		next->last = adjoint;
		inv->last->next = adjoint->inv;

		next = adjoint;
		inv->last = adjoint->inv;
		inv->root = mid;
	}
	else {
		inv->root->root = adjoint->inv;

		adjoint->next = adjoint->inv;
		adjoint->last = this;
		adjoint->loop = loop;
		adjoint->root = mid;

		adjoint->inv->next = inv;
		adjoint->inv->last = adjoint;
		adjoint->inv->loop = loop;
		adjoint->inv->root = inv->root;

		next = adjoint;
		inv->last = adjoint->inv;
		inv->root = mid;
	}
}

Face* Edge::moveRoot(_P p) {
	Point* og = root;
	if (last == inv) {
		//root is isolated

		root->setPosition(p);
		return nullptr;
	}
	else {
		Point* end = universe->createPoint();
		Edge* old = inv->next;
		end->root = this;
		end->position = p;

		root->root = old;

		last->next = old;
		old->last = last;

		last = inv;
		inv->next = this;
		root = end;

		//reface this and old
		if (loop == inv->loop) {
			//we have disconnected a loop
			loop->root = this;

			Face* novel = universe->createFace();
			novel->root = old;
			novel->reFace();
		}
		else {
			//we have merged two loops
			universe->removeFace(inv->loop);
			loop->reFace();
		}

		return old->loop;
	}
}

Face* Edge::insertAfter(Edge* target) {
	//remove from og

	Face* novel_a = nullptr;
	Face* novel_b = nullptr;

	if (last == inv) {
		universe->removePoint(root);
	}
	else {
		Edge* old = inv->next;

		root->root = old;

		last->next = old;
		old->last = last;

		//reface this and old
		if (loop == inv->loop) {
			//we have disconnected a loop
			loop->root = this;

			novel_a = universe->createFace();
			novel_a->root = old;
			novel_a->reFace();
		}
		else {
			//we have merged two loops
			universe->removeFace(inv->loop);
			loop->reFace();
		}
	}

	//insert elsewhere
	root = target->inv->root;

	inv->next = target->next;
	target->next->last = inv;

	target->next = this;
	last = target;

	if (target->loop == loop) {
		//we have split a loop
		loop->root = this;

		novel_b = universe->createFace();
		novel_b->root = inv;
		novel_b->reFace();
	}
	else {
		//we have joined two loops

		//is it the loop we just disconnected?
		if (target->loop == novel_a) {
			novel_a = nullptr;
		}

		universe->removeFace(target->loop);
		loop->reFace();
	}

	return novel_a;
}

Face* Edge::remove() {

	//if either point is isolated, we dont need to reface
	bool loose_strand = false;
	Face* novel = nullptr;

	if (next == inv) {
		universe->removePoint(inv->root);

		loose_strand = true;
	}
	else {
		inv->root->root = next;
		next->last = inv->last;
		inv->last->next = next;
	}

	if (last == inv) {
		universe->removePoint(root);

		loose_strand = true;
	}
	else{
		root->root = inv->next;
		last->next = inv->next;
		inv->next->last = last;
	}

	//we may be connecting or disconnecting two loops
	if (!loose_strand) {
		if (loop == inv->loop) {
			//we have disconnected a loop
			loop->root = next;
			loop->reFace();

			novel = universe->createFace();
			novel->root = last;
			novel->reFace();

		}
		else {
			//we have merged two loops
			loop->root = next;
			universe->removeFace(inv->loop);
			loop->reFace();
		}
	}

	universe->removeEdge(this);
	return novel;
}

void Edge::contract() {

	Edge* focus = next;

	do {
		focus->root = root;
		focus = focus->inv->next;
	} while (focus != next);

	root->root = next;

	last->next = next;
	next->last = last;

	inv->next->last = inv->last;
	inv->last->next = inv->next;

	universe->removePoint(inv->root);
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//         Face Definitions
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

Face::Face(DCEL* uni) {
	universe = uni;
}

void Face::reFace() {
	Edge* focus = root;
	do {
		focus->loop = this;
	} while (focus != root);
}

Edge const * Face::getRoot() const {
	return root;
}

int Face::getLoopSize() const {
	Edge const* focus = root;
	int count = 0;

	do {
		count++;

		focus = focus->next;
	} while (focus != root);

	return count;
}

FLL<Edge const *> Face::getLoopEdges() const {
	Edge const * focus = root;
	FLL<Edge const*> target;

	do {
		target.push(focus);

		focus = focus->next;
	} while (focus != root);

	return target;
}

void Face::getLoopEdges(FLL<Edge const *> &target) {
	Edge const * focus = root;

	do {
		target.push(focus);

		focus = focus->next;
	} while (focus != root);
}

FLL<_P> Face::getLoopPoints() const {
	FLL<_P> target;
	Edge const * focus = root;

	do {
		_P p = focus->root->getPosition();
		target.push(p);

		focus = focus->next;
	} while (focus != root);

	return target;
}

void Face::getLoopPoints(FLL<_P> &target) const {
	Edge const * focus = root;

	do {
		_P p = focus->root->getPosition();
		target.push(p);

		focus = focus->next;
	} while (focus != root);
}

FLL<Face const *> Face::getNeighbors() const {
	FLL<Face const *> target;
	Edge const * focus = root;

	do {
		Face const * canidate = focus->inv->loop;
		if (!target.contains(canidate)) {
			target.push(canidate);
		}
		focus = focus->next;
	} while (focus != root);

	return target;
}

void Face::getNeighbors(FLL<Face const *> &target) const {
	Edge const * focus = root;

	do {
		Face const * canidate = focus->inv->loop;
		if (!target.contains(canidate)) {
			target.push(canidate);
		}
		focus = focus->next;
	} while (focus != root);
}

faceRelation const Face::getPointRelation(_P const &test_point) const {
	Edge const * focus = root;
	int count = 0;

	bool is_best = false;
	float best_distance = 0;
	bool inside = false;

	do {
		const _P &start_vector = focus->root->getPosition();
		const _P &end_vector = focus->inv->root->getPosition();

		//does it sit on 
		int y_length = end_vector.Y - start_vector.Y;
		int y_offset = test_point.Y - start_vector.Y;

		if (y_length == 0) {
			if (y_offset == 0) {
				if ((start_vector.X <= test_point.X && test_point.X <= end_vector.X) ||
					(start_vector.X >= test_point.X && test_point.X >= end_vector.X)) {

					return faceRelation(faceRelationType::point_on_boundary, focus);
				}
			}
			focus = focus->next;
			continue;
		}
		else if ((y_offset <= y_length && y_offset >= 0) || y_offset >= y_length && y_offset <= 0) {
			int x_length = end_vector.X - start_vector.X;

			if ((x_length * y_offset) % y_length == 0) {
				//robust solution!
				int x = start_vector.X + (x_length * y_offset) / y_length;
				int distance = test_point.X - x;

				if (distance == 0) {
					return faceRelation(faceRelationType::point_on_boundary, focus);
				}
				else if (distance > 0 && (distance < best_distance || !is_best)) {
					is_best = true;
					best_distance = distance;

					inside = y_length > 0;
				}
				else {
				}
			}
			else {
				//floats, oh no!
				float x = (float)start_vector.X + (float)(x_length * y_offset) / (float)y_length;
				float distance = test_point.X - x;
				if (distance < 0) distance *= -1;

				if (distance > 0.f && (distance < best_distance || !is_best)) {
					is_best = true;
					best_distance = distance;

					inside = y_length > 0;
				}
			}
		}
		focus = focus->next;
	} while (focus != root);

	if (inside) {
		return faceRelation(faceRelationType::point_interior, nullptr);
	}
	else {
		return faceRelation(faceRelationType::point_exterior, nullptr);
	}
}

double Face::loopArea() const {
	Edge const * focus = root;
	double total = 0;

	_P A;
	_P B = focus->root->getPosition();

	do {
		A = B;
		B = focus->inv->root->getPosition();

		double width = A.X - B.X;
		double avg_height = (A.Y + B.Y) / 2;

		total += width * avg_height;

		focus = focus->next;

	} while (focus != root);

	return total;
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//         DCEL Definitions
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

DCEL::~DCEL() {
	auto focus_point = points.getHead();

	while (focus_point != nullptr) {
		delete focus_point->getValue();
		focus_point = focus_point->getNext();
	}

	auto focus_edge = edges.getHead();

	while (focus_edge != nullptr) {
		delete focus_edge->getValue();
		focus_edge = focus_edge->getNext();
	}

	auto focus_face = faces.getHead();

	while (focus_face != nullptr) {
		delete focus_face->getValue();
		focus_face = focus_face->getNext();
	}
}

Point* DCEL::createPoint() {
	Point* result = new Point(this);
	points.push(result);
	return result;
}

Edge* DCEL::createEdge(){
	Edge* result = new Edge(this);
	Edge* inverse = new Edge(this);

	edges.push(result);
	edges.push(inverse);

	result->inv = inverse;
	inverse->inv = result;

	return result;
}

Face* DCEL::createFace(){
	Face* result = new Face(this);
	faces.push(result);
	return result;
}


void DCEL::removePoint(Point* target) {
	//is it ACTUALLY disconnected? AHHHHHHHHH
	points.remove(target);

	delete target;
}

void DCEL::removeEdge(Edge* target) {
	//is it ACTUALLY disconnected? AHHHHHHHHH
	edges.remove(target);
	edges.remove(target->inv);

	delete target->inv;
	delete target;
}

void DCEL::removeFace(Face* target) {
	//is it ACTUALLY disconnected? AHHHHHHHHH
	faces.remove(target);

	delete target;
}


Edge* DCEL::addEdge(_P a, _P b) {
	Edge* result = createEdge();
	Point* A = createPoint();
	Point* B = createPoint();
	Face* loop = createFace();

	result->next = result->inv;
	result->last = result->inv;
	result->inv->next = result;
	result->inv->last = result;

	result->root = A;
	A->root = result;
	A->position = a;

	result->inv->root = B;
	B->root = result->inv;
	B->position = b;

	result->loop = loop;
	result->inv->loop = loop;

	return result;
}

Edge* DCEL::addEdge(Edge* a, _P b) {
	Edge* result = createEdge();
	Point* B = createPoint();

	result->next = result->inv;
	result->inv->last = result;

	result->root = a->inv->root;

	result->inv->root = B;
	B->root = result->inv;
	B->position = b;

	a->next->last = result->inv;
	result->inv->next = a->next;

	a->next = result;
	result->last = a;

	result->loop = a->loop;
	result->inv->loop = a->loop;

	return result;
}

Edge* DCEL::addEdge(Edge* a, Edge* b) {
	Edge* result = createEdge();
	Face* novel = nullptr;

	a->next->last = result->inv;
	result->inv->next = a->next;

	a->next = result;
	result->last = a;

	b->next->last = result;
	result->next = b->next;

	b->next = result->inv;
	result->inv->last = b;

	result->root = a->inv->root;
	result->inv->root = b->inv->root;

	if (a->loop == b->loop) {
		//we have split a loop

		a->loop->root = a;
		result->loop = a->loop;

		novel = createFace();
		novel->root = b;
		novel->reFace();
	}
	else {
		//we have joined two loops
		removeFace(b->loop);
		a->loop->reFace();
	}

	return result;
}

Face* DCEL::draw(FLL<_P> const &boundary) {
	auto track = boundary.getHead();
	
	_P a = track->getValue();
	track = track->getNext();
	_P b = track->getValue();
	track = track->getNext();

	Edge* start = addEdge(a, b);
	Edge* strand = start;
	
	while (track != nullptr) {
		b = track->getValue();
		track = track->getNext();

		strand = addEdge(strand, b);
	}

	addEdge(strand, start->inv);

	return start->loop;
}

/*void DCEL::sanityCheck() {
	for (auto edge : edges) {
		check(edge->inverse_edge->inverse_edge == edge);
		check(edge->next_edge->last_edge == edge);
		check(edge->last_edge->next_edge == edge);

		check(edge->inverse_edge->root_point == edge->next_edge->root_point);
		check(edge->face == edge->next_edge->face);
	}

	for (auto face : faces) {
		if (face->root_edge != nullptr) {

			auto focus = face->root_edge;
			int max_loop = 1000;

			do {
				max_loop--;

				check(focus->face == face);
				check(focus->getStartPoint() != focus->getEndPoint());
				check(focus->getStartPoint()->getPosition() != focus->getEndPoint()->getPosition());

				focus = focus->getNextEdge();

			} while (focus != face->root_edge && max_loop > 0);
		}
		for (auto edge : face->hole_edges) {

			auto focus = edge;
			int max_loop = 1000;

			do {
				max_loop--;

				check(focus->face == face);
				check(focus->getStartPoint() != focus->getEndPoint());
				check(focus->getStartPoint()->getPosition() != focus->getEndPoint()->getPosition());

				focus = focus->getNextEdge();

			} while (focus != edge && max_loop > 0);
		}
	}
}

//culls a polygon to the region represented by this face, then culls from tjos face that region to create a set of new regions
//interior regions are relevant to the proposed polygon
//exterior are regions not relevant to the proposed, but members of this still
bool DCEL::Face::subAllocateFace(const TArray<_P> &border, TArray<Face*> &interior_regions, TArray<Face*> &exterior_regions) {
	dad->sanityCheck();

	//any point is exterior, interior, or border-intersecting
	int f_i = 0;
	const int Num = border.Num();
	const double area = _P::Area(border);

	if (area == 0) {
		UE_LOG(LogTemp, Warning, TEXT("Attempt to SUBALLOCATE 0 area region???"));
		//return nullptr;
		dad->sanityCheck();
		//undefinded behavior!
		return false;
	}
	else if (area < 0) {
		//shape is oriented incorrectly
		TArray<_P> reveresed_boundary;
		for (int ii = Num - 1; ii >= 0; ii--) {
			reveresed_boundary.Push(border[ii]);
		}
		return subAllocateFace(reveresed_boundary, interior_regions, exterior_regions);
	}

	UE_LOG(LogTemp, Warning, TEXT("STARTING SUBALLOCATE"));

	TArray<interact_point*> interactions;
	TArray<bool> touched_holes;

	for (auto temp : hole_edges) {
		touched_holes.Push(false);
	}

	for (int ii = 0; ii < Num; ii++) {
		interactions.Push(new interact_point(border[ii]));
	}

	for (int ii = 0; ii < Num; ii++) {
		interactions[ii]->next = interactions[(ii + 1) % Num];
	}

	UE_LOG(LogTemp, Warning, TEXT("Creating interactions / Finding intersects"));

	TArray<_P> debug_self_border;
	auto *debug_focus = root_edge;
	do {
		debug_self_border.Push(debug_focus->getStartPoint()->getPosition());
		debug_focus = debug_focus->getNextEdge();
	} while (debug_focus != root_edge);

	interact_point* focus = interactions[0];
	do {
		//test point
		if (focus->state == unknown_region) {
			focus->state = getPointState(focus->point);
			UE_LOG(LogTemp, Warning, TEXT("Z: (%d, %d), type %d."), focus->point.X, focus->point.Y, focus->state);
		}

		if (focus->state > 2) {
			touched_holes[(focus->state - 3) / 2] = true;
		}

		//test for intersects
		_P intersect_point;
		interaction_state intersect_result = getFirstIntersect(focus->point, focus->next->point, intersect_point);

		if (intersect_result != unknown_region) {
			//create new point

			interact_point* temp = new interact_point(intersect_point, focus->next);

			interactions.Push(temp);
			focus->next = temp;
			focus->next->state = intersect_result;
		}
		_P mid = (focus->point + focus->next->point) / 2;
		focus->mid_state = getPointState(mid);
		UE_LOG(LogTemp, Warning, TEXT("M: (%d, %d), type %d"), mid.X, mid.Y, focus->mid_state);
		if (intersect_result != unknown_region) {
			UE_LOG(LogTemp, Warning, TEXT("I: (%d, %d), type %d"), intersect_point.X, intersect_point.Y, intersect_result);
		}

		focus = focus->next;

	} while (focus != interactions[0]);

	// validate progression
	focus = interactions[0];
	do {
		if (focus->mid_state == 0) {
			check(focus->state == 0 || focus->state == 1);
			check(focus->next->state == 0 || focus->next->state == 1);
		}
		else if (focus->mid_state == 1) {
			check(focus->state == 0 || focus->state == 1 || focus->state == 2);
			check(focus->next->state == 0 || focus->next->state == 1 || focus->next->state == 2);
		}
		else if (focus->mid_state == 2) {
			check(focus->state == 2 || focus->state % 2 == 1);
			check(focus->next->state == 2 || focus->next->state % 2 == 1);
		}
		else if (focus->mid_state % 2 == 1) {
			check(focus->state == 2 || focus->state == focus->mid_state || focus->state == focus->mid_state + 1);
			check(focus->next->state == 2 || focus->next->state == focus->mid_state || focus->next->state == focus->mid_state + 1);
		}
		else {// if (focus->mid_state % 2 == 0) {
			check(focus->mid_state % 2 == 0);
			check(focus->state == focus->mid_state - 1 || focus->state == focus->mid_state);
			check(focus->next->state == focus->mid_state - 1 || focus->next->state == focus->mid_state);
		}
		focus = focus->next;
	} while (focus != interactions[0]);

	//test for trivial containment cases
	bool all_exterior = true;
	bool all_interior = true;
	bool all_hole = true;
	bool boundary_connected = false;

	for (int index = 0; index < interactions.Num(); index++) {
		const int state = interactions[index]->state;

		if (state == 0) { //exterior
			all_interior = false;
			all_hole = false;
		}
		else if (state == 1) { //boundary
			all_hole = false;
			boundary_connected = true;
		}
		else if (state == 2) { //interior
			all_exterior = false;
			all_hole = false;
		}
		else if (state % 2 == 1) { //hole boundary
			all_exterior = false;
			all_interior = false;
			all_hole = false;
		}
		else { //hole interior
			all_exterior = false;
			all_interior = false;
		}
	}
	//if suggested contains existing, return existing as interior
	//if existing contains suggested, make a new hole and that is your result
	//if contained in a hole, return null
	if (all_exterior) {
		//if that boundary contains this, then it becomes this, otherwise it is culled to null
		if (root_edge != nullptr) {
			Face* test = dad->createFace(border);
			bool contained = true;
			bool sepperated = true;
			auto points = root_edge->listPoints();

			for (auto point : points) {
				if (!isPointContained(point, test->getRootEdge())) {
					contained = false;
				}
				else {
					sepperated = false;
				}
			}

			dad->removeFace(test);

			if (contained) {
				UE_LOG(LogTemp, Warning, TEXT("Trivial : target contained in suggested"));
				interior_regions.Push(this);
				dad->sanityCheck();
				return true;
			}
			if (sepperated) {
				UE_LOG(LogTemp, Warning, TEXT("Trivial : target does not overlap suggested"));
				dad->sanityCheck();
				return false;
			}
		}
	}


	//is the suggested region contained entirely within the target region
	if (all_interior && !boundary_connected) {
		UE_LOG(LogTemp, Warning, TEXT("Trivial : suggested contained in target"));
		Face* interior = dad->createFace(border, this);
		interior_regions.Push(interior);
		exterior_regions.Push(this);
		dad->sanityCheck();
		return true;
	}

	//is the suggested region entirely contained in another set of regions represented here as a hole
	if (all_hole) {
		UE_LOG(LogTemp, Warning, TEXT("Trivial : suggested contained in hole"));
		dad->sanityCheck();
		return false;
	}

	UE_LOG(LogTemp, Warning, TEXT("Standard Case"));

	TArray<strand*> strands;

	strand* current = new strand();

	bool mid_strand = false;

	bool made_loop = false;

	DCEL::Edge* last_edge = nullptr;


	TArray<DCEL::Edge*> eligible_edges;
	if (boundary_connected) {
		root_edge->listFaceLoop(eligible_edges);
	}
	for (auto edge : hole_edges) {
		edge->listFaceLoop(eligible_edges);
	}

	UE_LOG(LogTemp, Warning, TEXT("Discovering / Gluing Strands"));

	while (true) {
		if (mid_strand) {
			if (focus->state != internal_region) {
				UE_LOG(LogTemp, Warning, TEXT("strand ends at %d, %d"), focus->point.X, focus->point.Y);

				mid_strand = false;

				//for the end, we test inversely, to find the entering segment
				Edge* local_a = nullptr; //getContainingSegment(focus->point);
				TArray<_P> debug_compare;



				for (auto edge : eligible_edges) {
					debug_compare.Push(edge->getStartPoint()->getPosition());
					debug_compare.Push(edge->getEndPoint()->getPosition());

					if (_P::isOnSegment(focus->point, edge->getEndPoint()->getPosition(), edge->getStartPoint()->getPosition())) {
						local_a = edge;
						break;
					}
				}

				check(focus->state % 2 == 1);

				if (focus->point == local_a->getEndPoint()->getPosition()) {
					//find appropriate owning edge from root
					DCEL::Edge* rotation_focus = local_a;
					_P test = last_edge->getEndPoint()->getPosition();// -focus->point;
					//test.Normalize();
					do {
						//get the bisector and range
						_P center = rotation_focus->getEndPoint()->getPosition();
						_P right = rotation_focus->getStartPoint()->getPosition();// -rotation_focus->getEndPoint()->getPosition();
						_P left = rotation_focus->getNextEdge()->getEndPoint()->getPosition();// -rotation_focus->getEndPoint()->getPosition();
						//left.Normalize();
						//right.Normalize();

						if (_P::inRegionCW(test, right, center, left)) {//_P::inRegionCW(left, right, test)) {
							local_a = rotation_focus;
							break;
						}

						rotation_focus = rotation_focus->getNextEdge()->getInverseEdge();
					} while (rotation_focus != local_a);
				}
				else {
					local_a->subdivide(focus->point);

					eligible_edges.Push(local_a->getNextEdge());
				}

				local_a->insertAfter(last_edge);

				strands.Push(current);
				current = new strand();
			}
			else {
				UE_LOG(LogTemp, Warning, TEXT("continuing strand to %d, %d"), focus->point.X, focus->point.Y);

				Edge* temp = dad->makeEdge();
				Edge* temp_inverse = dad->makeEdge();

				temp->setInverse(temp_inverse);

				temp->setRoot(dad->makePoint(focus->point));

				temp_inverse->insertAfter(last_edge);

				last_edge = temp_inverse;
			}
		}
		if (made_loop && !mid_strand) {
			break;
		}
		if (!mid_strand && focus->state % 2 == 1) { //are we on a border

			Edge* local = nullptr; //getContainingSegment(focus->point);
			TArray<_P> debug_compare;

			for (auto edge : eligible_edges) {
				debug_compare.Push(edge->getStartPoint()->getPosition());
				debug_compare.Push(edge->getEndPoint()->getPosition());

				if (_P::isOnSegment(focus->point, edge->getStartPoint()->getPosition(), edge->getEndPoint()->getPosition())) {
					local = edge;
					break;
				}
			}

			if (focus->next->state == focus->state) { //is the next point on the same border

				if (!(focus->next->point == local->getEndPoint()->getPosition() ||
					_P::isOnSegment(focus->next->point, local->getStartPoint()->getPosition(), local->getEndPoint()->getPosition()))) { //is the next point on a different edge

					if (focus->mid_state == 2) { //is the mid point of this section interior to the region, or the hole

						UE_LOG(LogTemp, Warning, TEXT("short strand starts at %d, %d"), focus->point.X, focus->point.Y);

						mid_strand = true;

						if (focus->point == local->getStartPoint()->getPosition()) {
							//find appropriate owning edge from root
							DCEL::Edge* rotation_focus = local;
							_P test = focus->next->point;// -focus->point;
							//test.Normalize();
							do {
								//get the bisector and range
								_P center = rotation_focus->getStartPoint()->getPosition();
								_P left = rotation_focus->getEndPoint()->getPosition();// -rotation_focus->getStartPoint()->getPosition();
								_P right = rotation_focus->getLastEdge()->getStartPoint()->getPosition();// -rotation_focus->getStartPoint()->getPosition();
								//left.Normalize();
								//right.Normalize();

								if (_P::inRegionCW(test, right, center, left)) {
									local = rotation_focus;
									break;
								}

								rotation_focus = rotation_focus->getCWOfRoot();
							} while (rotation_focus != local);
						}
						else {
							local->subdivide(focus->point);

							local = local->getNextEdge();
							eligible_edges.Push(local);
						}

						current->interior_edge = dad->makeEdge();
						current->exterior_edge = dad->makeEdge();


						current->interior_edge->setInverse(current->exterior_edge);


						local->insertBefore(current->exterior_edge);

						last_edge = current->exterior_edge;
					}
				}
			}
			else if (focus->next->state == internal_region || focus->next->state % 2 == 1) { //is the next point interior, or crossing the interior to another boundary

				UE_LOG(LogTemp, Warning, TEXT("strand starts at %d, %d"), focus->point.X, focus->point.Y);

				mid_strand = true;

				if (focus->point == local->getStartPoint()->getPosition()) {
					//find appropriate owning edge from root
					DCEL::Edge* rotation_focus = local;
					_P test = focus->next->point;// -focus->point;
					//test.Normalize();
					do {
						//get the bisector and range
						_P center = rotation_focus->getStartPoint()->getPosition();
						_P left = rotation_focus->getEndPoint()->getPosition();// -rotation_focus->getStartPoint()->getPosition();
						_P right = rotation_focus->getLastEdge()->getStartPoint()->getPosition();// -rotation_focus->getStartPoint()->getPosition();
						//left.Normalize();
						//right.Normalize();

						if (_P::inRegionCW(test, right, center, left)) {
							local = rotation_focus;
							break;
						}

						rotation_focus = rotation_focus->getCWOfRoot();
					} while (rotation_focus != local);
				}
				else {
					local->subdivide(focus->point);

					local = local->getNextEdge();
					eligible_edges.Push(local);
				}

				current->interior_edge = dad->makeEdge();
				current->exterior_edge = dad->makeEdge();

				current->interior_edge->setInverse(current->exterior_edge);


				local->insertBefore(current->exterior_edge);

				last_edge = current->exterior_edge;
			}
			else {
				UE_LOG(LogTemp, Warning, TEXT("no strand to %d, %d"), focus->point.X, focus->point.Y);
			}
		}
		else if (!mid_strand) {
			UE_LOG(LogTemp, Warning, TEXT("no strand to %d, %d"), focus->point.X, focus->point.Y);
		}

		focus = focus->next;
		if (focus == interactions[0]) {
			made_loop = true;
		}
		if (made_loop && !mid_strand) {
			break;
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("Discovery Complete"));

	delete current;
	if (strands.Num() == 0) {
		for (auto focus : interactions) {
			delete focus;
		}
		dad->sanityCheck();
		UE_LOG(LogTemp, Warning, TEXT("No Strands? What does this imply..."));
		return false;
	}

	UE_LOG(LogTemp, Warning, TEXT("Unique Components Detection"));
	//find unique sections

	if (!boundary_connected) {
		exterior_regions.Push(this);
	}
	for (int ii = hole_edges.Num() - 1; ii >= 0; ii--) {
		if (touched_holes[ii]) {
			hole_edges.RemoveAt(ii);
		}
	}

	TArray<Edge*> existing_holes;
	for (auto hole : hole_edges) {
		existing_holes.Push(hole);
	}
	hole_edges.Empty();

	for (int index = 0; index < strands.Num(); index++) {
		if (strands[index]->unique_interior) {
			//trace from end to start and mark encountered starts, doesn't skips strands
			Edge* focus = strands[index]->interior_edge;

			do {
				for (int compare = index; compare < strands.Num(); compare++) {
					if (focus == strands[compare]->interior_edge) {

						strands[compare]->unique_interior = false;
						break;
					}
				}

				focus = focus->getNextEdge();
			} while (focus != strands[index]->interior_edge);

			Face* interior_face = dad->makeFace();

			interior_face->root_edge = strands[index]->interior_edge;
			interior_face->root_edge->reFaceLoop(interior_face);

			interior_regions.Push(interior_face);

		}

		if (strands[index]->unique_exterior) {
			//trace from start inverse to end inverse and mark encountered end inverses, skips strands
			Edge* focus = strands[index]->exterior_edge;

			do {
				bool found = false;

				for (int compare = index; compare < strands.Num(); compare++) {
					if (focus == strands[compare]->exterior_edge) {

						strands[compare]->unique_exterior = false;

						break;
					}
				}

				focus = focus->getNextEdge();
			} while (focus != strands[index]->exterior_edge);

			if (boundary_connected) {

				Face* exterior_face = dad->makeFace();

				exterior_face->root_edge = strands[index]->exterior_edge;
				exterior_face->root_edge->reFaceLoop(exterior_face);

				exterior_regions.Push(exterior_face);
			}
			else {
				hole_edges.Push(strands[index]->exterior_edge);
				strands[index]->exterior_edge->reFaceLoop(this);
			}
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("Placing holes"));

	//find hole positions
	for (auto hole : existing_holes) {

		// find containing region
		// insert into region holes
		bool found = false;
		for (auto region : interior_regions) {
			if (isPointContained(hole->getStartPoint()->getPosition(), region->root_edge)) {
				found = true;
				region->hole_edges.Push(hole);
				hole->reFaceLoop(region);
				break;
			}
		}
		if (!found) {
			for (auto region : exterior_regions) {
				if (isPointContained(hole->getStartPoint()->getPosition(), region->root_edge)) {
					found = true;
					region->hole_edges.Push(hole);
					hole->reFaceLoop(region);
					break;
				}
			}
		}
	}

	auto master = dad;

	if (boundary_connected) {
		master->removeFace(this);
	}

	master->sanityCheck();

	for (auto string : strands) {
		delete string;
	}

	for (auto focus : interactions) {
		delete focus;
	}

	return true;
}

//attempts to merge target faces to self by continuity across a shared edge section.
bool DCEL::Face::mergeWithFace(Face* target) {
	//cases
	//a) completely sorrounded by them (we are a hole in them)
	//b) completely sorrounding them (they are a hole in us)
	//c) partially meeting them (mark all boundary edges, find uniques, determine external boundary by wrapping
	//d) no meeting (no union occurs)

	bool found_merge = false;

	Edge* focus = root_edge;
	Edge* first_free = nullptr;

	do {
		if (focus->getInverseEdge()->getFace() == target) {
			found_merge = true;
			if (first_free != nullptr) { break; }
		}
		else if (first_free == nullptr) {
			first_free = focus;
			if (found_merge) { break; }
		}
		focus = focus->getNextEdge();
	} while (focus != root_edge);
	if (found_merge) {
		if (first_free == nullptr) { //------------------------------------------
									 //we are a hole, FILL IT
									 //list boundary edges, and points
			TArray<Edge*> edges;
			TArray<Point*> points;

			focus = root_edge;
			do {
				edges.Push(focus);
				points.Push(focus->getStartPoint());

				focus = focus->getNextEdge();
			} while (focus != root_edge);

			root_edge = target->root_edge;
			root_edge->reFaceLoop(this);

			//add their holes that aren't this to us
			for (auto edge : target->hole_edges) {
				if (edge->getInverseEdge()->getFace() != this) {
					edge->reFaceLoop(this);
					hole_edges.Push(edge);
				}
			}

			//delete ALL
			for (auto edge : edges) {
				dad->removeEdge(edge->getInverseEdge());
				dad->removeEdge(edge);
			}
			for (auto point : points) {
				dad->removePoint(point);
			}
			dad->removeFace(target);
		}
		else { //------------------------------------------
			   //they reside on our border


			   //add their holes that aren't this to us
			bool found_this = false;
			bool is_hole = false;
			for (auto edge : target->hole_edges) {

				if (!found_this) {
					focus = edge;

					do {
						if (focus->getInverseEdge()->getFace() == this) {
							found_this = true;
							is_hole = true;
							break;
						}
					} while (focus != edge);

					if (found_this) {
						continue;
					}
				}

				edge->reFaceLoop(this);
				hole_edges.Push(edge);
			}

			focus = first_free;
			bool mid_strand = false;
			bool made_loop = false;
			TArray<Edge*> strands;

			do {
				Edge* next = focus->getNextEdge();

				if (!mid_strand) {
					if (focus->getInverseEdge()->getFace() == target) {
						//strand starts
						//wrap the edges before, and the edge after

						focus->removeFromGraph();
						dad->removeEdge(focus->getInverseEdge());
						dad->removeEdge(focus);

						mid_strand = true;

						focus = next;
					}
					else {
						focus = next;
					}
				}
				else {
					if (focus->getInverseEdge()->getFace() == target) {
						//strand continues

						//remove the last point
						dad->removePoint(focus->getStartPoint());

						focus->removeFromGraph();
						dad->removeEdge(focus->getInverseEdge());
						dad->removeEdge(focus);

						focus = next;
					}
					else {
						//strand ends
						strands.Push(focus);

						mid_strand = false;

						focus = next;
					}
				}

				if (focus == first_free) {
					made_loop = true;
				}
				if (made_loop && !mid_strand) {
					break;
				}
			} while (true);


			if (is_hole) {
				root_edge = target->root_edge;
				root_edge->reFaceLoop(this);

				for (auto edge : strands) {
					edge->reFaceLoop(this);
					hole_edges.Push(edge);
				}
			}
			else {
				found_this = false;
				for (auto edge : strands) {
					edge->reFaceLoop(this);

					if (!found_this) {
						if (edge->loopArea() > 0) {
							found_this = true;
							root_edge = edge;
							continue;
						}
					}

					hole_edges.Push(edge);
				}
			}

			dad->removeFace(target);
		}

		dad->sanityCheck();
		cleanBorder();
		return true;
	}
	else { //they are bound to a hole, or not at all ============================================
		for (auto root : hole_edges) {

			found_merge = false;
			first_free = nullptr; //redundant, oh well

			focus = root;
			do {
				if (focus->getInverseEdge()->getFace() == target) {
					found_merge = true;
					if (first_free != nullptr) { break; }
				}
				else if (first_free == nullptr) {
					first_free = focus;
					if (found_merge) { break; }
				}
				focus = focus->getNextEdge();
			} while (focus != root);
			if (found_merge) {
				if (first_free == nullptr) {//------------------------------------------
											//they are a hole, FILL IT
											//list boundary edges, and points
					TArray<Edge*> edges;
					TArray<Point*> points;

					focus = root;
					do {
						edges.Push(focus);
						points.Push(focus->getStartPoint());

						focus = focus->getNextEdge();
					} while (focus != root);

					//add their holes that aren't this to us
					for (auto edge : target->hole_edges) {
						edge->reFaceLoop(this);
						hole_edges.Push(edge);
					}

					//delete ALL
					for (auto edge : edges) {
						dad->removeEdge(edge->getInverseEdge());
						dad->removeEdge(edge);
					}
					for (auto point : points) {
						dad->removePoint(point);
					}
					dad->removeFace(target);

					//remove the reference to this hole
					hole_edges.Remove(root);

				}
				else { //------------------------------------------
					   //add their holes to us
					bool found_this = false;
					bool is_hole = false;

					hole_edges.Remove(root);

					for (auto edge : target->hole_edges) {
						edge->reFaceLoop(this);
						hole_edges.Push(edge);
					}

					focus = first_free;
					bool mid_strand = false;
					bool made_loop = false;
					TArray<Edge*> strands;

					do {
						Edge* next = focus->getNextEdge();

						if (!mid_strand) {
							if (focus->getInverseEdge()->getFace() == target) {
								//strand starts
								//wrap the edges before, and the edge after

								focus->removeFromGraph();
								dad->removeEdge(focus->getInverseEdge());
								dad->removeEdge(focus);

								mid_strand = true;

								focus = next;
							}
							else {
								focus = next;
							}
						}
						else {
							if (focus->getInverseEdge()->getFace() == target) {
								//strand continues

								//remove the last point
								dad->removePoint(focus->getStartPoint());

								focus->removeFromGraph();
								dad->removeEdge(focus->getInverseEdge());
								dad->removeEdge(focus);

								focus = next;
							}
							else {
								//strand ends
								strands.Push(focus);

								mid_strand = false;

								focus = next;
							}
						}

						if (focus == first_free) {
							made_loop = true;
						}
						if (made_loop && !mid_strand) {
							break;
						}
					} while (true);


					for (auto edge : strands) {
						edge->reFaceLoop(this);
						hole_edges.Push(edge);
					}

					dad->removeFace(target);
				}

				dad->sanityCheck();
				cleanBorder();
				return true;
			}
		}

		dad->sanityCheck();
		return false;
	}
}


*/