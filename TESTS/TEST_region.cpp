#include "pch.h"
#include "..\DCEL_CUTS\DCEL_Region.h"

TEST(Region_Basics, CW_Face_Contains) {
	DCEL<Pint> space;

	FLL<Pint> boundary_big;
	{
		boundary_big.append(Pint(0, 0));
		boundary_big.append(Pint(0, 20));
		boundary_big.append(Pint(20, 20));
		boundary_big.append(Pint(20, 0));
	}

	auto target = space.draw(boundary_big);

	//inside simple
	EXPECT_EQ(getPointRelation(*target, Pint(2, 2)).type, FaceRelationType::point_interior);

	//outside simple
	EXPECT_EQ(getPointRelation(*target, Pint(-2, 2)).type, FaceRelationType::point_exterior);

	//boundary simple
	EXPECT_EQ(getPointRelation(*target, Pint(0, 2)).type, FaceRelationType::point_on_boundary);
}

TEST(Region_Basics, CCW_Face_Contains) {
	DCEL<Pint> space;

	FLL<Pint> boundary_big;
	{
		boundary_big.append(Pint(20, 0));
		boundary_big.append(Pint(20, 20));
		boundary_big.append(Pint(0, 20));
		boundary_big.append(Pint(0, 0));
	}

	auto target = space.draw(boundary_big);

	//inside simple
	EXPECT_EQ(getPointRelation(*target, Pint(2, 2)).type, FaceRelationType::point_exterior);

	//outside simple
	EXPECT_EQ(getPointRelation(*target, Pint(-2, 2)).type, FaceRelationType::point_interior);

	//boundary simple
	EXPECT_EQ(getPointRelation(*target, Pint(0, 2)).type, FaceRelationType::point_on_boundary);
}

TEST(Region_Basics, Region_Contains) {
	DCEL<Pint> space;

	FLL<Pint> boundary_big;
	{
		boundary_big.append(Pint(0, 0));
		boundary_big.append(Pint(0, 20));
		boundary_big.append(Pint(20, 20));
		boundary_big.append(Pint(20, 0));
	}

	Region * target = new Region(&space, boundary_big);

	//inside simple
	EXPECT_EQ(target->contains(Pint(2,2)).type, FaceRelationType::point_interior);

	//outside simple
	EXPECT_EQ(target->contains(Pint(-2, 2)).type, FaceRelationType::point_exterior);

	//boundary simple
	EXPECT_EQ(target->contains(Pint(0, 2)).type, FaceRelationType::point_on_boundary);
}


TEST(Face_Cuts, Hole) {
	DCEL<Pint> space;

	FLL<Pint> boundary_big;
	{
		boundary_big.append(Pint(0, 0));
		boundary_big.append(Pint(0, 20));
		boundary_big.append(Pint(20, 20));
		boundary_big.append(Pint(20, 0));
	}
	FLL<Pint> boundary_small_a;
	{
		boundary_small_a.append(Pint(2, 2));
		boundary_small_a.append(Pint(2, 4));
		boundary_small_a.append(Pint(4, 4));
		boundary_small_a.append(Pint(4, 2));
	}

	FLL<Region *> interior;
	FLL<Region *> exterior;

	Region * product = new Region(&space, boundary_big);

	subAllocate(product, boundary_small_a, interior, exterior);

	ASSERT_EQ(exterior.size(), 1);
	ASSERT_EQ(interior.size(), 1);

	auto ex_a = exterior.getHead()->getValue()->getBounds();
	auto in_a = interior.getHead()->getValue()->getBounds();

	ASSERT_EQ(ex_a->size(), 2);
	ASSERT_EQ(in_a->size(), 1);

	auto ex_a_focus = ex_a->getHead();
	auto ex_a_a = ex_a_focus->getValue();
	ex_a_focus = ex_a_focus->getNext();
	auto ex_a_b = ex_a_focus->getValue();

	auto in_focus = in_a->getHead();
	auto in_a_a = in_focus->getValue();

	EXPECT_EQ(Pint::area(ex_a_a->getLoopPoints()), 400);
	EXPECT_EQ(Pint::area(ex_a_b->getLoopPoints()), -4);

	EXPECT_EQ(Pint::area(in_a_a->getLoopPoints()), 4);

	EXPECT_EQ(in_a_a->getRoot()->getFace(), ex_a_b->getRoot()->getInv()->getFace());
}

