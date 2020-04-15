#ifdef __APPLE__
# include <OpenGL/gl.h>
# include <OpenGL/glu.h>
# include <GLUT/glut.h>
#else
# include <GL/gl.h>
# include <GL/glu.h>
# include <GL/glut.h>
#endif

#include <iostream>
#include <vector>
#include <time.h>
#include <math.h>
#include <list> 
#include <bits/stdc++.h> 

# define KEY_ESC 27
# define KEY_PLUS 43
# define KEY_MINUS 45
# define ENTER 13
# define KEY_B 98

using namespace std;

template<class G>
class _Edge {
public:
	typedef typename G::Node Node;
	typedef typename G::EE E;
	E data;
	Node *nodes[2];
	
	_Edge (Node *A, Node *B, E x) {
		nodes[0] = A;
		nodes[1] = B;
		data = x;
	}
};

template<class G>
class _Node {
public:
	typedef typename G::Edge Edge;
	typedef typename G::NN N;
	N data;
	int index;
	int distance;
	bool marked;
	vector<Edge *> edges;
	
	_Node (N n, int index) {
		data = n;
		this->index = index;
	}
	
	void InsertEdge (Edge *arista) {
		edges.push_back (arista);
	}
	
	void DisplayEdges () {
		for ( int i = 0; i < edges.size (); i++ ) {
			bool j = !(edges[i]->nodes[1]->data == data);
			cout << " -> " << edges[i]->nodes[j]->data.first <<","<< edges[i]->nodes[j]->data.second << "(" << edges[i]->data << ")";
		}
	}
};

template<class N, class E>
class Graph {
public:
	typedef Graph<N, E> self;
	typedef _Node<self> Node;
	typedef _Edge<self> Edge;
	typedef N NN;
	typedef E EE;
	vector<Node *> nodes;
	
	bool InsertNode (N n, int index) {
		Node *node = new Node (n, index);
		nodes.push_back (node);
		return true;
	}
	
	void InsertEdge (int A, int B, E e) {
		Edge *arista = new Edge (nodes[A], nodes[B], e);
		nodes[A]->InsertEdge (arista);
		nodes[B]->InsertEdge (arista);
	}
	
	bool RemoveEdge (Node *A, Node *B, E e) {
		if ( A and B ) {
			for ( int i = 0; i < A->edges.size (); i++ ) {
				if ( A->edges[i]->data == e and A->edges[i]->nodes[A->edges[i]->nodes[1]->data == B->data] == B ) {
					for ( int j = 0; j < B->edges.size (); j++ ) {
						if ( B->edges[j]->data == e and
							B->edges[j]->nodes[B->edges[j]->nodes[1]->data == A->data] == A ) {
							Edge *ed = (A->edges[i]);
							A->edges.erase (A->edges.begin () + i);
							if ( A != B )
								B->edges.erase (B->edges.begin () + j);
							delete ed;
							return true;
						}
					}
				}
			}
		}
		return false;
	}
	
	bool RemoveNode (int i) {
		Node *A = nodes[i];
		if ( A ) {
			while ( A->edges.size () != 0 ) { 
				Node *B = A->edges[0]->nodes[A->edges[0]->nodes[1] != A];
				E e = A->edges[0]->data;
				RemoveEdge (A, B, e);
			}
			nodes.erase (nodes.begin () + i);
			delete A;
			return true;
		}
		return false;
	}
	
	Node *findNode (N a) {
		for ( auto node : nodes )
			if ( node->data.first == a.first and node->data.second == a.second)
				return node;
		return nullptr;
	}
	
	double ff (N a, N b) {
		return sqrt (pow ((a.first - b.first), 2) + pow ((a.second - b.second), 2));
	}
	
	/// busqueda A*
	vector<pair<int,int> > findPathA (N inicial, N objetivo) {
		vector<pair<int,int>> camino;
		Node *Actual = nullptr, *NMin=nullptr;
		Edge *EMin = nullptr;
		double minV,total;
		E acumulado = 0;
		vector<Edge *> Ecamino;
		vector<Node * > Ncamino;
		if ((Actual = findNode(inicial)) and findNode(objetivo) ) {
			Ncamino.push_back (Actual);
			while ( Actual->data != objetivo ) {
				minV = INT_MAX;
				/*for ( int i = 0; i < Actual->edges.size (); ++i ) {
				Actual->edges[i]
				}*/
				for ( auto edge : Actual->edges ) {
					Node *B = edge->nodes[edge->nodes[0] == Actual];
					total = acumulado /*edge->data*/ + ff (B->data, objetivo);
					if ( total < minV ) {
						minV = total;
						NMin = B;
						EMin = edge;
					}
				}
				Actual = NMin;
				acumulado += EMin->data;
				Ncamino.push_back (NMin);
				Ecamino.push_back (EMin);
			}
			cout<<endl<<"Camino A*"<<endl;
			for ( auto node : Ncamino ) {
				cout << "(" << node->data.first << "," << node->data.second << "); ";
				camino.push_back(make_pair(node->data.first, node->data.second));
			}
		}
		
		return camino;
		
	}
	
