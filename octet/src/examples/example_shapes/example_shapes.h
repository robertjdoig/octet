////////////////////////////////////////////////////////////////////////////////
//
// (C) Andy Thomason 2012-2014
//
// Modular Framework for OpenGLES2 rendering on multiple platforms.
//
namespace octet {

  /// Scene containing a box with octet.
  class example_shapes : public app {
    // scene for drawing box
    ref<visual_scene> app_scene_;

  public:
    example_shapes(int argc, char **argv) : app(argc, argv) {
    }

    ~example_shapes() {
    }

    float rotAngle = 45;
    btVector3 ballForce = btVector3(0, 0, 0);

    mesh_instance *m_ball; 
    btRigidBody *rb_ball; 

  

    /// this is called once OpenGL is initialized
    void app_init() {
      app_scene_ = new visual_scene();
      app_scene_->create_default_camera_and_lights();
      app_scene_->get_camera_instance(0)->get_node()->translate(vec3(0, 4, 0));

      material *red = new material(vec4(1, 0.5, 0, 1));
      material *green = new material(vec4(0.5, 1, 0, 1));
      material *blue = new material(vec4(0, 0, 1, 1));
      material *black = new material(vec4(0, 0, 0, 1));

      mat4t mat;
      drawSphere(mat, vec3(-10, 0, 0), vec3(1, 1, 1), 0.5f, vec4(1, 1, 0, 1), true);
      
      btRigidBody *rBa;

      mat.loadIdentity();
      mat.translate(vec3(-6, 0, 0));
      //material *locMat = new material(_col);
      mesh_instance *ma = app_scene_->add_shape(mat, new mesh_box(vec3(0.8f, 0.1f, 10)), red, false);
      rBa = ma->get_node()->get_rigid_body();

      btRigidBody *rBb;
      mat.loadIdentity();
      mat.translate(vec3(-5, 0, 0));
      //material *locMat = new material(_col);
      mesh_instance *m = app_scene_->add_shape(mat, new mesh_box(vec3(0.8f, 0.1f, 10)), red, true);
      rBb = m->get_node()->get_rigid_body();

      

     btPoint2PointConstraint pToPCon = btPoint2PointConstraint(*rBa, *rBb, btVector3(1,0,0),btVector3(-1,0,0));
      
      /*
      btRigidBody *rb;
      for (int i = 0; i < 7; i++) {
        mat.loadIdentity();
        mat.translate(vec3(-8 + (i * 2)));
        //material *locMat = new material(_col);
        mesh_instance *m = app_scene->add_shape(mat, new mesh_box(vec3(0.8f, 0.1f, 10)), red, true);
        rb = m->get_node()->get_rigid_body();
      }
      */



      // ground
      drawBox(mat, vec3(-10, 0, 0), vec3(3, 1, 50), vec4(0.2f, 1, 0.2f, 1), false); //left
      drawBox(mat, vec3(10, 0, 0), vec3(3, 1, 50), vec4(0.2f, 1, 0.2f, 1), false); //right

      drawBox(mat, vec3(0, -3, 0), vec3(50, 1, 50), vec4(0.2f, 0.2f, 1.0f, 1), false); //water

    }

    void drawBox(mat4t _mat, vec3 _pos, vec3 _size, vec4 _col, bool isMovable) {
      _mat.loadIdentity();
      _mat.translate(_pos);
      material *locMat = new material(_col);
      mesh_instance *m = app_scene_->add_shape(_mat, new mesh_box(_size), locMat, isMovable);
      btRigidBody *rb = m->get_node()->get_rigid_body();
    
    }

    void drawSphere(mat4t _mat, vec3 _pos, vec3 _size, float _radius, vec4 _col, bool isMovable) {
      _mat.loadIdentity();
      _mat.translate(_pos);
      material *locMat = new material(_col);
     //app_scene->forceShape(_mat, new mesh_sphere(_size, _radius), locMat, ballForce, btVector3(0, 0, 0), isMovable);
      m_ball = app_scene_->add_shape(_mat, new mesh_sphere(_size, _radius), locMat, isMovable);
      rb_ball = m_ball->get_node()->get_rigid_body();
    }

    void move_Ball() {
      printf("%f %f %f\n", ballForce.x(), ballForce.y(), ballForce.z());

      // left and right arrows
      if (is_key_down(key_left)) {
        ballForce += btVector3(-0.5f, 0, 0);
      }
      else if (is_key_down(key_right)) {
        ballForce += btVector3(0.5f, 0, 0);
      }
      else {
        ballForce = btVector3(0, 0, 0);
      }
      rb_ball->applyForce(ballForce, btVector3(0, 0, 0));
    }

    // this is called to draw the world
    void draw_world(int x, int y, int w, int h) {
     
      move_Ball();

      int vx = 0, vy = 0;
      get_viewport_size(vx, vy);
      app_scene_->begin_render(vx, vy);

      // update matrices. assume 30 fps.
      app_scene_->update(1.0f / 30);
      // draw the scene
      app_scene_->render((float)vx / vy);

    }
  };
}
