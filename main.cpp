#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>

#include <stdio.h>

#include <math.h>
#include <string.h>


#define ESCAPE 27
GLuint texture;
GLuint texture1;
GLuint texture2;

GLint window; //binarni broj
GLint window2;
GLint Xsize=1500;
GLint Ysize=800;
float i,theta;
GLint nml=0,day=1;

char name3[]="PROJEKAT: 3D AUTO";

GLfloat xt=0.0,yt=0.0,zt=0.0,xw=0.0;   /* x,y,z translacija */
GLfloat tx=295,ty=62;
GLfloat xs=1.0,ys=1.0,zs=1.0;

GLfloat xangle=0.0,yangle=0.0,zangle=0.0,angle=0.0;   /* ugao za x,y,z */

GLfloat r=0,g=0,b=1;
GLint light=1;
int count=1,flg=1;
int view=0;
int flag1=0,aflag=1;            //to switch car driving mode
int flag2=0,wheelflag=0;   //to switch fog effect
GLUquadricObj *t;

//Funkcija za iscrtavanje texture
GLuint LoadTexture(const char* filename, int width, int height){
       GLuint texture;
       unsigned char* data;
       FILE* file;
       file=fopen(filename, "r");
       if(file==NULL)return 0;
       data=(unsigned char*)malloc(width * height * 3);
       fread(data,width * height * 3,1,file);
       fclose(file);
       glGenTextures(1,&texture);
       glBindTexture(GL_TEXTURE_2D,texture);
       glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
       //glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);
       glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
       glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
       glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
       glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
       glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,width,height,0,GL_RGB,GL_UNSIGNED_BYTE,data);
       free(data);
       return texture;
}

static void SpecialKeyFunc( int Key, int x, int y );

/* Simple  transformation routine */
GLvoid Transform(GLfloat Width, GLfloat Height)
{
  glViewport(0, 0, Width, Height);              /* Set the viewport */
  glMatrixMode(GL_PROJECTION);                  /* Select the projection matrix */
  glLoadIdentity();				/* Reset The Projection Matrix */
  gluPerspective(45.0,Width/Height,0.1,100.0);  /* Calculate The Aspect Ratio Of The Window */
  glMatrixMode(GL_MODELVIEW);                   /* Switch back to the modelview matrix */
}


/* A general OpenGL initialization function.  Sets all of the initial parameters. */
GLvoid InitGL(GLfloat Width, GLfloat Height)
{

  glClearColor(1.0, 1.0, 1.0, 1.0);
  glLineWidth(2.0);              /* Add line width,   ditto */
  Transform( Width, Height ); /* Perform the transformation */
  //newly added
  t=gluNewQuadric();
        gluQuadricDrawStyle(t, GLU_FILL);

glEnable(GL_LIGHTING);

glEnable(GL_LIGHT0);

// Create light components
GLfloat ambientLight[] = { 0.2f, 0.2f, 0.2f, 1.0f };
GLfloat diffuseLight[] = { 0.8f, 0.8f, 0.8, 1.0f };
GLfloat specularLight[] = { 0.5f, 0.5f, 0.5f, 1.0f };
GLfloat position[] = { 1.5f, 1.0f, 4.0f, 1.0f };

// Assign created components to GL_LIGHT0
glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
glLightfv(GL_LIGHT0, GL_POSITION, position);

}

/* The function called when our window is resized  */
GLvoid ReSizeGLScene(GLint Width, GLint Height)
{
  if (Height==0)     Height=1;                   /* Sanity checks */
  if (Width==0)      Width=1;
  Transform( Width, Height );                   /* Perform the transformation */
}

void init()
{
    glClearColor(0,0,0,0);
	glPointSize(5.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0,900.0,0.0,600.0,50.0,-50.0);
	glutPostRedisplay(); 		// request redisplay
}


/* The main drawing function

   In here we put all the OpenGL and calls to routines which manipulate
   the OpenGL state and environment.

   This is the function which will be called when a "redisplay" is requested.
*/

