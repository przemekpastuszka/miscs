#include <GL/glfw.h>
#include <stdlib.h>

struct Ball
{
	float x, y;
	float dirX, dirY;
};

void DisplayPlayers(float posA, float posB, Ball ball)
{
	
	
	glColor3f(1, 1, 1);
	
	/*float vertices[] = { 0.1, 0.1, 0.1, 2 };
	glVertexPointer(2, GL_FLOAT, 4, vertices);
	glEnableClientState(GL_VERTEX_ARRAY);
	glDrawArrays(GL_LINES, 0, 2);*/
	glBegin(GL_LINES);
	glVertex2f(-0.9, posA - 1);
	glVertex2f(-0.9, posA - 1 + 0.2);
	glVertex2f(0.9, posB - 1 );
	glVertex2f(0.9, posB - 1 + 0.2);
	glEnd();
	
	glBegin(GL_QUADS);
	glVertex2f(ball.x - 1 - 0.025, ball.y - 1  - 0.025);
	glVertex2f(ball.x - 1 + 0.025, ball.y - 1 - 0.025);
	glVertex2f(ball.x - 1 + 0.025, ball.y - 1 + 0.025);
	glVertex2f(ball.x - 1 - 0.025, ball.y - 1 + 0.025);
	glEnd();
	
	
}


void MoveTheBall(float playerA, float playerB, Ball& ball)
{
	if(ball.x < 0 || ball.x > 2)
		ball.x = ball.y = 1;
	
	if(ball.y + 0.025 > 2 || ball.y < 0.025)
		ball.dirY *= -1;
	if(ball.x - 0.025 <= 0.1 && (ball.x > 0.1) &&
		((ball.y - 0.025 > playerA && ball.y - 0.025 < playerA + 0.2) ||
		(ball.y + 0.025 > playerA && ball.y + 0.025 < playerA + 0.2)))
			ball.dirX *= -1;
			
	if(ball.x + 0.025 >= 1.9 && (ball.x < 1.9) &&
		((ball.y - 0.025 > playerB && ball.y - 0.025 < playerB + 0.2) ||
		(ball.y + 0.025 > playerB && ball.y + 0.025 < playerB + 0.2)))
			ball.dirX *= -1;
	
	ball.x += ball.dirX * 0.0005;
	ball.y += ball.dirY * 0.0005;
}

int main( void )
{
	int running = GL_TRUE;
	// Initialize GLFW
	if( !glfwInit() )
	{
		exit( EXIT_FAILURE );
	}
	// Open an OpenGL window
	if( !glfwOpenWindow( 800,600, 0,0,0,0,0,0, GLFW_WINDOW ) )
	{
		glfwTerminate();
		exit( EXIT_FAILURE );
	}
	float posA = 0, posB = 0;
	Ball ball;
	ball.dirX = ball.dirY = -1;
	ball.x = 1;
	ball.y = 1.5;
	
	glViewport(0, 0, 400, 600);
	
	// Main loop
	while( running )
	{
		glClear( GL_COLOR_BUFFER_BIT );
		
		
		glViewport(0, 0, 400, 600);
		
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		DisplayPlayers(posA, posB, ball);
		
		glViewport(400, 0, 400, 600);
		glTranslatef(0, 0.5, 0);
		glRotatef(90, 0, 0, 1);
		glScalef(0.5, 1, 2);
		
		DisplayPlayers(posA, posB, ball);
		
		glfwSwapBuffers();
		
		if(glfwGetKey('S'))
			posA -= 0.001;
		if(glfwGetKey('W'))
			posA += 0.001;
		if(glfwGetKey('K'))
			posB -= 0.001;
		if(glfwGetKey('I'))
			posB += 0.001;
			
		if(posA < 0)
			posA = 0;
		if(posA + 0.2 > 2)
			posA = 1.8;
		if(posB < 0)
			posB = 0;
		if(posB + 0.2 > 2)
			posB = 1.8;
		
		MoveTheBall(posA, posB, ball);
		
		running = !glfwGetKey( GLFW_KEY_ESC ) &&
		glfwGetWindowParam( GLFW_OPENED );
	}
	// Close window and terminate GLFW
	glfwTerminate();
	// Exit program
	exit( EXIT_SUCCESS );
}

