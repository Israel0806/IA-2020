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
#include <fstream>

using namespace std;
char matriz[20][20]={};
int filas=10;
int columnas=5;

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
	for (int i=0;i<filas;++i){
		for (int j=0;j<columnas;++j){
			if(matriz[filas-i-1][j]=='0'){
				glColor3f(1,1,1); glLineWidth(3);
			}else{
				glColor3f(0,0,0); glLineWidth(3);
			}
			glBegin(GL_POLYGON);
			
				glVertex2d(70*j,70*i);
				glVertex2d(70*(j+1),70*i);
				glVertex2d(70*(j+1),70*(i+1));
				glVertex2d(70*j,70*(i+1));
			
			glEnd();
		}
	}
	
	glutSwapBuffers();
}


GLvoid window_key (unsigned char key, int x, int y) {
	ofstream myfile;
	bool flag=0;
	string archivo;
	switch ( key ) {
	case 27:
		exit (0);
		break;
	case '1':
		archivo="1.txt";
		flag=1;
		break;
	case '2':
		archivo="2.txt";
		flag=1;
		break;
	case '3':
		archivo="3.txt";
		flag=1;
		break;
	case '4':
		archivo="4.txt";
		flag=1;
		break;
	case '5':
		archivo="5.txt";
		flag=1;
		break;
	case '6':
		archivo="6.txt";
		flag=1;
		break;
	case '7':
		archivo="7.txt";
		flag=1;
		break;
	case '8':
		archivo="8.txt";
		flag=1;
		break;
	case '9':
		archivo="9.txt";
		flag=1;
		break;
	case '0':
		archivo="0.txt";
		flag=1;
		break;
	}
	if(flag){
		ofstream myfile(archivo,  ios::out | ios::app);
		myfile << filas<<' '<<columnas<<endl;
		for(int i=0;i<filas;++i)
			myfile <<matriz[i]<<endl;
		myfile.close();
	}
	glutPostRedisplay();
}


GLvoid OnMouseClick (int button, int state, int x, int y) {
	if ( (button == GLUT_LEFT_BUTTON or button == GLUT_RIGHT_BUTTON) && state == GLUT_DOWN ) {		
		x/=70;
		y/=70;
		cout<<y<<"  "<<x<<endl;
		matriz[y][x]=(matriz[y][x]=='0')?'1':'0';
	}
	glutPostRedisplay ();
}

void initialize() {
	glutInitDisplayMode (GLUT_RGBA|GLUT_DOUBLE);
	glutInitWindowSize (columnas*70,filas*70);
	glutInitWindowPosition (100,100);
	glutCreateWindow ("Ventana OpenGL");
	glutDisplayFunc (display_cb);
	glutReshapeFunc (reshape_cb);
	glutKeyboardFunc(&window_key);
	glutMouseFunc (&OnMouseClick);
	glClearColor(1.f,1.f,1.f,1.f);
}

int main (int argc, char **argv) {
	
	cout<<"Ingresa el numero de filas: ";
	cin>>filas;
	cout<<"Ingresa el numero de columnas: ";
	cin>>columnas;
	for (int i=0;i<filas;++i)
		for (int j=0;j<columnas;++j)
			matriz[i][j]='0';
	glutInit (&argc, argv);
	initialize();
	glutMainLoop();
	return 0;
}
