#ifndef _GCM_TETR_MESH_1ST_ORDER_H
#define _GCM_TETR_MESH_1ST_ORDER_H  1

#include <sstream>
#include <algorithm>

#include "TetrMesh.h"
#include "../system/quick_math.h"
#include "../datatypes/ElasticNode.h"
#include "../datatypes/Tetrahedron_1st_order.h"
#include "../methods/GCM_Tetr_Elastic_Interpolation_1stOrder.h"
#include "../rheotypes/VoidRheologyCalculator.h"

using std::stringstream;
using std::sort;
using std::unique;

struct TetrMesh_1stOrder_stats
{
	int find_owner_tetr_quick_searches;
	int find_owner_tetr_long_searches;
};

class TetrMesh_1stOrder : public TetrMesh
{
public:
	TetrMesh_1stOrder();
	~TetrMesh_1stOrder();
	int load_msh_file(char* file_name);
        int load_node_ele_files(char* node_file_name, char* ele_file_name);
	int load_gmv_file(char* file_name);
	bool point_in_tetr(float x, float y, float z, Tetrahedron* tetr);
	bool point_in_tetr(float x, float y, float z, Tetrahedron_1st_order* tetr);
	Tetrahedron_1st_order* find_owner_tetr(float x, float y, float z, ElasticNode* node);
	int interpolate(ElasticNode* node, Tetrahedron* tetr);
	int log_quality_stats();
	int do_next_step();
	float get_max_possible_tau();
	float get_min_h();
	float get_max_h();

	vector<ElasticNode> nodes;
	vector<Tetrahedron_1st_order> tetrs;

	TetrMesh_1stOrder_stats mesh_stats;

private:
	void clear_mesh_stats();

	float tetr_h(int i);
	int do_next_part_step(float tau, int stage);
	void move_coords(float tau);
	int set_stress(float tau);
	int proceed_rheology();

	vector<ElasticNode> new_nodes;

	quick_math qm_engine;
};

#include "TetrMesh_1stOrder.inl"

#endif
