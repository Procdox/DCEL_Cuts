#include "pch.h"
#include "..\DCEL_CUTS\DCEL_Region.h"

TEST(Region_Basics, Region_Creation) {

	//setup
	DCEL<Pint> space;

	//results
	Region<Pint> * product, * null;

	//tested operations are performed within this block
	{
		FLL<Pint> boundary_big;
		{
			boundary_big.append(Pint(0, 0));
			boundary_big.append(Pint(0, 20));
			boundary_big.append(Pint(20, 20));
			boundary_big.append(Pint(20, 0));
		}

		product = space.region(boundary_big);

		auto null_face = (*product)[0]->getRoot()->getInv()->getFace();

		null = space.region(null_face);
	}

	//testing

	EXPECT_EQ(space.pointCount(), 4);
	EXPECT_EQ(space.edgeCount(), 8);
	EXPECT_EQ(space.faceCount(), 2);

	ASSERT_EQ(product->size(), 1);
	ASSERT_EQ(null->size(), 1);

	Face<Pint> * product_0 = (*product)[0];
	Face<Pint> * null_0 = (*null)[0];

	ASSERT_NE(product_0, nullptr);
	ASSERT_NE(null_0, nullptr);

	EXPECT_EQ(product_0->getLoopSize(), 4);
	EXPECT_EQ(null_0->getLoopSize(), 4);

	auto area_prod_0 = Pint::area(product_0->getLoopPoints());
	auto area_null_0 = Pint::area(null_0->getLoopPoints());

	EXPECT_TRUE(area_prod_0 == rto(400));
	EXPECT_TRUE(area_null_0 == rto(-400));

	EXPECT_TRUE(product_0->getNeighbors().contains(null_0));

	EXPECT_TRUE(null_0->getNeighbors().contains(product_0));
}

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

	auto product = space.region(boundary_big);

	//inside simple
	EXPECT_EQ(contains(product, Pint(2,2)).type, FaceRelationType::point_interior);

	//outside simple
	EXPECT_EQ(contains(product, Pint(-2, 2)).type, FaceRelationType::point_exterior);

	//boundary simple
	EXPECT_EQ(contains(product, Pint(0, 2)).type, FaceRelationType::point_on_boundary);
}


TEST(Face_Cuts, Hole) {

	//setup
	DCEL<Pint> space;
	
	//results
	FLL<Region<Pint> *> interior;
	FLL<Region<Pint> *> exterior;
	Region<Pint> * null;

	//tested operations are performed within this block
	{
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

		auto product = space.region(boundary_big);

		auto null_face = (*product)[0]->getRoot()->getInv()->getFace();

		null = space.region(null_face);

		subAllocate(product, boundary_small_a, exterior, interior);
	}

	//testing

	EXPECT_EQ(space.pointCount(), 8);
	EXPECT_EQ(space.edgeCount(), 16);
	EXPECT_EQ(space.faceCount(), 4);
	EXPECT_EQ(space.regionCount(), 3);

	ASSERT_EQ(interior.size(), 1);
	ASSERT_EQ(exterior.size(), 1);

	ASSERT_NE(interior[0], nullptr);
	ASSERT_NE(exterior[0], nullptr);

	Region<Pint> * interior_0 = interior[0];
	Region<Pint> * exterior_0 = exterior[0];

	ASSERT_NE(interior_0, nullptr);
	ASSERT_NE(exterior_0, nullptr);

	ASSERT_EQ(interior_0->size(), 1);
	ASSERT_EQ(exterior_0->size(), 2);
	ASSERT_EQ(null->size(), 1);

	Face<Pint> * interior_0_0 = (*interior_0)[0];
	Face<Pint> * exterior_0_0 = (*exterior_0)[0];
	Face<Pint> * exterior_0_1 = (*exterior_0)[1];
	Face<Pint> * null_0 = (*null)[0];

	ASSERT_NE(interior_0_0, nullptr);
	ASSERT_NE(exterior_0_0, nullptr);
	ASSERT_NE(exterior_0_1, nullptr);
	ASSERT_NE(null_0, nullptr);

	EXPECT_EQ(interior_0_0->getLoopSize(), 4);
	EXPECT_EQ(exterior_0_0->getLoopSize(), 4);
	EXPECT_EQ(exterior_0_1->getLoopSize(), 4);
	EXPECT_EQ(null_0->getLoopSize(), 4);

	auto area_in_0_0 = Pint::area(interior_0_0->getLoopPoints());
	auto area_ex_0_0 = Pint::area(exterior_0_0->getLoopPoints());
	auto area_ex_0_1 = Pint::area(exterior_0_1->getLoopPoints());
	auto area_null_0 = Pint::area(null_0->getLoopPoints());

	EXPECT_TRUE(area_in_0_0 == rto(4));
	EXPECT_TRUE(area_ex_0_0 == rto(-4));
	EXPECT_TRUE(area_ex_0_1 == rto(400));
	EXPECT_TRUE(area_null_0 == rto(-400));

	EXPECT_TRUE(interior_0_0->getNeighbors().contains(exterior_0_0));
	EXPECT_FALSE(interior_0_0->getNeighbors().contains(exterior_0_1));
	EXPECT_FALSE(interior_0_0->getNeighbors().contains(null_0));

	EXPECT_TRUE(exterior_0_0->getNeighbors().contains(interior_0_0));
	EXPECT_FALSE(exterior_0_0->getNeighbors().contains(exterior_0_1));
	EXPECT_FALSE(exterior_0_0->getNeighbors().contains(null_0));

	EXPECT_FALSE(exterior_0_1->getNeighbors().contains(interior_0_0));
	EXPECT_FALSE(exterior_0_1->getNeighbors().contains(exterior_0_0));
	EXPECT_TRUE(exterior_0_1->getNeighbors().contains(null_0));

	EXPECT_FALSE(null_0->getNeighbors().contains(interior_0_0));
	EXPECT_FALSE(null_0->getNeighbors().contains(exterior_0_0));
	EXPECT_TRUE(null_0->getNeighbors().contains(exterior_0_1));
}


TEST(Face_Cuts, Edge_Meeting_Cut) {

	//setup
	DCEL<Pint> space;

	//results
	FLL<Region<Pint> *> interior;
	FLL<Region<Pint> *> exterior;
	Region<Pint> * null;

	//tested operations are performed within this block
	{
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

		auto product = space.region(boundary_big);

		auto null_face = (*product)[0]->getRoot()->getInv()->getFace();

		null = space.region(null_face);

		subAllocate(product, boundary_small_a, exterior, interior);
	}

	//testing

	EXPECT_EQ(space.pointCount(), 8);
	EXPECT_EQ(space.edgeCount(), 18);
	EXPECT_EQ(space.faceCount(), 3);
	EXPECT_EQ(space.regionCount(), 3);

	ASSERT_EQ(interior.size(), 1);
	ASSERT_EQ(exterior.size(), 1);

	ASSERT_NE(interior[0], nullptr);
	ASSERT_NE(exterior[0], nullptr);

	Region<Pint> * interior_0 = interior[0];
	Region<Pint> * exterior_0 = exterior[0];

	ASSERT_NE(interior_0, nullptr);
	ASSERT_NE(exterior_0, nullptr);

	ASSERT_EQ(interior_0->size(), 1);
	ASSERT_EQ(exterior_0->size(), 1);
	ASSERT_EQ(null->size(), 1);

	Face<Pint> * interior_0_0 = (*interior_0)[0];
	Face<Pint> * exterior_0_0 = (*exterior_0)[0];
	Face<Pint> * null_0 = (*null)[0];

	ASSERT_NE(interior_0_0, nullptr);
	ASSERT_NE(exterior_0_0, nullptr);
	ASSERT_NE(null_0, nullptr);

	EXPECT_EQ(interior_0_0->getLoopSize(), 4);
	EXPECT_EQ(exterior_0_0->getLoopSize(), 8);
	EXPECT_EQ(null_0->getLoopSize(), 6);

	auto area_in_0_0 = Pint::area(interior_0_0->getLoopPoints());
	auto area_ex_0_0 = Pint::area(exterior_0_0->getLoopPoints());
	auto area_null_0 = Pint::area(null_0->getLoopPoints());

	EXPECT_TRUE(area_in_0_0 == rto(8));
	EXPECT_TRUE(area_ex_0_0 == rto(392));
	EXPECT_TRUE(area_null_0 == rto(-400));

	EXPECT_TRUE(interior_0_0->getNeighbors().contains(exterior_0_0));
	EXPECT_TRUE(interior_0_0->getNeighbors().contains(null_0));

	EXPECT_TRUE(exterior_0_0->getNeighbors().contains(interior_0_0));
	EXPECT_TRUE(exterior_0_0->getNeighbors().contains(null_0));

	EXPECT_TRUE(null_0->getNeighbors().contains(interior_0_0));
	EXPECT_TRUE(null_0->getNeighbors().contains(exterior_0_0));
}





TEST(Face_Cuts, Point_Meeting_Cut) {

	//setup
	DCEL<Pint> space;

	//results
	FLL<Region<Pint> *> interior;
	FLL<Region<Pint> *> exterior;
	Region<Pint> * null;

	//tested operations are performed within this block
	{
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

		auto product = space.region(boundary_big);

		auto null_face = (*product)[0]->getRoot()->getInv()->getFace();

		null = space.region(null_face);

		subAllocate(product, boundary_small_a, exterior, interior);
	}
	
	//testing

	EXPECT_EQ(space.pointCount(), 9);
	EXPECT_EQ(space.edgeCount(), 20);
	EXPECT_EQ(space.faceCount(), 3);
	EXPECT_EQ(space.regionCount(), 3);

	ASSERT_EQ(interior.size(), 1);
	ASSERT_EQ(exterior.size(), 1);

	ASSERT_NE(interior[0], nullptr);
	ASSERT_NE(exterior[0], nullptr);

	Region<Pint> * interior_0 = interior[0];
	Region<Pint> * exterior_0 = exterior[0];

	ASSERT_NE(interior_0, nullptr);
	ASSERT_NE(exterior_0, nullptr);

	ASSERT_EQ(interior_0->size(), 1);
	ASSERT_EQ(exterior_0->size(), 1);
	ASSERT_EQ(null->size(), 1);

	Face<Pint> * interior_0_0 = (*interior_0)[0];
	Face<Pint> * exterior_0_0 = (*exterior_0)[0];
	Face<Pint> * null_0 = (*null)[0];

	ASSERT_NE(interior_0_0, nullptr);
	ASSERT_NE(exterior_0_0, nullptr);
	ASSERT_NE(null_0, nullptr);

	EXPECT_EQ(interior_0_0->getLoopSize(), 4);
	EXPECT_EQ(exterior_0_0->getLoopSize(), 10);
	EXPECT_EQ(null_0->getLoopSize(), 6);

	auto area_in_0_0 = Pint::area(interior_0_0->getLoopPoints());
	auto area_ex_0_0 = Pint::area(exterior_0_0->getLoopPoints());
	auto area_null_0 = Pint::area(null_0->getLoopPoints());

	EXPECT_TRUE(area_in_0_0 == rto(4));
	EXPECT_TRUE(area_ex_0_0 == rto(392));
	EXPECT_TRUE(area_null_0 == rto(-396));

	EXPECT_TRUE(interior_0_0->getNeighbors().contains(exterior_0_0));
	EXPECT_FALSE(interior_0_0->getNeighbors().contains(null_0));

	EXPECT_TRUE(exterior_0_0->getNeighbors().contains(interior_0_0));
	EXPECT_TRUE(exterior_0_0->getNeighbors().contains(null_0));

	EXPECT_FALSE(null_0->getNeighbors().contains(interior_0_0));
	EXPECT_TRUE(null_0->getNeighbors().contains(exterior_0_0));
}

