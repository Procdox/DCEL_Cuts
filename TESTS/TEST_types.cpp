#include "pch.h"
#include "..\DCEL_CUTS\DCEL_types.h"

TEST(DCEL_Basics, add_edge) {
	DCEL space;
	_P A(0, 0);
	_P B(10, 0);

	Edge const * test_object = space.addEdge(A, B);

	Edge const * inverse = test_object->getInv();

	EXPECT_EQ(test_object->getNext(), inverse);
	EXPECT_EQ(test_object->getLast(), inverse);

	EXPECT_EQ(inverse->getNext(), test_object);
	EXPECT_EQ(inverse->getLast(), test_object);

	EXPECT_EQ(inverse->getFace(), test_object->getFace());

	EXPECT_EQ(test_object->getStart()->getPosition(), A);
	EXPECT_EQ(test_object->getEnd()->getPosition(), B);
}

TEST(DCEL_Basics, add_edge_pair) {
	DCEL space;
	_P A(0, 0);
	_P B(10, 0);
	_P C(10, 10);

	Edge * link_A = space.addEdge(A, B);
	Edge const * link_B = space.addEdge(link_A, C);

	Edge const * inv_A = link_A->getInv();
	Edge const * inv_B = link_B->getInv();

	EXPECT_EQ(link_A->getNext(), link_B);
	EXPECT_EQ(link_A->getLast(), inv_A);

	EXPECT_EQ(link_B->getNext(), inv_B);
	EXPECT_EQ(link_B->getLast(), link_A);

	EXPECT_EQ(inv_A->getNext(), link_A);
	EXPECT_EQ(inv_A->getLast(), inv_B);

	EXPECT_EQ(inv_B->getNext(), inv_A);
	EXPECT_EQ(inv_B->getLast(), link_B);

	EXPECT_EQ(link_A->getFace(), link_B->getFace());
	EXPECT_EQ(inv_A->getFace(), inv_B->getFace());
	EXPECT_EQ(link_B->getFace(), inv_B->getFace());

	EXPECT_EQ(link_B->getStart()->getPosition(), B);
	EXPECT_EQ(link_B->getEnd()->getPosition(), C);
}

TEST(DCEL_Basics, add_edge_chain) {
	DCEL space;
	_P A(0, 0);
	_P B(10, 0);
	_P C(10, 10);

	Edge * link_A = space.addEdge(A, B);
	Edge * link_B = space.addEdge(link_A, C);
	Edge * link_C = space.addEdge(link_B, link_A->getInv());


	Edge * inv_A = link_A->getInv();
	Edge * inv_B = link_B->getInv();
	Edge * inv_C = link_C->getInv();

	EXPECT_EQ(link_A->getNext(), link_B);
	EXPECT_EQ(link_A->getLast(), link_C);

	EXPECT_EQ(link_B->getNext(), link_C);
	EXPECT_EQ(link_B->getLast(), link_A);

	EXPECT_EQ(link_C->getNext(), link_A);
	EXPECT_EQ(link_C->getLast(), link_B);

	EXPECT_EQ(inv_A->getNext(), inv_C);
	EXPECT_EQ(inv_A->getLast(), inv_B);

	EXPECT_EQ(inv_B->getNext(), inv_A);
	EXPECT_EQ(inv_B->getLast(), inv_C);

	EXPECT_EQ(inv_C->getNext(), inv_B);
	EXPECT_EQ(inv_C->getLast(), inv_A);

	EXPECT_NE(link_A->getFace(), inv_A->getFace());

	EXPECT_EQ(link_A->getFace(), link_B->getFace());
	EXPECT_EQ(link_A->getFace(), link_C->getFace());
	
	EXPECT_EQ(inv_A->getFace(), inv_B->getFace());
	EXPECT_EQ(inv_A->getFace(), inv_C->getFace());

	EXPECT_EQ(link_C->getStart()->getPosition(), C);
	EXPECT_EQ(link_C->getEnd()->getPosition(), A);
}

TEST(DCEL_Basics, draw_area) {
	DCEL space;
	FLL<_P> boundary;
	Face* test_object;
	Edge* focus;
	
	boundary.append(_P(0, 0));
	boundary.append(_P(0, 20));
	boundary.append(_P(20, 20));
	boundary.append(_P(20, 0));

	test_object = space.draw(boundary);

	focus = test_object->getRoot();

	EXPECT_EQ(focus->getStart()->getPosition(), _P(0, 0));
	EXPECT_EQ(focus->getFace(), test_object);
	EXPECT_NE(focus->getInv()->getFace(), test_object);

	focus = focus->getNext();

	EXPECT_EQ(focus->getStart()->getPosition(), _P(0, 20));
	EXPECT_EQ(focus->getFace(), test_object);
	EXPECT_NE(focus->getInv()->getFace(), test_object);

	focus = focus->getNext();

	EXPECT_EQ(focus->getStart()->getPosition(), _P(20, 20));
	EXPECT_EQ(focus->getFace(), test_object);
	EXPECT_NE(focus->getInv()->getFace(), test_object);

	focus = focus->getNext();

	EXPECT_EQ(focus->getStart()->getPosition(), _P(20, 0));
	EXPECT_EQ(focus->getFace(), test_object);
	EXPECT_NE(focus->getInv()->getFace(), test_object);


	EXPECT_EQ(test_object->loopArea(), 400);
	EXPECT_EQ(test_object->getRoot()->getInv()->getFace()->loopArea(), -400);
}

TEST(DCEL_Basics, append_draw) {
	DCEL space;
	FLL<_P> boundary;
	Face* test_object;
	Edge* focus;

	boundary.append(_P(0, 0));
	boundary.append(_P(0, 20));
	boundary.append(_P(20, 20));
	boundary.append(_P(20, 0));

	test_object = space.draw(boundary);

	focus = space.addEdge(test_object->getRoot()->getInv(), _P(-20, 0));
	focus = space.addEdge(focus, _P(-20, 20));
	focus = space.addEdge(focus, test_object->getRoot()->getInv()->getLast());


	//test_object

	EXPECT_EQ(test_object->loopArea(), 400);
	EXPECT_EQ(focus->getFace()->loopArea(), 400);
	EXPECT_EQ(focus->getInv()->getFace()->loopArea(), -800);
}



