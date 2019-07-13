#include <iostream>
#include <math.h>

#if defined(__APPLE__) && defined(__MACH__)
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "delfem2/vec3.h"
#include "delfem2/mat3.h"
#include "delfem2/v23m3q.h"

#include "delfem2/funcs_gl.h"
#include "delfem2/gl_color.h"
#include "delfem2/funcs_glut.h"

class CRigidBodyState
{
public:
  CVector3 pos;
  CMatrix3 R;
  CVector3 velo;
  CVector3 Omega;
public:
  CRigidBodyState Step(double dt, const std::vector<CVector3>& vOpA) const {
    CRigidBodyState rb_out;
    rb_out.velo    = velo  + dt*vOpA[0];
    rb_out.Omega   = Omega + dt*vOpA[1];
    rb_out.pos     = pos   + dt*vOpA[2];
    CMatrix3 dR = Mat3_RotCartesian(dt*vOpA[3]);
    if( dR.isNaN() ) dR.SetZero();
    rb_out.R  = R*dR;
    return rb_out;
  }
};

class CRigidBodyInertia
{
public:
  double mass;
  CMatrix3 Irot;
  CMatrix3 invIrot;
};

class CRigidBodyForceModel
{
public:
  void GetForceTorque(CVector3& F, CVector3& T) const{
    F.SetZero();
    T.SetZero();
  }
};

std::vector<CVector3> VelocityRigidBody
(const CRigidBodyState& rbs,
 const CRigidBodyInertia& rbi,
 const CRigidBodyForceModel& rbfm)
{
  CVector3 F,T;
  rbfm.GetForceTorque(F,T);
  std::vector<CVector3> V(4);
  V[0] = (rbs.R*F)*(1.0/rbi.mass);
  V[1] = rbi.invIrot*( (rbs.Omega^(rbi.Irot*rbs.Omega)) + T);
  V[2] = rbs.velo;
  V[3] = rbs.Omega;
  return V;
}

CRigidBodyState StepTime_ForwardEuler
(double dt,
 const CRigidBodyState& rbIn, // current rigid body
 const CRigidBodyInertia& rbInertia,
 const CRigidBodyForceModel& rbForceModel)
{
  const std::vector<CVector3>& velo_vOpA = VelocityRigidBody(rbIn, rbInertia, rbForceModel);
  return rbIn.Step(dt, velo_vOpA);
}