TEST(Face_Cuts, Matching_Cut) {

	//setup
	DCEL<Pint> space;

	//results
	FLL<Region<Pint> *> interior;
	FLL<Region<Pint> *> exterior;
	Region<Pint> * null;


	//tested operations are performed within this block
	{
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

		auto product = space.region(boundary_big);
		
		auto null_face = (*product)[0]->getRoot()->getInv()->getFace();

		null = space.region(null_face);

		subAllocate(product, boundary_small_a, exterior, interior);
	}

	//testing

	EXPECT_EQ(space.pointCount(), 10);
	EXPECT_EQ(space.edgeCount(), 22);
	EXPECT_EQ(space.faceCount(), 3);
	EXPECT_EQ(space.regionCount(), 3);

	ASSERT_EQ(interior.size(), 1);
	ASSERT_EQ(exterior.size(), 1);

	ASSERT_NE(interior[0], nullptr);
	ASSERT_NE(exterior[0], nullptr);

	Region<Pint> * interior_0 = interior[0];
	Region<Pint> * exterior_0 = exterior[0];

	ASSERT_NE(interior_0, nullptr);
	ASSERT_NE(exterior_0, nullptr);

	ASSERT_EQ(interior_0->size(), 1);
	ASSERT_EQ(exterior_0->size(), 1);
	ASSERT_EQ(null->size(), 1);

	Face<Pint> * interior_0_0 = (*interior_0)[0];
	Face<Pint> * exterior_0_0 = (*exterior_0)[0];
	Face<Pint> * null_0 = (*null)[0];

	ASSERT_NE(interior_0_0, nullptr);
	ASSERT_NE(exterior_0_0, nullptr);
	ASSERT_NE(null_0, nullptr);

	EXPECT_EQ(interior_0_0->getLoopSize(), 4);
	EXPECT_EQ(exterior_0_0->getLoopSize(), 10);
	EXPECT_EQ(null_0->getLoopSize(), 8);

	auto area_in_0_0 = Pint::area(interior_0_0->getLoopPoints());
	auto area_ex_0_0 = Pint::area(exterior_0_0->getLoopPoints());
	auto area_null_0 = Pint::area(null_0->getLoopPoints());

	EXPECT_TRUE(area_in_0_0 == rto(4));
	EXPECT_TRUE(area_ex_0_0 == rto(392));
	EXPECT_TRUE(area_null_0 == rto(-396));

	EXPECT_TRUE(interior_0_0->getNeighbors().contains(exterior_0_0));
	EXPECT_TRUE(interior_0_0->getNeighbors().contains(null_0));

	EXPECT_TRUE(exterior_0_0->getNeighbors().contains(interior_0_0));
	EXPECT_TRUE(exterior_0_0->getNeighbors().contains(null_0));

	EXPECT_TRUE(null_0->getNeighbors().contains(interior_0_0));
	EXPECT_TRUE(null_0->getNeighbors().contains(exterior_0_0));
}

TEST(Face_Cuts, Before_Overlapping_Cut) {

	//setup
	DCEL<Pint> space;

	//results
	FLL<Region<Pint> *> interior;
	FLL<Region<Pint> *> exterior;
	Region<Pint> * null;


	//tested operations are performed within this block
	{
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

		auto product = space.region(boundary_big);

		auto null_face = (*product)[0]->getRoot()->getInv()->getFace();

		null = space.region(null_face);

		subAllocate(product, boundary_small_a, exterior, interior);

	}

	//testing

	EXPECT_EQ(space.pointCount(), 12);
	EXPECT_EQ(space.edgeCount(), 26);
	EXPECT_EQ(space.faceCount(), 3);
	EXPECT_EQ(space.regionCount(), 3);

	ASSERT_EQ(interior.size(), 1);
	ASSERT_EQ(exterior.size(), 1);

	ASSERT_NE(interior[0], nullptr);
	ASSERT_NE(exterior[0], nullptr);

	Region<Pint> * interior_0 = interior[0];
	Region<Pint> * exterior_0 = exterior[0];

	ASSERT_NE(interior_0, nullptr);
	ASSERT_NE(exterior_0, nullptr);

	ASSERT_EQ(interior_0->size(), 1);
	ASSERT_EQ(exterior_0->size(), 1);
	ASSERT_EQ(null->size(), 1);

	Face<Pint> * interior_0_0 = (*interior_0)[0];
	Face<Pint> * exterior_0_0 = (*exterior_0)[0];
	Face<Pint> * null_0 = (*null)[0];

	ASSERT_NE(interior_0_0, nullptr);
	ASSERT_NE(exterior_0_0, nullptr);
	ASSERT_NE(null_0, nullptr);

	EXPECT_EQ(interior_0_0->getLoopSize(), 5);
	EXPECT_EQ(exterior_0_0->getLoopSize(), 12);
	EXPECT_EQ(null_0->getLoopSize(), 9);

	auto area_in_0_0 = Pint::area(interior_0_0->getLoopPoints());
	auto area_ex_0_0 = Pint::area(exterior_0_0->getLoopPoints());
	auto area_null_0 = Pint::area(null_0->getLoopPoints());

	EXPECT_TRUE(area_in_0_0 == rto(4));
	EXPECT_TRUE(area_ex_0_0 == rto(392));
	EXPECT_TRUE(area_null_0 == rto(-396));

	EXPECT_TRUE(interior_0_0->getNeighbors().contains(exterior_0_0));
	EXPECT_TRUE(interior_0_0->getNeighbors().contains(null_0));

	EXPECT_TRUE(exterior_0_0->getNeighbors().contains(interior_0_0));
	EXPECT_TRUE(exterior_0_0->getNeighbors().contains(null_0));

	EXPECT_TRUE(null_0->getNeighbors().contains(interior_0_0));
	EXPECT_TRUE(null_0->getNeighbors().contains(exterior_0_0));
}


TEST(Face_Cuts, After_Overlapping_Cut) {

	//setup
	DCEL<Pint> space;

	//results
	FLL<Region<Pint> *> interior;
	FLL<Region<Pint> *> exterior;
	Region<Pint> * null;

	//tested operations are performed within this block
	{
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

		auto product = space.region(boundary_big);

		auto null_face = (*product)[0]->getRoot()->getInv()->getFace();

		null = space.region(null_face);

		subAllocate(product, boundary_small_a, exterior, interior);
	}

	//testing

	EXPECT_EQ(space.pointCount(), 12);
	EXPECT_EQ(space.edgeCount(), 26);
	EXPECT_EQ(space.faceCount(), 3);
	EXPECT_EQ(space.regionCount(), 3);

	ASSERT_EQ(interior.size(), 1);
	ASSERT_EQ(exterior.size(), 1);

	ASSERT_NE(interior[0], nullptr);
	ASSERT_NE(exterior[0], nullptr);

	Region<Pint> * interior_0 = interior[0];
	Region<Pint> * exterior_0 = exterior[0];

	ASSERT_NE(interior_0, nullptr);
	ASSERT_NE(exterior_0, nullptr);

	ASSERT_EQ(interior_0->size(), 1);
	ASSERT_EQ(exterior_0->size(), 1);
	ASSERT_EQ(null->size(), 1);

	Face<Pint> * interior_0_0 = (*interior_0)[0];
	Face<Pint> * exterior_0_0 = (*exterior_0)[0];
	Face<Pint> * null_0 = (*null)[0];

	ASSERT_NE(interior_0_0, nullptr);
	ASSERT_NE(exterior_0_0, nullptr);
	ASSERT_NE(null_0, nullptr);

	EXPECT_EQ(interior_0_0->getLoopSize(), 5);
	EXPECT_EQ(exterior_0_0->getLoopSize(), 12);
	EXPECT_EQ(null_0->getLoopSize(), 9);

	auto area_in_0_0 = Pint::area(interior_0_0->getLoopPoints());
	auto area_ex_0_0 = Pint::area(exterior_0_0->getLoopPoints());
	auto area_null_0 = Pint::area(null_0->getLoopPoints());

	EXPECT_TRUE(area_in_0_0 == rto(4));
	EXPECT_TRUE(area_ex_0_0 == rto(392));
	EXPECT_TRUE(area_null_0 == rto(-396));

	EXPECT_TRUE(interior_0_0->getNeighbors().contains(exterior_0_0));
	EXPECT_TRUE(interior_0_0->getNeighbors().contains(null_0));

	EXPECT_TRUE(exterior_0_0->getNeighbors().contains(interior_0_0));
	EXPECT_TRUE(exterior_0_0->getNeighbors().contains(null_0));

	EXPECT_TRUE(null_0->getNeighbors().contains(interior_0_0));
	EXPECT_TRUE(null_0->getNeighbors().contains(exterior_0_0));
}


TEST(Face_Cuts, Overlapping_Cut) {

	//setup
	DCEL<Pint> space;

	//results
	FLL<Region<Pint> *> interior;
	FLL<Region<Pint> *> exterior;
	Region<Pint> * null;

	{
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

		auto product = space.region(boundary_big);

		auto null_face = (*product)[0]->getRoot()->getInv()->getFace();

		null = space.region(null_face);

		subAllocate(product, boundary_small_a, exterior, interior);
	}

	//testing

	EXPECT_EQ(space.pointCount(), 12);
	EXPECT_EQ(space.edgeCount(), 26);
	EXPECT_EQ(space.faceCount(), 3);
	EXPECT_EQ(space.regionCount(), 3);

	ASSERT_EQ(interior.size(), 1);
	ASSERT_EQ(exterior.size(), 1);

	ASSERT_NE(interior[0], nullptr);
	ASSERT_NE(exterior[0], nullptr);

	Region<Pint> * interior_0 = interior[0];
	Region<Pint> * exterior_0 = exterior[0];

	ASSERT_NE(interior_0, nullptr);
	ASSERT_NE(exterior_0, nullptr);

	ASSERT_EQ(interior_0->size(), 1);
	ASSERT_EQ(exterior_0->size(), 1);
	ASSERT_EQ(null->size(), 1);

	Face<Pint> * interior_0_0 = (*interior_0)[0];
	Face<Pint> * exterior_0_0 = (*exterior_0)[0];
	Face<Pint> * null_0 = (*null)[0];

	ASSERT_NE(interior_0_0, nullptr);
	ASSERT_NE(exterior_0_0, nullptr);
	ASSERT_NE(null_0, nullptr);

	EXPECT_EQ(interior_0_0->getLoopSize(), 6);
	EXPECT_EQ(exterior_0_0->getLoopSize(), 12);
	EXPECT_EQ(null_0->getLoopSize(), 8);

	auto area_in_0_0 = Pint::area(interior_0_0->getLoopPoints());
	auto area_ex_0_0 = Pint::area(exterior_0_0->getLoopPoints());
	auto area_null_0 = Pint::area(null_0->getLoopPoints());

	EXPECT_TRUE(area_in_0_0 == rto(8));
	EXPECT_TRUE(area_ex_0_0 == rto(388));
	EXPECT_TRUE(area_null_0 == rto(-396));

	EXPECT_TRUE(interior_0_0->getNeighbors().contains(exterior_0_0));
	EXPECT_TRUE(interior_0_0->getNeighbors().contains(null_0));

	EXPECT_TRUE(exterior_0_0->getNeighbors().contains(interior_0_0));
	EXPECT_TRUE(exterior_0_0->getNeighbors().contains(null_0));

	EXPECT_TRUE(null_0->getNeighbors().contains(interior_0_0));
	EXPECT_TRUE(null_0->getNeighbors().contains(exterior_0_0));
}