TEST(Edge_Tests, subdivide) {
	DCEL space;
	_P A(0, 0);
	_P B(10, 0);
	_P C(10, 10);

	Edge * link_A = space.addEdge(A, C);
	link_A->subdivide(B);

	Edge const * link_B = link_A->getNext();

	Edge const * inv_A = link_A->getInv();
	Edge const * inv_B = link_B->getInv();

	EXPECT_EQ(link_A->getNext(), link_B);
	EXPECT_EQ(link_A->getLast(), inv_A);

	EXPECT_EQ(link_B->getNext(), inv_B);
	EXPECT_EQ(link_B->getLast(), link_A);

	EXPECT_EQ(inv_A->getNext(), link_A);
	EXPECT_EQ(inv_A->getLast(), inv_B);

	EXPECT_EQ(inv_B->getNext(), inv_A);
	EXPECT_EQ(inv_B->getLast(), link_B);

	EXPECT_EQ(link_B->getStart()->getPosition(), B);
	EXPECT_EQ(link_B->getEnd()->getPosition(), C);
}

TEST(DCEL_Basics, Sidedness) {
	DCEL space;

	for (int target_size = 4; target_size < 10; target_size++)
	{
		FLL<_P> boundary;
		for (int ii = 0; ii < target_size; ii++) {
			boundary.push(_P(ii,ii));
		}

		Face const * product = space.draw(boundary);
		Face const * null = product->getRoot()->getInv()->getFace();

		EXPECT_EQ(product->getLoopSize(), target_size);
		EXPECT_EQ(null->getLoopSize(), target_size);
	}
}