CRigidBodyState StepTime_RungeKutta4
(double dt,
 const CRigidBodyState& rb0, // current rigid body
 const CRigidBodyInertia& rbInertia,
 const CRigidBodyForceModel& rbForceModel)
{
  const std::vector<CVector3>& vrb1 = VelocityRigidBody(rb0, rbInertia, rbForceModel);
  const CRigidBodyState& rb1 = rb0.Step(dt*0.5, vrb1);
  const std::vector<CVector3>& vrb2 = VelocityRigidBody(rb1, rbInertia, rbForceModel);
  const CRigidBodyState& rb2 = rb0.Step(dt*0.5, vrb2);
  const std::vector<CVector3>& vrb3 = VelocityRigidBody(rb2, rbInertia, rbForceModel);
  const CRigidBodyState& rb3 = rb0.Step(dt*1.0, vrb3);
  const std::vector<CVector3>& vrb4 = VelocityRigidBody(rb3, rbInertia, rbForceModel);
  std::vector<CVector3> vrb1234(4);
  vrb1234[0] = vrb1[0]+2*vrb2[0]+2*vrb3[0]+vrb4[0];
  vrb1234[1] = vrb1[1]+2*vrb2[1]+2*vrb3[1]+vrb4[1];
  vrb1234[2] = vrb1[2]+2*vrb2[2]+2*vrb3[2]+vrb4[2];
  vrb1234[3] = vrb1[3]+2*vrb2[3]+2*vrb3[3]+vrb4[3];
  return rb0.Step(dt/6.0, vrb1234);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

// display data
bool is_animation;

CGlutWindowManager window;

double dt;
CRigidBodyState rbs;
CRigidBodyInertia rbi;
CRigidBodyForceModel rbfm;


void myGlutDisplay(void)
{
  //	::glClearColor(0.2f, 0.7f, 0.7f ,1.0f);
  ::glClearColor(1.0f, 1.0f, 1.0f ,1.0f);
  ::glClearStencil(0);
  ::glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
  ::glEnable(GL_DEPTH_TEST);
  
  ::glEnable(GL_POLYGON_OFFSET_FILL );
  ::glPolygonOffset( 1.1f, 4.0f );
  window.SetGL_Camera();
  
  DrawBackground();
  
  ::glMatrixMode(GL_MODELVIEW);
  ::glPushMatrix();
  {
    double mMV[16] = {1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1};
    rbs.R.AffineMatrixTrans(mMV);
    ::glMultMatrixd(mMV);
  }
  ::glColorMaterial(GL_FRONT_AND_BACK,GL_DIFFUSE);
  ::glDisable(GL_LIGHTING);
  ::glColor3d(0,0,0);
  ::glutWireTeapot(1.01);
  ::glEnable(GL_LIGHTING);
  ::glColor3d(1,1,1);
  ::glutSolidTeapot(1.0);
  ::glPopMatrix();
  
  ShowFPS();
  ::glutSwapBuffers();
}

void myGlutIdle(){
  
  if( is_animation ){
//    rbs = StepTime_ForwardEuler(dt, rbs, rbi, rbfm);
    rbs = StepTime_RungeKutta4(dt, rbs, rbi, rbfm);
  }
  ::glutPostRedisplay();
}


void myGlutResize(int w, int h)
{
  ::glViewport(0,0,w,h);
  ::glutPostRedisplay();
}

void myGlutSpecial(int Key, int x, int y)
{
  window.glutSpecial(Key, x, y);
  ::glutPostRedisplay();
}

void myGlutMotion( int x, int y )
{
  window.glutMotion(x, y);
  ::glutPostRedisplay();
}

void myGlutMouse(int button, int state, int x, int y)
{
  window.glutMouse(button, state, x, y);
  ::glutPostRedisplay();
}

void myGlutKeyboard(unsigned char Key, int x, int y)
{
  switch(Key)
  {
    case 'q':
    case 'Q':
    case '\033':
      exit(0);  /* '\033' ? ESC ? ASCII ??? */
    case 'a':
      is_animation = !is_animation;
      break;
      
    case '1':
      break;
    case '2':
      break;
    case '3':
      break;
    case '4':
      break;
    case 'i': // one iteration
      break;
    case 'd': // change draw mode
      break;
    case 'f': //
      break;
    case 's': //
      break;
    case ' ':
    {
      static int ifile = 0;
      ifile++;
      if( ifile >= 8 ){ ifile=0; }
    }
  }
  ::glutPostRedisplay();
}


int main(int argc,char* argv[])
{
  glutInit(&argc, argv);
  
  // Initialize GLUT window 3D
  glutInitWindowPosition(200,200);
  glutInitWindowSize(400, 300);
  glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA|GLUT_DEPTH|GLUT_STENCIL);
  glutCreateWindow("3D View");
  glutDisplayFunc(myGlutDisplay);
  glutIdleFunc(myGlutIdle);
  glutReshapeFunc(myGlutResize);
  glutMotionFunc(myGlutMotion);
  glutMouseFunc(myGlutMouse);
  glutKeyboardFunc(myGlutKeyboard);
  glutSpecialFunc(myGlutSpecial);
  
  ////////////////////////
  
  rbi.mass = 1.0;
  {
    rbi.Irot = CMatrix3::Zero();
    CVector3 ex(1,0,0), ey(0,1,0), ez(0,0,1);
    rbi.Irot += 1.0*Mat3_OuterProduct(ex,ex);
    rbi.Irot += 3.0*Mat3_OuterProduct(ey,ey);
    rbi.Irot += 5.0*Mat3_OuterProduct(ez,ez);
  }
  rbi.invIrot = rbi.Irot.Inverse();
  
  rbs.pos = CVector3(0,0,0);
  rbs.R = CMatrix3::Identity();
  rbs.velo = CVector3(0,0,0);
  rbs.Omega = CVector3(1,1,1);
  
  dt = 0.05;
  
  ////////////////////////
  
  window.camera.view_height = 2.0;
  setSomeLighting();
  
  glutMainLoop();
  return 0;
}

