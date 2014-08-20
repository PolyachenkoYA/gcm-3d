cl1 = 0.1;
r=0.95;
l=15.0;
Point(1) = {0, -3, 0, cl1};
Point(2) = {r, -3, 0, cl1};
Point(3) = {0, -2.05, 0, cl1};
Point(4) = {0, -3, l, cl1};
Point(5) = {0, -2.05, l, cl1};
Point(6) = {r, -3, l, cl1};
Point(7) = {-r, -3, l, cl1};
Point(8) = {0, -3.95, l, cl1};
Point(9) = {0, -3.95, 0, cl1};
Point(10) = {-r, -3, 0, cl1};
Circle(1) = {6, 4, 5};
Circle(2) = {5, 4, 7};
Circle(3) = {7, 4, 8};
Circle(4) = {8, 4, 6};
Circle(5) = {9, 1, 10};
Circle(6) = {10, 1, 3};
Circle(7) = {3, 1, 2};
Circle(8) = {2, 1, 9};
Line(13) = {8, 9};
Line(14) = {2, 6};
Line(15) = {7, 10};
Line(16) = {3, 5};
Line Loop(10) = {4, 1, 2, 3};
Plane Surface(10) = {10};
Line Loop(12) = {8, 5, 6, 7};
Plane Surface(12) = {12};
Line Loop(18) = {4, -14, 8, -13};
Ruled Surface(18) = {18};
Line Loop(20) = {5, -15, 3, 13};
Ruled Surface(20) = {20};
Line Loop(22) = {2, 15, 6, 16};
Ruled Surface(22) = {22};
Line Loop(24) = {1, -16, 7, 14};
Ruled Surface(24) = {24};
Surface Loop(26) = {18, 10, 24, 22, 20, 12};
Volume(26) = {26};