/*
TEST(Face_Cuts, Edge_Meeting_Cut) {
	DCEL* space = new DCEL();

	FLL<Pint> boundary_big;
	{
		boundary_big.append(Pint(0, 0));
		boundary_big.append(Pint(0, 20));
		boundary_big.append(Pint(20, 20));
		boundary_big.append(Pint(20, 0));
	}
	FLL<Pint> boundary_small_a;
	{
		boundary_small_a.append(Pint(0, 2));
		boundary_small_a.append(Pint(0, 4));
		boundary_small_a.append(Pint(4, 4));
		boundary_small_a.append(Pint(4, 2));
	}

	FLL<DCEL::Face*> interior;
	FLL<DCEL::Face*> exterior;

	DCEL::Face* product = space->createFace(boundary_big);
	DCEL::Face* null = product->getRootEdge()->getInverseEdge()->getFace();
	product->subAllocateFace(boundary_small_a, interior, exterior);

	EXPECT_EQ(exterior.size(), 1);
	EXPECT_EQ(interior.size(), 1);

	EXPECT_EQ(interior[0]->getHoleCount(), 0);
	EXPECT_EQ(exterior[0]->getHoleCount(), 0);
	EXPECT_EQ(null->getHoleCount(), 1);

	EXPECT_EQ(interior[0]->borderCount(10), 4);
	EXPECT_EQ(exterior[0]->borderCount(10), 8);
	EXPECT_EQ(null->holeBorderCount(0, 10), 6);

	EXPECT_EQ(interior[0]->getRootEdge()->loopArea(), 8);
	EXPECT_EQ(exterior[0]->getRootEdge()->loopArea(), 392);
	EXPECT_EQ(null->getHole(0)->loopArea(), -400);
}

TEST(Face_Cuts, Point_Meeting_Cut) {
	DCEL* space = new DCEL();

	FLL<Pint> boundary_big;
	{
		boundary_big.append(Pint(2, 0));
		boundary_big.append(Pint(2, 2));
		boundary_big.append(Pint(0, 2));
		boundary_big.append(Pint(0, 20));
		boundary_big.append(Pint(20, 20));
		boundary_big.append(Pint(20, 0));
	}
	FLL<Pint> boundary_small_a;
	{
		boundary_small_a.append(Pint(2, 2));
		boundary_small_a.append(Pint(2, 4));
		boundary_small_a.append(Pint(4, 4));
		boundary_small_a.append(Pint(4, 2));
	}

	FLL<DCEL::Face*> interior;
	FLL<DCEL::Face*> exterior;

	DCEL::Face* product = space->createFace(boundary_big);
	DCEL::Face* null = product->getRootEdge()->getInverseEdge()->getFace();
	product->subAllocateFace(boundary_small_a, interior, exterior);

	EXPECT_EQ(exterior.size(), 1);
	EXPECT_EQ(interior.size(), 1);

	EXPECT_EQ(interior[0]->getHoleCount(), 0);
	EXPECT_EQ(exterior[0]->getHoleCount(), 0);
	EXPECT_EQ(null->getHoleCount(), 1);

	EXPECT_EQ(interior[0]->borderCount(10), 4);
	EXPECT_EQ(exterior[0]->borderCount(20), 10);
	EXPECT_EQ(null->holeBorderCount(0, 20), 6);

	EXPECT_EQ(interior[0]->getRootEdge()->loopArea(), 4);
	EXPECT_EQ(exterior[0]->getRootEdge()->loopArea(), 392);
	EXPECT_EQ(null->getHole(0)->loopArea(), -396);
}

TEST(Face_Cuts, Matching_Cut) {
	DCEL* space = new DCEL();

	FLL<Pint> boundary_big;
	{
		boundary_big.append(Pint(0, 0));
		boundary_big.append(Pint(0, 2));
		boundary_big.append(Pint(2, 2));
		boundary_big.append(Pint(2, 4));
		boundary_big.append(Pint(0, 4));
		boundary_big.append(Pint(0, 20));
		boundary_big.append(Pint(20, 20));
		boundary_big.append(Pint(20, 0));
	}
	FLL<Pint> boundary_small_a;
	{
		boundary_small_a.append(Pint(2, 2));
		boundary_small_a.append(Pint(2, 4));
		boundary_small_a.append(Pint(4, 4));
		boundary_small_a.append(Pint(4, 2));
	}

	FLL<DCEL::Face*> interior;
	FLL<DCEL::Face*> exterior;

	DCEL::Face* product = space->createFace(boundary_big);
	DCEL::Face* null = product->getRootEdge()->getInverseEdge()->getFace();
	product->subAllocateFace(boundary_small_a, interior, exterior);

	EXPECT_EQ(exterior.size(), 1);
	EXPECT_EQ(interior.size(), 1);

	EXPECT_EQ(interior[0]->getHoleCount(), 0);
	EXPECT_EQ(exterior[0]->getHoleCount(), 0);
	EXPECT_EQ(null->getHoleCount(), 1);

	EXPECT_EQ(interior[0]->borderCount(10), 4);
	EXPECT_EQ(exterior[0]->borderCount(20), 10);
	EXPECT_EQ(null->holeBorderCount(0, 10), 8);

	EXPECT_EQ(interior[0]->getRootEdge()->loopArea(), 4);
	EXPECT_EQ(exterior[0]->getRootEdge()->loopArea(), 392);
	EXPECT_EQ(null->getHole(0)->loopArea(), -396);
}

TEST(Face_Cuts, Before_Overlapping_Cut) {
	DCEL* space = new DCEL();

	FLL<Pint> boundary_big;
	{
		boundary_big.append(Pint(0, 0));
		boundary_big.append(Pint(0, 2));
		boundary_big.append(Pint(2, 2));
		boundary_big.append(Pint(2, 4));
		boundary_big.append(Pint(0, 4));
		boundary_big.append(Pint(0, 20));
		boundary_big.append(Pint(20, 20));
		boundary_big.append(Pint(20, 0));
	}
	FLL<Pint> boundary_small_a;
	{
		boundary_small_a.append(Pint(2, 1));
		boundary_small_a.append(Pint(2, 3));
		boundary_small_a.append(Pint(4, 3));
		boundary_small_a.append(Pint(4, 1));
	}

	FLL<DCEL::Face*> interior;
	FLL<DCEL::Face*> exterior;

	DCEL::Face* product = space->createFace(boundary_big);
	DCEL::Face* null = product->getRootEdge()->getInverseEdge()->getFace();
	product->subAllocateFace(boundary_small_a, interior, exterior);

	auto debug_array = exterior[0]->getRootEdge()->listPoints();

	EXPECT_EQ(exterior.size(), 1);
	EXPECT_EQ(interior.size(), 1);

	EXPECT_EQ(interior[0]->getHoleCount(), 0);
	EXPECT_EQ(exterior[0]->getHoleCount(), 0);
	EXPECT_EQ(null->getHoleCount(), 1);

	EXPECT_EQ(interior[0]->borderCount(10), 5);
	EXPECT_EQ(exterior[0]->borderCount(20), 12);
	EXPECT_EQ(null->holeBorderCount(0, 10), 9);

	EXPECT_EQ(interior[0]->getRootEdge()->loopArea(), 4);
	EXPECT_EQ(exterior[0]->getRootEdge()->loopArea(), 392);
	EXPECT_EQ(null->getHole(0)->loopArea(), -396);
}

TEST(Face_Cuts, After_Overlapping_Cut) {
	DCEL* space = new DCEL();

	FLL<Pint> boundary_big;
	{
		boundary_big.append(Pint(0, 0));
		boundary_big.append(Pint(0, 2));
		boundary_big.append(Pint(2, 2));
		boundary_big.append(Pint(2, 4));
		boundary_big.append(Pint(0, 4));
		boundary_big.append(Pint(0, 20));
		boundary_big.append(Pint(20, 20));
		boundary_big.append(Pint(20, 0));
	}
	FLL<Pint> boundary_small_a;
	{
		boundary_small_a.append(Pint(1, 4));
		boundary_small_a.append(Pint(1, 6));
		boundary_small_a.append(Pint(3, 6));
		boundary_small_a.append(Pint(3, 4));
	}

	FLL<DCEL::Face*> interior;
	FLL<DCEL::Face*> exterior;

	DCEL::Face* product = space->createFace(boundary_big);
	DCEL::Face* null = product->getRootEdge()->getInverseEdge()->getFace();
	product->subAllocateFace(boundary_small_a, interior, exterior);

	auto debug_array = exterior[0]->getRootEdge()->listPoints();

	EXPECT_EQ(exterior.size(), 1);
	EXPECT_EQ(interior.size(), 1);

	EXPECT_EQ(interior[0]->getHoleCount(), 0);
	EXPECT_EQ(exterior[0]->getHoleCount(), 0);
	EXPECT_EQ(null->getHoleCount(), 1);

	EXPECT_EQ(interior[0]->borderCount(10), 5);
	EXPECT_EQ(exterior[0]->borderCount(20), 12);
	EXPECT_EQ(null->holeBorderCount(0, 10), 9);

	EXPECT_EQ(interior[0]->getRootEdge()->loopArea(), 4);
	EXPECT_EQ(exterior[0]->getRootEdge()->loopArea(), 392);
	EXPECT_EQ(null->getHole(0)->loopArea(), -396);
}

TEST(Face_Cuts, Overlapping_Cut) {
	DCEL* space = new DCEL();

	FLL<Pint> boundary_big;
	{
		boundary_big.append(Pint(0, 0));
		boundary_big.append(Pint(0, 2));
		boundary_big.append(Pint(2, 2));
		boundary_big.append(Pint(2, 4));
		boundary_big.append(Pint(0, 4));
		boundary_big.append(Pint(0, 20));
		boundary_big.append(Pint(20, 20));
		boundary_big.append(Pint(20, 0));
	}
	FLL<Pint> boundary_small_a;
	{
		boundary_small_a.append(Pint(2, 1));
		boundary_small_a.append(Pint(2, 5));
		boundary_small_a.append(Pint(4, 5));
		boundary_small_a.append(Pint(4, 1));
	}

	FLL<DCEL::Face*> interior;
	FLL<DCEL::Face*> exterior;

	DCEL::Face* product = space->createFace(boundary_big);
	DCEL::Face* null = product->getRootEdge()->getInverseEdge()->getFace();
	product->subAllocateFace(boundary_small_a, interior, exterior);

	auto debug_array = exterior[0]->getRootEdge()->listPoints();

	EXPECT_EQ(exterior.size(), 1);
	EXPECT_EQ(interior.size(), 1);

	EXPECT_EQ(interior[0]->getHoleCount(), 0);
	EXPECT_EQ(exterior[0]->getHoleCount(), 0);
	EXPECT_EQ(null->getHoleCount(), 1);

	EXPECT_EQ(interior[0]->borderCount(10), 6);
	EXPECT_EQ(exterior[0]->borderCount(20), 12);
	EXPECT_EQ(null->holeBorderCount(0, 10), 8);

	EXPECT_EQ(interior[0]->getRootEdge()->loopArea(), 8);
	EXPECT_EQ(exterior[0]->getRootEdge()->loopArea(), 388);
	EXPECT_EQ(null->getHole(0)->loopArea(), -396);
}

TEST(Face_Cuts, Stacked_Holes) {
	DCEL* space = new DCEL();

	FLL<Pint> boundary_big;
	{
		boundary_big.append(Pint(0, 0));
		boundary_big.append(Pint(0, 20));
		boundary_big.append(Pint(20, 20));
		boundary_big.append(Pint(20, 0));
	}
	FLL<Pint> boundary_small_a;
	{
		boundary_small_a.append(Pint(2, 2));
		boundary_small_a.append(Pint(2, 18));
		boundary_small_a.append(Pint(4, 18));
		boundary_small_a.append(Pint(4, 2));
	}
	FLL<Pint> boundary_small_b;
	{
		boundary_small_b.append(Pint(0, 4));
		boundary_small_b.append(Pint(0, 16));
		boundary_small_b.append(Pint(2, 16));
		boundary_small_b.append(Pint(2, 4));
	}

	FLL<DCEL::Face*> interiorPintrep;
	FLL<DCEL::Face*> exteriorPintrep;

	FLL<DCEL::Face*> interior;
	FLL<DCEL::Face*> exterior;

	DCEL::Face* product = space->createFace(boundary_big);
	DCEL::Face* null = product->getRootEdge()->getInverseEdge()->getFace();
	product->subAllocateFace(boundary_small_b, interiorPintrep, exteriorPintrep);
	exteriorPintrep[0]->subAllocateFace(boundary_small_a, interior, exterior);

	//auto test = exterior[0]->getHole(0)->listPoints();

	EXPECT_EQ(exteriorPintrep.size(), 1);
	EXPECT_EQ(interiorPintrep.size(), 1);
	EXPECT_EQ(exterior.size(), 1);
	EXPECT_EQ(interior.size(), 1);

	EXPECT_EQ(interiorPintrep[0]->getHoleCount(), 0);
	EXPECT_EQ(interior[0]->getHoleCount(), 0);
	EXPECT_EQ(exterior[0]->getHoleCount(), 0);
	EXPECT_EQ(null->getHoleCount(), 1);

	EXPECT_EQ(interiorPintrep[0]->borderCount(10), 4);
	EXPECT_EQ(interior[0]->borderCount(10), 6);
	EXPECT_EQ(exterior[0]->borderCount(20), 12);
	EXPECT_EQ(null->holeBorderCount(0, 20), 6);

	EXPECT_EQ(interiorPintrep[0]->getRootEdge()->loopArea(), 24);
	EXPECT_EQ(interior[0]->getRootEdge()->loopArea(), 32);
	EXPECT_EQ(exterior[0]->getRootEdge()->loopArea(), 344);
	EXPECT_EQ(null->getHole(0)->loopArea(), -400);
}

TEST(Face_Cuts, Horshoe_Cut) {
	DCEL* space = new DCEL();

	FLL<Pint> boundary_big;
	{
		boundary_big.append(Pint(0, 0));
		boundary_big.append(Pint(0, 20));
		boundary_big.append(Pint(20, 20));
		boundary_big.append(Pint(20, 0));
	}
	FLL<Pint> boundary_small_a;
	{
		boundary_small_a.append(Pint(4, -8));
		boundary_small_a.append(Pint(4, 4));
		boundary_small_a.append(Pint(8, 4));
		boundary_small_a.append(Pint(8, -4));
		boundary_small_a.append(Pint(12, -4));
		boundary_small_a.append(Pint(12, 4));
		boundary_small_a.append(Pint(16, 4));
		boundary_small_a.append(Pint(16, -8));
	}

	FLL<DCEL::Face*> interior;
	FLL<DCEL::Face*> exterior;

	DCEL::Face* product = space->createFace(boundary_big);
	DCEL::Face* null = product->getRootEdge()->getInverseEdge()->getFace();
	product->subAllocateFace(boundary_small_a, interior, exterior);

	EXPECT_EQ(exterior.size(), 1);
	EXPECT_EQ(interior.size(), 2);

	EXPECT_EQ(interior[0]->getHoleCount(), 0);
	EXPECT_EQ(interior[1]->getHoleCount(), 0);
	EXPECT_EQ(exterior[0]->getHoleCount(), 0);
	EXPECT_EQ(null->getHoleCount(), 1);

	EXPECT_EQ(interior[0]->borderCount(10), 4);
	EXPECT_EQ(interior[1]->borderCount(10), 4);
	EXPECT_EQ(exterior[0]->borderCount(20), 12);
	EXPECT_EQ(null->holeBorderCount(0, 20), 8);

	EXPECT_EQ(interior[0]->getRootEdge()->loopArea(), 16);
	EXPECT_EQ(interior[0]->getRootEdge()->loopArea(), 16);
	EXPECT_EQ(exterior[0]->getRootEdge()->loopArea(), 368);
	EXPECT_EQ(null->getHole(0)->loopArea(), -400);
}

TEST(Face_Cuts, Corner_Meeting_Cut) {
	DCEL* space = new DCEL();

	FLL<Pint> boundary_big;
	{
		boundary_big.append(Pint(0, 0));
		boundary_big.append(Pint(0, 20));
		boundary_big.append(Pint(20, 20));
		boundary_big.append(Pint(20, 0));
	}
	FLL<Pint> boundary_small_a;
	{
		boundary_small_a.append(Pint(0, 0));
		boundary_small_a.append(Pint(0, 4));
		boundary_small_a.append(Pint(4, 4));
		boundary_small_a.append(Pint(4, 0));
	}

	FLL<DCEL::Face*> interior;
	FLL<DCEL::Face*> exterior;

	DCEL::Face* product = space->createFace(boundary_big);
	DCEL::Face* null = product->getRootEdge()->getInverseEdge()->getFace();
	product->subAllocateFace(boundary_small_a, interior, exterior);

	EXPECT_EQ(exterior.size(), 1);
	EXPECT_EQ(interior.size(), 1);

	EXPECT_EQ(interior[0]->getHoleCount(), 0);
	EXPECT_EQ(exterior[0]->getHoleCount(), 0);
	EXPECT_EQ(null->getHoleCount(), 1);

	EXPECT_EQ(interior[0]->borderCount(10), 4);
	EXPECT_EQ(exterior[0]->borderCount(10), 6);
	EXPECT_EQ(null->holeBorderCount(0, 10), 6);

	EXPECT_EQ(interior[0]->getRootEdge()->loopArea(), 16);
	EXPECT_EQ(exterior[0]->getRootEdge()->loopArea(), 384);
	EXPECT_EQ(null->getHole(0)->loopArea(), -400);
}

TEST(Face_Cuts, Edge_Crossing_Cut) {
	DCEL* space = new DCEL();

	FLL<Pint> boundary_big;
	{
		boundary_big.append(Pint(0, 0));
		boundary_big.append(Pint(0, 20));
		boundary_big.append(Pint(20, 20));
		boundary_big.append(Pint(20, 0));
	}
	FLL<Pint> boundary_small_a;
	{
		boundary_small_a.append(Pint(-4, 2));
		boundary_small_a.append(Pint(-4, 4));
		boundary_small_a.append(Pint(4, 4));
		boundary_small_a.append(Pint(4, 2));
	}

	FLL<DCEL::Face*> interior;
	FLL<DCEL::Face*> exterior;

	DCEL::Face* product = space->createFace(boundary_big);
	DCEL::Face* null = product->getRootEdge()->getInverseEdge()->getFace();
	product->subAllocateFace(boundary_small_a, interior, exterior);

	EXPECT_EQ(exterior.size(), 1);
	EXPECT_EQ(interior.size(), 1);

	EXPECT_EQ(interior[0]->getHoleCount(), 0);
	EXPECT_EQ(exterior[0]->getHoleCount(), 0);
	EXPECT_EQ(null->getHoleCount(), 1);

	EXPECT_EQ(interior[0]->borderCount(10), 4);
	EXPECT_EQ(exterior[0]->borderCount(10), 8);
	EXPECT_EQ(null->holeBorderCount(0, 10), 6);

	EXPECT_EQ(interior[0]->getRootEdge()->loopArea(), 8);
	EXPECT_EQ(exterior[0]->getRootEdge()->loopArea(), 392);
	EXPECT_EQ(null->getHole(0)->loopArea(), -400);
}

TEST(Face_Cuts, Corner_Crossing_Cut) {
	DCEL* space = new DCEL();

	FLL<Pint> boundary_big;
	{
		boundary_big.append(Pint(0, 0));
		boundary_big.append(Pint(0, 20));
		boundary_big.append(Pint(20, 20));
		boundary_big.append(Pint(20, 0));
	}
	FLL<Pint> boundary_small_a;
	{
		boundary_small_a.append(Pint(-4, -4));
		boundary_small_a.append(Pint(-4, 4));
		boundary_small_a.append(Pint(4, 4));
		boundary_small_a.append(Pint(4, -4));
	}

	FLL<DCEL::Face*> interior;
	FLL<DCEL::Face*> exterior;

	DCEL::Face* product = space->createFace(boundary_big);
	DCEL::Face* null = product->getRootEdge()->getInverseEdge()->getFace();
	product->subAllocateFace(boundary_small_a, interior, exterior);

	EXPECT_EQ(exterior.size(), 1);
	EXPECT_EQ(interior.size(), 1);

	EXPECT_EQ(interior[0]->getHoleCount(), 0);
	EXPECT_EQ(exterior[0]->getHoleCount(), 0);
	EXPECT_EQ(null->getHoleCount(), 1);

	EXPECT_EQ(interior[0]->borderCount(10), 4);
	EXPECT_EQ(exterior[0]->borderCount(10), 6);
	EXPECT_EQ(null->holeBorderCount(0, 10), 6);

	EXPECT_EQ(interior[0]->getRootEdge()->loopArea(), 16);
	EXPECT_EQ(exterior[0]->getRootEdge()->loopArea(), 384);
	EXPECT_EQ(null->getHole(0)->loopArea(), -400);
}

TEST(Face_Cuts, Exterior_Cut) {
	DCEL* space = new DCEL();

	FLL<Pint> boundary_big;
	{
		boundary_big.append(Pint(0, 0));
		boundary_big.append(Pint(0, 20));
		boundary_big.append(Pint(20, 20));
		boundary_big.append(Pint(20, 0));
	}
	FLL<Pint> boundary_small_a;
	{
		boundary_small_a.append(Pint(-4, 2));
		boundary_small_a.append(Pint(-4, 4));
		boundary_small_a.append(Pint(-2, 4));
		boundary_small_a.append(Pint(-2, 2));
	}

	FLL<DCEL::Face*> interior;
	FLL<DCEL::Face*> exterior;

	DCEL::Face* product = space->createFace(boundary_big);
	product->subAllocateFace(boundary_small_a, interior, exterior);

	EXPECT_EQ(exterior.size(), 1);
	EXPECT_EQ(interior.size(), 0);

	EXPECT_EQ(exterior[0], product);
}

TEST(Face_Cuts, Encapsulating_Cut) {
	DCEL* space = new DCEL();

	FLL<Pint> boundary_big;
	{
		boundary_big.append(Pint(0, 0));
		boundary_big.append(Pint(0, 20));
		boundary_big.append(Pint(20, 20));
		boundary_big.append(Pint(20, 0));
	}
	FLL<Pint> boundary_small_a;
	{
		boundary_small_a.append(Pint(-2, -2));
		boundary_small_a.append(Pint(-2, 22));
		boundary_small_a.append(Pint(22, 22));
		boundary_small_a.append(Pint(22, -2));
	}

	FLL<DCEL::Face*> interior;
	FLL<DCEL::Face*> exterior;

	DCEL::Face* product = space->createFace(boundary_big);
	DCEL::Face* null = product->getRootEdge()->getInverseEdge()->getFace();
	product->subAllocateFace(boundary_small_a, interior, exterior);

	EXPECT_EQ(exterior.size(), 0);
	EXPECT_EQ(interior.size(), 1);

	EXPECT_EQ(interior[0], product);
	EXPECT_EQ(interior[0]->getRootEdge()->getInverseEdge()->getFace(), null);

}

TEST(Face_Cuts, Seperate_Holes) {
	DCEL* space = new DCEL();

	FLL<Pint> boundary_big;
	{
		boundary_big.append(Pint(0, 0));
		boundary_big.append(Pint(0, 20));
		boundary_big.append(Pint(20, 20));
		boundary_big.append(Pint(20, 0));
	}
	FLL<Pint> boundary_small_a;
	{
		boundary_small_a.append(Pint(2, 2));
		boundary_small_a.append(Pint(2, 4));
		boundary_small_a.append(Pint(4, 4));
		boundary_small_a.append(Pint(4, 2));
	}
	FLL<Pint> boundary_small_b;
	{
		boundary_small_b.append(Pint(2, 16));
		boundary_small_b.append(Pint(2, 18));
		boundary_small_b.append(Pint(4, 18));
		boundary_small_b.append(Pint(4, 16));
	}

	FLL<DCEL::Face*> interiorPintrep;
	FLL<DCEL::Face*> exteriorPintrep;

	FLL<DCEL::Face*> interior;
	FLL<DCEL::Face*> exterior;

	DCEL::Face* product = space->createFace(boundary_big);
	DCEL::Face* null = product->getRootEdge()->getInverseEdge()->getFace();
	product->subAllocateFace(boundary_small_a, interiorPintrep, exteriorPintrep);
	product->subAllocateFace(boundary_small_b, interior, exterior);

	EXPECT_EQ(exteriorPintrep.size(), 1);
	EXPECT_EQ(interiorPintrep.size(), 1);
	EXPECT_EQ(exterior.size(), 1);
	EXPECT_EQ(interior.size(), 1);

	EXPECT_EQ(exterior[0]->getHoleCount(), 2);
	EXPECT_EQ(interiorPintrep[0]->borderCount(10), 4);
	EXPECT_EQ(interior[0]->borderCount(10), 4);
	EXPECT_EQ(exterior[0]->borderCount(10), 4);
	EXPECT_EQ(null->holeBorderCount(0, 10), 4);

	EXPECT_EQ(interiorPintrep[0]->getRootEdge()->loopArea(), 4);
	EXPECT_EQ(interior[0]->getRootEdge()->loopArea(), 4);
	EXPECT_EQ(exterior[0]->getRootEdge()->loopArea(), 400);
	EXPECT_EQ(null->getHole(0)->loopArea(), -400);

	EXPECT_EQ(exterior[0]->getHole(0)->loopArea(), -4);
	EXPECT_EQ(exterior[0]->getHole(1)->loopArea(), -4);


	EXPECT_EQ(exterior[0]->getHole(0)->getInverseEdge()->getFace(), interiorPintrep[0]);
	EXPECT_EQ(exterior[0]->getHole(1)->getInverseEdge()->getFace(), interior[0]);
}

TEST(Face_Cuts, Adjacent_Meeting_Holes) {
	DCEL* space = new DCEL();

	FLL<Pint> boundary_big;
	{
		boundary_big.append(Pint(0, 0));
		boundary_big.append(Pint(0, 20));
		boundary_big.append(Pint(20, 20));
		boundary_big.append(Pint(20, 0));
	}
	FLL<Pint> boundary_small_a;
	{
		boundary_small_a.append(Pint(2, 2));
		boundary_small_a.append(Pint(2, 18));
		boundary_small_a.append(Pint(4, 18));
		boundary_small_a.append(Pint(4, 2));
	}
	FLL<Pint> boundary_small_b;
	{
		boundary_small_b.append(Pint(4, 4));
		boundary_small_b.append(Pint(4, 16));
		boundary_small_b.append(Pint(6, 16));
		boundary_small_b.append(Pint(6, 4));
	}

	FLL<DCEL::Face*> interiorPintrep;
	FLL<DCEL::Face*> exteriorPintrep;

	FLL<DCEL::Face*> interior;
	FLL<DCEL::Face*> exterior;

	DCEL::Face* product = space->createFace(boundary_big);
	product->subAllocateFace(boundary_small_a, interiorPintrep, exteriorPintrep);
	product->subAllocateFace(boundary_small_b, interior, exterior);

	EXPECT_EQ(exteriorPintrep.size(), 1);
	EXPECT_EQ(interiorPintrep.size(), 1);
	EXPECT_EQ(exterior.size(), 1);
	EXPECT_EQ(interior.size(), 1);

	EXPECT_EQ(exterior[0]->getHoleCount(), 1);
	EXPECT_EQ(interiorPintrep[0]->borderCount(10), 6);
	EXPECT_EQ(interior[0]->borderCount(10), 4);
	EXPECT_EQ(exterior[0]->holeBorderCount(0, 10), 8);

	EXPECT_EQ(interiorPintrep[0]->getRootEdge()->loopArea(), 32);
	EXPECT_EQ(interior[0]->getRootEdge()->loopArea(), 24);
	EXPECT_EQ(exterior[0]->getRootEdge()->loopArea(), 400);
	EXPECT_EQ(exterior[0]->getHole(0)->loopArea(), -56);
}

TEST(Face_Cuts, Adjacent_Crossing_Holes) {
	DCEL* space = new DCEL();

	FLL<Pint> boundary_big;
	{
		boundary_big.append(Pint(0, 0));
		boundary_big.append(Pint(0, 20));
		boundary_big.append(Pint(20, 20));
		boundary_big.append(Pint(20, 0));
	}
	FLL<Pint> boundary_small_a;
	{
		boundary_small_a.append(Pint(2, 2));
		boundary_small_a.append(Pint(2, 18));
		boundary_small_a.append(Pint(4, 18));
		boundary_small_a.append(Pint(4, 2));
	}
	FLL<Pint> boundary_small_b;
	{
		boundary_small_b.append(Pint(3, 4));
		boundary_small_b.append(Pint(3, 16));
		boundary_small_b.append(Pint(6, 16));
		boundary_small_b.append(Pint(6, 4));
	}

	FLL<DCEL::Face*> interiorPintrep;
	FLL<DCEL::Face*> exteriorPintrep;

	FLL<DCEL::Face*> interior;
	FLL<DCEL::Face*> exterior;

	DCEL::Face* product = space->createFace(boundary_big);
	product->subAllocateFace(boundary_small_a, interiorPintrep, exteriorPintrep);
	product->subAllocateFace(boundary_small_b, interior, exterior);

	EXPECT_EQ(exteriorPintrep.size(), 1);
	EXPECT_EQ(interiorPintrep.size(), 1);
	EXPECT_EQ(exterior.size(), 1);
	EXPECT_EQ(interior.size(), 1);

	EXPECT_EQ(exterior[0]->getHoleCount(), 1);
	EXPECT_EQ(interiorPintrep[0]->borderCount(10), 6);
	EXPECT_EQ(interior[0]->borderCount(10), 4);
	EXPECT_EQ(exterior[0]->holeBorderCount(0, 10), 8);

	EXPECT_EQ(interiorPintrep[0]->getRootEdge()->loopArea(), 32);
	EXPECT_EQ(interior[0]->getRootEdge()->loopArea(), 24);
	EXPECT_EQ(exterior[0]->getRootEdge()->loopArea(), 400);
	EXPECT_EQ(exterior[0]->getHole(0)->loopArea(), -56);
}

TEST(Face_Cuts, Connecting_Holes) {
	DCEL* space = new DCEL();

	FLL<Pint> boundary_big;
	{
		boundary_big.append(Pint(0, 0));
		boundary_big.append(Pint(0, 20));
		boundary_big.append(Pint(20, 20));
		boundary_big.append(Pint(20, 0));
	}
	FLL<Pint> boundary_small_a;
	{
		boundary_small_a.append(Pint(2, 2));
		boundary_small_a.append(Pint(2, 18));
		boundary_small_a.append(Pint(4, 18));
		boundary_small_a.append(Pint(4, 2));
	}
	FLL<Pint> boundary_small_b;
	{
		boundary_small_b.append(Pint(0, 4));
		boundary_small_b.append(Pint(0, 16));
		boundary_small_b.append(Pint(2, 16));
		boundary_small_b.append(Pint(2, 4));
	}

	FLL<DCEL::Face*> interiorPintrep;
	FLL<DCEL::Face*> exteriorPintrep;

	FLL<DCEL::Face*> interior;
	FLL<DCEL::Face*> exterior;

	DCEL::Face* product = space->createFace(boundary_big);
	DCEL::Face* null = product->getRootEdge()->getInverseEdge()->getFace();
	product->subAllocateFace(boundary_small_a, interiorPintrep, exteriorPintrep);
	product->subAllocateFace(boundary_small_b, interior, exterior);

	//auto test = exterior[0]->getHole(0)->listPoints();

	EXPECT_EQ(exteriorPintrep.size(), 1);
	EXPECT_EQ(interiorPintrep.size(), 1);
	EXPECT_EQ(exterior.size(), 1);
	EXPECT_EQ(interior.size(), 1);

	EXPECT_EQ(interiorPintrep[0]->getHoleCount(), 0);
	EXPECT_EQ(interior[0]->getHoleCount(), 0);
	EXPECT_EQ(exterior[0]->getHoleCount(), 0);
	EXPECT_EQ(null->getHoleCount(), 1);

	EXPECT_EQ(interiorPintrep[0]->borderCount(10), 6);
	EXPECT_EQ(interior[0]->borderCount(10), 4);
	EXPECT_EQ(exterior[0]->borderCount(20), 12);
	EXPECT_EQ(null->holeBorderCount(0, 20), 6);

	EXPECT_EQ(interiorPintrep[0]->getRootEdge()->loopArea(), 32);
	EXPECT_EQ(interior[0]->getRootEdge()->loopArea(), 24);
	EXPECT_EQ(exterior[0]->getRootEdge()->loopArea(), 344);
	EXPECT_EQ(null->getHole(0)->loopArea(), -400);
}

TEST(Face_Cuts, Connecting_Several_Holes) {
	DCEL* space = new DCEL();

	FLL<Pint> boundary_big;
	{
		boundary_big.append(Pint(0, 0));
		boundary_big.append(Pint(0, 20));
		boundary_big.append(Pint(20, 20));
		boundary_big.append(Pint(20, 0));
	}
	FLL<Pint> boundary_small_a;
	{
		boundary_small_a.append(Pint(2, 2));
		boundary_small_a.append(Pint(2, 18));
		boundary_small_a.append(Pint(4, 18));
		boundary_small_a.append(Pint(4, 2));
	}
	FLL<Pint> boundary_small_b;
	{
		boundary_small_b.append(Pint(16, 2));
		boundary_small_b.append(Pint(16, 18));
		boundary_small_b.append(Pint(18, 18));
		boundary_small_b.append(Pint(18, 2));
	}
	FLL<Pint> boundary_small_c;
	{
		boundary_small_c.append(Pint(4, 5));
		boundary_small_c.append(Pint(4, 15));
		boundary_small_c.append(Pint(16, 15));
		boundary_small_c.append(Pint(16, 5));
	}

	FLL<DCEL::Face*> interiorPintrep_a;
	FLL<DCEL::Face*> exteriorPintrep_a;

	FLL<DCEL::Face*> interiorPintrep_b;
	FLL<DCEL::Face*> exteriorPintrep_b;

	FLL<DCEL::Face*> interior;
	FLL<DCEL::Face*> exterior;

	DCEL::Face* product = space->createFace(boundary_big);
	product->subAllocateFace(boundary_small_a, interiorPintrep_a, exteriorPintrep_a);
	product->subAllocateFace(boundary_small_b, interiorPintrep_b, exteriorPintrep_b);
	product->subAllocateFace(boundary_small_c, interior, exterior);


	EXPECT_EQ(exteriorPintrep_a.size(), 1);
	EXPECT_EQ(interiorPintrep_a.size(), 1);
	EXPECT_EQ(exteriorPintrep_b.size(), 1);
	EXPECT_EQ(interiorPintrep_b.size(), 1);
	EXPECT_EQ(exterior.size(), 1);
	EXPECT_EQ(interior.size(), 1);

	EXPECT_EQ(exterior[0]->getHoleCount(), 1);
	EXPECT_EQ(interiorPintrep_a[0]->borderCount(10), 6);
	EXPECT_EQ(interiorPintrep_b[0]->borderCount(10), 6);
	EXPECT_EQ(interior[0]->borderCount(10), 4);
	EXPECT_EQ(exterior[0]->borderCount(20), 4);
	EXPECT_EQ(exterior[0]->holeBorderCount(0, 20), 12);

	EXPECT_EQ(interiorPintrep_a[0]->getRootEdge()->loopArea(), 32);
	EXPECT_EQ(interiorPintrep_b[0]->getRootEdge()->loopArea(), 32);
	EXPECT_EQ(interior[0]->getRootEdge()->loopArea(), 120);
	EXPECT_EQ(exterior[0]->getRootEdge()->loopArea(), 400);
	EXPECT_EQ(exterior[0]->getHole(0)->loopArea(), -184);
}

TEST(Face_Cuts, Splitting_Cut) {
	DCEL* space = new DCEL();

	FLL<Pint> boundary_big;
	{
		boundary_big.append(Pint(0, 0));
		boundary_big.append(Pint(0, 20));
		boundary_big.append(Pint(20, 20));
		boundary_big.append(Pint(20, 0));
	}
	FLL<Pint> boundary_small;
	{
		boundary_small.append(Pint(0, 5));
		boundary_small.append(Pint(0, 15));
		boundary_small.append(Pint(20, 15));
		boundary_small.append(Pint(20, 5));
	}
	FLL<DCEL::Face*> interior;
	FLL<DCEL::Face*> exterior;

	DCEL::Face* product = space->createFace(boundary_big);
	product->subAllocateFace(boundary_small, interior, exterior);

	EXPECT_EQ(exterior.size(), 2);
	EXPECT_EQ(interior.size(), 1);

	EXPECT_EQ(exterior[0]->getHoleCount(), 0);
	EXPECT_EQ(interior[0]->borderCount(10), 4);
	EXPECT_EQ(exterior[0]->borderCount(10), 4);
	EXPECT_EQ(exterior[1]->borderCount(10), 4);

	EXPECT_EQ(interior[0]->getRootEdge()->loopArea(), 200);
	EXPECT_EQ(exterior[0]->getRootEdge()->loopArea(), 100);
	EXPECT_EQ(exterior[1]->getRootEdge()->loopArea(), 100);
}

TEST(Face_Cuts, Divided_Crossing_Holes) {
	DCEL* space = new DCEL();

	FLL<Pint> boundary_big;
	{
		boundary_big.append(Pint(0, 0));
		boundary_big.append(Pint(0, 20));
		boundary_big.append(Pint(20, 20));
		boundary_big.append(Pint(20, 0));
	}
	FLL<Pint> boundary_small_a;
	{
		boundary_small_a.append(Pint(8, 2));
		boundary_small_a.append(Pint(8, 18));
		boundary_small_a.append(Pint(12, 18));
		boundary_small_a.append(Pint(12, 2));
	}
	FLL<Pint> boundary_small_b;
	{
		boundary_small_b.append(Pint(2, 8));
		boundary_small_b.append(Pint(2, 12));
		boundary_small_b.append(Pint(18, 12));
		boundary_small_b.append(Pint(18, 8));
	}

	FLL<DCEL::Face*> interiorPintrep;
	FLL<DCEL::Face*> exteriorPintrep;

	FLL<DCEL::Face*> interior;
	FLL<DCEL::Face*> exterior;

	DCEL::Face* product = space->createFace(boundary_big);
	product->subAllocateFace(boundary_small_a, interiorPintrep, exteriorPintrep);
	product->subAllocateFace(boundary_small_b, interior, exterior);

	EXPECT_EQ(exteriorPintrep.size(), 1);
	EXPECT_EQ(interiorPintrep.size(), 1);
	EXPECT_EQ(exterior.size(), 1);
	EXPECT_EQ(interior.size(), 2);

	EXPECT_EQ(exterior[0]->getHoleCount(), 1);
	EXPECT_EQ(interiorPintrep[0]->borderCount(10), 8);
	EXPECT_EQ(interior[0]->borderCount(10), 4);
	EXPECT_EQ(interior[1]->borderCount(10), 4);
	EXPECT_EQ(exterior[0]->holeBorderCount(0, 20), 12);

	EXPECT_EQ(interiorPintrep[0]->getRootEdge()->loopArea(), 64);
	EXPECT_EQ(interior[0]->getRootEdge()->loopArea(), 24);
	EXPECT_EQ(interior[1]->getRootEdge()->loopArea(), 24);
	EXPECT_EQ(exterior[0]->getRootEdge()->loopArea(), 400);
	EXPECT_EQ(exterior[0]->getHole(0)->loopArea(), -112);
}

TEST(Face_Cuts, Triangles_Crossing_Cut) {
	DCEL* space = new DCEL();

	FLL<Pint> boundary_big;
	{
		boundary_big.append(Pint(0, 0));
		boundary_big.append(Pint(0, 20));
		boundary_big.append(Pint(20, 20));
		boundary_big.append(Pint(20, 0));
	}
	FLL<Pint> boundary_small_a;
	{
		boundary_small_a.append(Pint(2, -1));
		boundary_small_a.append(Pint(2, 2));
		boundary_small_a.append(Pint(4, 0));
		boundary_small_a.append(Pint(6, 2));
		boundary_small_a.append(Pint(6, -1));
	}

	FLL<DCEL::Face*> interior;
	FLL<DCEL::Face*> exterior;

	DCEL::Face* product = space->createFace(boundary_big);
	product->subAllocateFace(boundary_small_a, interior, exterior);

	EXPECT_EQ(interior.size(), 2);
	EXPECT_EQ(exterior.size(), 1);

	EXPECT_EQ(interior[0]->getHoleCount(), 0);
	EXPECT_EQ(interior[1]->getHoleCount(), 0);
	EXPECT_EQ(exterior[0]->getHoleCount(), 0);


	EXPECT_EQ(interior[0]->borderCount(10), 3);
	EXPECT_EQ(interior[1]->borderCount(10), 3);
	EXPECT_EQ(exterior[0]->borderCount(10), 9);


	EXPECT_EQ(interior[0]->getRootEdge()->loopArea(), 2);
	EXPECT_EQ(interior[1]->getRootEdge()->loopArea(), 2);
	EXPECT_EQ(exterior[0]->getRootEdge()->loopArea(), 396);
}

TEST(Face_Cuts, DividedPintoint_Meet_Cut) {
	DCEL* space = new DCEL();

	FLL<Pint> boundary_big;
	{
		boundary_big.append(Pint(2, 0));
		boundary_big.append(Pint(2, 2));
		boundary_big.append(Pint(0, 2));
		boundary_big.append(Pint(0, 20));
		boundary_big.append(Pint(20, 20));
		boundary_big.append(Pint(20, 0));
	}
	FLL<Pint> boundary_small_a;
	{
		boundary_small_a.append(Pint(2, 2));
		boundary_small_a.append(Pint(2, 4));
		boundary_small_a.append(Pint(4, 4));
		boundary_small_a.append(Pint(4, 2));
	}
	FLL<Pint> boundary_small_b;
	{
		boundary_small_b.append(Pint(1, 1));
		boundary_small_b.append(Pint(1, 3));
		boundary_small_b.append(Pint(3, 3));
		boundary_small_b.append(Pint(3, 1));
	}

	FLL<DCEL::Face*> interiorPintrep;
	FLL<DCEL::Face*> exteriorPintrep;

	FLL<DCEL::Face*> interior;
	FLL<DCEL::Face*> exterior;

	DCEL::Face* product = space->createFace(boundary_big);
	product->subAllocateFace(boundary_small_a, interiorPintrep, exteriorPintrep);
	exteriorPintrep[0]->subAllocateFace(boundary_small_b, interior, exterior);

	EXPECT_EQ(interiorPintrep.size(), 1);
	EXPECT_EQ(interior.size(), 2);
	EXPECT_EQ(exterior.size(), 1);

	EXPECT_EQ(interiorPintrep[0]->getHoleCount(), 0);
	EXPECT_EQ(interior[0]->getHoleCount(), 0);
	EXPECT_EQ(interior[1]->getHoleCount(), 0);
	EXPECT_EQ(exterior[0]->getHoleCount(), 0);

	EXPECT_EQ(interiorPintrep[0]->borderCount(10), 6);
	EXPECT_EQ(interior[0]->borderCount(10), 4);
	EXPECT_EQ(interior[1]->borderCount(10), 4);
	EXPECT_EQ(exterior[0]->borderCount(20), 14);

	EXPECT_EQ(interiorPintrep[0]->getRootEdge()->loopArea(), 4);
	EXPECT_EQ(interior[0]->getRootEdge()->loopArea(), 1);
	EXPECT_EQ(interior[1]->getRootEdge()->loopArea(), 1);
	EXPECT_EQ(exterior[0]->getRootEdge()->loopArea(), 390);
}
*/