TEST(Face_Cuts, Stacked_Holes) {

	//setup
	DCEL<Pint> space;

	//results
	FLL<Region<Pint> *> interior;
	FLL<Region<Pint> *> exterior;
	Region<Pint> * null;

	//tested operations are performed within this block
	{
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

		FLL<Region<Pint> *> exterior_med;

		auto product = space.region(boundary_big);

		auto null_face = (*product)[0]->getRoot()->getInv()->getFace();

		null = space.region(null_face);

		subAllocate(product, boundary_small_a, exterior_med, interior);

		subAllocate(exterior_med[0], boundary_small_b, exterior, interior);
	}
	
	//testing

	EXPECT_EQ(space.pointCount(), 12);
	EXPECT_EQ(space.edgeCount(), 28);
	EXPECT_EQ(space.faceCount(), 4);
	EXPECT_EQ(space.regionCount(), 4);

	ASSERT_EQ(interior.size(), 2);
	ASSERT_EQ(exterior.size(), 1);

	ASSERT_NE(interior[0], nullptr);
	ASSERT_NE(interior[1], nullptr);
	ASSERT_NE(exterior[0], nullptr);

	Region<Pint> * interior_0 = interior[0];
	Region<Pint> * interior_1 = interior[1];
	Region<Pint> * exterior_0 = exterior[0];

	ASSERT_NE(interior_0, nullptr);
	ASSERT_NE(interior_1, nullptr);
	ASSERT_NE(exterior_0, nullptr);

	ASSERT_EQ(interior_0->size(), 1);
	ASSERT_EQ(interior_1->size(), 1);
	ASSERT_EQ(exterior_0->size(), 1);
	ASSERT_EQ(null->size(), 1);

	Face<Pint> * interior_0_0 = (*interior_0)[0];
	Face<Pint> * interior_1_0 = (*interior_1)[0];
	Face<Pint> * exterior_0_0 = (*exterior_0)[0];
	Face<Pint> * null_0 = (*null)[0];

	ASSERT_NE(interior_0_0, nullptr);
	ASSERT_NE(interior_1_0, nullptr);
	ASSERT_NE(exterior_0_0, nullptr);
	ASSERT_NE(null_0, nullptr);

	EXPECT_EQ(interior_0_0->getLoopSize(), 4);
	EXPECT_EQ(interior_1_0->getLoopSize(), 6);
	EXPECT_EQ(exterior_0_0->getLoopSize(), 12);
	EXPECT_EQ(null_0->getLoopSize(), 6);

	auto area_in_0_0 = Pint::area(interior_0_0->getLoopPoints());
	auto area_in_1_0 = Pint::area(interior_1_0->getLoopPoints());
	auto area_ex_0_0 = Pint::area(exterior_0_0->getLoopPoints());
	auto area_null_0 = Pint::area(null_0->getLoopPoints());

	EXPECT_TRUE(area_in_0_0 == rto(24));
	EXPECT_TRUE(area_in_1_0 == rto(32));
	EXPECT_TRUE(area_ex_0_0 == rto(344));
	EXPECT_TRUE(area_null_0 == rto(-400));

	EXPECT_TRUE(interior_0_0->getNeighbors().contains(interior_1_0));
	EXPECT_TRUE(interior_0_0->getNeighbors().contains(exterior_0_0));
	EXPECT_TRUE(interior_0_0->getNeighbors().contains(null_0));

	EXPECT_TRUE(interior_1_0->getNeighbors().contains(interior_0_0));
	EXPECT_TRUE(interior_1_0->getNeighbors().contains(exterior_0_0));
	EXPECT_FALSE(interior_1_0->getNeighbors().contains(null_0));

	EXPECT_TRUE(exterior_0_0->getNeighbors().contains(interior_0_0));
	EXPECT_TRUE(exterior_0_0->getNeighbors().contains(interior_1_0));
	EXPECT_TRUE(exterior_0_0->getNeighbors().contains(null_0));

	EXPECT_TRUE(null_0->getNeighbors().contains(interior_0_0));
	EXPECT_FALSE(null_0->getNeighbors().contains(interior_1_0));
	EXPECT_TRUE(null_0->getNeighbors().contains(exterior_0_0));
}


TEST(Face_Cuts, Horshoe_Cut) {

	//setup
	DCEL<Pint> space;

	//results
	FLL<Region<Pint> *> interior;
	FLL<Region<Pint> *> exterior;
	Region<Pint> * null;

	//tested operations are performed within this block
	{
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

		auto product = space.region(boundary_big);

		auto null_face = (*product)[0]->getRoot()->getInv()->getFace();

		null = space.region(null_face);

		subAllocate(product, boundary_small_a, exterior, interior);
	}

	//testing 

	EXPECT_EQ(space.pointCount(), 12);
	EXPECT_EQ(space.edgeCount(), 28);
	EXPECT_EQ(space.faceCount(), 4);
	EXPECT_EQ(space.regionCount(), 4);

	ASSERT_EQ(interior.size(), 2);
	ASSERT_EQ(exterior.size(), 1);

	ASSERT_NE(interior[0], nullptr);
	ASSERT_NE(interior[1], nullptr);
	ASSERT_NE(exterior[0], nullptr);

	Region<Pint> * interior_0 = interior[0];
	Region<Pint> * interior_1 = interior[1];
	Region<Pint> * exterior_0 = exterior[0];

	ASSERT_NE(interior_0, nullptr);
	ASSERT_NE(interior_1, nullptr);
	ASSERT_NE(exterior_0, nullptr);

	ASSERT_EQ(interior_0->size(), 1);
	ASSERT_EQ(interior_1->size(), 1);
	ASSERT_EQ(exterior_0->size(), 1);
	ASSERT_EQ(null->size(), 1);

	Face<Pint> * interior_0_0 = (*interior_0)[0];
	Face<Pint> * interior_1_0 = (*interior_1)[0];
	Face<Pint> * exterior_0_0 = (*exterior_0)[0];
	Face<Pint> * null_0 = (*null)[0];

	ASSERT_NE(interior_0_0, nullptr);
	ASSERT_NE(interior_1_0, nullptr);
	ASSERT_NE(exterior_0_0, nullptr);
	ASSERT_NE(null_0, nullptr);

	EXPECT_EQ(interior_0_0->getLoopSize(), 4);
	EXPECT_EQ(interior_1_0->getLoopSize(), 4);
	EXPECT_EQ(exterior_0_0->getLoopSize(), 12);
	EXPECT_EQ(null_0->getLoopSize(), 8);

	auto area_in_0_0 = Pint::area(interior_0_0->getLoopPoints());
	auto area_in_1_0 = Pint::area(interior_1_0->getLoopPoints());
	auto area_ex_0_0 = Pint::area(exterior_0_0->getLoopPoints());
	auto area_null_0 = Pint::area(null_0->getLoopPoints());

	EXPECT_TRUE(area_in_0_0 == rto(16));
	EXPECT_TRUE(area_in_1_0 == rto(16));
	EXPECT_TRUE(area_ex_0_0 == rto(368));
	EXPECT_TRUE(area_null_0 == rto(-400));

	EXPECT_TRUE(interior_0_0->getNeighbors().contains(exterior_0_0));
	EXPECT_FALSE(interior_0_0->getNeighbors().contains(interior_1_0));
	EXPECT_TRUE(interior_0_0->getNeighbors().contains(null_0));

	EXPECT_FALSE(interior_1_0->getNeighbors().contains(interior_0_0));
	EXPECT_TRUE(interior_1_0->getNeighbors().contains(exterior_0_0));
	EXPECT_TRUE(interior_1_0->getNeighbors().contains(null_0));

	EXPECT_TRUE(exterior_0_0->getNeighbors().contains(interior_0_0));
	EXPECT_TRUE(exterior_0_0->getNeighbors().contains(interior_1_0));
	EXPECT_TRUE(exterior_0_0->getNeighbors().contains(null_0));

	EXPECT_TRUE(null_0->getNeighbors().contains(interior_0_0));
	EXPECT_TRUE(null_0->getNeighbors().contains(interior_1_0));
	EXPECT_TRUE(null_0->getNeighbors().contains(exterior_0_0));
}

TEST(Face_Cuts, Corner_Meeting_Cut) {
	
	//setup
	DCEL<Pint> space;

	//results
	FLL<Region<Pint> *> interior;
	FLL<Region<Pint> *> exterior;
	Region<Pint> * null;

	//tested operations are performed within this block

	{
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

		auto product = space.region(boundary_big);

		auto null_face = (*product)[0]->getRoot()->getInv()->getFace();

		null = space.region(null_face);

		subAllocate(product, boundary_small_a, exterior, interior);
	}

	//testing

	EXPECT_EQ(space.pointCount(), 7);
	EXPECT_EQ(space.edgeCount(), 16);
	EXPECT_EQ(space.faceCount(), 3);
	EXPECT_EQ(space.regionCount(), 3);

	ASSERT_EQ(interior.size(), 1);
	ASSERT_EQ(exterior.size(), 1);

	ASSERT_NE(interior[0], nullptr);
	ASSERT_NE(exterior[0], nullptr);

	Region<Pint> * interior_0 = interior[0];
	Region<Pint> * exterior_0 = exterior[0];

	ASSERT_NE(interior_0, nullptr);
	ASSERT_NE(exterior_0, nullptr);

	ASSERT_EQ(interior_0->size(), 1);
	ASSERT_EQ(exterior_0->size(), 1);
	ASSERT_EQ(null->size(), 1);

	Face<Pint> * interior_0_0 = (*interior_0)[0];
	Face<Pint> * exterior_0_0 = (*exterior_0)[0];
	Face<Pint> * null_0 = (*null)[0];

	ASSERT_NE(interior_0_0, nullptr);
	ASSERT_NE(exterior_0_0, nullptr);
	ASSERT_NE(null_0, nullptr);

	EXPECT_EQ(interior_0_0->getLoopSize(), 4);
	EXPECT_EQ(exterior_0_0->getLoopSize(), 6);
	EXPECT_EQ(null_0->getLoopSize(), 6);

	auto area_in_0_0 = Pint::area(interior_0_0->getLoopPoints());
	auto area_ex_0_0 = Pint::area(exterior_0_0->getLoopPoints());
	auto area_null_0 = Pint::area(null_0->getLoopPoints());

	EXPECT_TRUE(area_in_0_0 == rto(16));
	EXPECT_TRUE(area_ex_0_0 == rto(384));
	EXPECT_TRUE(area_null_0 == rto(-400));

	EXPECT_TRUE(interior_0_0->getNeighbors().contains(exterior_0_0));
	EXPECT_TRUE(interior_0_0->getNeighbors().contains(null_0));

	EXPECT_TRUE(exterior_0_0->getNeighbors().contains(interior_0_0));
	EXPECT_TRUE(exterior_0_0->getNeighbors().contains(null_0));

	EXPECT_TRUE(null_0->getNeighbors().contains(interior_0_0));
	EXPECT_TRUE(null_0->getNeighbors().contains(exterior_0_0));
}

