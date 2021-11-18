double distance(double x1, double y1, double x2, double y2);
int compare(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4);
void label_max_edge(const uint e, sdsl::pemb<> &pe, bit_vector &edges_to_label, vector<double> &points);
bool is_frontier_edge(const uint e, sdsl::pemb<> &pe, bit_vector &max_edges, vector<double> &points);