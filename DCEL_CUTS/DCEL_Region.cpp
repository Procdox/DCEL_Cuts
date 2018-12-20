#include "DCEL_Region.h"

FaceRelation const getPointRelation(Face<Pint> const &rel, Pint const &test_point) {
	Edge<Pint> const * focus = rel.getRoot();
	int count = 0;

	bool is_best = false;
	float best_distance = 0;
	bool inside = false;

	do {
		const Pint &start_vector = focus->getStart()->getPosition();
		const Pint &end_vector = focus->getEnd()->getPosition();

		//does it sit on
		int y_length = end_vector.Y - start_vector.Y;
		int y_offset = test_point.Y - start_vector.Y;

		if (y_length == 0) {
			if (y_offset == 0) {
				if ((start_vector.X <= test_point.X && test_point.X <= end_vector.X) ||
					(start_vector.X >= test_point.X && test_point.X >= end_vector.X)) {

					return FaceRelation(FaceRelationType::point_on_boundary, focus);
				}
			}
			focus = focus->getNext();
			continue;
		}
		else if ((y_offset <= y_length && y_offset >= 0) || y_offset >= y_length && y_offset <= 0) {
			int x_length = end_vector.X - start_vector.X;

			if ((x_length * y_offset) % y_length == 0) {
				//robust solution!
				int x = start_vector.X + (x_length * y_offset) / y_length;
				int distance = test_point.X - x;

				if (distance == 0) {
					return FaceRelation(FaceRelationType::point_on_boundary, focus);
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
		focus = focus->getNext();
	} while (focus != rel.getRoot());

	if (inside) {
		return FaceRelation(FaceRelationType::point_interior, nullptr);
	}
	else {
		return FaceRelation(FaceRelationType::point_exterior, nullptr);
	}
}

FaceRelation Region::contains(Pint const &test_point) {
	auto focus = Boundaries.getHead();
	while (focus != nullptr) {

		FaceRelation result = getPointRelation(*focus->getValue(), test_point);
		if (result.type != FaceRelationType::point_interior) {
			return result;
		}

		focus = focus->getNext();
	}

	return FaceRelation(FaceRelationType::point_interior, nullptr);
}

bool Region::merge(Region * target) {
	//since both areas are continuous, its trivial that only one or no boundary pair can touch
	
	//regions are either strictly internal, strictly external, or weakly external to boundaries

	//if a boundary contains any part of a region, it can't touch that region (
	//valve timing control solenoid 348
	//fluids 145, 79, 99
	//differential 109
	//oil change 45
	//oil leak 295 
	//spark plugs 170

	auto focus_local = Boundaries.getHead();
	auto focus_target  = target->Boundaries.getHead();
	

	while (focus_local != nullptr) {
		focus_target = target->Boundaries.getHead();
		auto neighbors = focus_local->getValue()->getNeighbors();

		while (focus_target != nullptr) {

			if (neighbors.contains(focus_target->getValue())) {

				break;
			}


			focus_target = focus_target->getNext();
		}
		if (focus_target != nullptr) {
			break;
		}

		focus_local = focus_local->getNext();
	}
	if (focus_local == nullptr) return false;

	//we have found a boundary pair that neighbors one another, merge them

	auto tbd = focus_local->getValue();

	auto tba = tbd->mergeWithFace(focus_target->getValue());

	//now edit the boundary lists of both regions

	Boundaries.absorb(tba);

	Boundaries.remove(tbd);

	target->Boundaries.clear();

	return true;
}



/* subAllocateFace
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
}*/
