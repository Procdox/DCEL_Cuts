# DCEL Cuts

This library enables "gridless" allocations of 2D space.

It does this via an integer bound doubly connected edge list structure with supplied geometric boolean and graph edit operations.

Simply put, you can divide space into irregular polygons as you like.

# Testing

Test cases are supplied in a dependent project.
gTest is supplied as a submodule and linked to.
gTest must be manually compiled. This can be done by opening \DCEL_Cuts\googletest\googletest\msvc\2010\gtest.sln and compiling the gtest project. No other gtest projects are needed.
