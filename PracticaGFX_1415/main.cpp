
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <string>
#include <GL/glew.h>
#include <GL/freeglut.h>


using namespace std;

/* Array de posicions dels vertex del triangle */
GLfloat triangleVertices[] = {
     0.0,  0.8,
    -0.8, -0.8,
     0.8, -0.8
};

/* Index del polygon del triangle */
GLuint triangleIndices[] = {
     0,1,2
};

/* Objectes per emmagatzemar la informacio del triangle */
GLuint vboVerticesID, vboIndicesID, vaoID;


GLuint shaderProgram;
GLuint vertexShader;
GLuint fragmentShader;



/*
Funcio per facilitar la carrega de fitxers.
*/
const char * LoadFromFile(const string filename){
	ifstream fp;
	fp.open(filename.c_str(), ios_base::in);
	if(fp) {		 
		string line, buffer;
		while(getline(fp, line)) {
			buffer.append(line);
			buffer.append("\r\n");
		}		 
		return strdup(buffer.c_str());
	} else {
		cerr<<"Error loading shader: "<<filename<<endl;
		return NULL;
	}
}
 
/*
Aquesta funcio crea tots els elements de openGL necesaris
com ara shon shaders, vertex buffer objects o altres.
*/
void initResources()
{
	/* Creem la shader carregantla del fitxer */
	vertexShader = glCreateShader (GL_VERTEX_SHADER);
	const char * shaderSource = LoadFromFile("shader.vert");
	glShaderSource (vertexShader, 1, &shaderSource, NULL);
	GLint status;
	glCompileShader (vertexShader);
	glGetShaderiv (vertexShader, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE) {
		GLint infoLogLength;		
		glGetShaderiv (vertexShader, GL_INFO_LOG_LENGTH, &infoLogLength);
		GLchar *infoLog= new GLchar[infoLogLength];
		glGetShaderInfoLog (vertexShader, infoLogLength, NULL, infoLog);
		cerr<<"Compile log: "<<infoLog<<endl;
		delete [] infoLog;
	}
	free((char *)shaderSource);


	fragmentShader = glCreateShader (GL_FRAGMENT_SHADER);
	const char * shaderSource2 = LoadFromFile("shader.frag");
	glShaderSource (fragmentShader, 1, &shaderSource2, NULL);
	

	glCompileShader (fragmentShader);
	glGetShaderiv (fragmentShader, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE) {
		GLint infoLogLength;		
		glGetShaderiv (vertexShader, GL_INFO_LOG_LENGTH, &infoLogLength);
		GLchar *infoLog= new GLchar[infoLogLength];
		glGetShaderInfoLog (fragmentShader, infoLogLength, NULL, infoLog);
		cerr<<"Compile log: "<<infoLog<<endl;
		delete [] infoLog;
	}
	free((char *)shaderSource2);

	/* Creem el programa unint les dues shaders */
	shaderProgram = glCreateProgram();
	glAttachShader (shaderProgram, vertexShader);
	glAttachShader (shaderProgram, fragmentShader);


	glLinkProgram (shaderProgram);
	glGetProgramiv (shaderProgram, GL_LINK_STATUS, &status);
	if (status == GL_FALSE) {
		GLint infoLogLength;
		
		glGetProgramiv (shaderProgram, GL_INFO_LOG_LENGTH, &infoLogLength);
		GLchar *infoLog= new GLchar[infoLogLength];
		glGetProgramInfoLog (shaderProgram, infoLogLength, NULL, infoLog);
		cerr<<"Link log: "<<infoLog<<endl;
		delete [] infoLog;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);


	/* Carreguem la infomacio del triangle en els objectes corresponents */
	glGenVertexArrays(1, &vaoID);
	glGenBuffers (1, &vboVerticesID);
	glGenBuffers (1, &vboIndicesID);

	glBindVertexArray(vaoID);
		glBindBuffer (GL_ARRAY_BUFFER, vboVerticesID);
		glBufferData (GL_ARRAY_BUFFER, sizeof(triangleVertices), &triangleVertices[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(glGetAttribLocation(shaderProgram,"coord2d"));
		glVertexAttribPointer (glGetAttribLocation(shaderProgram,"coord2d"), 2, GL_FLOAT, GL_FALSE,sizeof(GLfloat)*2,0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIndicesID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(triangleIndices), &triangleIndices[0], GL_STATIC_DRAW);
	
	glBindVertexArray(0);

}

/* 
Funcio de pintant que serveix per mostrar per pantalla el
que nosaltres desitjem. 
*/
void onDisplay()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glBindVertexArray(vaoID);
	glUseProgram(shaderProgram);			
		
	glDrawElements(GL_TRIANGLES, sizeof(triangleIndices)/sizeof(triangleIndices[0]), GL_UNSIGNED_INT, 0);

	glUseProgram(0);
	glBindVertexArray(0);

	glutSwapBuffers();
}
 
/*
Funcio encarregada d'alliberar els recursos pels quals hem demanat memoria
*/
void freeResources()
{
	glDeleteBuffers(1, &vboVerticesID);
	glDeleteBuffers(1, &vboIndicesID);
	glDeleteVertexArrays(1, &vaoID);
	
}
 

int main(int argc, char* argv[])
{
	/* Inicialitzem el entorn de glut per tal de crear una finestra */
	glutInit(&argc, argv);
	glutInitContextVersion(3,0);
	glutInitDisplayMode(GLUT_RGBA|GLUT_DOUBLE);
	glutInitWindowSize(640, 480);
	glutCreateWindow("Practica GFX 1");
 
	/* Inicialitzem OpenGL */
	GLenum glew_status = glewInit();
	if (glew_status != GLEW_OK){
		fprintf(stderr, "Error: %s\n", glewGetErrorString(glew_status));
		return EXIT_FAILURE;
	}
 
	/* Carreguem tots els recursos */
	initResources();
  
    /* Preparem les funcions de callback i iniciem el bucle principal */
	glutDisplayFunc(onDisplay);
	glutMainLoop();
  
 
	/* Un cop hem acabat alliberem memoria */
	freeResources();
	return EXIT_SUCCESS;
}