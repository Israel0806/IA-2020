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
#include <math.h>
#include <string>

# define KEY_ESC 27
# define KEY_PLUS 43
# define KEY_MINUS 45
# define KEY_ENTER 13
# define KEY_B 98
# define KEY_R 114

# define PI 3.14159265358979323846  
#define font18 GLUT_BITMAP_HELVETICA_18
#define font12 GLUT_BITMAP_9_BY_15
#define font24 GLUT_BITMAP_TIMES_ROMAN_24
#define Radius 5

# define colorRed glColor3f(1,0,0)
# define colorYellow glColor3f(1,0.843,0)
# define colorPurple glColor3f(0.502, 0.000, 0.502)
# define colorGreen glColor3f(0,0.502,0)
# define colorBlue glColor3f(0,0,0.804)
# define colorBlack glColor3f(0,0,0)
# define colorWhite glColor3f(1,1,1)

using namespace std;
int selectedHeight = 2;

struct strJuego {
	/// X = 0 ,  O = 1
	int lista[3][3] = { {-1,-1,-1},{-1,-1,-1},{-1,-1,-1} };
	int jugadasRestantes = 9;
	int utilidad;
	
	void computeUtility () {
		int Xs = 0, Os = 0;
		for ( int i = 0; i < 3; ++i ) {
			bool horX, horO;
			bool verX, verO;
			horX = true, horO = true;
			verX = true, verO = true;
			for ( int j = 0; j < 3; ++j ) {
				if ( lista[i][j] == 1 ) 
					horX = false; /// horizontal para X
				if ( lista[i][j] == 0 ) 
					horO = false; /// horizontal para O
				
				if ( lista[j][i] == 1 ) 
					verX = false; /// vertical para X
				if ( lista[j][i] == 0 ) 
					verO = false; /// vertical para O
			}
			if ( horX ) ++Xs; if ( horO ) ++Os;
			if ( verX ) ++Xs; if ( verO ) ++Os;
		}
		/// diagonal
		bool digX = true, digO = true;
		bool _digX = true, _digO = true;
		for ( int i = 0; i < 3; ++i ) {
			if ( lista[i][i] == 1 ) digX = false; /// diagonal(\) para X
			if ( lista[i][i] == 0 ) digO = false; /// diagonal(\) para O
			
			if ( lista[i][2 - i] == 1 ) _digX = false; /// diagonal(/) para X
			if ( lista[i][2 - i] == 0 ) _digO = false; /// diagonal(/) para O
		}
		if ( digX ) ++Xs;  if ( digO ) ++Os;
		if ( _digX ) ++Xs; if ( _digO ) ++Os;
		utilidad = - Xs + Os;
	}
	
	void print() {
		cout<<"Utilidad: "<<utilidad<<endl;
		for ( int i = 0; i < 3; ++i ) {
			for ( int j = 0; j < 3; ++j ) {
				cout << lista[i][j] << " ";
			}
			cout<<endl;
		}
		cout<<endl;
	}
	
	void reset() {
		for ( int i = 0; i < 3; ++i ) {
			for ( int j = 0; j < 3; ++j ) {
				lista[i][j] = -1;
			}
		}
		jugadasRestantes = 9;
	}
	
	void copy (strJuego &newTablero) {
		for ( int i = 0; i < 3; ++i ) {
			for ( int j = 0; j < 3; ++j ) {
				lista[i][j] = newTablero.lista[i][j];
				jugadasRestantes = newTablero.jugadasRestantes;
			}
		}
	}
};

class Node {
public:
	strJuego estadoActual;
	int value, alpha, beta, height;
	int camino;
	Node *back; /// padre
	vector<Node *> nodes;
	
