cl1 = 1;
Point(1) = {0.6003, -5.5, 0, cl1};
Point(2) = {0.4003, -5.5, 0, cl1};
Point(3) = {0.4003, -0.4003, 0, cl1};
Point(4) = {3.0003, -0.4003, 0, cl1};
Point(5) = {3.0003, -0.6003, 0, cl1};
Point(6) = {0.6003, -0.6003, 0, cl1};
Point(7) = {0.6003, -5.5, 13.5, cl1};
Point(8) = {0.4003, -5.5, 13.5, cl1};
Point(9) = {0.4003, -0.4003, 13.5, cl1};
Point(10) = {3.0003, -0.4003, 13.5, cl1};
Point(11) = {3.0003, -0.6003, 13.5, cl1};
Point(12) = {0.6003, -0.6003, 13.5, cl1};
Line(1) = {10, 9};
Line(2) = {9, 8};
Line(3) = {8, 7};
Line(4) = {7, 12};
Line(5) = {12, 11};
Line(6) = {11, 10};
Line(7) = {10, 4};
Line(8) = {4, 3};
Line(9) = {3, 2};
Line(10) = {2, 1};
Line(11) = {1, 6};
Line(12) = {6, 5};
Line(13) = {5, 4};
Line(14) = {5, 11};
Line(15) = {12, 6};
Line(16) = {9, 3};
Line(17) = {8, 2};
Line(18) = {1, 7};
Line Loop(20) = {6, 1, 2, 3, 4, 5};
Plane Surface(20) = {20};
Line Loop(22) = {12, 13, 8, 9, 10, 11};
Plane Surface(22) = {22};
Line Loop(24) = {7, 8, -16, -1};
Plane Surface(24) = {24};
Line Loop(26) = {16, 9, -17, -2};
Plane Surface(26) = {26};
Line Loop(28) = {3, -18, -10, -17};
Plane Surface(28) = {28};
Line Loop(30) = {18, 4, 15, -11};
Plane Surface(30) = {30};
Line Loop(32) = {5, -14, -12, -15};
Plane Surface(32) = {32};
Line Loop(34) = {14, 6, 7, -13};
Plane Surface(34) = {34};
Surface Loop(36) = {26, 24, 34, 32, 20, 28, 30, 22};
Volume(36) = {36};
