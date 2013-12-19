// Display a color cube
//
// Colors are assigned to each vertex and then the rasterizer interpolates
//   those colors across the triangles.  We us an orthographic projection
//   as the default projetion.

#include "Angel.h"
#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>    

typedef Angel::vec4  color4;
typedef Angel::vec4  point4;

const int NumVertices = 4000;//(6 faces)(2 triangles/face)(3 vertices/triangle)

point4 points[NumVertices];
color4 colors[NumVertices];

int maze[10][10];
// RGBA olors
color4 vertex_colors[8] = {
    color4( 0.0, 0.0, 0.0, 1.0 ),  // black
    color4( 1.0, 0.0, 0.0, 1.0 ),  // red
    color4( 1.0, 1.0, 0.0, 1.0 ),  // yellow
    color4( 0.0, 1.0, 0.0, 1.0 ),  // green
    color4( 0.0, 0.0, 1.0, 1.0 ),  // blue
    color4( 1.0, 0.0, 1.0, 1.0 ),  // magenta
    color4( 1.0, 1.0, 1.0, 1.0 ),  // white
    color4( 0.0, 1.0, 1.0, 1.0 )   // cyan
};

// Vertices of a unit cube centered at origin, sides aligned with axes


point4 vertices[8] = {
    point4( -0.05, -0.05,  0.05, 1.0 ),
    point4( -0.05,  0.05,  0.05, 1.0 ),
    point4(  0.05,  0.05,  0.05, 1.0 ),
    point4(  0.05, -0.05,  0.05, 1.0 ),
    point4( -0.05, -0.05, -0.05, 1.0 ),
    point4( -0.05,  0.05, -0.05, 1.0 ),
    point4(  0.05,  0.05, -0.05, 1.0 ),
    point4(  0.05, -0.05, -0.05, 1.0 )
};



int previousX = 0;
int previousY = 0;

int previous2X = 0;
int previous2Y = 0;

// Array of rotation angles (in degrees) for each coordinate axis
enum { Xaxis = 0, Yaxis = 1, Zaxis = 2, NumAxes = 3 };
int      Axis = Xaxis;
GLfloat  Theta[NumAxes] = { 0.0, 0.0, 0.0 };

GLuint  theta;  // The location of the "theta" shader uniform variable
GLuint toScale;
GLfloat ToScale = 1;
GLuint transform;
GLfloat Transform;
GLuint transform2;
GLfloat Transform2;
GLuint transform3;
GLfloat Transform3;


//----------------------------------------------------------------------------

// quad generates two triangles for each face and assigns colors
//    to the vertices
int Index;
void

quad( point4 a, point4 b, point4 c, point4 d , int a1, int b1, int c1, int d1)
{
    colors[Index] = vertex_colors[a1]; points[Index] = a; Index++;
    colors[Index] = vertex_colors[b1]; points[Index] = b; Index++;
    colors[Index] = vertex_colors[c1]; points[Index] = c; Index++;
    colors[Index] = vertex_colors[a1]; points[Index] = a; Index++;
    colors[Index] = vertex_colors[c1]; points[Index] = c; Index++;
    colors[Index] = vertex_colors[d1]; points[Index] = d; Index++;
}

// generate 12 triangles: 36 vertices and 36 colors
void
colorcube()
{

	for (int i = 0; i < 10; i++){
		for (int j = 0; j < 10; j++){
			maze[i][j] = rand() % 2;
		}
	}

	Index = 0;
	int counter = 0;
	int x = 0, y = 0;

	for (GLfloat i = -1; i < 0.8; i+=0.2) // for y
	{
		x = 0;
		for (GLfloat j = -1; j < 0.8; j+=0.2)// for x
		{
			//std::cout<<j<<"    "<<i<<std::endl;
			point4 pointZero =point4( j, i,  0.1, 1.0 ); 
			point4 pointOne = point4( j,  i +0.2,  0.1, 1.0 ); 
			point4 pointTwo = point4(  j+0.2,  i+0.2,  0.1, 1.0 ); 
			point4 pointTree =point4(  j+0.2, i,  0.1, 1.0 ); 
			point4 pointFour =point4( j, i,  -0.1, 1.0 ); 
			point4 pointFive =point4( j,  i+0.2,  -0.1, 1.0 ); 
			point4 pointSix = point4(  j+0.2,  i+0.2,  -0.1, 1.0 ); 
			point4 pointSeven=point4(  j+0.2, i,  -0.1, 1.0 ); 

			std::cout<<maze[x][y];
			quad( pointOne, pointZero, pointTree, pointTwo, 1, 0, 3, 2 ); // front side  ok
			if(maze[x][y]){
			quad( pointTwo, pointTree, pointSeven, pointSix, 2, 3, 7, 6 ); //right side
			quad( pointTree, pointZero, pointFour, pointSeven, 3, 0, 4, 7 ); // down side
			quad( pointSix, pointFive, pointOne, pointTwo, 6, 5, 1, 2 ); // up side
			}
			quad( pointFour, pointFive, pointSix, pointSeven, 4, 5, 6, 7 ); // back side   ok
			if(maze[x][y])
			quad( pointFive, pointFour, pointZero, pointOne, 5, 4, 0, 1 ); // left side
			
			x++;
		}
		std::cout<<std::endl;
		y++;
	}
}

