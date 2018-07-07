extern void print_vertice_info (char msg[], s_vertice *vertice);
extern void print_vertices_info (s_vertices *vertices, int rank);
extern s_vertice *bisect_arista (int vertice1, int vertice2, s_vertices *vertices);
extern int *search_longest_arista (s_triangulo *triangulo, s_vertices *vertices);
extern void search_vertices_status (s_triangulos *triangulos, s_vertices *vertices);

extern int *bisect_triangulo (int trianguloID, s_triangulos *triangulos, s_vertices *vertices, int rank);
extern void print_triangulos_info (s_triangulos *triangulos, int rank);
extern void print_triangulo_info (char msg[], s_triangulo *triangulo);

extern int search_triangulo_seleccionado (s_triangulos *triangulos, int particion);
extern int search_triangulo_by_vertices (s_triangulos *triangulos, int vertice_a, int vertice_b, int n);
extern int check_triangulo_conforme (s_triangulo *triangulo, s_vertices *vertices);
extern int *bisect_triangulo_conf (int trianguloID, int vertices_t0, int vertices_t1, s_triangulos *triangulos, s_vertices *vertices, int rank);