// FACE CUTS
/*
TEST(Face_Cuts, Hole) {
	DCEL* space = new DCEL();

	std::vector<_P> boundary_big;
	{
		boundary_big.push_back(_P(0, 0));
		boundary_big.push_back(_P(0, 20));
		boundary_big.push_back(_P(20, 20));
		boundary_big.push_back(_P(20, 0));
	}
	std::vector<_P> boundary_small_a;
	{
		boundary_small_a.push_back(_P(2, 2));
		boundary_small_a.push_back(_P(2, 4));
		boundary_small_a.push_back(_P(4, 4));
		boundary_small_a.push_back(_P(4, 2));
	}

	std::vector<DCEL::Face*> interior;
	std::vector<DCEL::Face*> exterior;

	DCEL::Face* product = space->createFace(boundary_big);
	DCEL::Face* null = product->getRootEdge()->getInverseEdge()->getFace();
	product->subAllocateFace(boundary_small_a, interior, exterior);

	EXPECT_EQ(exterior.size(), 1);
	EXPECT_EQ(interior.size(), 1);

	EXPECT_EQ(exterior[0]->getHoleCount(), 1);
	EXPECT_EQ(interior[0]->getHoleCount(), 0);
	EXPECT_EQ(null->getHoleCount(), 1);

	EXPECT_EQ(exterior[0]->getRootEdge()->loopArea(), 400);
	EXPECT_EQ(exterior[0]->getHole(0)->loopArea(), -4);
	EXPECT_EQ(interior[0]->getRootEdge()->loopArea(), 4);
	EXPECT_EQ(null->getHole(0)->loopArea(), -400);

	EXPECT_EQ(interior[0], product->getHole(0)->getInverseEdge()->getFace());
}

TEST(Face_Cuts, Edge_Meeting_Cut) {
	DCEL* space = new DCEL();

	std::vector<_P> boundary_big;
	{
		boundary_big.push_back(_P(0, 0));
		boundary_big.push_back(_P(0, 20));
		boundary_big.push_back(_P(20, 20));
		boundary_big.push_back(_P(20, 0));
	}
	std::vector<_P> boundary_small_a;
	{
		boundary_small_a.push_back(_P(0, 2));
		boundary_small_a.push_back(_P(0, 4));
		boundary_small_a.push_back(_P(4, 4));
		boundary_small_a.push_back(_P(4, 2));
	}

	std::vector<DCEL::Face*> interior;
	std::vector<DCEL::Face*> exterior;

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

	std::vector<_P> boundary_big;
	{
		boundary_big.push_back(_P(2, 0));
		boundary_big.push_back(_P(2, 2));
		boundary_big.push_back(_P(0, 2));
		boundary_big.push_back(_P(0, 20));
		boundary_big.push_back(_P(20, 20));
		boundary_big.push_back(_P(20, 0));
	}
	std::vector<_P> boundary_small_a;
	{
		boundary_small_a.push_back(_P(2, 2));
		boundary_small_a.push_back(_P(2, 4));
		boundary_small_a.push_back(_P(4, 4));
		boundary_small_a.push_back(_P(4, 2));
	}

	std::vector<DCEL::Face*> interior;
	std::vector<DCEL::Face*> exterior;

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

	std::vector<_P> boundary_big;
	{
		boundary_big.push_back(_P(0, 0));
		boundary_big.push_back(_P(0, 2));
		boundary_big.push_back(_P(2, 2));
		boundary_big.push_back(_P(2, 4));
		boundary_big.push_back(_P(0, 4));
		boundary_big.push_back(_P(0, 20));
		boundary_big.push_back(_P(20, 20));
		boundary_big.push_back(_P(20, 0));
	}
	std::vector<_P> boundary_small_a;
	{
		boundary_small_a.push_back(_P(2, 2));
		boundary_small_a.push_back(_P(2, 4));
		boundary_small_a.push_back(_P(4, 4));
		boundary_small_a.push_back(_P(4, 2));
	}

	std::vector<DCEL::Face*> interior;
	std::vector<DCEL::Face*> exterior;

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

	std::vector<_P> boundary_big;
	{
		boundary_big.push_back(_P(0, 0));
		boundary_big.push_back(_P(0, 2));
		boundary_big.push_back(_P(2, 2));
		boundary_big.push_back(_P(2, 4));
		boundary_big.push_back(_P(0, 4));
		boundary_big.push_back(_P(0, 20));
		boundary_big.push_back(_P(20, 20));
		boundary_big.push_back(_P(20, 0));
	}
	std::vector<_P> boundary_small_a;
	{
		boundary_small_a.push_back(_P(2, 1));
		boundary_small_a.push_back(_P(2, 3));
		boundary_small_a.push_back(_P(4, 3));
		boundary_small_a.push_back(_P(4, 1));
	}

	std::vector<DCEL::Face*> interior;
	std::vector<DCEL::Face*> exterior;

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

	std::vector<_P> boundary_big;
	{
		boundary_big.push_back(_P(0, 0));
		boundary_big.push_back(_P(0, 2));
		boundary_big.push_back(_P(2, 2));
		boundary_big.push_back(_P(2, 4));
		boundary_big.push_back(_P(0, 4));
		boundary_big.push_back(_P(0, 20));
		boundary_big.push_back(_P(20, 20));
		boundary_big.push_back(_P(20, 0));
	}
	std::vector<_P> boundary_small_a;
	{
		boundary_small_a.push_back(_P(1, 4));
		boundary_small_a.push_back(_P(1, 6));
		boundary_small_a.push_back(_P(3, 6));
		boundary_small_a.push_back(_P(3, 4));
	}

	std::vector<DCEL::Face*> interior;
	std::vector<DCEL::Face*> exterior;

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

	std::vector<_P> boundary_big;
	{
		boundary_big.push_back(_P(0, 0));
		boundary_big.push_back(_P(0, 2));
		boundary_big.push_back(_P(2, 2));
		boundary_big.push_back(_P(2, 4));
		boundary_big.push_back(_P(0, 4));
		boundary_big.push_back(_P(0, 20));
		boundary_big.push_back(_P(20, 20));
		boundary_big.push_back(_P(20, 0));
	}
	std::vector<_P> boundary_small_a;
	{
		boundary_small_a.push_back(_P(2, 1));
		boundary_small_a.push_back(_P(2, 5));
		boundary_small_a.push_back(_P(4, 5));
		boundary_small_a.push_back(_P(4, 1));
	}

	std::vector<DCEL::Face*> interior;
	std::vector<DCEL::Face*> exterior;

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

	std::vector<_P> boundary_big;
	{
		boundary_big.push_back(_P(0, 0));
		boundary_big.push_back(_P(0, 20));
		boundary_big.push_back(_P(20, 20));
		boundary_big.push_back(_P(20, 0));
	}
	std::vector<_P> boundary_small_a;
	{
		boundary_small_a.push_back(_P(2, 2));
		boundary_small_a.push_back(_P(2, 18));
		boundary_small_a.push_back(_P(4, 18));
		boundary_small_a.push_back(_P(4, 2));
	}
	std::vector<_P> boundary_small_b;
	{
		boundary_small_b.push_back(_P(0, 4));
		boundary_small_b.push_back(_P(0, 16));
		boundary_small_b.push_back(_P(2, 16));
		boundary_small_b.push_back(_P(2, 4));
	}

	std::vector<DCEL::Face*> interior_prep;
	std::vector<DCEL::Face*> exterior_prep;

	std::vector<DCEL::Face*> interior;
	std::vector<DCEL::Face*> exterior;

	DCEL::Face* product = space->createFace(boundary_big);
	DCEL::Face* null = product->getRootEdge()->getInverseEdge()->getFace();
	product->subAllocateFace(boundary_small_b, interior_prep, exterior_prep);
	exterior_prep[0]->subAllocateFace(boundary_small_a, interior, exterior);

	//auto test = exterior[0]->getHole(0)->listPoints();

	EXPECT_EQ(exterior_prep.size(), 1);
	EXPECT_EQ(interior_prep.size(), 1);
	EXPECT_EQ(exterior.size(), 1);
	EXPECT_EQ(interior.size(), 1);

	EXPECT_EQ(interior_prep[0]->getHoleCount(), 0);
	EXPECT_EQ(interior[0]->getHoleCount(), 0);
	EXPECT_EQ(exterior[0]->getHoleCount(), 0);
	EXPECT_EQ(null->getHoleCount(), 1);

	EXPECT_EQ(interior_prep[0]->borderCount(10), 4);
	EXPECT_EQ(interior[0]->borderCount(10), 6);
	EXPECT_EQ(exterior[0]->borderCount(20), 12);
	EXPECT_EQ(null->holeBorderCount(0, 20), 6);

	EXPECT_EQ(interior_prep[0]->getRootEdge()->loopArea(), 24);
	EXPECT_EQ(interior[0]->getRootEdge()->loopArea(), 32);
	EXPECT_EQ(exterior[0]->getRootEdge()->loopArea(), 344);
	EXPECT_EQ(null->getHole(0)->loopArea(), -400);
}

TEST(Face_Cuts, Horshoe_Cut) {
	DCEL* space = new DCEL();

	std::vector<_P> boundary_big;
	{
		boundary_big.push_back(_P(0, 0));
		boundary_big.push_back(_P(0, 20));
		boundary_big.push_back(_P(20, 20));
		boundary_big.push_back(_P(20, 0));
	}
	std::vector<_P> boundary_small_a;
	{
		boundary_small_a.push_back(_P(4, -8));
		boundary_small_a.push_back(_P(4, 4));
		boundary_small_a.push_back(_P(8, 4));
		boundary_small_a.push_back(_P(8, -4));
		boundary_small_a.push_back(_P(12, -4));
		boundary_small_a.push_back(_P(12, 4));
		boundary_small_a.push_back(_P(16, 4));
		boundary_small_a.push_back(_P(16, -8));
	}

	std::vector<DCEL::Face*> interior;
	std::vector<DCEL::Face*> exterior;

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

	std::vector<_P> boundary_big;
	{
		boundary_big.push_back(_P(0, 0));
		boundary_big.push_back(_P(0, 20));
		boundary_big.push_back(_P(20, 20));
		boundary_big.push_back(_P(20, 0));
	}
	std::vector<_P> boundary_small_a;
	{
		boundary_small_a.push_back(_P(0, 0));
		boundary_small_a.push_back(_P(0, 4));
		boundary_small_a.push_back(_P(4, 4));
		boundary_small_a.push_back(_P(4, 0));
	}

	std::vector<DCEL::Face*> interior;
	std::vector<DCEL::Face*> exterior;

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

	std::vector<_P> boundary_big;
	{
		boundary_big.push_back(_P(0, 0));
		boundary_big.push_back(_P(0, 20));
		boundary_big.push_back(_P(20, 20));
		boundary_big.push_back(_P(20, 0));
	}
	std::vector<_P> boundary_small_a;
	{
		boundary_small_a.push_back(_P(-4, 2));
		boundary_small_a.push_back(_P(-4, 4));
		boundary_small_a.push_back(_P(4, 4));
		boundary_small_a.push_back(_P(4, 2));
	}

	std::vector<DCEL::Face*> interior;
	std::vector<DCEL::Face*> exterior;

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

	std::vector<_P> boundary_big;
	{
		boundary_big.push_back(_P(0, 0));
		boundary_big.push_back(_P(0, 20));
		boundary_big.push_back(_P(20, 20));
		boundary_big.push_back(_P(20, 0));
	}
	std::vector<_P> boundary_small_a;
	{
		boundary_small_a.push_back(_P(-4, -4));
		boundary_small_a.push_back(_P(-4, 4));
		boundary_small_a.push_back(_P(4, 4));
		boundary_small_a.push_back(_P(4, -4));
	}

	std::vector<DCEL::Face*> interior;
	std::vector<DCEL::Face*> exterior;

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

	std::vector<_P> boundary_big;
	{
		boundary_big.push_back(_P(0, 0));
		boundary_big.push_back(_P(0, 20));
		boundary_big.push_back(_P(20, 20));
		boundary_big.push_back(_P(20, 0));
	}
	std::vector<_P> boundary_small_a;
	{
		boundary_small_a.push_back(_P(-4, 2));
		boundary_small_a.push_back(_P(-4, 4));
		boundary_small_a.push_back(_P(-2, 4));
		boundary_small_a.push_back(_P(-2, 2));
	}

	std::vector<DCEL::Face*> interior;
	std::vector<DCEL::Face*> exterior;

	DCEL::Face* product = space->createFace(boundary_big);
	product->subAllocateFace(boundary_small_a, interior, exterior);

	EXPECT_EQ(exterior.size(), 1);
	EXPECT_EQ(interior.size(), 0);

	EXPECT_EQ(exterior[0], product);
}

TEST(Face_Cuts, Encapsulating_Cut) {
	DCEL* space = new DCEL();

	std::vector<_P> boundary_big;
	{
		boundary_big.push_back(_P(0, 0));
		boundary_big.push_back(_P(0, 20));
		boundary_big.push_back(_P(20, 20));
		boundary_big.push_back(_P(20, 0));
	}
	std::vector<_P> boundary_small_a;
	{
		boundary_small_a.push_back(_P(-2, -2));
		boundary_small_a.push_back(_P(-2, 22));
		boundary_small_a.push_back(_P(22, 22));
		boundary_small_a.push_back(_P(22, -2));
	}

	std::vector<DCEL::Face*> interior;
	std::vector<DCEL::Face*> exterior;

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

	std::vector<_P> boundary_big;
	{
		boundary_big.push_back(_P(0, 0));
		boundary_big.push_back(_P(0, 20));
		boundary_big.push_back(_P(20, 20));
		boundary_big.push_back(_P(20, 0));
	}
	std::vector<_P> boundary_small_a;
	{
		boundary_small_a.push_back(_P(2, 2));
		boundary_small_a.push_back(_P(2, 4));
		boundary_small_a.push_back(_P(4, 4));
		boundary_small_a.push_back(_P(4, 2));
	}
	std::vector<_P> boundary_small_b;
	{
		boundary_small_b.push_back(_P(2, 16));
		boundary_small_b.push_back(_P(2, 18));
		boundary_small_b.push_back(_P(4, 18));
		boundary_small_b.push_back(_P(4, 16));
	}

	std::vector<DCEL::Face*> interior_prep;
	std::vector<DCEL::Face*> exterior_prep;

	std::vector<DCEL::Face*> interior;
	std::vector<DCEL::Face*> exterior;

	DCEL::Face* product = space->createFace(boundary_big);
	DCEL::Face* null = product->getRootEdge()->getInverseEdge()->getFace();
	product->subAllocateFace(boundary_small_a, interior_prep, exterior_prep);
	product->subAllocateFace(boundary_small_b, interior, exterior);

	EXPECT_EQ(exterior_prep.size(), 1);
	EXPECT_EQ(interior_prep.size(), 1);
	EXPECT_EQ(exterior.size(), 1);
	EXPECT_EQ(interior.size(), 1);

	EXPECT_EQ(exterior[0]->getHoleCount(), 2);
	EXPECT_EQ(interior_prep[0]->borderCount(10), 4);
	EXPECT_EQ(interior[0]->borderCount(10), 4);
	EXPECT_EQ(exterior[0]->borderCount(10), 4);
	EXPECT_EQ(null->holeBorderCount(0, 10), 4);

	EXPECT_EQ(interior_prep[0]->getRootEdge()->loopArea(), 4);
	EXPECT_EQ(interior[0]->getRootEdge()->loopArea(), 4);
	EXPECT_EQ(exterior[0]->getRootEdge()->loopArea(), 400);
	EXPECT_EQ(null->getHole(0)->loopArea(), -400);

	EXPECT_EQ(exterior[0]->getHole(0)->loopArea(), -4);
	EXPECT_EQ(exterior[0]->getHole(1)->loopArea(), -4);


	EXPECT_EQ(exterior[0]->getHole(0)->getInverseEdge()->getFace(), interior_prep[0]);
	EXPECT_EQ(exterior[0]->getHole(1)->getInverseEdge()->getFace(), interior[0]);
}

TEST(Face_Cuts, Adjacent_Meeting_Holes) {
	DCEL* space = new DCEL();

	std::vector<_P> boundary_big;
	{
		boundary_big.push_back(_P(0, 0));
		boundary_big.push_back(_P(0, 20));
		boundary_big.push_back(_P(20, 20));
		boundary_big.push_back(_P(20, 0));
	}
	std::vector<_P> boundary_small_a;
	{
		boundary_small_a.push_back(_P(2, 2));
		boundary_small_a.push_back(_P(2, 18));
		boundary_small_a.push_back(_P(4, 18));
		boundary_small_a.push_back(_P(4, 2));
	}
	std::vector<_P> boundary_small_b;
	{
		boundary_small_b.push_back(_P(4, 4));
		boundary_small_b.push_back(_P(4, 16));
		boundary_small_b.push_back(_P(6, 16));
		boundary_small_b.push_back(_P(6, 4));
	}

	std::vector<DCEL::Face*> interior_prep;
	std::vector<DCEL::Face*> exterior_prep;

	std::vector<DCEL::Face*> interior;
	std::vector<DCEL::Face*> exterior;

	DCEL::Face* product = space->createFace(boundary_big);
	product->subAllocateFace(boundary_small_a, interior_prep, exterior_prep);
	product->subAllocateFace(boundary_small_b, interior, exterior);

	EXPECT_EQ(exterior_prep.size(), 1);
	EXPECT_EQ(interior_prep.size(), 1);
	EXPECT_EQ(exterior.size(), 1);
	EXPECT_EQ(interior.size(), 1);

	EXPECT_EQ(exterior[0]->getHoleCount(), 1);
	EXPECT_EQ(interior_prep[0]->borderCount(10), 6);
	EXPECT_EQ(interior[0]->borderCount(10), 4);
	EXPECT_EQ(exterior[0]->holeBorderCount(0, 10), 8);

	EXPECT_EQ(interior_prep[0]->getRootEdge()->loopArea(), 32);
	EXPECT_EQ(interior[0]->getRootEdge()->loopArea(), 24);
	EXPECT_EQ(exterior[0]->getRootEdge()->loopArea(), 400);
	EXPECT_EQ(exterior[0]->getHole(0)->loopArea(), -56);
}

TEST(Face_Cuts, Adjacent_Crossing_Holes) {
	DCEL* space = new DCEL();

	std::vector<_P> boundary_big;
	{
		boundary_big.push_back(_P(0, 0));
		boundary_big.push_back(_P(0, 20));
		boundary_big.push_back(_P(20, 20));
		boundary_big.push_back(_P(20, 0));
	}
	std::vector<_P> boundary_small_a;
	{
		boundary_small_a.push_back(_P(2, 2));
		boundary_small_a.push_back(_P(2, 18));
		boundary_small_a.push_back(_P(4, 18));
		boundary_small_a.push_back(_P(4, 2));
	}
	std::vector<_P> boundary_small_b;
	{
		boundary_small_b.push_back(_P(3, 4));
		boundary_small_b.push_back(_P(3, 16));
		boundary_small_b.push_back(_P(6, 16));
		boundary_small_b.push_back(_P(6, 4));
	}

	std::vector<DCEL::Face*> interior_prep;
	std::vector<DCEL::Face*> exterior_prep;

	std::vector<DCEL::Face*> interior;
	std::vector<DCEL::Face*> exterior;

	DCEL::Face* product = space->createFace(boundary_big);
	product->subAllocateFace(boundary_small_a, interior_prep, exterior_prep);
	product->subAllocateFace(boundary_small_b, interior, exterior);

	EXPECT_EQ(exterior_prep.size(), 1);
	EXPECT_EQ(interior_prep.size(), 1);
	EXPECT_EQ(exterior.size(), 1);
	EXPECT_EQ(interior.size(), 1);

	EXPECT_EQ(exterior[0]->getHoleCount(), 1);
	EXPECT_EQ(interior_prep[0]->borderCount(10), 6);
	EXPECT_EQ(interior[0]->borderCount(10), 4);
	EXPECT_EQ(exterior[0]->holeBorderCount(0, 10), 8);

	EXPECT_EQ(interior_prep[0]->getRootEdge()->loopArea(), 32);
	EXPECT_EQ(interior[0]->getRootEdge()->loopArea(), 24);
	EXPECT_EQ(exterior[0]->getRootEdge()->loopArea(), 400);
	EXPECT_EQ(exterior[0]->getHole(0)->loopArea(), -56);
}

TEST(Face_Cuts, Connecting_Holes) {
	DCEL* space = new DCEL();

	std::vector<_P> boundary_big;
	{
		boundary_big.push_back(_P(0, 0));
		boundary_big.push_back(_P(0, 20));
		boundary_big.push_back(_P(20, 20));
		boundary_big.push_back(_P(20, 0));
	}
	std::vector<_P> boundary_small_a;
	{
		boundary_small_a.push_back(_P(2, 2));
		boundary_small_a.push_back(_P(2, 18));
		boundary_small_a.push_back(_P(4, 18));
		boundary_small_a.push_back(_P(4, 2));
	}
	std::vector<_P> boundary_small_b;
	{
		boundary_small_b.push_back(_P(0, 4));
		boundary_small_b.push_back(_P(0, 16));
		boundary_small_b.push_back(_P(2, 16));
		boundary_small_b.push_back(_P(2, 4));
	}

	std::vector<DCEL::Face*> interior_prep;
	std::vector<DCEL::Face*> exterior_prep;

	std::vector<DCEL::Face*> interior;
	std::vector<DCEL::Face*> exterior;

	DCEL::Face* product = space->createFace(boundary_big);
	DCEL::Face* null = product->getRootEdge()->getInverseEdge()->getFace();
	product->subAllocateFace(boundary_small_a, interior_prep, exterior_prep);
	product->subAllocateFace(boundary_small_b, interior, exterior);

	//auto test = exterior[0]->getHole(0)->listPoints();

	EXPECT_EQ(exterior_prep.size(), 1);
	EXPECT_EQ(interior_prep.size(), 1);
	EXPECT_EQ(exterior.size(), 1);
	EXPECT_EQ(interior.size(), 1);

	EXPECT_EQ(interior_prep[0]->getHoleCount(), 0);
	EXPECT_EQ(interior[0]->getHoleCount(), 0);
	EXPECT_EQ(exterior[0]->getHoleCount(), 0);
	EXPECT_EQ(null->getHoleCount(), 1);

	EXPECT_EQ(interior_prep[0]->borderCount(10), 6);
	EXPECT_EQ(interior[0]->borderCount(10), 4);
	EXPECT_EQ(exterior[0]->borderCount(20), 12);
	EXPECT_EQ(null->holeBorderCount(0, 20), 6);

	EXPECT_EQ(interior_prep[0]->getRootEdge()->loopArea(), 32);
	EXPECT_EQ(interior[0]->getRootEdge()->loopArea(), 24);
	EXPECT_EQ(exterior[0]->getRootEdge()->loopArea(), 344);
	EXPECT_EQ(null->getHole(0)->loopArea(), -400);
}

TEST(Face_Cuts, Connecting_Several_Holes) {
	DCEL* space = new DCEL();

	std::vector<_P> boundary_big;
	{
		boundary_big.push_back(_P(0, 0));
		boundary_big.push_back(_P(0, 20));
		boundary_big.push_back(_P(20, 20));
		boundary_big.push_back(_P(20, 0));
	}
	std::vector<_P> boundary_small_a;
	{
		boundary_small_a.push_back(_P(2, 2));
		boundary_small_a.push_back(_P(2, 18));
		boundary_small_a.push_back(_P(4, 18));
		boundary_small_a.push_back(_P(4, 2));
	}
	std::vector<_P> boundary_small_b;
	{
		boundary_small_b.push_back(_P(16, 2));
		boundary_small_b.push_back(_P(16, 18));
		boundary_small_b.push_back(_P(18, 18));
		boundary_small_b.push_back(_P(18, 2));
	}
	std::vector<_P> boundary_small_c;
	{
		boundary_small_c.push_back(_P(4, 5));
		boundary_small_c.push_back(_P(4, 15));
		boundary_small_c.push_back(_P(16, 15));
		boundary_small_c.push_back(_P(16, 5));
	}

	std::vector<DCEL::Face*> interior_prep_a;
	std::vector<DCEL::Face*> exterior_prep_a;

	std::vector<DCEL::Face*> interior_prep_b;
	std::vector<DCEL::Face*> exterior_prep_b;

	std::vector<DCEL::Face*> interior;
	std::vector<DCEL::Face*> exterior;

	DCEL::Face* product = space->createFace(boundary_big);
	product->subAllocateFace(boundary_small_a, interior_prep_a, exterior_prep_a);
	product->subAllocateFace(boundary_small_b, interior_prep_b, exterior_prep_b);
	product->subAllocateFace(boundary_small_c, interior, exterior);


	EXPECT_EQ(exterior_prep_a.size(), 1);
	EXPECT_EQ(interior_prep_a.size(), 1);
	EXPECT_EQ(exterior_prep_b.size(), 1);
	EXPECT_EQ(interior_prep_b.size(), 1);
	EXPECT_EQ(exterior.size(), 1);
	EXPECT_EQ(interior.size(), 1);

	EXPECT_EQ(exterior[0]->getHoleCount(), 1);
	EXPECT_EQ(interior_prep_a[0]->borderCount(10), 6);
	EXPECT_EQ(interior_prep_b[0]->borderCount(10), 6);
	EXPECT_EQ(interior[0]->borderCount(10), 4);
	EXPECT_EQ(exterior[0]->borderCount(20), 4);
	EXPECT_EQ(exterior[0]->holeBorderCount(0, 20), 12);

	EXPECT_EQ(interior_prep_a[0]->getRootEdge()->loopArea(), 32);
	EXPECT_EQ(interior_prep_b[0]->getRootEdge()->loopArea(), 32);
	EXPECT_EQ(interior[0]->getRootEdge()->loopArea(), 120);
	EXPECT_EQ(exterior[0]->getRootEdge()->loopArea(), 400);
	EXPECT_EQ(exterior[0]->getHole(0)->loopArea(), -184);
}

TEST(Face_Cuts, Splitting_Cut) {
	DCEL* space = new DCEL();

	std::vector<_P> boundary_big;
	{
		boundary_big.push_back(_P(0, 0));
		boundary_big.push_back(_P(0, 20));
		boundary_big.push_back(_P(20, 20));
		boundary_big.push_back(_P(20, 0));
	}
	std::vector<_P> boundary_small;
	{
		boundary_small.push_back(_P(0, 5));
		boundary_small.push_back(_P(0, 15));
		boundary_small.push_back(_P(20, 15));
		boundary_small.push_back(_P(20, 5));
	}
	std::vector<DCEL::Face*> interior;
	std::vector<DCEL::Face*> exterior;

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

	std::vector<_P> boundary_big;
	{
		boundary_big.push_back(_P(0, 0));
		boundary_big.push_back(_P(0, 20));
		boundary_big.push_back(_P(20, 20));
		boundary_big.push_back(_P(20, 0));
	}
	std::vector<_P> boundary_small_a;
	{
		boundary_small_a.push_back(_P(8, 2));
		boundary_small_a.push_back(_P(8, 18));
		boundary_small_a.push_back(_P(12, 18));
		boundary_small_a.push_back(_P(12, 2));
	}
	std::vector<_P> boundary_small_b;
	{
		boundary_small_b.push_back(_P(2, 8));
		boundary_small_b.push_back(_P(2, 12));
		boundary_small_b.push_back(_P(18, 12));
		boundary_small_b.push_back(_P(18, 8));
	}

	std::vector<DCEL::Face*> interior_prep;
	std::vector<DCEL::Face*> exterior_prep;

	std::vector<DCEL::Face*> interior;
	std::vector<DCEL::Face*> exterior;

	DCEL::Face* product = space->createFace(boundary_big);
	product->subAllocateFace(boundary_small_a, interior_prep, exterior_prep);
	product->subAllocateFace(boundary_small_b, interior, exterior);

	EXPECT_EQ(exterior_prep.size(), 1);
	EXPECT_EQ(interior_prep.size(), 1);
	EXPECT_EQ(exterior.size(), 1);
	EXPECT_EQ(interior.size(), 2);

	EXPECT_EQ(exterior[0]->getHoleCount(), 1);
	EXPECT_EQ(interior_prep[0]->borderCount(10), 8);
	EXPECT_EQ(interior[0]->borderCount(10), 4);
	EXPECT_EQ(interior[1]->borderCount(10), 4);
	EXPECT_EQ(exterior[0]->holeBorderCount(0, 20), 12);

	EXPECT_EQ(interior_prep[0]->getRootEdge()->loopArea(), 64);
	EXPECT_EQ(interior[0]->getRootEdge()->loopArea(), 24);
	EXPECT_EQ(interior[1]->getRootEdge()->loopArea(), 24);
	EXPECT_EQ(exterior[0]->getRootEdge()->loopArea(), 400);
	EXPECT_EQ(exterior[0]->getHole(0)->loopArea(), -112);
}

TEST(Face_Cuts, Triangles_Crossing_Cut) {
	DCEL* space = new DCEL();

	std::vector<_P> boundary_big;
	{
		boundary_big.push_back(_P(0, 0));
		boundary_big.push_back(_P(0, 20));
		boundary_big.push_back(_P(20, 20));
		boundary_big.push_back(_P(20, 0));
	}
	std::vector<_P> boundary_small_a;
	{
		boundary_small_a.push_back(_P(2, -1));
		boundary_small_a.push_back(_P(2, 2));
		boundary_small_a.push_back(_P(4, 0));
		boundary_small_a.push_back(_P(6, 2));
		boundary_small_a.push_back(_P(6, -1));
	}

	std::vector<DCEL::Face*> interior;
	std::vector<DCEL::Face*> exterior;

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

TEST(Face_Cuts, Divided_Point_Meet_Cut) {
	DCEL* space = new DCEL();

	std::vector<_P> boundary_big;
	{
		boundary_big.push_back(_P(2, 0));
		boundary_big.push_back(_P(2, 2));
		boundary_big.push_back(_P(0, 2));
		boundary_big.push_back(_P(0, 20));
		boundary_big.push_back(_P(20, 20));
		boundary_big.push_back(_P(20, 0));
	}
	std::vector<_P> boundary_small_a;
	{
		boundary_small_a.push_back(_P(2, 2));
		boundary_small_a.push_back(_P(2, 4));
		boundary_small_a.push_back(_P(4, 4));
		boundary_small_a.push_back(_P(4, 2));
	}
	std::vector<_P> boundary_small_b;
	{
		boundary_small_b.push_back(_P(1, 1));
		boundary_small_b.push_back(_P(1, 3));
		boundary_small_b.push_back(_P(3, 3));
		boundary_small_b.push_back(_P(3, 1));
	}

	std::vector<DCEL::Face*> interior_prep;
	std::vector<DCEL::Face*> exterior_prep;

	std::vector<DCEL::Face*> interior;
	std::vector<DCEL::Face*> exterior;

	DCEL::Face* product = space->createFace(boundary_big);
	product->subAllocateFace(boundary_small_a, interior_prep, exterior_prep);
	exterior_prep[0]->subAllocateFace(boundary_small_b, interior, exterior);

	EXPECT_EQ(interior_prep.size(), 1);
	EXPECT_EQ(interior.size(), 2);
	EXPECT_EQ(exterior.size(), 1);

	EXPECT_EQ(interior_prep[0]->getHoleCount(), 0);
	EXPECT_EQ(interior[0]->getHoleCount(), 0);
	EXPECT_EQ(interior[1]->getHoleCount(), 0);
	EXPECT_EQ(exterior[0]->getHoleCount(), 0);

	EXPECT_EQ(interior_prep[0]->borderCount(10), 6);
	EXPECT_EQ(interior[0]->borderCount(10), 4);
	EXPECT_EQ(interior[1]->borderCount(10), 4);
	EXPECT_EQ(exterior[0]->borderCount(20), 14);

	EXPECT_EQ(interior_prep[0]->getRootEdge()->loopArea(), 4);
	EXPECT_EQ(interior[0]->getRootEdge()->loopArea(), 1);
	EXPECT_EQ(interior[1]->getRootEdge()->loopArea(), 1);
	EXPECT_EQ(exterior[0]->getRootEdge()->loopArea(), 390);
}
*/