TEST(Face_Cuts, Edge_Crossing_Cut) {
	
	//setup
	DCEL<Pint> space;

	//results
	FLL<Region<Pint> *> interior;
	FLL<Region<Pint> *> exterior;
	Region<Pint> * null;

	//tested operations are performed within this block
	{
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

		auto product = space.region(boundary_big);

		auto null_face = (*product)[0]->getRoot()->getInv()->getFace();

		null = space.region(null_face);

		subAllocate(product, boundary_small_a, exterior, interior);
	}

	//testing

	EXPECT_EQ(space.pointCount(), 8);
	EXPECT_EQ(space.edgeCount(), 18);
	EXPECT_EQ(space.faceCount(), 3);
	EXPECT_EQ(space.regionCount(), 3);

	ASSERT_EQ(interior.size(), 1);
	ASSERT_EQ(exterior.size(), 1);

	ASSERT_NE(interior[0], nullptr);
	ASSERT_NE(exterior[0], nullptr);

	Region<Pint> * interior_0 = interior[0];
	Region<Pint> * exterior_0 = exterior[0];

	ASSERT_NE(interior_0, nullptr);
	ASSERT_NE(exterior_0, nullptr);

	ASSERT_EQ(interior_0->size(), 1);
	ASSERT_EQ(exterior_0->size(), 1);
	ASSERT_EQ(null->size(), 1);

	Face<Pint> * interior_0_0 = (*interior_0)[0];
	Face<Pint> * exterior_0_0 = (*exterior_0)[0];
	Face<Pint> * null_0 = (*null)[0];

	ASSERT_NE(interior_0_0, nullptr);
	ASSERT_NE(exterior_0_0, nullptr);
	ASSERT_NE(null_0, nullptr);

	EXPECT_EQ(interior_0_0->getLoopSize(), 4);
	EXPECT_EQ(exterior_0_0->getLoopSize(), 8);
	EXPECT_EQ(null_0->getLoopSize(), 6);

	auto area_in_0_0 = Pint::area(interior_0_0->getLoopPoints());
	auto area_ex_0_0 = Pint::area(exterior_0_0->getLoopPoints());
	auto area_null_0 = Pint::area(null_0->getLoopPoints());

	EXPECT_TRUE(area_in_0_0 == rto(8));
	EXPECT_TRUE(area_ex_0_0 == rto(392));
	EXPECT_TRUE(area_null_0 == rto(-400));

	EXPECT_TRUE(interior_0_0->getNeighbors().contains(exterior_0_0));
	EXPECT_TRUE(interior_0_0->getNeighbors().contains(null_0));

	EXPECT_TRUE(exterior_0_0->getNeighbors().contains(interior_0_0));
	EXPECT_TRUE(exterior_0_0->getNeighbors().contains(null_0));

	EXPECT_TRUE(null_0->getNeighbors().contains(interior_0_0));
	EXPECT_TRUE(null_0->getNeighbors().contains(exterior_0_0));
}

TEST(Face_Cuts, Corner_Crossing_Cut) {
	
	//setup
	DCEL<Pint> space;

	//results
	FLL<Region<Pint> *> interior;
	FLL<Region<Pint> *> exterior;
	Region<Pint> * null;

	//tested operations are performed within this block
	{
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

		auto product = space.region(boundary_big);

		auto null_face = (*product)[0]->getRoot()->getInv()->getFace();

		null = space.region(null_face);

		subAllocate(product, boundary_small_a, exterior, interior);
	}

	//testing

	EXPECT_EQ(space.pointCount(), 7);
	EXPECT_EQ(space.edgeCount(), 16);
	EXPECT_EQ(space.faceCount(), 3);
	EXPECT_EQ(space.regionCount(), 3);

	ASSERT_EQ(interior.size(), 1);
	ASSERT_EQ(exterior.size(), 1);

	ASSERT_NE(interior[0], nullptr);
	ASSERT_NE(exterior[0], nullptr);

	Region<Pint> * interior_0 = interior[0];
	Region<Pint> * exterior_0 = exterior[0];

	ASSERT_NE(interior_0, nullptr);
	ASSERT_NE(exterior_0, nullptr);

	ASSERT_EQ(interior_0->size(), 1);
	ASSERT_EQ(exterior_0->size(), 1);
	ASSERT_EQ(null->size(), 1);

	Face<Pint> * interior_0_0 = (*interior_0)[0];
	Face<Pint> * exterior_0_0 = (*exterior_0)[0];
	Face<Pint> * null_0 = (*null)[0];

	ASSERT_NE(interior_0_0, nullptr);
	ASSERT_NE(exterior_0_0, nullptr);
	ASSERT_NE(null_0, nullptr);

	EXPECT_EQ(interior_0_0->getLoopSize(), 4);
	EXPECT_EQ(exterior_0_0->getLoopSize(), 6);
	EXPECT_EQ(null_0->getLoopSize(), 6);

	auto area_in_0_0 = Pint::area(interior_0_0->getLoopPoints());
	auto area_ex_0_0 = Pint::area(exterior_0_0->getLoopPoints());
	auto area_null_0 = Pint::area(null_0->getLoopPoints());

	EXPECT_TRUE(area_in_0_0 == rto(16));
	EXPECT_TRUE(area_ex_0_0 == rto(384));
	EXPECT_TRUE(area_null_0 == rto(-400));

	EXPECT_TRUE(interior_0_0->getNeighbors().contains(exterior_0_0));
	EXPECT_TRUE(interior_0_0->getNeighbors().contains(null_0));

	EXPECT_TRUE(exterior_0_0->getNeighbors().contains(interior_0_0));
	EXPECT_TRUE(exterior_0_0->getNeighbors().contains(null_0));

	EXPECT_TRUE(null_0->getNeighbors().contains(interior_0_0));
	EXPECT_TRUE(null_0->getNeighbors().contains(exterior_0_0));
}

TEST(Face_Cuts, Exterior_Cut) {
	
	//setup
	DCEL<Pint> space;

	//results
	FLL<Region<Pint> *> interior;
	FLL<Region<Pint> *> exterior;
	Region<Pint> * null;

	//tested operations are performed within this block
	{
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

		auto product = space.region(boundary_big);

		auto null_face = (*product)[0]->getRoot()->getInv()->getFace();

		null = space.region(null_face);

		subAllocate(product, boundary_small_a, exterior, interior);
	}

	//testing

	EXPECT_EQ(space.pointCount(), 4);
	EXPECT_EQ(space.edgeCount(), 8);
	EXPECT_EQ(space.faceCount(), 2);
	EXPECT_EQ(space.regionCount(), 2);

	ASSERT_EQ(interior.size(), 0);
	ASSERT_EQ(exterior.size(), 1);

}

TEST(Face_Cuts, Encapsulating_Cut) {
	
	//setup
	DCEL<Pint> space;

	//results
	FLL<Region<Pint> *> interior;
	FLL<Region<Pint> *> exterior;
	Region<Pint> * null;

	//tested operations are performed within this block
	{
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

		auto product = space.region(boundary_big);

		auto null_face = (*product)[0]->getRoot()->getInv()->getFace();

		null = space.region(null_face);

		subAllocate(product, boundary_small_a, exterior, interior);
	}

	//testing

	EXPECT_EQ(space.pointCount(), 4);
	EXPECT_EQ(space.edgeCount(), 8);
	EXPECT_EQ(space.faceCount(), 2);
	EXPECT_EQ(space.regionCount(), 2);

	ASSERT_EQ(interior.size(), 1);
	ASSERT_EQ(exterior.size(), 0);

	ASSERT_NE(interior[0], nullptr);

	Region<Pint> * interior_0 = interior[0];

	ASSERT_NE(interior_0, nullptr);

	ASSERT_EQ(interior_0->size(), 1);
	ASSERT_EQ(null->size(), 1);

	Face<Pint> * interior_0_0 = (*interior_0)[0];
	Face<Pint> * null_0 = (*null)[0];

	ASSERT_NE(interior_0_0, nullptr);
	ASSERT_NE(null_0, nullptr);

	EXPECT_EQ(interior_0_0->getLoopSize(), 4);
	EXPECT_EQ(null_0->getLoopSize(), 4);

	auto area_in_0_0 = Pint::area(interior_0_0->getLoopPoints());
	auto area_null_0 = Pint::area(null_0->getLoopPoints());

	EXPECT_TRUE(area_in_0_0 == rto(400));
	EXPECT_TRUE(area_null_0 == rto(-400));

	EXPECT_TRUE(interior_0_0->getNeighbors().contains(null_0));

	EXPECT_TRUE(null_0->getNeighbors().contains(interior_0_0));

}

