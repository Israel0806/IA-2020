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
#include <bits/stdc++.h> 

# define KEY_ESC 27
# define KEY_PLUS 43
# define KEY_MINUS 45
# define ENTER 13
# define KEY_B 98

using namespace std;

class Node {
public:
	int estadoActual[3][3] = { {-1,-1,-1},{-1,-1,-1},{-1,-1,-1} };
	int value, alpha, beta, height;
	vector<Node *> nodes;
	
	Node (int alpha, int beta, int height) {
		this->alpha = alpha;
		this->beta = beta;
		this->height = height;
	}
	
	~Node () {
		while ( !nodes.empty () ) {
			Node *node = nodes.back ();
			delete node;
			nodes.pop_back ();
		}
	}
};

class Tree {
	Node *root;
public:
	Tree () {
		root = NULL;
	}
	
	Node *returnRoot () {
		return root;
	}
	
	Tree (int maxHeight) {
		int height = 0;
		root = new Node (INT_MIN, INT_MAX, height);
		createTree (root, height + 1, maxHeight);
	}
	
	/// creo todos los nodos con 9 posibilidades aunque no se usen todas
	void createTree (Node *actual, int height, int maxHeight) {
		if ( !maxHeight ) return;
		for ( int i = 0; i < 9; ++i ) {
			Node *node = new Node (INT_MIN, INT_MAX, height);
			actual->nodes.push_back (node);
			createTree (actual->nodes.back (), height + 1, maxHeight - 1);
		}
	}
	
	~Tree () {
		delete root;
	}
};


Tree *tree = new Tree (2);


GLvoid window_key(unsigned char key, int x, int y) {
	switch (key) {
	case KEY_ESC:
		exit(0);
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
	glutKeyboardFunc(&window_key);
	gluOrtho2D(0,w,0,h);
	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity ();
}

void display_cb() {
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1,1,0); glLineWidth(3);

	
	glutSwapBuffers();
}

void initialize() {
	glutInitDisplayMode (GLUT_RGBA|GLUT_DOUBLE);
	glutInitWindowSize (640,480);
	glutInitWindowPosition (100,100);
	glutCreateWindow ("Ventana OpenGL");
	glutDisplayFunc (display_cb);
	glutReshapeFunc (reshape_cb);
	glClearColor(1.f,1.f,1.f,1.f);
}

int main (int argc, char **argv) {
	glutInit (&argc, argv);
	initialize();
	glutMainLoop();
	return 0;
}