GLvoid DrawGLScene()
{


  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	/* Clear The Screen And The Depth Buffer */
  if(count==1)
	InitGL(Xsize,Ysize);
  if(aflag==1)/* Initialize our window. */
  glClearColor(1,1,1,1);
  else
	  glClearColor(0.1,0.1,0.1,0);
  glPushMatrix();
  glLoadIdentity();
  glTranslatef(-1.0,0.0,-3.5);
  glRotatef(xangle,1.0,0.0,0.0);
  glRotatef(yangle,0.0,1.0,0.0);
  glRotatef(zangle,0.0,0.0,1.0);
  glTranslatef(xt,yt,zt);
  glScalef(xs,ys,zs);
  glEnable(GL_COLOR_MATERIAL);
  glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

  if(flag2==1)
  {
  GLfloat fogcolour[4]={1.0,1.0,1.0,1.0};

  glFogfv(GL_FOG_COLOR,fogcolour);              /* Define the fog colour */
  glFogf(GL_FOG_DENSITY,0.1);                   /* How dense */
  glFogi(GL_FOG_MODE,GL_EXP);                   /* exponential decay */
  glFogf(GL_FOG_START,3.0);                   /* Where wwe start fogging */
  glFogf(GL_FOG_END,100.0);                       /* end */
  glHint(GL_FOG_HINT, GL_FASTEST);              /* compute per vertex */
  glEnable(GL_FOG);/* ENABLE */
  }
  if(flag2==0)
  {
	  glDisable(GL_FOG);
  }

if(!aflag){
  glBegin(GL_POINTS);
  glColor3f(1,1,1);
  glPointSize(200.0);
  int ccount=0;
  float x=10,y=10;
  while(ccount<20)
  {
	  glVertex2f(x,y);
	  x+=10;
	  y+=10;
	  if(y>Ysize) y-=10;
	  if(x>Xsize) x-=10;
	  ccount++;
  }
  glEnd();}

  glColor3f(1.0,.75,0.0);
  glPointSize(30.0);
  glBegin(GL_POINTS);
  glVertex3f(0.2,0.3,0.3);
  glVertex3f(0.2,0.3,0.5);
  glEnd();
  glPointSize(200.0);



  glBegin(GL_QUADS);

  /* Vrh auta-kocke*/

 glColor3f(r,g,b);
  glVertex3f( 0.2, 0.4,0.6);
  glVertex3f(0.6, 0.5,0.6);
  glVertex3f(0.6, 0.5,0.2);
  glVertex3f( 0.2,0.4,0.2);

  /* dno auta-kocke*/
  glVertex3f( 0.2,0.2,0.6);
  glVertex3f(0.6,0.2,0.6);
  glVertex3f(0.6,0.2,0.2);
  glVertex3f( 0.2,0.2,0.2);

  /* prednja strana*/
  glVertex3f( 0.2,0.2,0.6);
  glVertex3f(0.2, 0.4,0.6);
  glVertex3f(0.2,0.4,0.2);
  glVertex3f( 0.2,0.2,0.2);

  /* Zadnja strana.*/
  glVertex3f(0.6,0.2,0.6);
  glVertex3f(0.6,0.5,0.6);
  glVertex3f(0.6,0.5,0.2);
  glVertex3f( 0.6,0.2,0.2);

  // leva strana
  glVertex3f(0.2,0.2,0.6);
  glVertex3f(0.6,0.2,0.6);
  glVertex3f(0.6,0.5,0.6);
  glVertex3f(0.2,0.4,0.6);

  /* Desna strana */
  glVertex3f(0.2,0.2,0.2);
  glVertex3f( 0.6,0.2,0.2);
  glVertex3f( 0.6,0.5,0.2);
  glVertex3f( 0.2,0.4,0.2);
  glEnd();

  glEnable(GL_TEXTURE_2D);//Pokrenuti texture

//****************************************************************************
    //Krov auta
 glBindTexture(GL_TEXTURE_2D,texture);
glBegin(GL_QUADS);

 glColor3f(1.0,1.0,1.0);
 glTexCoord2d(0.0, 0.0); glVertex3f(0.7,0.65,0.6);
 glTexCoord2d(0.0, 1.0); glVertex3f(0.7,0.65,0.2);
 glTexCoord2d(1.0, 1.0); glVertex3f(1.7,0.65,0.2);
 glTexCoord2d(1.0, 0.0); glVertex3f(1.7,0.65,0.6);
glEnd();
	 glDisable(GL_TEXTURE_2D);
glBegin(GL_QUADS);
    //gornji deo gepeka
   glColor3f(r,g,b);
  glVertex3f( 1.8, 0.5,0.6);
  glVertex3f(1.8, 0.5,0.2);
  glVertex3f(2.1, 0.4, 0.2);
  glVertex3f(2.1,0.4,0.6);

 //Dno gepeka
  glVertex3f( 2.1,0.2,0.6);
  glVertex3f(2.1,0.2,0.2);
  glVertex3f(1.8,0.2,0.6);
  glVertex3f( 1.8,0.2,0.6);


  glVertex3f(2.1,0.4,0.6);
  glVertex3f(2.1,0.4,0.2);
  glVertex3f(2.1,0.2,0.2);
  glVertex3f(2.1,0.2,0.6);

  /* leva strana*/
 glVertex3f(1.8,0.2,0.2);
  glVertex3f(1.8,0.5,0.2);
  glVertex3f(2.1,0.4,0.2);
  glVertex3f(2.1,0.2,0.2);

  /* Right of cube */
  glVertex3f(1.8,0.2,0.6);
  glVertex3f(1.8,0.5,0.6);
  glVertex3f(2.1,0.4,0.6);
  glVertex3f(2.1,0.2,0.6);

  //Srednji deo

  //Prednja strana gde su vrata
 glVertex3f( 0.6, 0.5,0.6);
  glVertex3f(0.6, 0.2,0.6);
  glVertex3f(1.8, 0.2, 0.6);
  glVertex3f(1.8,0.5,0.6);

  /* bottom of cube*/

  glVertex3f( 0.6,0.2,0.6);
  glVertex3f(0.6,0.2,0.2);
  glVertex3f(1.8,0.2,0.2);
  glVertex3f( 1.8,0.2,0.6);

  /* back of cube.*/
   glVertex3f(0.6,0.5,0.2);
  glVertex3f(0.6,0.2,0.2);
  glVertex3f(1.8,0.2,0.2);
  glVertex3f(1.8,0.5,0.2);

 glColor3f(1.0,1.0,1.0);
  glVertex3f( 0.77, 0.63,0.2);
  glVertex3f(0.75, 0.5,0.2);
  glVertex3f(1.2, 0.5, 0.2);
  glVertex3f( 1.22,0.63,0.2);

  glVertex3f(1.27,0.63,.2);
  glVertex3f(1.25,0.5,0.2);
  glVertex3f(1.65,0.5,0.2);
  glVertex3f(1.67,0.63,0.2);

  glColor3f(r,g,b);
  glVertex3f(0.7,0.65,0.2);
  glVertex3f(0.7,0.5,.2);
  glVertex3f(0.75,0.5,0.2);
  glVertex3f(0.77,0.65,0.2);

  //glColor3f(1.0,1.0,1.0);
  glVertex3f(1.2,0.65,0.2);
  glVertex3f(1.2,0.5,.2);
  glVertex3f(1.25,0.5,0.2);
  glVertex3f(1.27,0.65,0.2);


 glVertex3f(1.65,0.65,0.2);
  glVertex3f(1.65,0.5,.2);
  glVertex3f(1.7,0.5,0.2);
  glVertex3f(1.7,0.65,0.2);
//Linije iznad prozora
  glVertex3f( 0.75, 0.65,0.2);
  glVertex3f(0.75, 0.63,0.2);
  glVertex3f(1.7, 0.63, 0.2);
  glVertex3f( 1.7,0.65,0.2);

//Linije iznad prozora
  glVertex3f( 0.75, 0.65,0.6);
  glVertex3f(0.75, 0.63,0.6);
  glVertex3f(1.7, 0.63, 0.6);
  glVertex3f( 1.7,0.65,0.6);

 glColor3f(1.0,1.0,1.0);
  glVertex3f( 0.77, 0.63,0.6);
  glVertex3f(0.75, 0.5,0.6);        //quad front window
  glVertex3f(1.2, 0.5, 0.6);
  glVertex3f( 1.22,0.63,0.6);

  glVertex3f(1.27,0.63,.6);
  glVertex3f(1.25,0.5,0.6);        //quad back window
  glVertex3f(1.65,0.5,0.6);
  glVertex3f(1.67,0.63,0.6);

  glColor3f(r,g,b);
  glVertex3f(0.7,0.65,0.6);
  glVertex3f(0.7,0.5,.6);       //first separation
  glVertex3f(0.75,0.5,0.6);
  glVertex3f(0.77,0.65,0.6);

  glVertex3f(1.2,0.65,0.6);
  glVertex3f(1.2,0.5,.6);       //second separation
  glVertex3f(1.25,0.5,0.6);
  glVertex3f(1.27,0.65,0.6);

  glVertex3f(1.65,0.65,0.6);
  glVertex3f(1.65,0.5,.6);
  glVertex3f(1.7,0.5,0.6);
  glVertex3f(1.7,0.65,0.6);
  glEnd();




glEnable(GL_TEXTURE_2D);
glBindTexture(GL_TEXTURE_2D,texture2);
glBegin(GL_QUADS);
 //prednje staklo
 glColor3f(0.3,0.3,0.3);
 glTexCoord2d(0.0,0.0);   glVertex3f( 0.6, 0.5,0.6);
  glTexCoord2d(0.0,220.0);  glVertex3f(0.6, 0.5,0.2);
 glTexCoord2d(220.0,220.0);   glVertex3f(0.7, 0.65, 0.2);
 glTexCoord2d(220.0,0.0);   glVertex3f( 0.7,0.65,0.6);

  //zadnje staklo
  glVertex3f(1.7,0.65,.6);
  glVertex3f(1.7,0.65,0.2);
  glVertex3f(1.8,0.5,0.2);
  glVertex3f(1.8,0.5,0.6);
glEnd();
glDisable(GL_TEXTURE_2D);

if(flag1)
  {
	glPushMatrix();
	glTranslatef(xw,0,0);
glEnd();

glEnable(GL_TEXTURE_2D);
glBindTexture(GL_TEXTURE_2D,texture1);
glBegin(GL_QUADS);

	//Livada preko puta
  glColor3f(0,1,0);
 glTexCoord2d(0.0,0.0); glVertex3f(-100,0.1,-100);
 glTexCoord2d(0.0,1900.0); glVertex3f(-100,0.1,0);
 glTexCoord2d(1900.0,500.0); glVertex3f(100,0.1,0);
 glTexCoord2d(500.0,0.0); glVertex3f(100,0.1,-100);
glDisable(GL_TEXTURE_2D);
//Desna strana puta
  glColor3f(0.7,0.7,0.7);
  glVertex3f(-100,0.1,0);
  glVertex3f(-100,0.1,0.45);
  glVertex3f(100,0.1,0.45);
  glVertex3f(100,0.1,0);

//Linija na put
  glColor3f(1.0,1.0,1.0);
  glVertex3f(-100,0.1,0.45);
  glVertex3f(-100,0.1,0.55);
  glVertex3f(100,0.1,0.55);
  glVertex3f(100,0.1,0.45);

  //Leva strana puta
  glColor3f(0.7,0.7,0.7);
  glVertex3f(-100,0.1,0.55);
  glVertex3f(-100,0.1,1);
  glVertex3f(100,0.1,1);
  glVertex3f(100,0.1,0.55);
  glEnd();

glEnable(GL_TEXTURE_2D);//Pokrenuti texture

glBindTexture(GL_TEXTURE_2D,texture1);
glBegin(GL_QUADS);
  //Trava prva
 glColor3f(0,1,0);
  glTexCoord2d(0.0, 0.0); glVertex3f(-100,0.1,1);
 glTexCoord2d(0.0, 1900.0);  glVertex3f(-100,0.1,100);
  glTexCoord2d(1900.0, 500.0);  glVertex3f(100,0.1,100);
 glTexCoord2d(500.0, 0.0); glVertex3f(100,0.1,1);
      glPopMatrix();
  glEnd();
  glDisable(GL_TEXTURE_2D);

  }
  glEnd();

  if(wheelflag)
  {
	  glPushMatrix();
	  glTranslatef(xw,0,0);
	  glColor3f(0.0,0.0,1.0);
	  glBegin(GL_QUADS);
	  for(i=0;i<200;i+=0.2)
	  {
	      //prvi blokovi do puta
		  glVertex3f(-100+i,0,1);
		  glVertex3f(-99.9+i,0,1);
		  glVertex3f(-99.9+i,0.2,1);
		  glVertex3f(-100+i,0.2,1);
		  i+=0.5;
	  }
	  for(i=0;i<200;i+=0.2)
	  {
		//Drugi blokovi do puta
		  glVertex3f(-100+i,0,0);
		  glVertex3f(-99.9+i,0,0);
		  glVertex3f(-99.9+i,0.2,0);
		  glVertex3f(-100+i,0.2,0);
		  i+=0.5;
	  }
	  glEnd();
	  glPopMatrix();
  }


glBegin(GL_TRIANGLES);


 //Mali prozorcici do soferfsajbne trouglovi
  glColor3f(0.3,0.3,0.3);
  glVertex3f( 0.6, 0.5,0.6);
  glVertex3f( 0.7,0.65,0.6);
  glVertex3f(0.7,0.5,0.6);

  glVertex3f( 0.6, 0.5,0.2);
  glVertex3f( 0.7,0.65,0.2);
  glVertex3f(0.7,0.5,0.2);

  glVertex3f( 1.7, 0.65,0.2);
  glVertex3f( 1.8,0.5,0.2);
  glVertex3f( 1.7,0.5,0.2);

  glVertex3f( 1.7, 0.65,0.6);
  glVertex3f( 1.8,0.5,0.6);
  glVertex3f(1.7,0.5,0.6);

glEnd();

glPushMatrix();

glColor3f(0.7,0.7,0.7);
   glTranslatef(1.65,0.2,0.3);
   glRotatef(90.0,0,1,0);
   gluCylinder(t,0.02,0.03,.5,10,10);
glPopMatrix();


glColor3f(0.7,0.7,0.7);
glPushMatrix();
  glBegin(GL_LINE_STRIP);
	  for(theta=0;theta<360;theta=theta+20)
	  {
  glVertex3f(0.6,0.2,0.62);
  glVertex3f(0.6+(0.08*(cos(((theta+angle)*3.14)/180))),0.2+(0.08*(sin(((theta+angle)*3.14)/180))),0.62);
	  }
glEnd();

glBegin(GL_LINE_STRIP);
	  for(theta=0;theta<360;theta=theta+20)
	  {
  glVertex3f(0.6,0.2,0.18);
  glVertex3f(0.6+(0.08*(cos(((theta+angle)*3.14)/180))),0.2+(0.08*(sin(((theta+angle)*3.14)/180))),0.18);
	  }
glEnd();

glBegin(GL_LINE_STRIP);
for(theta=0;theta<360;theta=theta+20)
	  {
  glVertex3f(1.7,0.2,0.18);
  glVertex3f(1.7+(0.08*(cos(((theta+angle)*3.14)/180))),0.2+(0.08*(sin(((theta+angle)*3.14)/180))),0.18);
	  }
glEnd();

glBegin(GL_LINE_STRIP);
for(theta=0;theta<360;theta=theta+20)
	  {
  glVertex3f(1.7,0.2,0.62);
 glVertex3f(1.7+(0.08*(cos(((theta+angle)*3.14)/180))),0.2+(0.08*(sin(((theta+angle)*3.14)/180))),0.62);
	  }
glEnd();
  glTranslatef(0.6,0.2,0.6);
  glColor3f(0,0,0);
  glutSolidTorus(0.025,0.07,10,25);

  glTranslatef(0,0,-0.4);
  glutSolidTorus(0.025,0.07,10,25);

  glTranslatef(1.1,0,0);
  glutSolidTorus(0.025,0.07,10,25);

  glTranslatef(0,0,0.4);
  glutSolidTorus(0.025,0.07,10,25);
  glPopMatrix();
//*************************************************************
  glPopMatrix();
  glEnable(GL_DEPTH_TEST);
  glutPostRedisplay();
  glutSwapBuffers();

}