	Node (int alpha, int beta, int height, Node *back) {
		this->alpha = alpha;
		this->beta = beta;
		this->height = height;
		this->back = back;
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
	int maxHeight;
	Node *root;
public:
	Tree () {
		root = NULL;
	}
	
	Node *returnRoot () {
		return root;
	}
	
	Tree (int maxHeight) {
		this->maxHeight = maxHeight;
		root = new Node (INT_MIN, INT_MAX, 0, nullptr);
		createTree (root, 1, maxHeight);
	}
	/// maxHeight: 4 3 2 1 0
	/// height:    1 2 3 4
	/// creo todos los nodos con (10 - height) posibilidades aunque no se usen todas
	void createTree (Node *actual, int height, int maxHeight) {
		if ( !maxHeight ) return;
		for ( int i = 0; i < 10 - height; ++i ) {
//			cout<<height<<" ";
			Node *node = new Node (INT_MIN, INT_MAX, height, actual);
			actual->nodes.push_back (node);
			createTree (actual->nodes.back (), height + 1, maxHeight - 1);
		}
//		cout<<endl;
	}
	
	void generarArbol (strJuego &tablero) {
		root->estadoActual.copy (tablero);
		generarArbol (root, 0);
	}
	
	/// al inicio de cada generacion de arbol
	void generarArbol (Node *actual, int jugador) {
		if ( actual->height == maxHeight or actual->estadoActual.jugadasRestantes == 0 )
			actual->estadoActual.computeUtility (); /// si es hoja calculas utilidad
		else {
			agregarJugada (actual, jugador); /// segundo nivel
			for ( int i = 0; i < actual->estadoActual.jugadasRestantes; ++i ) 
				generarArbol (actual->nodes[i], !jugador); /// si no es hoja generas el arbol hijo
			
			if ( actual->height % 2 )
				maximo (actual);
			else
				minimo (actual);
		}
	}

	/// agregar la jugada en los nodos hijos de actual
	void agregarJugada (Node *actual, int jugador) {
		int pos = 0;
		for ( int i = 0; i < 3; ++i ) {
			for ( int j = 0; j < 3; ++j ) {
				if ( actual->estadoActual.lista[i][j] == -1 ) {
					actual->nodes[pos]->estadoActual.copy(actual->estadoActual);
					actual->nodes[pos]->estadoActual.lista[i][j] = jugador;
					actual->nodes[pos++]->estadoActual.jugadasRestantes--;
				}
			}
		}
	}
	
	void maximo (Node *actual) {
		int maximo = INT_MIN;
		int elegido;
		for ( int i = 0; i < actual->estadoActual.jugadasRestantes; i++ ) {
			if ( maximo < actual->nodes[i]->estadoActual.utilidad ) {
				maximo = actual->nodes[i]->estadoActual.utilidad;
				elegido = i;
			}
		}
		actual->estadoActual.utilidad = maximo;
		actual->camino = elegido;
	}
	
	void minimo (Node *actual) {
		int minimo = INT_MAX;
		int elegido;
		for ( int i = 0; i < actual->estadoActual.jugadasRestantes; i++ ) {
			if ( minimo > actual->nodes[i]->estadoActual.utilidad ) {
				minimo = actual->nodes[i]->estadoActual.utilidad;
				elegido = i;
			}
		}
		actual->estadoActual.utilidad = minimo;
		actual->camino = elegido;
	}
	
	/// generar arbol
	void checkArbol (strJuego &tablero) {
		/// hacer la jugada 
		generarArbol (tablero);
		
		/// elegir la jugada
		Node *siguiente_jugada = root->nodes[root->camino];
		
		/// cambiar el tablero
		tablero.copy (siguiente_jugada->estadoActual);
	}
	
	
	~Tree () {
		delete root;
	}
};


Tree *tree = nullptr;
strJuego tablero;
/// turno es quien comenzo
/// ganador es quien gano el juego
/// -1 se grafica el juego, 0 gano la computadora y 1 gano el jugador
int turnoInicial, turno = 0, ganador = -1;

/// funcion sacada de internet para dibujar un circulo a partir de una linea con muchos vertices
void drawHollowCircle (GLfloat x, GLfloat y, GLfloat radius) {
	int i;
	int lineAmount = 100; //# of triangles used to draw circle
	//GLfloat radius = 0.8f; //radius
	GLfloat twicePi = 2.0f * PI;
	glBegin (GL_LINE_LOOP);
	for ( i = 0; i <= lineAmount; i++ ) {
		glVertex2f (
					x + (radius * cos (i * twicePi / lineAmount)),
					y + (radius * sin (i * twicePi / lineAmount))
					);
	}
	glEnd ();
}

void iniciarJuego () {
	string a;
	while(true) {
		int op;
		cout << "Selecciona la dificultad:" << endl;
		cout << "1. Facil" << endl;
		cout << "2. Medio" << endl;
		cout << "3. Dificil" << endl;
		cin>>op;
		if(op == 1) {
			selectedHeight = 4;
			break;
		} else if(op == 2) {
			selectedHeight = 6;
			break;
		} else if(op == 3){
			selectedHeight = 8;
			break;
		} else {
			cout << "opcion incorrecta"<<endl;
		}
	}
	tree = new Tree (selectedHeight);
	cout << "Deseas iniciar tu(si/no)" << char (63) << endl;
	cin>>a;
	turno = 1;
	if ( a == "no" or a == "NO" ) {
		turnoInicial = 0;
		tree->checkArbol(tablero);
	} else {
		turnoInicial = 1;
	}
}

/// segun la posicion del mouse cambia el tablero segun el jugador actual
bool marcarCasilla (int x, int y) {
	if ( x <= 266 ) {
		if ( y <= 186 ) { /// pos 2,0
			if ( tablero.lista[2][0] == -1 ) {
				tablero.lista[2][0] = turno;
				return true;
			}
		} else if ( y <= 372 ) { /// pos 1,0
			if ( tablero.lista[1][0] == -1 ) {
				tablero.lista[1][0] = turno;
				return true;
			}
		} else { /// pos 0,0
			if ( tablero.lista[0][0] == -1 ) {
				tablero.lista[0][0] = turno;
				return true;
			}
		}
	} else if ( x <= 532 ) {
		if ( y <= 186 ) { /// pos 2,1
			if ( tablero.lista[2][1] == -1 ) {
				tablero.lista[2][1] = turno;
				return true;
			}
		} else if ( y <= 372 ) { /// pos 1,1
			if ( tablero.lista[1][1] == -1 ) {
				tablero.lista[1][1] = turno;
				return true;
			}
		} else { /// pos 0,1
			if ( tablero.lista[0][1] == -1 ) {
				tablero.lista[0][1] = turno;
				return true;
			}
		}
	} else {
		if ( y <= 186 ) { /// pos 2,2
			if ( tablero.lista[2][2] == -1 ) {
				tablero.lista[2][2] = turno;
				return true;
			}
		} else if ( y <= 372 ) { /// pos 1,2
			if ( tablero.lista[1][2] == -1 ) {
				tablero.lista[1][2] = turno;
				return true;
			}
		} else { /// pos 0,2
			if ( tablero.lista[0][2] == -1 ) {
				tablero.lista[0][2] = turno;
				return true;
			}
		}
	}
	return false;
}

/// verifica si alguien gano el juego
bool verificarJuego () {
	if( !tablero.jugadasRestantes )
		ganador = 2;
	bool horX, horO;
	bool verX, verO;
	bool digX = true, digO = true;
	bool _digX = true, _digO = true;
	for ( int i = 0; i < 3; ++i ) {
		horX = true, horO = true;
		verX = true, verO = true;
		for ( int j = 0; j < 3; ++j ) {
			if ( tablero.lista[i][j] != 0 ) horX = false; /// horizontal para X
			if ( tablero.lista[i][j] != 1 ) horO = false; /// horizontal para O
			
			if ( tablero.lista[j][i] != 0 ) verX = false; /// vertical para X
			if ( tablero.lista[j][i] != 1 ) verO = false; /// vertical para O
		}
		if(horX or verX) {ganador = 0; return true;}
		if(horO or verO) {ganador = 1; return true;}
	}
	/// diagonal
	for ( int i = 0; i < 3; ++i ) {
		if ( tablero.lista[i][i] != 0 ) digX = false; /// diagonal(\) para X
		if ( tablero.lista[i][i] != 1 ) digO = false; /// diagonal(\) para O
		
		if ( tablero.lista[i][2 - i] != 0 ) _digX = false; /// diagonal(/) para X
		if ( tablero.lista[i][2 - i] != 1 ) _digO = false; /// diagonal(/) para O
	}
	if (digX or _digX) {ganador = 0; return true;}
	if (digO or _digO) {ganador = 1; return true;}
}

/// dibujar Xs en la posicion indicada
void dibujarX (int pos) {
	glLineWidth (5);
	colorPurple;
	switch ( pos ) {
	case 0:
		glBegin (GL_LINES);
		/// arriba izq     a  abajo der
		glVertex2i (80, 520); glVertex2i (206, 412);
		/// abajo izq     a  arriba der
		glVertex2i (80, 412); glVertex2i (206, 520);
		glEnd ();
		break;
	case 1:
		glBegin (GL_LINES);
		/// arriba izq     a  abajo der
		glVertex2i (326, 520); glVertex2i (472, 412);
		/// abajo izq     a  arriba der
		glVertex2i (326, 412); glVertex2i (472, 520);
		glEnd ();
		break;
	case 2:
		glBegin (GL_LINES);
		/// arriba izq     a  abajo der
		glVertex2i (592, 520); glVertex2i (720, 412);
		/// abajo izq     a  arriba der
		glVertex2i (592, 412); glVertex2i (720, 520);
		glEnd ();
		break;
	case 3:
		glBegin (GL_LINES);
		/// arriba izq     a  abajo der
		glVertex2i (80, 342); glVertex2i (206, 226);
		/// abajo izq     a  arriba der
		glVertex2i (80, 226); glVertex2i (206, 342);
		glEnd ();
		break;
	case 4:
		glBegin (GL_LINES);
		/// arriba izq     a  abajo der
		glVertex2i (326, 342); glVertex2i (472, 226);
		/// abajo izq     a  arriba der
		glVertex2i (326, 226); glVertex2i (472, 342);
		glEnd ();
		break;
	case 5:
		glBegin (GL_LINES);
		/// arriba izq     a  abajo der
		glVertex2i (592, 342); glVertex2i (720, 226);
		/// abajo izq     a  arriba der
		glVertex2i (592, 226); glVertex2i (720, 342);
		glEnd ();
		break;
	case 6:
		glBegin (GL_LINES);
		/// arriba izq     a  abajo der
		glVertex2i (80, 146); glVertex2i (206, 40);
		/// abajo izq     a  arriba der
		glVertex2i (80, 40); glVertex2i (206, 146);
		glEnd ();
		break;
	case 7:
		glBegin (GL_LINES);
		/// arriba izq     a  abajo der
		glVertex2i (326, 146); glVertex2i (472, 40);
		/// abajo izq     a  arriba der
		glVertex2i (326, 40); glVertex2i (472, 146);
		glEnd ();
		break;
	case 8:
		glBegin (GL_LINES);
		/// arriba izq     a  abajo der
		glVertex2i (592, 146); glVertex2i (720, 40);
		/// abajo izq     a  arriba der
		glVertex2i (592, 40); glVertex2i (720, 146);
		glEnd ();
		break;
	}
}

/// dibujar Os en la posicion indicada
void dibujarO (int pos) {
	glLineWidth (5);
	colorBlue;
	switch ( pos ) {
	case 0:
		drawHollowCircle (143, 466, 70);
		break;
	case 1:
		drawHollowCircle (399, 466, 70);
		break;
	case 2:
		drawHollowCircle (656, 466, 70);
		break;
	case 3:
		drawHollowCircle (143, 279, 70);
		break;
	case 4:
		drawHollowCircle (399, 279, 70);
		break;
	case 5:
		drawHollowCircle (656, 279, 70);
		break;
	case 6:
		drawHollowCircle (143, 103, 70);
		break;
	case 7:
		drawHollowCircle (399, 103, 70);
		break;
	case 8:
		drawHollowCircle (656, 103, 70);
		break;
	}
}

void restartGame () {
	tablero.reset();
	ganador = -1;
	turno = 1;
	if(!turnoInicial)
		tree->checkArbol (tablero);
}

GLvoid window_key (unsigned char key, int x, int y) {
	switch ( key ) {
	case KEY_ESC:
		exit (0);
		break;
	case KEY_R: /// Restart
		restartGame();
		break;
	default:
		break;
	}
	glutPostRedisplay();
}

void reshape_cb (int w, int h) {
	if ( w == 0 || h == 0 ) return;
	glViewport (0, 0, w, h);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	glutKeyboardFunc (&window_key);
	gluOrtho2D (0, w, 0, h);
	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity ();
}

GLvoid OnMouseClick (int button, int state, int x, int y) {
	y = 600 - y;
	if ( (button == GLUT_LEFT_BUTTON or button == GLUT_RIGHT_BUTTON) && state == GLUT_DOWN ) {		
		if (ganador != -1) {
			if( x > 300 and x < 550 and y > 270 and y < 370) 
				restartGame();
		} else {
			/// pintar casilla correcta del usuario
			if ( marcarCasilla (x, y) ) {
				tablero.jugadasRestantes--;
				if( !verificarJuego () and tablero.jugadasRestantes ) { /// verificar si termina o no el juego
					/// pintar casilla de la IA
					tree->checkArbol (tablero); /// generar arbol y/o pinte casilla
					verificarJuego (); /// verificar si termina o no el juego
				}
			}
		}
	}
	glutPostRedisplay ();
}

void dibujarTablero () {
	colorRed;
	char jugador1[14] = { 'T','u','r','n','o',':',' ','J','u','g','a','d','o','r' };
	char jugador2[18] = { 'T','u','r','n','o',':',' ','C','o','m','p','u','t','a','d','o','r','a' };
	
	glRasterPos2i (330, 580); /// pos donde ira el texto
	if ( turno ) {
		for ( int i = 0; i < 14; ++i )
			glutBitmapCharacter (font18, jugador1[i]); /// para dibujar texto
	} else {
		for ( int i = 0; i < 18; ++i )
			glutBitmapCharacter (font18, jugador2[i]);
	}
	
	/// grid del tablero
	colorBlack;
	glBegin (GL_LINES);
	glVertex2i (266, 20); glVertex2i (266, 560);
	glVertex2i (532, 20); glVertex2i (532, 560);
	
	glVertex2d (20, 186); glVertex2i (780, 186);
	glVertex2d (20, 372); glVertex2i (780, 372);
	glEnd ();
	
	/// dibujar Xs y Os
	int pos = 0;
	for ( int i = 0; i < 3; ++i ) {
		for ( int j = 0; j < 3; ++j ) {
			if ( tablero.lista[i][j] == 0 ) {
				dibujarX (pos);
			} else if ( tablero.lista[i][j] == 1 ) {
				dibujarO (pos);
			}
			++pos;
		}
	}
}

/// dibujar la pantalla indicando quien gano
void pantallaGanador(int ganador) {
	glClear(GL_COLOR_BUFFER_BIT);
	char jugador1[16] = { 'G','a','n','a','d','o','r',':',' ','J','u','g','a','d','o','r' };
	char jugador2[20] = { 'G','a','n','a','d','o','r',':',' ','C','o','m','p','u','t','a','d','o','r','a' };
	char empate[6] = { 'E','m','p','a','t','e'};
	char *jugar = "Play Again";
	/// jugador
	if (ganador == 1) { 
		colorGreen;
		glRasterPos2i (330, 400); /// pos donde ira el texto
			for ( int i = 0; i < 16; ++i )
			glutBitmapCharacter (font24, jugador1[i]); /// para dibujar texto
	} else if(ganador == 0){ /// computadora
		colorRed;
		glRasterPos2i (310, 400); /// pos donde ira el texto
		for ( int i = 0; i < 20; ++i )
			glutBitmapCharacter (font24, jugador2[i]);
	} else {
		colorRed;
		glRasterPos2i (380, 400); /// pos donde ira el texto
		for ( int i = 0; i < 6; ++i )
			glutBitmapCharacter (font24, empate[i]);
	}	
	
	colorGreen;
		glBegin(GL_POLYGON);
		glVertex2i(300,370); /// top left
		glVertex2i(550,370); /// top right
		glVertex2i(550,270); /// bottom right
		glVertex2i(300,270); /// bottom left
	glEnd();
	
	colorWhite;
	glRasterPos2i (370, 310); /// pos donde ira el texto
	for ( int i = 0; i < 10; ++i )
		glutBitmapCharacter (font24, jugar[i]);
	
}

void display_cb () {
	glClear (GL_COLOR_BUFFER_BIT);
	glColor3f (1, 1, 0); glLineWidth (3);
	
	/// seguir el juego
	if ( ganador == -1 ) 
		dibujarTablero ();
	else /// gano el jugador 
		pantallaGanador (ganador);
	
	glutSwapBuffers ();
}

void initialize () {
	glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowSize (800, 600);
	glutInitWindowPosition (100, 100);
	glutCreateWindow ("Tres en raya");
	glutDisplayFunc (display_cb);
	glutReshapeFunc (reshape_cb);
	glutMouseFunc (&OnMouseClick);
	glClearColor (1.f, 1.f, 1.f, 1.f);
}

int main (int argc, char **argv) {
	iniciarJuego ();
	glutInit (&argc, argv);
	initialize ();
	glutMainLoop ();
	return 0;
}


