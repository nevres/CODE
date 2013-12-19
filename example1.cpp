// Display a color cube
//
// Colors are assigned to each vertex and then the rasterizer interpolates
//   those colors across the triangles.  We us an orthographic projection
//   as the default projetion.

#include "Angel.h"
#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>    

#define MAX 61  // 30 * 2 + 1
#define CELL 900  // 30 * 30
#define WALL 1
#define PATH 0

typedef Angel::vec4  color4;
typedef Angel::vec4  point4;

int maze[MAX][MAX];
const int NumVertices = 4356;

point4 points[NumVertices];
color4 colors[NumVertices];


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

int is_closed(int maze[MAX][MAX], int x, int y)
{
    if(maze[x - 1][y]  == WALL
       && maze[x][y - 1] == WALL
       && maze[x][y + 1] == WALL
       && maze[x + 1][y] == WALL
    )
        return 1;
 
    return 0;
}

void init_maze(int maze[MAX][MAX])
{
     for(int a = 0; a < MAX; a++)
     {
         for(int b = 0; b < MAX; b++)
         {
             if(a % 2 == 0 || b % 2 == 0)
                 maze[a][b] = 1;
             else
                 maze[a][b] = PATH;
         }
     }
}
 
void maze_generator(int indeks, int maze[MAX][MAX], int backtrack_x[CELL], int backtrack_y[CELL], int x, int y, int n, int visited)
{
    if(visited < n * n)
    {
        int neighbour_valid = -1;
        int neighbour_x[4];
        int neighbour_y[4];
        int step[4];
 
        int x_next;
        int y_next;
 
        if(x - 2 > 0 && is_closed(maze, x - 2, y))  // upside
        {
            neighbour_valid++;
            neighbour_x[neighbour_valid]=x - 2;;
            neighbour_y[neighbour_valid]=y;
            step[neighbour_valid]=1;
        }
 
        if(y - 2 > 0 && is_closed(maze, x, y - 2))  // leftside
        {
            neighbour_valid++;
            neighbour_x[neighbour_valid]=x;
            neighbour_y[neighbour_valid]=y - 2;
            step[neighbour_valid]=2;
        }
 
        if(y + 2 < n * 2 + 1 && is_closed(maze, x, y + 2))  // rightside
        {
            neighbour_valid++;
            neighbour_x[neighbour_valid]=x;
            neighbour_y[neighbour_valid]=y + 2;
            step[neighbour_valid]=3;
 
        }
 
        if(x + 2 < n * 2 + 1 && is_closed(maze, x + 2, y))  // downside
        {
            neighbour_valid++;
            neighbour_x[neighbour_valid]=x+2;
            neighbour_y[neighbour_valid]=y;
            step[neighbour_valid]=4;
        }
 
        if(neighbour_valid == -1)
        {
            // backtrack
            x_next = backtrack_x[indeks];
            y_next = backtrack_y[indeks];
            indeks--;
        }
 
        if(neighbour_valid!=-1)
        {
            int randomization = neighbour_valid + 1;
            int random = rand()%randomization;
            x_next = neighbour_x[random];
            y_next = neighbour_y[random];
            indeks++;
            backtrack_x[indeks] = x_next;
            backtrack_y[indeks] = y_next;
 
            int rstep = step[random];
 
            if(rstep == 1)
                maze[x_next+1][y_next] = PATH;
            else if(rstep == 2)
                maze[x_next][y_next + 1] = PATH;
            else if(rstep == 3)
                maze[x_next][y_next - 1] = PATH;
            else if(rstep == 4)
                maze[x_next - 1][y_next] = PATH;
            visited++;
        }
 
        maze_generator(indeks, maze, backtrack_x, backtrack_y, x_next, y_next, n, visited);
    }
}

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

	
	Index = 0;
	int counter = 0;
	int x = 0, y = 0;

	for (GLfloat i = -1; i < 0.9; i+=0.18) // for y
	{
		x = 0;
		for (GLfloat j = -1; j < 0.9; j+=0.18)// for x
		{

			//std::cout<<j<<"    "<<i<<std::endl;
			point4 pointZero =point4( j, i,  0.1, 1.0 ); 
			point4 pointOne = point4( j,  i +0.181,  0.1, 1.0 ); 
			point4 pointTwo = point4(  j+0.181,  i+0.181,  0.1, 1.0 ); 
			point4 pointTree =point4(  j+0.181, i,  0.1, 1.0 ); 
			point4 pointFour =point4( j, i,  -0.1, 1.0 ); 
			point4 pointFive =point4( j,  i+0.181,  -0.1, 1.0 ); 
			point4 pointSix = point4(  j+0.181,  i+0.181,  -0.1, 1.0 ); 
			point4 pointSeven=point4(  j+0.181, i,  -0.1, 1.0 ); 

			std::cout<<maze[x][y];
			quad( pointOne, pointZero, pointTree, pointTwo, 1, 0, 3, 2 ); // front side 
			if(maze[x][y]){
			quad( pointTwo, pointTree, pointSeven, pointSix, 2, 3, 7, 6 ); //right side
			quad( pointTree, pointZero, pointFour, pointSeven, 3, 0, 4, 7 ); // down side
			quad( pointSix, pointFive, pointOne, pointTwo, 6, 5, 1, 2 ); // up side
			quad( pointFour, pointFive, pointSix, pointSeven, 4, 5, 6, 7 ); // back side   ok
			quad( pointFive, pointFour, pointZero, pointOne, 5, 4, 0, 1 ); // left side
			}
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
	
    int size = 5;
    int indeks = 0;
    int backtrack_x[CELL];
    int backtrack_y[CELL];
    init_maze(maze);
    backtrack_x[indeks] = 1;
    backtrack_y[indeks] = 1;
    maze_generator(indeks, maze, backtrack_x, backtrack_y, 1, 1, size, 1);
	
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
