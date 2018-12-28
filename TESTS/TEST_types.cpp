#include "pch.h"
#include "..\DCEL_CUTS\DCEL_Point.h"
#include "..\DCEL_CUTS\DCEL_types.h"

TEST(DCEL_Basics, add_edge) {
	DCEL<Pint> space;
	Pint A(0, 0);
	Pint B(10, 0);

	Edge<Pint> const * test_object = space.addEdge(A, B);

	Edge<Pint> const * inverse = test_object->getInv();

	EXPECT_EQ(test_object->getNext(), inverse);
	EXPECT_EQ(test_object->getLast(), inverse);

	EXPECT_EQ(inverse->getNext(), test_object);
	EXPECT_EQ(inverse->getLast(), test_object);

	EXPECT_EQ(inverse->getFace(), test_object->getFace());

	EXPECT_EQ(test_object->getStart()->getPosition(), A);
	EXPECT_EQ(test_object->getEnd()->getPosition(), B);
}

TEST(DCEL_Basics, add_edgePintair) {
	DCEL<Pint> space;
	Pint A(0, 0);
	Pint B(10, 0);
	Pint C(10, 10);

	Edge<Pint> * link_A = space.addEdge(A, B);
	Edge<Pint> const * link_B = space.addEdge(link_A, C);

	Edge<Pint> const * inv_A = link_A->getInv();
	Edge<Pint> const * inv_B = link_B->getInv();

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
	DCEL<Pint> space;
	Pint A(0, 0);
	Pint B(10, 0);
	Pint C(10, 10);

	Edge<Pint> * link_A = space.addEdge(A, B);
	Edge<Pint> * link_B = space.addEdge(link_A, C);
	Edge<Pint> * link_C = space.addEdge(link_B, link_A->getInv());


	Edge<Pint> * inv_A = link_A->getInv();
	Edge<Pint> * inv_B = link_B->getInv();
	Edge<Pint> * inv_C = link_C->getInv();

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
	DCEL<Pint> space;
	FLL<Pint> boundary;
	Face<Pint> * test_object;
	Edge<Pint> * focus;
	
	boundary.append(Pint(0, 0));
	boundary.append(Pint(0, 20));
	boundary.append(Pint(20, 20));
	boundary.append(Pint(20, 0));

	test_object = space.draw(boundary);

	focus = test_object->getRoot();

	EXPECT_EQ(focus->getStart()->getPosition(), Pint(0, 0));
	EXPECT_EQ(focus->getFace(), test_object);
	EXPECT_NE(focus->getInv()->getFace(), test_object);

	focus = focus->getNext();

	EXPECT_EQ(focus->getStart()->getPosition(), Pint(0, 20));
	EXPECT_EQ(focus->getFace(), test_object);
	EXPECT_NE(focus->getInv()->getFace(), test_object);

	focus = focus->getNext();

	EXPECT_EQ(focus->getStart()->getPosition(), Pint(20, 20));
	EXPECT_EQ(focus->getFace(), test_object);
	EXPECT_NE(focus->getInv()->getFace(), test_object);

	focus = focus->getNext();

	EXPECT_EQ(focus->getStart()->getPosition(), Pint(20, 0));
	EXPECT_EQ(focus->getFace(), test_object);
	EXPECT_NE(focus->getInv()->getFace(), test_object);


	//EXPECT_EQ(test_object->loopArea(), 400);
	//EXPECT_EQ(test_object->getRoot()->getInv()->getFace()->loopArea(), -400);
}

TEST(DCEL_Basics, append_draw) {
	DCEL<Pint> space;
	FLL<Pint> boundary;
	Face<Pint> * test_object;
	Edge<Pint> * focus;

	boundary.append(Pint(0, 0));
	boundary.append(Pint(0, 20));
	boundary.append(Pint(20, 20));
	boundary.append(Pint(20, 0));

	test_object = space.draw(boundary);

	focus = space.addEdge(test_object->getRoot()->getInv(), Pint(-20, 0));
	focus = space.addEdge(focus, Pint(-20, 20));
	focus = space.addEdge(focus, test_object->getRoot()->getInv()->getLast());


	//test_object

	//EXPECT_EQ(test_object->loopArea(), 400);
	//EXPECT_EQ(focus->getFace()->loopArea(), 400);
	//EXPECT_EQ(focus->getInv()->getFace()->loopArea(), -800);
}



TEST(Edge_Tests, subdivide) {
	DCEL<Pint> space;
	Pint A(0, 0);
	Pint B(10, 0);
	Pint C(10, 10);

	Edge<Pint> * link_A = space.addEdge(A, C);
	link_A->subdivide(B);

	Edge<Pint> const * link_B = link_A->getNext();

	Edge<Pint> const * inv_A = link_A->getInv();
	Edge<Pint> const * inv_B = link_B->getInv();

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
	DCEL<Pint> space;

	for (int target_size = 4; target_size < 10; target_size++)
	{
		FLL<Pint> boundary;
		for (int ii = 0; ii < target_size; ii++) {
			boundary.push(Pint(ii,ii));
		}

		Face<Pint> const * product = space.draw(boundary);
		Face<Pint> const * null = product->getRoot()->getInv()->getFace();

		EXPECT_EQ(product->getLoopSize(), target_size);
		EXPECT_EQ(null->getLoopSize(), target_size);
	}
}