//----------------------------------------------------------------------------

// OpenGL initialization
void
init()
{
    colorcube();

    // Create a vertex array object
    GLuint vao;
    glGenVertexArrays( 1, &vao );
    glBindVertexArray( vao );

    // Create and initialize a buffer object
    GLuint buffer;
    glGenBuffers( 1, &buffer );
    glBindBuffer( GL_ARRAY_BUFFER, buffer );
    glBufferData( GL_ARRAY_BUFFER, sizeof(points) + sizeof(colors),
 NULL, GL_STATIC_DRAW );
    glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(points), points );
    glBufferSubData( GL_ARRAY_BUFFER, sizeof(points), sizeof(colors), colors );

    // Load shaders and use the resulting shader program
    GLuint program = InitShader( "vshader36.glsl", "fshader36.glsl" );
    glUseProgram( program );

    // set up vertex arrays
    GLuint vPosition = glGetAttribLocation( program, "vPosition" );
    glEnableVertexAttribArray( vPosition );
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0,
  BUFFER_OFFSET(0) );

    GLuint vColor = glGetAttribLocation( program, "vColor" ); 
    glEnableVertexAttribArray( vColor );
    glVertexAttribPointer( vColor, 4, GL_FLOAT, GL_FALSE, 0,
  BUFFER_OFFSET(sizeof(points)) );

    theta = glGetUniformLocation( program, "theta" );
toScale = glGetUniformLocation(program, "toScale");
transform = glGetUniformLocation(program, "transform");
transform2 = glGetUniformLocation(program, "transform2");
transform3 = glGetUniformLocation(program, "transform3");
    glEnable( GL_DEPTH_TEST );
    glClearColor( 1.0, 1.0, 1.0, 1.0 );
}

//----------------------------------------------------------------------------

void
display( void )
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
glUniform1f(toScale,ToScale);
glUniform1f(transform,Transform);
glUniform1f(transform2,Transform2);
glUniform1f(transform3,Transform3);

    glUniform3fv( theta, 1, Theta );
    glDrawArrays( GL_TRIANGLES, 0, NumVertices );

    glutSwapBuffers();
}

//----------------------------------------------------------------------------

void
keyboard( unsigned char key, int x, int y )
{
    switch( key ) {
case 033: // Escape Key
case 'q': case 'Q':
   exit( EXIT_SUCCESS );
   break;
case 't':
Transform = 0.4;

case 's':
    
    ToScale  += 0.1;
    break;
case 'w':
    ToScale -= 0.1;
	break;
case 'k':
    Theta[Zaxis] -= 2;
	if ( Theta[Zaxis] <0 ) 
		Theta[Zaxis] += 360.0;
	break;
case 'l':
    Theta[Zaxis] += 2;
	if ( Theta[Zaxis] > 360.0 )
		Theta[Zaxis] -= 360.0;
	break;
	}
glutPostRedisplay();
}

//----------------------------------------------------------------------------

void
mouse( int button, int state, int x, int y )
{
	if ( state == GLUT_DOWN ) {
	switch( button ) {
	case 3:
		if(ToScale<=1.9){
		ToScale  += 0.1;
		break;}else
        break;	
	case 4: 
		if(ToScale >= 0.2){
		ToScale -= 0.1;
		break;}else break;
    
	case GLUT_LEFT_BUTTON:
		break;
    case GLUT_RIGHT_BUTTON:  
	
	int first =  20*(((float)x / 512) -0.5);
	Transform2 =-(float)first/10;
	int second =  20*(1-((float)y / 512) -0.5);;
	Transform3 = -(float)second/10;
	break;
		}
	}
glutPostRedisplay();
}


//----------------------------------------------------------------------------

void
idle( void )
{
    /*
Theta[Axis] += 0.5;

    if ( Theta[Axis] > 360.0 ) {
Theta[Axis] -= 360.0;
    }*/
    
    glutPostRedisplay();
}

void motion (int x, int y){

    if(y > previousY)
        Theta[Xaxis]-= 2;
    
    else Theta[Xaxis]+= 2;

    if(x > previousX)
        Theta[Yaxis]-= 2;

    else Theta[Yaxis]+= 2;


    previousX = x;
    previousY = y;
    glutPostRedisplay();
}

//----------------------------------------------------------------------------

int
main( int argc, char **argv )
{
	srand (time(NULL));
    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
    glutInitWindowSize( 512, 512 );
    //glutInitContextVersion( 3, 2 );
    //hoglutInitContextProfile( GLUT_CORE_PROFILE );
    glutCreateWindow( "Color Cube" );

    glewInit();

    init();

    glutDisplayFunc( display );
    glutKeyboardFunc( keyboard );
    glutMouseFunc( mouse );
    glutIdleFunc( idle );
    glutMotionFunc(motion);
    glutMainLoop();
    return 0;
}