TEST(Face_Cuts, Seperate_Holes) {
	
	//setup
	DCEL<Pint> space;

	//results
	FLL<Region<Pint> *> interior;
	FLL<Region<Pint> *> exterior;
	Region<Pint> * null;

	//tested operations are performed within this block
	{
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

		FLL<Region<Pint> *> exterior_med;

		auto product = space.region(boundary_big);

		auto null_face = (*product)[0]->getRoot()->getInv()->getFace();

		null = space.region(null_face);

		subAllocate(product, boundary_small_a, exterior_med, interior);

		subAllocate(exterior_med[0], boundary_small_b, exterior, interior);
	}

	//testing

	EXPECT_EQ(space.pointCount(), 12);
	EXPECT_EQ(space.edgeCount(), 24);
	EXPECT_EQ(space.faceCount(), 6);
	EXPECT_EQ(space.regionCount(), 4);

	ASSERT_EQ(interior.size(), 2);
	ASSERT_EQ(exterior.size(), 1);

	ASSERT_NE(interior[0], nullptr);
	ASSERT_NE(interior[1], nullptr);
	ASSERT_NE(exterior[0], nullptr);

	Region<Pint> * interior_0 = interior[0];
	Region<Pint> * interior_1 = interior[1];
	Region<Pint> * exterior_0 = exterior[0];

	ASSERT_NE(interior_0, nullptr);
	ASSERT_NE(interior_1, nullptr);
	ASSERT_NE(exterior_0, nullptr);

	ASSERT_EQ(interior_0->size(), 1);
	ASSERT_EQ(interior_1->size(), 1);
	ASSERT_EQ(exterior_0->size(), 3);
	ASSERT_EQ(null->size(), 1);

	Face<Pint> * interior_0_0 = (*interior_0)[0];
	Face<Pint> * interior_1_0 = (*interior_1)[0];
	Face<Pint> * exterior_0_0 = (*exterior_0)[0];
	Face<Pint> * exterior_0_1 = (*exterior_0)[1];
	Face<Pint> * exterior_0_2 = (*exterior_0)[2];
	Face<Pint> * null_0 = (*null)[0];

	ASSERT_NE(interior_0_0, nullptr);
	ASSERT_NE(interior_1_0, nullptr);
	ASSERT_NE(exterior_0_0, nullptr);
	ASSERT_NE(exterior_0_1, nullptr);
	ASSERT_NE(exterior_0_2, nullptr);
	ASSERT_NE(null_0, nullptr);

	EXPECT_EQ(interior_0_0->getLoopSize(), 4);
	EXPECT_EQ(interior_1_0->getLoopSize(), 4);
	EXPECT_EQ(exterior_0_0->getLoopSize(), 4);
	EXPECT_EQ(exterior_0_1->getLoopSize(), 4);
	EXPECT_EQ(exterior_0_2->getLoopSize(), 4);
	EXPECT_EQ(null_0->getLoopSize(), 4);

	auto area_in_0_0 = Pint::area(interior_0_0->getLoopPoints());
	auto area_in_1_0 = Pint::area(interior_1_0->getLoopPoints());
	auto area_ex_0_0 = Pint::area(exterior_0_0->getLoopPoints());
	auto area_ex_0_1 = Pint::area(exterior_0_1->getLoopPoints());
	auto area_ex_0_2 = Pint::area(exterior_0_2->getLoopPoints());
	auto area_null_0 = Pint::area(null_0->getLoopPoints());

	EXPECT_TRUE(area_in_0_0 == rto(4));
	EXPECT_TRUE(area_in_1_0 == rto(4));
	EXPECT_TRUE(area_ex_0_0 == rto(-4));
	EXPECT_TRUE(area_ex_0_1 == rto(400));
	EXPECT_TRUE(area_ex_0_2 == rto(-4));
	EXPECT_TRUE(area_null_0 == rto(-400));

	EXPECT_FALSE(interior_0_0->getNeighbors().contains(interior_1_0));
	EXPECT_TRUE(interior_0_0->getNeighbors().contains(exterior_0_0));
	EXPECT_FALSE(interior_0_0->getNeighbors().contains(exterior_0_1));
	EXPECT_FALSE(interior_0_0->getNeighbors().contains(exterior_0_2));
	EXPECT_FALSE(interior_0_0->getNeighbors().contains(null_0));

	EXPECT_FALSE(interior_1_0->getNeighbors().contains(interior_0_0));
	EXPECT_FALSE(interior_1_0->getNeighbors().contains(exterior_0_0));
	EXPECT_FALSE(interior_1_0->getNeighbors().contains(exterior_0_1));
	EXPECT_TRUE(interior_1_0->getNeighbors().contains(exterior_0_2));
	EXPECT_FALSE(interior_1_0->getNeighbors().contains(null_0));

	EXPECT_TRUE(exterior_0_0->getNeighbors().contains(interior_0_0));
	EXPECT_FALSE(exterior_0_0->getNeighbors().contains(interior_1_0));
	EXPECT_FALSE(exterior_0_0->getNeighbors().contains(exterior_0_1));
	EXPECT_FALSE(exterior_0_0->getNeighbors().contains(exterior_0_2));
	EXPECT_FALSE(exterior_0_0->getNeighbors().contains(null_0));

	EXPECT_FALSE(exterior_0_1->getNeighbors().contains(interior_0_0));
	EXPECT_FALSE(exterior_0_1->getNeighbors().contains(interior_1_0));
	EXPECT_FALSE(exterior_0_1->getNeighbors().contains(exterior_0_0));
	EXPECT_FALSE(exterior_0_1->getNeighbors().contains(exterior_0_2));
	EXPECT_TRUE(exterior_0_1->getNeighbors().contains(null_0));

	EXPECT_FALSE(exterior_0_2->getNeighbors().contains(interior_0_0));
	EXPECT_TRUE(exterior_0_2->getNeighbors().contains(interior_1_0));
	EXPECT_FALSE(exterior_0_2->getNeighbors().contains(exterior_0_0));
	EXPECT_FALSE(exterior_0_2->getNeighbors().contains(exterior_0_1));
	EXPECT_FALSE(exterior_0_2->getNeighbors().contains(null_0));

	EXPECT_FALSE(null_0->getNeighbors().contains(interior_0_0));
	EXPECT_FALSE(null_0->getNeighbors().contains(interior_1_0));
	EXPECT_FALSE(null_0->getNeighbors().contains(exterior_0_0));
	EXPECT_TRUE(null_0->getNeighbors().contains(exterior_0_1));
	EXPECT_FALSE(null_0->getNeighbors().contains(exterior_0_2));
}

TEST(Face_Cuts, Adjacent_Meeting_Holes) {
	
	//setup
	DCEL<Pint> space;

	//results
	FLL<Region<Pint> *> interior;
	FLL<Region<Pint> *> exterior;
	Region<Pint> * null;

	//tested operations are performed within this block
	{
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

		FLL<Region<Pint> *> exterior_med;

		auto product = space.region(boundary_big);

		auto null_face = (*product)[0]->getRoot()->getInv()->getFace();

		null = space.region(null_face);

		subAllocate(product, boundary_small_a, exterior_med, interior);

		subAllocate(exterior_med[0], boundary_small_b, exterior, interior);
	}

	//testing

	EXPECT_EQ(space.pointCount(), 12);
	EXPECT_EQ(space.edgeCount(), 26);
	EXPECT_EQ(space.faceCount(), 5);
	EXPECT_EQ(space.regionCount(), 4);

	ASSERT_EQ(interior.size(), 2);
	ASSERT_EQ(exterior.size(), 1);

	ASSERT_NE(interior[0], nullptr);
	ASSERT_NE(interior[1], nullptr);
	ASSERT_NE(exterior[0], nullptr);

	Region<Pint> * interior_0 = interior[0];
	Region<Pint> * interior_1 = interior[1];
	Region<Pint> * exterior_0 = exterior[0];

	ASSERT_NE(interior_0, nullptr);
	ASSERT_NE(interior_1, nullptr);
	ASSERT_NE(exterior_0, nullptr);

	ASSERT_EQ(interior_0->size(), 1);
	ASSERT_EQ(interior_1->size(), 1);
	ASSERT_EQ(exterior_0->size(), 2);
	ASSERT_EQ(null->size(), 1);

	Face<Pint> * interior_0_0 = (*interior_0)[0];
	Face<Pint> * interior_1_0 = (*interior_1)[0];
	Face<Pint> * exterior_0_0 = (*exterior_0)[0];
	Face<Pint> * exterior_0_1 = (*exterior_0)[1];
	Face<Pint> * null_0 = (*null)[0];

	ASSERT_NE(interior_0_0, nullptr);
	ASSERT_NE(interior_1_0, nullptr);
	ASSERT_NE(exterior_0_0, nullptr);
	ASSERT_NE(null_0, nullptr);

	EXPECT_EQ(interior_0_0->getLoopSize(), 4);
	EXPECT_EQ(interior_1_0->getLoopSize(), 6);
	EXPECT_EQ(exterior_0_0->getLoopSize(), 8);
	EXPECT_EQ(exterior_0_1->getLoopSize(), 4);
	EXPECT_EQ(null_0->getLoopSize(), 4);

	auto area_in_0_0 = Pint::area(interior_0_0->getLoopPoints());
	auto area_in_1_0 = Pint::area(interior_1_0->getLoopPoints());
	auto area_ex_0_0 = Pint::area(exterior_0_0->getLoopPoints());
	auto area_ex_0_1 = Pint::area(exterior_0_1->getLoopPoints());
	auto area_null_0 = Pint::area(null_0->getLoopPoints());

	EXPECT_TRUE(area_in_0_0 == rto(24));
	EXPECT_TRUE(area_in_1_0 == rto(32));
	EXPECT_TRUE(area_ex_0_0 == rto(-56));
	EXPECT_TRUE(area_ex_0_1 == rto(400));
	EXPECT_TRUE(area_null_0 == rto(-400));

	EXPECT_TRUE(interior_0_0->getNeighbors().contains(interior_1_0));
	EXPECT_TRUE(interior_0_0->getNeighbors().contains(exterior_0_0));
	EXPECT_FALSE(interior_0_0->getNeighbors().contains(exterior_0_1));
	EXPECT_FALSE(interior_0_0->getNeighbors().contains(null_0));

	EXPECT_TRUE(interior_1_0->getNeighbors().contains(interior_0_0));
	EXPECT_TRUE(interior_1_0->getNeighbors().contains(exterior_0_0));
	EXPECT_FALSE(interior_1_0->getNeighbors().contains(exterior_0_1));
	EXPECT_FALSE(interior_1_0->getNeighbors().contains(null_0));

	EXPECT_TRUE(exterior_0_0->getNeighbors().contains(interior_0_0));
	EXPECT_TRUE(exterior_0_0->getNeighbors().contains(interior_1_0));
	EXPECT_FALSE(exterior_0_0->getNeighbors().contains(exterior_0_1));
	EXPECT_FALSE(exterior_0_0->getNeighbors().contains(null_0));

	EXPECT_FALSE(exterior_0_1->getNeighbors().contains(interior_0_0));
	EXPECT_FALSE(exterior_0_1->getNeighbors().contains(interior_1_0));
	EXPECT_FALSE(exterior_0_1->getNeighbors().contains(exterior_0_0));
	EXPECT_TRUE(exterior_0_1->getNeighbors().contains(null_0));

	EXPECT_FALSE(null_0->getNeighbors().contains(interior_0_0));
	EXPECT_FALSE(null_0->getNeighbors().contains(interior_1_0));
	EXPECT_FALSE(null_0->getNeighbors().contains(exterior_0_0));
	EXPECT_TRUE(null_0->getNeighbors().contains(exterior_0_1));
}

TEST(Face_Cuts, Adjacent_Crossing_Holes) {
	
	//setup
	DCEL<Pint> space;

	//results
	FLL<Region<Pint> *> interior;
	FLL<Region<Pint> *> exterior;
	Region<Pint> * null;

	//tested operations are performed within this block
	{
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

		FLL<Region<Pint> *> exterior_med;

		auto product = space.region(boundary_big);

		auto null_face = (*product)[0]->getRoot()->getInv()->getFace();

		null = space.region(null_face);

		subAllocate(product, boundary_small_a, exterior_med, interior);

		subAllocate(exterior_med[0], boundary_small_b, exterior, interior);
	}

	EXPECT_EQ(space.pointCount(), 12);
	EXPECT_EQ(space.edgeCount(), 26);
	EXPECT_EQ(space.faceCount(), 5);
	EXPECT_EQ(space.regionCount(), 4);

	ASSERT_EQ(interior.size(), 2);
	ASSERT_EQ(exterior.size(), 1);

	ASSERT_NE(interior[0], nullptr);
	ASSERT_NE(interior[1], nullptr);
	ASSERT_NE(exterior[0], nullptr);

	Region<Pint> * interior_0 = interior[0];
	Region<Pint> * interior_1 = interior[1];
	Region<Pint> * exterior_0 = exterior[0];

	ASSERT_NE(interior_0, nullptr);
	ASSERT_NE(interior_1, nullptr);
	ASSERT_NE(exterior_0, nullptr);

	ASSERT_EQ(interior_0->size(), 1);
	ASSERT_EQ(interior_1->size(), 1);
	ASSERT_EQ(exterior_0->size(), 2);
	ASSERT_EQ(null->size(), 1);

	Face<Pint> * interior_0_0 = (*interior_0)[0];
	Face<Pint> * interior_1_0 = (*interior_1)[0];
	Face<Pint> * exterior_0_0 = (*exterior_0)[0];
	Face<Pint> * exterior_0_1 = (*exterior_0)[1];
	Face<Pint> * null_0 = (*null)[0];

	ASSERT_NE(interior_0_0, nullptr);
	ASSERT_NE(interior_1_0, nullptr);
	ASSERT_NE(exterior_0_0, nullptr);
	ASSERT_NE(null_0, nullptr);

	EXPECT_EQ(interior_0_0->getLoopSize(), 4);
	EXPECT_EQ(interior_1_0->getLoopSize(), 6);
	EXPECT_EQ(exterior_0_0->getLoopSize(), 8);
	EXPECT_EQ(exterior_0_1->getLoopSize(), 4);
	EXPECT_EQ(null_0->getLoopSize(), 4);

	auto area_in_0_0 = Pint::area(interior_0_0->getLoopPoints());
	auto area_in_1_0 = Pint::area(interior_1_0->getLoopPoints());
	auto area_ex_0_0 = Pint::area(exterior_0_0->getLoopPoints());
	auto area_ex_0_1 = Pint::area(exterior_0_1->getLoopPoints());
	auto area_null_0 = Pint::area(null_0->getLoopPoints());

	EXPECT_TRUE(area_in_0_0 == rto(24));
	EXPECT_TRUE(area_in_1_0 == rto(32));
	EXPECT_TRUE(area_ex_0_0 == rto(-56));
	EXPECT_TRUE(area_ex_0_1 == rto(400));
	EXPECT_TRUE(area_null_0 == rto(-400));

	EXPECT_TRUE(interior_0_0->getNeighbors().contains(interior_1_0));
	EXPECT_TRUE(interior_0_0->getNeighbors().contains(exterior_0_0));
	EXPECT_FALSE(interior_0_0->getNeighbors().contains(exterior_0_1));
	EXPECT_FALSE(interior_0_0->getNeighbors().contains(null_0));

	EXPECT_TRUE(interior_1_0->getNeighbors().contains(interior_0_0));
	EXPECT_TRUE(interior_1_0->getNeighbors().contains(exterior_0_0));
	EXPECT_FALSE(interior_1_0->getNeighbors().contains(exterior_0_1));
	EXPECT_FALSE(interior_1_0->getNeighbors().contains(null_0));

	EXPECT_TRUE(exterior_0_0->getNeighbors().contains(interior_0_0));
	EXPECT_TRUE(exterior_0_0->getNeighbors().contains(interior_1_0));
	EXPECT_FALSE(exterior_0_0->getNeighbors().contains(exterior_0_1));
	EXPECT_FALSE(exterior_0_0->getNeighbors().contains(null_0));

	EXPECT_FALSE(exterior_0_1->getNeighbors().contains(interior_0_0));
	EXPECT_FALSE(exterior_0_1->getNeighbors().contains(interior_1_0));
	EXPECT_FALSE(exterior_0_1->getNeighbors().contains(exterior_0_0));
	EXPECT_TRUE(exterior_0_1->getNeighbors().contains(null_0));

	EXPECT_FALSE(null_0->getNeighbors().contains(interior_0_0));
	EXPECT_FALSE(null_0->getNeighbors().contains(interior_1_0));
	EXPECT_FALSE(null_0->getNeighbors().contains(exterior_0_0));
	EXPECT_TRUE(null_0->getNeighbors().contains(exterior_0_1));
}