	/// busqueda BFS
	vector<pair<int, int>> findPathBlind(N inicial, N objetivo) {
		
		vector<pair<int, int>> camino;
		Node* Actual = nullptr;
		vector<Node* > Ncamino;
		if( !findNode(objetivo)) {
			cout<<"La coordenada final no existe"<<endl;
		}
		else if (!(Actual = findNode(inicial)) ) {
			cout<<"La coordenada inicial no existe"<<endl;
		} else {
			list<Node *> cola;
			cola.push_back(Actual);
			Ncamino.push_back(Actual);
			while (!cola.empty() && cola.front()->data != objetivo) {
				Actual = cola.front();
				for (auto edge : Actual->edges) {
					Node* B = edge->nodes[edge->nodes[0] == Actual];
					if (!B->marked) {
						cola.push_back(B);
						Ncamino.push_back(B);
						B->marked = true;
					}
				}
				cola.pop_front();
			}
			if(!cola.empty())
				   Ncamino.push_back(cola.front());
			cout<<endl<<"Camino por amplitud"<<endl;
			for (auto node : Ncamino) {
				cout << "(" << node->data.first << "," << node->data.second << "); ";
				camino.push_back(make_pair(node->data.first, node->data.second));
				node->marked = false;
			}
		}
		return camino;
	}
	
	void PrintGraph () {
		cout << "Number of nodes: " << nodes.size () << endl << endl;
		//typename vector<Node *>::iterator it;
		//for ( it = nodes.begin (); it != nodes.end (); ++it ) {
		for(auto it : nodes ){
			cout << "(" << it->data.first << "," << it->data.second << ") ";
			it->DisplayEdges ();
			cout << endl;
		}
	}
};

Graph<pair<int, int>, int> *graph = new Graph<pair<int, int>, int>;
vector<pair<int,int>> caminoA;
vector<pair<int,int>> caminoBlind;

bool b = false;

GLvoid window_key(unsigned char key, int x, int y) {
	switch (key) {
	case KEY_ESC:
		exit(0);
		break;
	case ENTER:
	{
		caminoA.clear();
		caminoBlind.clear();
		pair<int, int> inicial;
		pair<int, int> objetivo;
		cout << endl << "Ingresa las coordenadas iniciales: "; cin >> inicial.first>> inicial.second;
		cout << "Ingresa las coordenadas objetivos: "; cin >> objetivo.first >> objetivo.second;
		
		caminoA = graph->findPathA (inicial, objetivo);
		caminoBlind = graph->findPathBlind (inicial, objetivo);
		glutPostRedisplay();
		break;
	}
	case KEY_B:
		b = !b;
		glutPostRedisplay();
		break;
	default:
		break;
	}
}
void reshape_cb (int w, int h) {
	if (w==0||h==0) return;
	glViewport(0,0,w,h);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	gluOrtho2D(0,w,0,h);
	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity ();
}

void display_cb() {
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1,0,0); glLineWidth(3);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0f, 100, 100, 0.0f, 0.0f, 1.0f);
	
	glPointSize(3);
	for(auto node : graph->nodes) {
		glBegin(GL_POINTS);
			glVertex2i(node->data.first, node->data.second);
		glEnd();
		
		if(b) {
			glLineWidth(1);
			glBegin(GL_LINES);
			for( auto edge : node->edges) {
				auto* B = edge->nodes[ edge->nodes[0] == node ];
				glVertex2i(node->data.first, node->data.second);
				glVertex2i(B->data.first, B->data.second);
			}
			glEnd();
		}
	}
	
	glLineWidth(5);
	glColor3f(0,0,1);
	glBegin(GL_LINE_STRIP);
		for( auto node : caminoA) {
			glVertex2i(node.first, node.second);
		}
	glEnd();
	glLineWidth(2);
	glColor3f(0,1,0);
	glBegin(GL_LINE_STRIP);
		for( auto node : caminoBlind) {
			glVertex2i(node.first, node.second);
		}
	glEnd();
	
	glutSwapBuffers();
}


void initialize() {
	glutInitDisplayMode (GLUT_RGBA|GLUT_DOUBLE);
	glutInitWindowSize (1000,700);
	glutInitWindowPosition (100,100);
	glutCreateWindow ("Ventana OpenGL");
	glutKeyboardFunc(&window_key);
	glutDisplayFunc (display_cb);
	glutReshapeFunc (reshape_cb);
	glClearColor(1.f,1.f,1.f,1.f);
}

int main (int argc, char **argv) {
	srand (time (NULL));
	
	/// crear nodos
	for ( int i = 0; i < 100; ++i )
		for ( int j = 0; j < 100; ++j ) 
			graph->InsertNode (make_pair (i, j), i + j);
	
	/// crear aristas
	/// i es eje y
	/// j es eje x
	for ( int i = 0; i < 100; ++i ) {
		for ( int j = 0; j < 100; ++j ) {
			if(i<99)
				graph->InsertEdge (100*i+j, 100*(i+1)+j, 1); // arriba
			
			if(j<99)
				graph->InsertEdge (100*i+j, 100*i+j+1, 1); // derecha
			
			if(j<99 and i<99)
				graph->InsertEdge (100 * i + j, 100 * (i+1) + j+1, 1); //diagonal derecha
			
			if(j>0 and i<99)
				graph->InsertEdge (100 * i + j, 100 * (i+1) + j-1, 1); //diagonal izquierda
		}
	}
	
	/// remover nodos
	for ( int i = 0; i < 2000; ++i ) 
		graph->RemoveNode (rand () % (10000-i));
	
	
	
	glutInit (&argc, argv);
	initialize();
	glutMainLoop();
	delete graph;
	return 0;
}