void NormalKey(GLubyte key, GLint x, GLint y)
{
    switch ( key )    {
     case ESCAPE : printf("escape pressed. exit.\n");
	               glutDestroyWindow(window);
	               exit(0);
                                   break;


      case 'x': xangle += 5.0;
                      glutPostRedisplay();
                      break;

       case 'X':xangle -= 5.0;
                       glutPostRedisplay();
                       break;

      case 'y':
        yangle += 5.0;
        glutPostRedisplay();
        break;

     case 'Y':
        yangle -= 5.0;
        glutPostRedisplay();
        break;

     case 'z':
        zangle += 5.0;
        glutPostRedisplay();
        break;

     case 'Z':
        zangle -= 5.0;
        glutPostRedisplay();
        break;

      case 'u':                          /* Move up */
        yt += 0.2;
        glutPostRedisplay();
        break;

      case 'U':
        yt -= 0.2;                      /* Move down */
        glutPostRedisplay();
        break;

      case 'f':                          /* Move forward */
        zt += 0.2;
        glutPostRedisplay();
        break;

      case 'F':
        zt -= 0.2;                      /* Move away */
        glutPostRedisplay();
        break;

	  case 's':zs+=.2;
		  glutPostRedisplay();
		  break;

	  case 'S':zs-=0.2;
		  glutPostRedisplay();
		  break;

	  case 'a':ys+=.2;
		  glutPostRedisplay();
		  break;

	  case 'A':ys-=0.2;
		  glutPostRedisplay();
		  break;

	  case 'q':xs+=.2;
		  glutPostRedisplay();
		  break;

	  case 'Q':xs-=0.2;
		  glutPostRedisplay();
		  break;


     default:
	break;
    }

}