TEST(Face_Cuts, Connecting_Holes) {
	
	//setup
	DCEL<Pint> space;

	//results
	FLL<Region<Pint> *> interior;
	FLL<Region<Pint> *> exterior;
	Region<Pint> * null;

	//tested operations are performed within this block
	{
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

		FLL<Region<Pint> *> exterior_med;

		auto product = space.region(boundary_big);

		auto null_face = (*product)[0]->getRoot()->getInv()->getFace();

		null = space.region(null_face);

		subAllocate(product, boundary_small_a, exterior_med, interior);

		subAllocate(exterior_med[0], boundary_small_b, exterior, interior);
	}

	//testing

	EXPECT_EQ(space.pointCount(), 12);
	EXPECT_EQ(space.edgeCount(), 28);
	EXPECT_EQ(space.faceCount(), 4);
	EXPECT_EQ(space.regionCount(), 4);

	ASSERT_EQ(interior.size(), 2);
	ASSERT_EQ(exterior.size(), 1);

	ASSERT_NE(interior[0], nullptr);
	ASSERT_NE(interior[1], nullptr);
	ASSERT_NE(exterior[0], nullptr);

	Region<Pint> * interior_0 = interior[0];
	Region<Pint> * interior_1 = interior[1];
	Region<Pint> * exterior_0 = exterior[0];

	ASSERT_NE(interior_0, nullptr);
	ASSERT_NE(interior_1, nullptr);
	ASSERT_NE(exterior_0, nullptr);

	ASSERT_EQ(interior_0->size(), 1);
	ASSERT_EQ(interior_1->size(), 1);
	ASSERT_EQ(exterior_0->size(), 1);
	ASSERT_EQ(null->size(), 1);

	Face<Pint> * interior_0_0 = (*interior_0)[0];
	Face<Pint> * interior_1_0 = (*interior_1)[0];
	Face<Pint> * exterior_0_0 = (*exterior_0)[0];
	Face<Pint> * null_0 = (*null)[0];

	ASSERT_NE(interior_0_0, nullptr);
	ASSERT_NE(interior_1_0, nullptr);
	ASSERT_NE(exterior_0_0, nullptr);
	ASSERT_NE(null_0, nullptr);

	EXPECT_EQ(interior_0_0->getLoopSize(), 4);
	EXPECT_EQ(interior_1_0->getLoopSize(), 6);
	EXPECT_EQ(exterior_0_0->getLoopSize(), 12);
	EXPECT_EQ(null_0->getLoopSize(), 6);

	auto area_in_0_0 = Pint::area(interior_0_0->getLoopPoints());
	auto area_in_1_0 = Pint::area(interior_1_0->getLoopPoints());
	auto area_ex_0_0 = Pint::area(exterior_0_0->getLoopPoints());
	auto area_null_0 = Pint::area(null_0->getLoopPoints());

	EXPECT_TRUE(area_in_0_0 == rto(24));
	EXPECT_TRUE(area_in_1_0 == rto(32));
	EXPECT_TRUE(area_ex_0_0 == rto(344));
	EXPECT_TRUE(area_null_0 == rto(-400));

	EXPECT_TRUE(interior_0_0->getNeighbors().contains(exterior_0_0));
	EXPECT_TRUE(interior_0_0->getNeighbors().contains(interior_1_0));
	EXPECT_TRUE(interior_0_0->getNeighbors().contains(null_0));

	EXPECT_TRUE(interior_1_0->getNeighbors().contains(interior_0_0));
	EXPECT_TRUE(interior_1_0->getNeighbors().contains(exterior_0_0));
	EXPECT_FALSE(interior_1_0->getNeighbors().contains(null_0));

	EXPECT_TRUE(exterior_0_0->getNeighbors().contains(interior_0_0));
	EXPECT_TRUE(exterior_0_0->getNeighbors().contains(interior_1_0));
	EXPECT_TRUE(exterior_0_0->getNeighbors().contains(null_0));

	EXPECT_TRUE(null_0->getNeighbors().contains(interior_0_0));
	EXPECT_FALSE(null_0->getNeighbors().contains(interior_1_0));
	EXPECT_TRUE(null_0->getNeighbors().contains(exterior_0_0));
}

TEST(Face_Cuts, Connecting_Several_Holes) {
	
	//setup
	DCEL<Pint> space;

	//results
	FLL<Region<Pint> *> interior;
	FLL<Region<Pint> *> exterior;
	Region<Pint> * null;

	//tested operations are performed within this block
	{
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

		FLL<Region<Pint> *> exterior_med_a;
		FLL<Region<Pint> *> exterior_med_b;

		auto product = space.region(boundary_big);

		auto null_face = (*product)[0]->getRoot()->getInv()->getFace();

		null = space.region(null_face);

		subAllocate(product, boundary_small_a, exterior_med_a, interior);

		subAllocate(exterior_med_a[0], boundary_small_b, exterior_med_b, interior);

		subAllocate(exterior_med_b[0], boundary_small_c, exterior, interior);
	}

	//testing

	EXPECT_EQ(space.pointCount(), 16);
	EXPECT_EQ(space.edgeCount(), 36);
	EXPECT_EQ(space.faceCount(), 6);
	EXPECT_EQ(space.regionCount(), 5);

	ASSERT_EQ(interior.size(), 3);
	ASSERT_EQ(exterior.size(), 1);

	ASSERT_NE(interior[0], nullptr);
	ASSERT_NE(interior[1], nullptr);
	ASSERT_NE(interior[2], nullptr);
	ASSERT_NE(exterior[0], nullptr);

	Region<Pint> * interior_0 = interior[0];
	Region<Pint> * interior_1 = interior[1];
	Region<Pint> * interior_2 = interior[2];
	Region<Pint> * exterior_0 = exterior[0];

	ASSERT_NE(interior_0, nullptr);
	ASSERT_NE(interior_1, nullptr);
	ASSERT_NE(interior_2, nullptr);
	ASSERT_NE(exterior_0, nullptr);

	ASSERT_EQ(interior_0->size(), 1);
	ASSERT_EQ(interior_1->size(), 1);
	ASSERT_EQ(interior_2->size(), 1);
	ASSERT_EQ(exterior_0->size(), 2);
	ASSERT_EQ(null->size(), 1);

	Face<Pint> * interior_0_0 = (*interior_0)[0];
	Face<Pint> * interior_1_0 = (*interior_1)[0];
	Face<Pint> * interior_2_0 = (*interior_2)[0];
	Face<Pint> * exterior_0_0 = (*exterior_0)[0];
	Face<Pint> * exterior_0_1 = (*exterior_0)[1];
	Face<Pint> * null_0 = (*null)[0];

	ASSERT_NE(interior_0_0, nullptr);
	ASSERT_NE(interior_1_0, nullptr);
	ASSERT_NE(interior_2_0, nullptr);
	ASSERT_NE(exterior_0_0, nullptr);
	ASSERT_NE(exterior_0_1, nullptr);
	ASSERT_NE(null_0, nullptr);

	EXPECT_EQ(interior_0_0->getLoopSize(), 4);
	EXPECT_EQ(interior_1_0->getLoopSize(), 6);
	EXPECT_EQ(interior_2_0->getLoopSize(), 6);
	EXPECT_EQ(exterior_0_0->getLoopSize(), 12);
	EXPECT_EQ(exterior_0_1->getLoopSize(), 4);
	EXPECT_EQ(null_0->getLoopSize(), 4);

	auto area_in_0_0 = Pint::area(interior_0_0->getLoopPoints());
	auto area_in_1_0 = Pint::area(interior_1_0->getLoopPoints());
	auto area_in_2_0 = Pint::area(interior_2_0->getLoopPoints());
	auto area_ex_0_0 = Pint::area(exterior_0_0->getLoopPoints());
	auto area_ex_0_1 = Pint::area(exterior_0_1->getLoopPoints());
	auto area_null_0 = Pint::area(null_0->getLoopPoints());

	EXPECT_TRUE(area_in_0_0 == rto(120));
	EXPECT_TRUE(area_in_1_0 == rto(32));
	EXPECT_TRUE(area_in_2_0 == rto(32));
	EXPECT_TRUE(area_ex_0_0 == rto(-184));
	EXPECT_TRUE(area_ex_0_1 == rto(400));
	EXPECT_TRUE(area_null_0 == rto(-400));

	EXPECT_TRUE(interior_0_0->getNeighbors().contains(interior_1_0));
	EXPECT_TRUE(interior_0_0->getNeighbors().contains(interior_2_0));
	EXPECT_TRUE(interior_0_0->getNeighbors().contains(exterior_0_0));
	EXPECT_FALSE(interior_0_0->getNeighbors().contains(exterior_0_1));
	EXPECT_FALSE(interior_0_0->getNeighbors().contains(null_0));

	EXPECT_TRUE(interior_1_0->getNeighbors().contains(interior_0_0));
	EXPECT_FALSE(interior_1_0->getNeighbors().contains(interior_2_0));
	EXPECT_TRUE(interior_1_0->getNeighbors().contains(exterior_0_0));
	EXPECT_FALSE(interior_1_0->getNeighbors().contains(exterior_0_1));
	EXPECT_FALSE(interior_1_0->getNeighbors().contains(null_0));

	EXPECT_TRUE(interior_2_0->getNeighbors().contains(interior_0_0));
	EXPECT_FALSE(interior_2_0->getNeighbors().contains(interior_1_0));
	EXPECT_TRUE(interior_2_0->getNeighbors().contains(exterior_0_0));
	EXPECT_FALSE(interior_2_0->getNeighbors().contains(exterior_0_1));
	EXPECT_FALSE(interior_2_0->getNeighbors().contains(null_0));

	EXPECT_TRUE(exterior_0_0->getNeighbors().contains(interior_0_0));
	EXPECT_TRUE(exterior_0_0->getNeighbors().contains(interior_1_0));
	EXPECT_TRUE(exterior_0_0->getNeighbors().contains(interior_2_0));
	EXPECT_FALSE(exterior_0_0->getNeighbors().contains(exterior_0_1));
	EXPECT_FALSE(exterior_0_0->getNeighbors().contains(null_0));

	EXPECT_FALSE(exterior_0_1->getNeighbors().contains(interior_0_0));
	EXPECT_FALSE(exterior_0_1->getNeighbors().contains(interior_1_0));
	EXPECT_FALSE(exterior_0_1->getNeighbors().contains(interior_2_0));
	EXPECT_FALSE(exterior_0_1->getNeighbors().contains(exterior_0_0));
	EXPECT_TRUE(exterior_0_1->getNeighbors().contains(null_0));

	EXPECT_FALSE(null_0->getNeighbors().contains(interior_0_0));
	EXPECT_FALSE(null_0->getNeighbors().contains(interior_1_0));
	EXPECT_FALSE(null_0->getNeighbors().contains(interior_2_0));
	EXPECT_FALSE(null_0->getNeighbors().contains(exterior_0_0));
	EXPECT_TRUE(null_0->getNeighbors().contains(exterior_0_1));
}

