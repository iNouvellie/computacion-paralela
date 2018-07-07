// Tipos de Datos 
#define uint unsigned int
#define uchar unsigned char

// Valores logicos 
#define TRUE 1
#define FALSE 0

#define FRONTERA_BIT 0x1
#define INTERFAZ_BIT 0x2
#define ENABLED_BIT  0x4
#define SELECTED_BIT 0x8


typedef struct {
	float x;
	float y;
	uchar status;  // FRONTERA_BIT, INTERFAZ_BIT, ENABLED_BIT
} s_vertice;

struct s_vertice_node2 {
	s_vertice *vertice;
	struct s_vertice_node2 *next;
};

typedef struct s_vertice_node2 s_vertice_node;

typedef struct {
	int size;
	int size_array;
	s_vertice *data_array;
	struct s_vertice_node2 *data_list;
} s_vertices;

// estructuras TRIANGULOS
typedef struct { 
	uint particionID;
	uchar status;  // SELECTED_BIT, ENABLED_BIT
	int vertice[3];
	int parentID;
	int rootID;
} s_triangulo;

struct s_triangulo_node2 {
	s_triangulo *triangulo;
	struct s_triangulo_node2 *next;
};

typedef struct {
	int size;             // numero total de triangulos (nulos y activos)
	int size_array;       // numero de triangulos activos
	s_triangulo *data_array;
	struct s_triangulo_node2 *data_list;
} s_triangulos;

typedef struct s_triangulo_node2 s_triangulo_node;

typedef struct {
	s_triangulo triangulo;
	s_vertice vertice[3];	
} s_triangulo_con_vertices;


// Funciones VERTICES
extern void init_vertices (s_vertice *data_array, int size_array, s_vertices *vertices);
extern int add_vertice (s_vertice *vertice, s_vertices *vertices);
extern s_vertice *get_vertice (int verticeID, s_vertices *vertices);
extern int get_vertice_id (s_vertice *vertice, s_vertices *vertices);

// Funciones TRIANGULOS
extern void init_triangulos (s_triangulo *data_array, int size_array, s_triangulos *triangulos);
extern int add_triangulo (s_triangulo *triangulo, s_triangulos *triangulos);
extern s_triangulo *get_triangulo (int trianguloID, s_triangulos *triangulos);
extern int get_triangulo_id (s_triangulo *triangulo, s_triangulos *triangulos);