// FACE MERGE

TEST(FACE_Merge, append_merge) {
	DCEL space;
	FLL<_P> boundary;
	FLL<Face*> results;
	Face* test_object;
	Face* partner;
	Edge* focus;

	boundary.append(_P(0, 0));
	boundary.append(_P(0, 20));
	boundary.append(_P(20, 20));
	boundary.append(_P(20, 0));

	test_object = space.draw(boundary);

	focus = space.addEdge(test_object->getRoot()->getInv(), _P(-20, 0));
	focus = space.addEdge(focus, _P(-20, 20));
	focus = space.addEdge(focus, test_object->getRoot()->getInv()->getLast());

	partner = focus->getFace();

	test_object->mergeWithFace(partner, results);

	EXPECT_EQ(test_object->loopArea(), 800);
	EXPECT_EQ(focus->getInv()->getFace()->loopArea(), -800);

	EXPECT_FALSE(results.empty());
}
TEST(FACE_Merge, entire_merge) {
	DCEL space;
	FLL<_P> boundary;
	FLL<Face*> results;
	Face* test_object;
	Face* outside;

	boundary.append(_P(0, 0));
	boundary.append(_P(0, 20));
	boundary.append(_P(20, 20));
	boundary.append(_P(20, 0));

	test_object = space.draw(boundary);

	outside = test_object->getRoot()->getInv()->getFace();

	test_object->mergeWithFace(outside, results);

	EXPECT_TRUE(results.empty());
}