TEST(Face_Cuts, Splitting_Cut) {
	
	//setup
	DCEL<Pint> space;

	//results
	FLL<Region<Pint> *> interior;
	FLL<Region<Pint> *> exterior;
	Region<Pint> * null;

	//tested operations are performed within this block
	{
		FLL<Pint> boundary_big;
		{
			boundary_big.append(Pint(0, 0));
			boundary_big.append(Pint(0, 20));
			boundary_big.append(Pint(20, 20));
			boundary_big.append(Pint(20, 0));
		}
		FLL<Pint> boundary_small_a;
		{
			boundary_small_a.append(Pint(0, 5));
			boundary_small_a.append(Pint(0, 15));
			boundary_small_a.append(Pint(20, 15));
			boundary_small_a.append(Pint(20, 5));
		}

		auto product = space.region(boundary_big);

		auto null_face = (*product)[0]->getRoot()->getInv()->getFace();

		null = space.region(null_face);

		subAllocate(product, boundary_small_a, exterior, interior);
	}

	//testing

	EXPECT_EQ(space.pointCount(), 8);
	EXPECT_EQ(space.edgeCount(), 20);
	EXPECT_EQ(space.faceCount(), 4);
	EXPECT_EQ(space.regionCount(), 4);

	ASSERT_EQ(interior.size(), 1);
	ASSERT_EQ(exterior.size(), 2);

	ASSERT_NE(interior[0], nullptr);
	ASSERT_NE(exterior[0], nullptr);
	ASSERT_NE(exterior[1], nullptr);

	Region<Pint> * interior_0 = interior[0];
	Region<Pint> * exterior_0 = exterior[0];
	Region<Pint> * exterior_1 = exterior[1];

	ASSERT_NE(interior_0, nullptr);
	ASSERT_NE(exterior_0, nullptr);
	ASSERT_NE(exterior_1, nullptr);

	ASSERT_EQ(interior_0->size(), 1);
	ASSERT_EQ(exterior_0->size(), 1);
	ASSERT_EQ(exterior_1->size(), 1);
	ASSERT_EQ(null->size(), 1);

	Face<Pint> * interior_0_0 = (*interior_0)[0];
	Face<Pint> * exterior_0_0 = (*exterior_0)[0];
	Face<Pint> * exterior_1_0 = (*exterior_1)[0];
	Face<Pint> * null_0 = (*null)[0];

	ASSERT_NE(interior_0_0, nullptr);
	ASSERT_NE(exterior_0_0, nullptr);
	ASSERT_NE(exterior_1_0, nullptr);
	ASSERT_NE(null_0, nullptr);

	EXPECT_EQ(interior_0_0->getLoopSize(), 4);
	EXPECT_EQ(exterior_0_0->getLoopSize(), 4);
	EXPECT_EQ(exterior_1_0->getLoopSize(), 4);
	EXPECT_EQ(null_0->getLoopSize(), 8);

	auto area_in_0_0 = Pint::area(interior_0_0->getLoopPoints());
	auto area_ex_0_0 = Pint::area(exterior_0_0->getLoopPoints());
	auto area_ex_1_0 = Pint::area(exterior_1_0->getLoopPoints());
	auto area_null_0 = Pint::area(null_0->getLoopPoints());

	EXPECT_TRUE(area_in_0_0 == rto(200));
	EXPECT_TRUE(area_ex_0_0 == rto(100));
	EXPECT_TRUE(area_ex_1_0 == rto(100));
	EXPECT_TRUE(area_null_0 == rto(-400));

	EXPECT_TRUE(interior_0_0->getNeighbors().contains(exterior_0_0));
	EXPECT_TRUE(interior_0_0->getNeighbors().contains(exterior_1_0));
	EXPECT_TRUE(interior_0_0->getNeighbors().contains(null_0));

	EXPECT_TRUE(exterior_0_0->getNeighbors().contains(interior_0_0));
	EXPECT_FALSE(exterior_0_0->getNeighbors().contains(exterior_1_0));
	EXPECT_TRUE(exterior_0_0->getNeighbors().contains(null_0));

	EXPECT_TRUE(exterior_1_0->getNeighbors().contains(interior_0_0));
	EXPECT_FALSE(exterior_1_0->getNeighbors().contains(exterior_0_0));
	EXPECT_TRUE(exterior_1_0->getNeighbors().contains(null_0));

	EXPECT_TRUE(null_0->getNeighbors().contains(interior_0_0));
	EXPECT_TRUE(null_0->getNeighbors().contains(exterior_0_0));
	EXPECT_TRUE(null_0->getNeighbors().contains(exterior_1_0));
}

TEST(Face_Cuts, Divided_Crossing_Holes) {
	
	//setup
	DCEL<Pint> space;

	//results
	FLL<Region<Pint> *> interior;
	FLL<Region<Pint> *> exterior;
	Region<Pint> * null;

	//tested operations are performed within this block
	{
		FLL<Pint> boundary_big;
		{
			boundary_big.append(Pint(0, 0));
			boundary_big.append(Pint(0, 20));
			boundary_big.append(Pint(20, 20));
			boundary_big.append(Pint(20, 0));
		}
		FLL<Pint> boundary_small_a;
		{
			boundary_small_a.append(Pint(8, 4));
			boundary_small_a.append(Pint(8, 16));
			boundary_small_a.append(Pint(12, 16));
			boundary_small_a.append(Pint(12, 4));
		}
		FLL<Pint> boundary_small_b;
		{
			boundary_small_b.append(Pint(4, 8));
			boundary_small_b.append(Pint(4, 12));
			boundary_small_b.append(Pint(16, 12));
			boundary_small_b.append(Pint(16, 8));
		}

		FLL<Region<Pint> *> exterior_med;

		auto product = space.region(boundary_big);

		auto null_face = (*product)[0]->getRoot()->getInv()->getFace();

		null = space.region(null_face);

		subAllocate(product, boundary_small_a, exterior_med, interior);

		subAllocate(exterior_med[0], boundary_small_b, exterior, interior);
	}

	//testing

	EXPECT_EQ(space.pointCount(), 16);
	EXPECT_EQ(space.edgeCount(), 36);
	EXPECT_EQ(space.faceCount(), 6);
	EXPECT_EQ(space.regionCount(), 5);

	ASSERT_EQ(interior.size(), 3);
	ASSERT_EQ(exterior.size(), 1);

	ASSERT_NE(interior[0], nullptr);
	ASSERT_NE(interior[1], nullptr);
	ASSERT_NE(interior[2], nullptr);
	ASSERT_NE(exterior[0], nullptr);

	Region<Pint> * interior_0 = interior[0];
	Region<Pint> * interior_1 = interior[1];
	Region<Pint> * interior_2 = interior[2];
	Region<Pint> * exterior_0 = exterior[0];

	ASSERT_NE(interior_0, nullptr);
	ASSERT_NE(interior_1, nullptr);
	ASSERT_NE(interior_2, nullptr);
	ASSERT_NE(exterior_0, nullptr);

	ASSERT_EQ(interior_0->size(), 1);
	ASSERT_EQ(interior_1->size(), 1);
	ASSERT_EQ(interior_2->size(), 1);
	ASSERT_EQ(exterior_0->size(), 2);
	ASSERT_EQ(null->size(), 1);

	Face<Pint> * interior_0_0 = (*interior_0)[0];
	Face<Pint> * interior_1_0 = (*interior_1)[0];
	Face<Pint> * interior_2_0 = (*interior_2)[0];
	Face<Pint> * exterior_0_0 = (*exterior_0)[0];
	Face<Pint> * exterior_0_1 = (*exterior_0)[1];
	Face<Pint> * null_0 = (*null)[0];

	ASSERT_NE(interior_0_0, nullptr);
	ASSERT_NE(interior_1_0, nullptr);
	ASSERT_NE(interior_2_0, nullptr);
	ASSERT_NE(exterior_0_0, nullptr);
	ASSERT_NE(exterior_0_1, nullptr);
	ASSERT_NE(null_0, nullptr);

	EXPECT_EQ(interior_0_0->getLoopSize(), 4);
	EXPECT_EQ(interior_1_0->getLoopSize(), 4);
	EXPECT_EQ(interior_2_0->getLoopSize(), 8);
	EXPECT_EQ(exterior_0_0->getLoopSize(), 12);
	EXPECT_EQ(exterior_0_1->getLoopSize(), 4);
	EXPECT_EQ(null_0->getLoopSize(), 4);

	auto area_in_0_0 = Pint::area(interior_0_0->getLoopPoints());
	auto area_in_1_0 = Pint::area(interior_1_0->getLoopPoints());
	auto area_in_2_0 = Pint::area(interior_2_0->getLoopPoints());
	auto area_ex_0_0 = Pint::area(exterior_0_0->getLoopPoints());
	auto area_ex_0_1 = Pint::area(exterior_0_1->getLoopPoints());
	auto area_null_0 = Pint::area(null_0->getLoopPoints());

	EXPECT_TRUE(area_in_0_0 == rto(16));
	EXPECT_TRUE(area_in_1_0 == rto(16));
	EXPECT_TRUE(area_in_2_0 == rto(48));
	EXPECT_TRUE(area_ex_0_0 == rto(-80));
	EXPECT_TRUE(area_ex_0_1 == rto(400));
	EXPECT_TRUE(area_null_0 == rto(-400));

	EXPECT_FALSE(interior_0_0->getNeighbors().contains(interior_1_0));
	EXPECT_TRUE(interior_0_0->getNeighbors().contains(interior_2_0));
	EXPECT_TRUE(interior_0_0->getNeighbors().contains(exterior_0_0));
	EXPECT_FALSE(interior_0_0->getNeighbors().contains(exterior_0_1));
	EXPECT_FALSE(interior_0_0->getNeighbors().contains(null_0));

	EXPECT_FALSE(interior_1_0->getNeighbors().contains(interior_0_0));
	EXPECT_TRUE(interior_1_0->getNeighbors().contains(interior_2_0));
	EXPECT_TRUE(interior_1_0->getNeighbors().contains(exterior_0_0));
	EXPECT_FALSE(interior_1_0->getNeighbors().contains(exterior_0_1));
	EXPECT_FALSE(interior_1_0->getNeighbors().contains(null_0));

	EXPECT_TRUE(interior_2_0->getNeighbors().contains(interior_0_0));
	EXPECT_TRUE(interior_2_0->getNeighbors().contains(interior_1_0));
	EXPECT_TRUE(interior_2_0->getNeighbors().contains(exterior_0_0));
	EXPECT_FALSE(interior_2_0->getNeighbors().contains(exterior_0_1));
	EXPECT_FALSE(interior_2_0->getNeighbors().contains(null_0));

	EXPECT_TRUE(exterior_0_0->getNeighbors().contains(interior_0_0));
	EXPECT_TRUE(exterior_0_0->getNeighbors().contains(interior_1_0));
	EXPECT_TRUE(exterior_0_0->getNeighbors().contains(interior_2_0));
	EXPECT_FALSE(exterior_0_0->getNeighbors().contains(exterior_0_1));
	EXPECT_FALSE(exterior_0_0->getNeighbors().contains(null_0));

	EXPECT_FALSE(exterior_0_1->getNeighbors().contains(interior_0_0));
	EXPECT_FALSE(exterior_0_1->getNeighbors().contains(interior_1_0));
	EXPECT_FALSE(exterior_0_1->getNeighbors().contains(interior_2_0));
	EXPECT_FALSE(exterior_0_1->getNeighbors().contains(exterior_0_0));
	EXPECT_TRUE(exterior_0_1->getNeighbors().contains(null_0));

	EXPECT_FALSE(null_0->getNeighbors().contains(interior_0_0));
	EXPECT_FALSE(null_0->getNeighbors().contains(interior_1_0));
	EXPECT_FALSE(null_0->getNeighbors().contains(interior_2_0));
	EXPECT_FALSE(null_0->getNeighbors().contains(exterior_0_0));
	EXPECT_TRUE(null_0->getNeighbors().contains(exterior_0_1));
}