// Face<Pint> CUTS
/*
TEST(FACE_Merge, append_merge) {
	DCEL<Pint> space;
	FLL<Pint> boundary;
	FLL<Face<Pint> *> results;
	Face<Pint> * test_object;
	Face<Pint> * partner;
	Edge<Pint> * focus;

	boundary.append(Pint(0, 0));
	boundary.append(Pint(0, 20));
	boundary.append(Pint(20, 20));
	boundary.append(Pint(20, 0));

	test_object = space.draw(boundary);

	focus = space.addEdge(test_object->getRoot()->getInv(), Pint(-20, 0));
	focus = space.addEdge(focus, Pint(-20, 20));
	focus = space.addEdge(focus, test_object->getRoot()->getInv()->getLast());

	partner = focus->getFace();

	results = test_object->mergeWithFace(partner);

	//EXPECT_EQ(test_object->loopArea(), 800);
	//EXPECT_EQ(focus->getInv()->getFace()->loopArea(), -800);

	EXPECT_FALSE(results.empty());
}
TEST(FACE_Merge, entire_merge) {
	DCEL<Pint> space;
	FLL<Pint> boundary;
	FLL<Face<Pint> *> results;
	Face<Pint> * test_object;
	Face<Pint> * outside;

	boundary.append(Pint(0, 0));
	boundary.append(Pint(0, 20));
	boundary.append(Pint(20, 20));
	boundary.append(Pint(20, 0));

	test_object = space.draw(boundary);

	outside = test_object->getRoot()->getInv()->getFace();

	results = test_object->mergeWithFace(outside);

	EXPECT_TRUE(results.empty());
}

TEST(FACE_Merge, merge_gap) {
	DCEL<Pint> space;
	FLL<Pint> boundary;
	FLL<Face<Pint> *> results;
	Face<Pint> * test_object;
	Face<Pint> * outside;
	Edge<Pint> * focus;

	boundary.append(Pint(0, 0));
	boundary.append(Pint(0, 5));
	boundary.append(Pint(5, 10));
	boundary.append(Pint(0, 15));
	boundary.append(Pint(0, 20));
	boundary.append(Pint(10, 10));

	test_object = space.draw(boundary);


	focus = space.addEdge(test_object->getRoot()->getInv(), Pint(-10, 10));
	space.addEdge(focus, test_object->getRoot()->getNext()->getNext()->getNext()->getNext()->getInv());
	space.addEdge(test_object->getRoot()->getNext()->getInv(), test_object->getRoot()->getNext()->getNext()->getNext()->getInv());

	outside = test_object->getRoot()->getInv()->getFace();

	results = test_object->mergeWithFace(outside);

	EXPECT_EQ(results.size(), 2);
}

/*
TEST(Face_Merge, Hole_Absorbed) {
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

	FLL<DCEL::Face*> interior;
	FLL<DCEL::Face*> exterior;

	DCEL::Face* product = space->createFace(boundary_big);
	product->subAllocateFace(boundary_small_a, interior, exterior);

	EXPECT_TRUE(exterior[0]->mergeWithFace(interior[0]));

	EXPECT_EQ(exterior[0]->getHoleCount(), 0);
	EXPECT_EQ(exterior[0]->getRootEdge()->loopArea(), 400);
}

TEST(Face_Merge, Interior_On_Hole) {
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
		boundary_small_b.append(Pint(4, 2));
		boundary_small_b.append(Pint(4, 4));
		boundary_small_b.append(Pint(6, 4));
		boundary_small_b.append(Pint(6, 2));
	}

	FLL<DCEL::Face*> interiorPintrep;
	FLL<DCEL::Face*> exteriorPintrep;

	FLL<DCEL::Face*> interior;
	FLL<DCEL::Face*> exterior;

	DCEL::Face* product = space->createFace(boundary_big);
	product->subAllocateFace(boundary_small_a, interiorPintrep, exteriorPintrep);
	exteriorPintrep[0]->subAllocateFace(boundary_small_b, interior, exterior);

	EXPECT_TRUE(exterior[0]->mergeWithFace(interior[0]));

	EXPECT_EQ(exterior[0]->getHoleCount(), 1);
	EXPECT_EQ(exterior[0]->getRootEdge()->loopArea(), 400);
	EXPECT_EQ(exterior[0]->getHole(0)->loopArea(), -4);
	EXPECT_EQ(interiorPintrep[0]->getRootEdge()->loopArea(), 4);

}

TEST(Face_Merge, Bridging_Hole) {
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
		boundary_small_b.append(Pint(0, 1));
		boundary_small_b.append(Pint(0, 5));
		boundary_small_b.append(Pint(2, 5));
		boundary_small_b.append(Pint(2, 1));
	}

	FLL<DCEL::Face*> interiorPintrep;
	FLL<DCEL::Face*> exteriorPintrep;

	FLL<DCEL::Face*> interior;
	FLL<DCEL::Face*> exterior;

	DCEL::Face* product = space->createFace(boundary_big);
	DCEL::Face* null = product->getRootEdge()->getInverseEdge()->getFace();
	product->subAllocateFace(boundary_small_a, interiorPintrep, exteriorPintrep);
	exteriorPintrep[0]->subAllocateFace(boundary_small_b, interior, exterior);

	auto debug_null = null->getHole(0)->listPoints();
	auto debug_ext = exterior[0]->getRootEdge()->listPoints();
	auto debug_a = interior[0]->getRootEdge()->listPoints();
	auto debug_b = interiorPintrep[0]->getRootEdge()->listPoints();

	EXPECT_TRUE(exterior[0]->mergeWithFace(interior[0]));

	auto debug_null_after = null->getHole(0)->listPoints();
	auto debug_ext_after = exterior[0]->getRootEdge()->listPoints();
	auto debug_b_after = interiorPintrep[0]->getRootEdge()->listPoints();

	EXPECT_EQ(interiorPintrep[0]->getHoleCount(), 0);
	EXPECT_EQ(exterior[0]->getHoleCount(), 1);
	EXPECT_EQ(null->getHoleCount(), 1);

	EXPECT_EQ(interiorPintrep[0]->borderCount(10), 4);
	EXPECT_EQ(exterior[0]->borderCount(10), 6);
	EXPECT_EQ(null->holeBorderCount(0, 10), 6);

	EXPECT_EQ(exterior[0]->getRootEdge()->loopArea(), 400);
	EXPECT_EQ(exterior[0]->getHole(0)->loopArea(), -4);
	EXPECT_EQ(interiorPintrep[0]->getRootEdge()->loopArea(), 4);
	EXPECT_EQ(null->getHole(0)->loopArea(), -400);

}


TEST(Face_Merge, Hole_Merged_Out) {
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

	FLL<DCEL::Face*> interior;
	FLL<DCEL::Face*> exterior;

	DCEL::Face* product = space->createFace(boundary_big);
	product->subAllocateFace(boundary_small_a, interior, exterior);

	EXPECT_TRUE(interior[0]->mergeWithFace(exterior[0]));

	EXPECT_EQ(interior[0]->getHoleCount(), 0);
	EXPECT_EQ(interior[0]->getRootEdge()->loopArea(), 400);
}

TEST(Face_Merge, Internal_On_Boundary) {
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
	product->subAllocateFace(boundary_small_a, interior, exterior);

	EXPECT_TRUE(interior[0]->mergeWithFace(exterior[0]));

	EXPECT_EQ(interior[0]->getHoleCount(), 0);
	EXPECT_EQ(interior[0]->getRootEdge()->loopArea(), 400);
}

TEST(Face_Merge, External_On_Boundary) {
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
		boundary_small_a.append(Pint(-4, 0));
		boundary_small_a.append(Pint(-4, 4));
		boundary_small_a.append(Pint(0, 4));
		boundary_small_a.append(Pint(0, 0));
	}

	FLL<DCEL::Face*> interior;
	FLL<DCEL::Face*> exterior;

	DCEL::Face* product = space->createFace(boundary_big);
	DCEL::Face* null = product->getRootEdge()->getInverseEdge()->getFace();
	null->subAllocateFace(boundary_small_a, interior, exterior);

	EXPECT_TRUE(interior[0]->mergeWithFace(product));

	EXPECT_EQ(interior[0]->getHoleCount(), 0);
	EXPECT_EQ(interior[0]->getRootEdge()->loopArea(), 416);
}

TEST(Face_Merge, External_On_Hole) {
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
		boundary_small_b.append(Pint(4, 2));
		boundary_small_b.append(Pint(4, 4));
		boundary_small_b.append(Pint(6, 4));
		boundary_small_b.append(Pint(6, 2));
	}

	FLL<DCEL::Face*> interiorPintrep;
	FLL<DCEL::Face*> exteriorPintrep;

	FLL<DCEL::Face*> interior;
	FLL<DCEL::Face*> exterior;

	DCEL::Face* product = space->createFace(boundary_big);
	product->subAllocateFace(boundary_small_a, interiorPintrep, exteriorPintrep);
	exteriorPintrep[0]->subAllocateFace(boundary_small_b, interior, exterior);

	EXPECT_TRUE(exterior[0]->mergeWithFace(interior[0]));

	EXPECT_EQ(exterior[0]->getHoleCount(), 1);
	EXPECT_EQ(exterior[0]->getRootEdge()->loopArea(), 400);
	EXPECT_EQ(exterior[0]->getHole(0)->loopArea(), -4);
	EXPECT_EQ(interiorPintrep[0]->getRootEdge()->loopArea(), 4);

}*/