static void SpecialKeyFunc( int Key, int x, int y )
{
	switch ( Key ) {
	case GLUT_KEY_RIGHT:
		if(!wheelflag)
		xt += 0.2;
		if(wheelflag)
		{
		angle+=5;
		xw+=0.2;
		}
	    glutPostRedisplay();
		break;

                    case GLUT_KEY_LEFT:
		if(!wheelflag)
		xt -= 0.2;
		if(wheelflag)
		{
		angle+=5;
		xw-=0.2;
		}
        glutPostRedisplay();
		break;
	}
}

void myMenu(int id)
{
		if (id==1)
	{
		flag1=0;
		wheelflag=0;
	glutPostRedisplay();

	}
	if(id ==2)
	{
		flag1=1;
		flag2=0;
		wheelflag=0;
		xangle += 5.0;
		glutPostRedisplay();
	}
	if(id==3)
	{
		flag2=1;
		wheelflag=0;
		xangle += 5.0;
		glutPostRedisplay();
	}
	if (id==4)
	{
		 wheelflag=1;
		 glutPostRedisplay();
	}
	if (id==5)
	{
               if(day)
                {

		if(light)
		{
			count++;
			glDisable(GL_LIGHTING);
			glDisable(GL_LIGHT0);
			light=0;
		}
		else
		{
			count--;
			light=1;
			glEnable(GL_LIGHTING);
			glEnable(GL_LIGHT0);
		}
		glutPostRedisplay();
                }
               else
                {

                  if(nml==0 && flag2==2)
                     {
                       flag2=0;
                       nml=1;
                     }
                   else
                     {
                         flag2=2;
                 nml=0;

                 aflag=0;
                 day=0;

	         glClearColor(0.1,0.1,0.1,0);
	         GLfloat fogcolour[4]={0.0,0.0,0.0,1.0};

                 glFogfv(GL_FOG_COLOR,fogcolour);              /* Define the fog colour */
                 glFogf(GL_FOG_DENSITY,0.5);                   /* How dense */
                 glFogi(GL_FOG_MODE,GL_EXP);                   /* exponential decay */
                         /* end */
                glHint(GL_FOG_HINT, GL_FASTEST);              /* compute per vertex */
                glEnable(GL_FOG);


	        glutPostRedisplay();
                 }
             }

	}


       if(id==12)
	{
	aflag=1;
        day=1;
	glClearColor(1,1,1,1);
	glDisable(GL_FOG);
	glutPostRedisplay();
	}

	if(id==13)
	{
	aflag=0;
        day=0;
	flag2=2;
	glClearColor(0.1,0.1,0.1,0);
	GLfloat fogcolour[4]={0.0,0.0,0.0,1.0};

  glFogfv(GL_FOG_COLOR,fogcolour);              /* Define the fog colour */
  glFogf(GL_FOG_DENSITY,0.5);                   /* How dense */
  glFogi(GL_FOG_MODE,GL_EXP);                   /* exponential decay */
                         /* end */
  glHint(GL_FOG_HINT, GL_FASTEST);              /* compute per vertex */
  glEnable(GL_FOG);

  glutPostRedisplay();
       }
}