TEST(Face_Cuts, Triangles_Crossing_Cut) {
	
	//setup
	DCEL<Pint> space;

	//results
	FLL<Region<Pint> *> interior;
	FLL<Region<Pint> *> exterior;
	Region<Pint> * null;

	//tested operations are performed within this block
	{
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

		auto product = space.region(boundary_big);

		auto null_face = (*product)[0]->getRoot()->getInv()->getFace();

		null = space.region(null_face);

		subAllocate(product, boundary_small_a, exterior, interior);
	}

	//testing

	EXPECT_EQ(space.pointCount(), 9);
	EXPECT_EQ(space.edgeCount(), 22);
	EXPECT_EQ(space.faceCount(), 4);
	EXPECT_EQ(space.regionCount(), 4);

	ASSERT_EQ(interior.size(), 2);
	ASSERT_EQ(exterior.size(), 1);

	ASSERT_NE(interior[0], nullptr);
	ASSERT_NE(interior[1], nullptr);
	ASSERT_NE(exterior[0], nullptr);

	Region<Pint> * interior_0 = interior[0];
	Region<Pint> * interior_1 = interior[1];
	Region<Pint> * exterior_0 = exterior[0];

	ASSERT_NE(interior_0, nullptr);
	ASSERT_NE(interior_1, nullptr);
	ASSERT_NE(exterior_0, nullptr);

	ASSERT_EQ(interior_0->size(), 1);
	ASSERT_EQ(interior_1->size(), 1);
	ASSERT_EQ(exterior_0->size(), 1);
	ASSERT_EQ(null->size(), 1);

	Face<Pint> * interior_0_0 = (*interior_0)[0];
	Face<Pint> * interior_1_0 = (*interior_1)[0];
	Face<Pint> * exterior_0_0 = (*exterior_0)[0];
	Face<Pint> * null_0 = (*null)[0];

	ASSERT_NE(interior_0_0, nullptr);
	ASSERT_NE(interior_1_0, nullptr);
	ASSERT_NE(exterior_0_0, nullptr);
	ASSERT_NE(null_0, nullptr);

	EXPECT_EQ(interior_0_0->getLoopSize(), 3);
	EXPECT_EQ(interior_1_0->getLoopSize(), 3);
	EXPECT_EQ(exterior_0_0->getLoopSize(), 9);
	EXPECT_EQ(null_0->getLoopSize(), 7);

	auto area_in_0_0 = Pint::area(interior_0_0->getLoopPoints());
	auto area_in_1_0 = Pint::area(interior_1_0->getLoopPoints());
	auto area_ex_0_0 = Pint::area(exterior_0_0->getLoopPoints());
	auto area_null_0 = Pint::area(null_0->getLoopPoints());

	EXPECT_TRUE(area_in_0_0 == rto(2));
	EXPECT_TRUE(area_in_1_0 == rto(2));
	EXPECT_TRUE(area_ex_0_0 == rto(396));
	EXPECT_TRUE(area_null_0 == rto(-400));

	EXPECT_FALSE(interior_0_0->getNeighbors().contains(interior_1_0));
	EXPECT_TRUE(interior_0_0->getNeighbors().contains(exterior_0_0));
	EXPECT_TRUE(interior_0_0->getNeighbors().contains(null_0));

	EXPECT_FALSE(interior_1_0->getNeighbors().contains(interior_0_0));
	EXPECT_TRUE(interior_1_0->getNeighbors().contains(exterior_0_0));
	EXPECT_TRUE(interior_1_0->getNeighbors().contains(null_0));

	EXPECT_TRUE(exterior_0_0->getNeighbors().contains(interior_0_0));
	EXPECT_TRUE(exterior_0_0->getNeighbors().contains(interior_1_0));
	EXPECT_TRUE(exterior_0_0->getNeighbors().contains(null_0));

	EXPECT_TRUE(null_0->getNeighbors().contains(interior_0_0));
	EXPECT_TRUE(null_0->getNeighbors().contains(interior_1_0));
	EXPECT_TRUE(null_0->getNeighbors().contains(exterior_0_0));
}

TEST(Face_Cuts, Divided_Meet_Cut) {
	
	//setup
	DCEL<Pint> space;

	//results
	FLL<Region<Pint> *> interior;
	FLL<Region<Pint> *> exterior;
	Region<Pint> * null;

	//tested operations are performed within this block
	{
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

		FLL<Region<Pint> *> exterior_med;

		auto product = space.region(boundary_big);

		auto null_face = (*product)[0]->getRoot()->getInv()->getFace();

		null = space.region(null_face);

		subAllocate(product, boundary_small_a, exterior_med, interior);

		subAllocate(exterior_med[0], boundary_small_b, exterior, interior);
	}

	//testing

	EXPECT_EQ(space.pointCount(), 15);
	EXPECT_EQ(space.edgeCount(), 36);
	EXPECT_EQ(space.faceCount(), 5);
	EXPECT_EQ(space.regionCount(), 5);

	ASSERT_EQ(interior.size(), 3);
	ASSERT_EQ(exterior.size(), 1);

	ASSERT_NE(interior[0], nullptr);
	ASSERT_NE(interior[1], nullptr);
	ASSERT_NE(interior[2], nullptr);
	ASSERT_NE(exterior[0], nullptr);

	Region<Pint> * interior_0 = interior[0];
	Region<Pint> * interior_1 = interior[1];
	Region<Pint> * interior_2 = interior[2];
	Region<Pint> * exterior_0 = exterior[0];

	ASSERT_NE(interior_0, nullptr);
	ASSERT_NE(interior_1, nullptr);
	ASSERT_NE(interior_2, nullptr);
	ASSERT_NE(exterior_0, nullptr);

	ASSERT_EQ(interior_0->size(), 1);
	ASSERT_EQ(interior_1->size(), 1);
	ASSERT_EQ(interior_2->size(), 1);
	ASSERT_EQ(exterior_0->size(), 1);
	ASSERT_EQ(null->size(), 1);

	Face<Pint> * interior_0_0 = (*interior_0)[0];
	Face<Pint> * interior_1_0 = (*interior_1)[0];
	Face<Pint> * interior_2_0 = (*interior_2)[0];
	Face<Pint> * exterior_0_0 = (*exterior_0)[0];
	Face<Pint> * null_0 = (*null)[0];

	ASSERT_NE(interior_0_0, nullptr);
	ASSERT_NE(interior_1_0, nullptr);
	ASSERT_NE(interior_2_0, nullptr);
	ASSERT_NE(exterior_0_0, nullptr);
	ASSERT_NE(null_0, nullptr);

	EXPECT_EQ(interior_0_0->getLoopSize(), 4);
	EXPECT_EQ(interior_1_0->getLoopSize(), 4);
	EXPECT_EQ(interior_2_0->getLoopSize(), 6);
	EXPECT_EQ(exterior_0_0->getLoopSize(), 14);
	EXPECT_EQ(null_0->getLoopSize(), 8);

	auto area_in_0_0 = Pint::area(interior_0_0->getLoopPoints());
	auto area_in_1_0 = Pint::area(interior_1_0->getLoopPoints());
	auto area_in_2_0 = Pint::area(interior_2_0->getLoopPoints());
	auto area_ex_0_0 = Pint::area(exterior_0_0->getLoopPoints());
	auto area_null_0 = Pint::area(null_0->getLoopPoints());

	EXPECT_TRUE(area_in_0_0 == rto(1));
	EXPECT_TRUE(area_in_1_0 == rto(1));
	EXPECT_TRUE(area_in_2_0 == rto(4));
	EXPECT_TRUE(area_ex_0_0 == rto(390));
	EXPECT_TRUE(area_null_0 == rto(-396));

	EXPECT_FALSE(interior_0_0->getNeighbors().contains(interior_1_0));
	EXPECT_TRUE(interior_0_0->getNeighbors().contains(interior_2_0));
	EXPECT_TRUE(interior_0_0->getNeighbors().contains(exterior_0_0));
	EXPECT_TRUE(interior_0_0->getNeighbors().contains(null_0));

	EXPECT_FALSE(interior_1_0->getNeighbors().contains(interior_0_0));
	EXPECT_TRUE(interior_1_0->getNeighbors().contains(interior_2_0));
	EXPECT_TRUE(interior_1_0->getNeighbors().contains(exterior_0_0));
	EXPECT_TRUE(interior_1_0->getNeighbors().contains(null_0));

	EXPECT_TRUE(interior_2_0->getNeighbors().contains(interior_0_0));
	EXPECT_TRUE(interior_2_0->getNeighbors().contains(interior_1_0));
	EXPECT_TRUE(interior_2_0->getNeighbors().contains(exterior_0_0));
	EXPECT_FALSE(interior_2_0->getNeighbors().contains(null_0));

	EXPECT_TRUE(exterior_0_0->getNeighbors().contains(interior_0_0));
	EXPECT_TRUE(exterior_0_0->getNeighbors().contains(interior_1_0));
	EXPECT_TRUE(exterior_0_0->getNeighbors().contains(interior_2_0));
	EXPECT_TRUE(exterior_0_0->getNeighbors().contains(null_0));

	EXPECT_TRUE(null_0->getNeighbors().contains(interior_0_0));
	EXPECT_TRUE(null_0->getNeighbors().contains(interior_1_0));
	EXPECT_FALSE(null_0->getNeighbors().contains(interior_2_0));
	EXPECT_TRUE(null_0->getNeighbors().contains(exterior_0_0));
}