TEST(FACE_Merge, merge_gap) {
	DCEL space;
	FLL<_P> boundary;
	FLL<Face*> results;
	Face* test_object;
	Face* outside;
	Edge* focus;

	boundary.append(_P(0, 0));
	boundary.append(_P(0, 5));
	boundary.append(_P(5, 10));
	boundary.append(_P(0, 15));
	boundary.append(_P(0, 20));
	boundary.append(_P(10, 10));

	test_object = space.draw(boundary);


	focus = space.addEdge(test_object->getRoot()->getInv(), _P(-10, 10));
	space.addEdge(focus, test_object->getRoot()->getNext()->getNext()->getNext()->getNext()->getInv());
	space.addEdge(test_object->getRoot()->getNext()->getInv(), test_object->getRoot()->getNext()->getNext()->getNext()->getInv());

	outside = test_object->getRoot()->getInv()->getFace();

	test_object->mergeWithFace(outside, results);

	EXPECT_EQ(results.size(), 2);
}

/*
TEST(Face_Merge, Hole_Absorbed) {
	DCEL* space = new DCEL();

	std::vector<_P> boundary_big;
	{
		boundary_big.push_back(_P(0, 0));
		boundary_big.push_back(_P(0, 20));
		boundary_big.push_back(_P(20, 20));
		boundary_big.push_back(_P(20, 0));
	}
	std::vector<_P> boundary_small_a;
	{
		boundary_small_a.push_back(_P(2, 2));
		boundary_small_a.push_back(_P(2, 4));
		boundary_small_a.push_back(_P(4, 4));
		boundary_small_a.push_back(_P(4, 2));
	}

	std::vector<DCEL::Face*> interior;
	std::vector<DCEL::Face*> exterior;

	DCEL::Face* product = space->createFace(boundary_big);
	product->subAllocateFace(boundary_small_a, interior, exterior);

	EXPECT_TRUE(exterior[0]->mergeWithFace(interior[0]));

	EXPECT_EQ(exterior[0]->getHoleCount(), 0);
	EXPECT_EQ(exterior[0]->getRootEdge()->loopArea(), 400);
}

TEST(Face_Merge, Interior_On_Hole) {
	DCEL* space = new DCEL();

	std::vector<_P> boundary_big;
	{
		boundary_big.push_back(_P(0, 0));
		boundary_big.push_back(_P(0, 20));
		boundary_big.push_back(_P(20, 20));
		boundary_big.push_back(_P(20, 0));
	}
	std::vector<_P> boundary_small_a;
	{
		boundary_small_a.push_back(_P(2, 2));
		boundary_small_a.push_back(_P(2, 4));
		boundary_small_a.push_back(_P(4, 4));
		boundary_small_a.push_back(_P(4, 2));
	}
	std::vector<_P> boundary_small_b;
	{
		boundary_small_b.push_back(_P(4, 2));
		boundary_small_b.push_back(_P(4, 4));
		boundary_small_b.push_back(_P(6, 4));
		boundary_small_b.push_back(_P(6, 2));
	}

	std::vector<DCEL::Face*> interior_prep;
	std::vector<DCEL::Face*> exterior_prep;

	std::vector<DCEL::Face*> interior;
	std::vector<DCEL::Face*> exterior;

	DCEL::Face* product = space->createFace(boundary_big);
	product->subAllocateFace(boundary_small_a, interior_prep, exterior_prep);
	exterior_prep[0]->subAllocateFace(boundary_small_b, interior, exterior);

	EXPECT_TRUE(exterior[0]->mergeWithFace(interior[0]));

	EXPECT_EQ(exterior[0]->getHoleCount(), 1);
	EXPECT_EQ(exterior[0]->getRootEdge()->loopArea(), 400);
	EXPECT_EQ(exterior[0]->getHole(0)->loopArea(), -4);
	EXPECT_EQ(interior_prep[0]->getRootEdge()->loopArea(), 4);

}

TEST(Face_Merge, Bridging_Hole) {
	DCEL* space = new DCEL();

	std::vector<_P> boundary_big;
	{
		boundary_big.push_back(_P(0, 0));
		boundary_big.push_back(_P(0, 20));
		boundary_big.push_back(_P(20, 20));
		boundary_big.push_back(_P(20, 0));
	}
	std::vector<_P> boundary_small_a;
	{
		boundary_small_a.push_back(_P(2, 2));
		boundary_small_a.push_back(_P(2, 4));
		boundary_small_a.push_back(_P(4, 4));
		boundary_small_a.push_back(_P(4, 2));
	}
	std::vector<_P> boundary_small_b;
	{
		boundary_small_b.push_back(_P(0, 1));
		boundary_small_b.push_back(_P(0, 5));
		boundary_small_b.push_back(_P(2, 5));
		boundary_small_b.push_back(_P(2, 1));
	}

	std::vector<DCEL::Face*> interior_prep;
	std::vector<DCEL::Face*> exterior_prep;

	std::vector<DCEL::Face*> interior;
	std::vector<DCEL::Face*> exterior;

	DCEL::Face* product = space->createFace(boundary_big);
	DCEL::Face* null = product->getRootEdge()->getInverseEdge()->getFace();
	product->subAllocateFace(boundary_small_a, interior_prep, exterior_prep);
	exterior_prep[0]->subAllocateFace(boundary_small_b, interior, exterior);

	auto debug_null = null->getHole(0)->listPoints();
	auto debug_ext = exterior[0]->getRootEdge()->listPoints();
	auto debug_a = interior[0]->getRootEdge()->listPoints();
	auto debug_b = interior_prep[0]->getRootEdge()->listPoints();

	EXPECT_TRUE(exterior[0]->mergeWithFace(interior[0]));

	auto debug_null_after = null->getHole(0)->listPoints();
	auto debug_ext_after = exterior[0]->getRootEdge()->listPoints();
	auto debug_b_after = interior_prep[0]->getRootEdge()->listPoints();

	EXPECT_EQ(interior_prep[0]->getHoleCount(), 0);
	EXPECT_EQ(exterior[0]->getHoleCount(), 1);
	EXPECT_EQ(null->getHoleCount(), 1);

	EXPECT_EQ(interior_prep[0]->borderCount(10), 4);
	EXPECT_EQ(exterior[0]->borderCount(10), 6);
	EXPECT_EQ(null->holeBorderCount(0, 10), 6);

	EXPECT_EQ(exterior[0]->getRootEdge()->loopArea(), 400);
	EXPECT_EQ(exterior[0]->getHole(0)->loopArea(), -4);
	EXPECT_EQ(interior_prep[0]->getRootEdge()->loopArea(), 4);
	EXPECT_EQ(null->getHole(0)->loopArea(), -400);

}


TEST(Face_Merge, Hole_Merged_Out) {
	DCEL* space = new DCEL();

	std::vector<_P> boundary_big;
	{
		boundary_big.push_back(_P(0, 0));
		boundary_big.push_back(_P(0, 20));
		boundary_big.push_back(_P(20, 20));
		boundary_big.push_back(_P(20, 0));
	}
	std::vector<_P> boundary_small_a;
	{
		boundary_small_a.push_back(_P(2, 2));
		boundary_small_a.push_back(_P(2, 4));
		boundary_small_a.push_back(_P(4, 4));
		boundary_small_a.push_back(_P(4, 2));
	}

	std::vector<DCEL::Face*> interior;
	std::vector<DCEL::Face*> exterior;

	DCEL::Face* product = space->createFace(boundary_big);
	product->subAllocateFace(boundary_small_a, interior, exterior);

	EXPECT_TRUE(interior[0]->mergeWithFace(exterior[0]));

	EXPECT_EQ(interior[0]->getHoleCount(), 0);
	EXPECT_EQ(interior[0]->getRootEdge()->loopArea(), 400);
}

TEST(Face_Merge, Internal_On_Boundary) {
	DCEL* space = new DCEL();

	std::vector<_P> boundary_big;
	{
		boundary_big.push_back(_P(0, 0));
		boundary_big.push_back(_P(0, 20));
		boundary_big.push_back(_P(20, 20));
		boundary_big.push_back(_P(20, 0));
	}
	std::vector<_P> boundary_small_a;
	{
		boundary_small_a.push_back(_P(0, 2));
		boundary_small_a.push_back(_P(0, 4));
		boundary_small_a.push_back(_P(4, 4));
		boundary_small_a.push_back(_P(4, 2));
	}

	std::vector<DCEL::Face*> interior;
	std::vector<DCEL::Face*> exterior;

	DCEL::Face* product = space->createFace(boundary_big);
	product->subAllocateFace(boundary_small_a, interior, exterior);

	EXPECT_TRUE(interior[0]->mergeWithFace(exterior[0]));

	EXPECT_EQ(interior[0]->getHoleCount(), 0);
	EXPECT_EQ(interior[0]->getRootEdge()->loopArea(), 400);
}

TEST(Face_Merge, External_On_Boundary) {
	DCEL* space = new DCEL();

	std::vector<_P> boundary_big;
	{
		boundary_big.push_back(_P(0, 0));
		boundary_big.push_back(_P(0, 20));
		boundary_big.push_back(_P(20, 20));
		boundary_big.push_back(_P(20, 0));
	}
	std::vector<_P> boundary_small_a;
	{
		boundary_small_a.push_back(_P(-4, 0));
		boundary_small_a.push_back(_P(-4, 4));
		boundary_small_a.push_back(_P(0, 4));
		boundary_small_a.push_back(_P(0, 0));
	}

	std::vector<DCEL::Face*> interior;
	std::vector<DCEL::Face*> exterior;

	DCEL::Face* product = space->createFace(boundary_big);
	DCEL::Face* null = product->getRootEdge()->getInverseEdge()->getFace();
	null->subAllocateFace(boundary_small_a, interior, exterior);

	EXPECT_TRUE(interior[0]->mergeWithFace(product));

	EXPECT_EQ(interior[0]->getHoleCount(), 0);
	EXPECT_EQ(interior[0]->getRootEdge()->loopArea(), 416);
}

TEST(Face_Merge, External_On_Hole) {
	DCEL* space = new DCEL();

	std::vector<_P> boundary_big;
	{
		boundary_big.push_back(_P(0, 0));
		boundary_big.push_back(_P(0, 20));
		boundary_big.push_back(_P(20, 20));
		boundary_big.push_back(_P(20, 0));
	}
	std::vector<_P> boundary_small_a;
	{
		boundary_small_a.push_back(_P(2, 2));
		boundary_small_a.push_back(_P(2, 4));
		boundary_small_a.push_back(_P(4, 4));
		boundary_small_a.push_back(_P(4, 2));
	}
	std::vector<_P> boundary_small_b;
	{
		boundary_small_b.push_back(_P(4, 2));
		boundary_small_b.push_back(_P(4, 4));
		boundary_small_b.push_back(_P(6, 4));
		boundary_small_b.push_back(_P(6, 2));
	}

	std::vector<DCEL::Face*> interior_prep;
	std::vector<DCEL::Face*> exterior_prep;

	std::vector<DCEL::Face*> interior;
	std::vector<DCEL::Face*> exterior;

	DCEL::Face* product = space->createFace(boundary_big);
	product->subAllocateFace(boundary_small_a, interior_prep, exterior_prep);
	exterior_prep[0]->subAllocateFace(boundary_small_b, interior, exterior);

	EXPECT_TRUE(exterior[0]->mergeWithFace(interior[0]));

	EXPECT_EQ(exterior[0]->getHoleCount(), 1);
	EXPECT_EQ(exterior[0]->getRootEdge()->loopArea(), 400);
	EXPECT_EQ(exterior[0]->getHole(0)->loopArea(), -4);
	EXPECT_EQ(interior_prep[0]->getRootEdge()->loopArea(), 4);

}*/