void colorMenu(int id)
{
		if (id==6)
	{
		r=1;
		g=0;
		b=1;
	glutPostRedisplay();

	}
	if(id ==7)
	{
		r=0.8;
		b=g=0;
		glutPostRedisplay();
	}
	if(id==8)
	{
	    g=1;
		r=0.3;
		b=0.8;
		glutPostRedisplay();
	}
	if (id==9)
	{
		 r=b=g=0;
		 glutPostRedisplay();
	}
	if(id==10)
	{
		b=0;
		r=g=1;
        glutPostRedisplay();
	}
	if(id==11)
	{
		b=r=g=.7;
        glutPostRedisplay();
	}

}

void myreshape(int w,int h)
{
        glViewport(0,0,w,h);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        if(w<=h)
                glOrtho(-2.0,2.0,-2.0*(GLfloat)h/(GLfloat)w,2.0*(GLfloat)h/(GLfloat)w,-10.0,10.0);
        else
                glOrtho(-2.0*(GLfloat)w/(GLfloat)h,2.0*(GLfloat)w/(GLfloat)h,-2.0,2.0,-10.0,10.0);
        glMatrixMode(GL_MODELVIEW);
        glutPostRedisplay();
}


int main(int argc, char **argv)
{
    //Kreiranje prozora

  glutInit(&argc, argv);

  glutInitDisplayMode(GLUT_RGBA |
                      GLUT_DOUBLE|
                      GLUT_DEPTH);

  glutInitWindowSize(Xsize,Ysize); //Velicina prozora 1500*800
  glutInitWindowPosition(0,0);

  glutCreateWindow("Voznja auta");//Kreiranje prozora

 texture=LoadTexture("C:/Users/Ajla/Desktop/ProjekatAuto/bin/Debug/drvo.raw",100, 100);

 texture1=LoadTexture("C:/Users/Ajla/Desktop/ProjekatAuto/bin/Debug/trava2.raw",100, 100);

 texture2=LoadTexture("C:/Users/Ajla/Desktop/ProjekatAuto/bin/Debug/staklo.raw",100, 100);

  glutReshapeFunc(myreshape);
  glutDisplayFunc(DrawGLScene);        //Funkcija za sva crtanja
  glutReshapeFunc(ReSizeGLScene);
  glutKeyboardFunc(NormalKey);         /*Normal key is pressed */
  glutSpecialFunc( SpecialKeyFunc );
  InitGL(Xsize,Ysize);
  int submenu=glutCreateMenu(colorMenu); //Kreiranje menija za boje
  glutAddMenuEntry("ljubicasta", 6);
	glutAddMenuEntry("crvena", 7);
	glutAddMenuEntry("zelena",8);
	glutAddMenuEntry("crna",9);
	glutAddMenuEntry("zuta",10);
	glutAddMenuEntry("siva",11);
  glutCreateMenu(myMenu);
	glutAddMenuEntry("Auto", 1);
	glutAddMenuEntry("Voznja auta", 2);
	glutAddMenuEntry("Magla",3);
	glutAddMenuEntry("Ivice puta",4);
	glutAddMenuEntry("osvetljenje",5);
	glutAddSubMenu("Boja auta",submenu);
	glutAddMenuEntry("Dan",12);
	glutAddMenuEntry("Noc",13);
	glutAttachMenu(GLUT_RIGHT_BUTTON);


  glutMainLoop();
  return 1;
}

