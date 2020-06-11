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
#include <vector>
#include <bits/stdc++.h> 
#include <math.h>
#include <string>

using namespace std;
bool matriz[20][20]={};//matriz ligada a la interfaz interactiva
//tamaño de la matriz
int filas;
int columnas;

struct perceptron{
	int validos[10];//cantidad de patrones analizados para cada número
	vector<double> pesos[10];//matriz de pesos, un vector para cada una de las 10 neuronas
	double bias;
	void extraerDatos(int i, vector<vector<bool> > &matricesEntrenamiento, vector<int> &correspondiente){
		//busca todos los archivos de la forma [0-9].txt y los carga en los parametros por referencia
		//es llamado por la funcion void entrenar
		validos[i]=0;
		ifstream ifs;
		ifs.open ( to_string(i)+".txt", std::ifstream::in);
		string aux;
		ifs>>aux;
		while (ifs.good()) {
			int n= stoi(aux);
			ifs>>aux;
			int m= stoi(aux);
			if(n==filas && m==columnas){
				validos[i]++;
				matricesEntrenamiento.push_back(vector<bool>(filas*columnas,0));
				correspondiente.push_back(i);
				for(int j=0;j<n;++j){
					ifs>>aux;
					cout << aux<<endl;
					for(int k=0;k<m;++k){
						matricesEntrenamiento.back()[j*m+k]=(aux[k]=='1');
					}
				}
			}else{
				for(int j=0;j<n;++j){
					ifs>>aux;
				}
			}
			ifs>>aux;
		}
		ifs.close();
	}
	void entrenar(double tasaAprendizaje){
		//reinicia todos los pesos y el bias
		bias=0.0;
		for(int i=0;i<10;++i){
			pesos[i].resize(filas*columnas,0.0);
		}
		
		//extrae los patrones de los archivos txt
		vector<vector<bool> > matricesEntrenamiento;
		vector<int> correspondiente;
		for(int i=0;i<10;++i){
			extraerDatos(i,matricesEntrenamiento,correspondiente);
		}
		
		//entrenamiento para cada uno de las 10 neuronas, solo si tiene patrones validos
		for(int i=0;i<10;++i){
			if(validos[i]>0){
				bool algunoNoCoincide=true;
				
				while(algunoNoCoincide){
					algunoNoCoincide=false;
					for(auto it=0;it<matricesEntrenamiento.size();it++){
						double resultado=bias;
						for(int j=0;j<matricesEntrenamiento[it].size();++j){
							resultado+=matricesEntrenamiento[it][j]*pesos[i][j];
							
						}
						bool y=(resultado>0);
						if(y!=(i==correspondiente[it]) ){
							algunoNoCoincide=true;
							//reajuste
							int diferencia=1;
							if(y)
								diferencia=-1;
							
							bias+=diferencia;
							for(int j=0;j<matricesEntrenamiento[it].size();++j){
								pesos[i][j]+=diferencia*tasaAprendizaje*matricesEntrenamiento[it][j];
							}
						}
					}	
				}
				cout<<i<<" Entrenado";
			}
		}
	}
	vector<int> reconocer(){
		vector<int> posibles;
		for(int k=0;k<10;++k){
			if(validos[k]>0){
				double resultado=bias;
				for(int i=0;i<filas;++i){
					for(int j=0;j<columnas;++j){
						resultado+=pesos[k][i*columnas+j]*matriz[i][j];
					}
				}
				if(resultado>0)
					posibles.push_back(k);
			}
		}
		return posibles;
	}
};

perceptron percep;


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
			glColor3f(0,0,0); glLineWidth(3);
			glBegin(GL_POLYGON);
			glVertex2d(70*j,70*i);
			glVertex2d(70*(j+1),70*i);
			glVertex2d(70*(j+1),70*(i+1));
			glVertex2d(70*j,70*(i+1));
			glEnd();
			if(!matriz[filas-i-1][j]){
				glBegin(GL_POLYGON);
				glColor3f(1,1,1); glLineWidth(3);
				glVertex2d(70*j+2,70*i+2);
				glVertex2d(70*(j+1)-2,70*i+2);
				glVertex2d(70*(j+1)-2,70*(i+1)-2);
				glVertex2d(70*j+2,70*(i+1)-2);
				glEnd();
			}
		}
	}
	vector<int> posibles=percep.reconocer();
	glColor3f(1,0,0); glLineWidth(3);
	for(int i=0;i<posibles.size(); ++i){
		cout<<posibles[i]<<endl;
		glRasterPos3f(70*columnas+10, (filas*35)-i*50, 0);
		string aux="Se Reconoce: " +to_string(posibles[i]);
		for (int j = 0; j < aux.length(); j++) {
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, aux[j]);  // Updates the position
		}
	}
	glutSwapBuffers();
}





GLvoid OnMouseClick (int button, int state, int x, int y) {
	if ( (button == GLUT_LEFT_BUTTON or button == GLUT_RIGHT_BUTTON) && state == GLUT_DOWN ) {		
		x/=70;
		y/=70;
		//cout<<y<<"  "<<x<<endl;
		matriz[y][x]= !matriz[y][x];
	}
	glutPostRedisplay ();
}

void initialize() {
	glutInitDisplayMode (GLUT_RGBA|GLUT_DOUBLE);
	glutInitWindowSize (columnas*70+200,filas*70);
	glutInitWindowPosition (100,100);
	glutCreateWindow ("Ventana OpenGL");
	glutDisplayFunc (display_cb);
	glutReshapeFunc (reshape_cb);
	//glutKeyboardFunc(&window_key);
	glutMouseFunc (&OnMouseClick);
	glClearColor(1.f,1.f,1.f,1.f);
}

int main (int argc, char **argv) {
	
	cout<<"Ingresa el numero de filas: ";
	cin>>filas;
	cout<<"Ingresa el numero de columnas: ";
	cin>>columnas;
	percep.entrenar(0.5);
	glutInit (&argc, argv);
	initialize();
	glutMainLoop();
	return 0;
}

