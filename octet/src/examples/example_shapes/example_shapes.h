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

    btTransform ctWorldTransform;
   
    mesh_instance *m_lBank; 
    btRigidBody *rb_lBank;

    int no_planks = 9;
    mesh_instance *m_bridge[9]; 
    btRigidBody *rb_bridge[9];

    mesh_instance *m_bridge1; 
    btRigidBody *rb_bridge1;

    mesh_instance *m_bridge2;
    btRigidBody *rb_bridge2;



    /// this is called once OpenGL is initialized
    void app_init() {
      app_scene_ = new visual_scene();
      app_scene_->create_default_camera_and_lights();
      app_scene_->get_camera_instance(0)->get_node()->translate(vec3(0, 4, 0));
      
      material *orange = new material(vec4(1, 0.5, 0, 1));
      material *green = new material(vec4(0.5, 1, 0, 1));
      material *blue = new material(vec4(0, 0, 1, 1));
      material *black = new material(vec4(0, 0, 0, 1));

      mat4t mat;
      drawSphere(mat, vec3(0, 5, 0), vec3(1, 1, 1), 0.5f, vec4(1, 1, 0, 1), true);
      
      mat.loadIdentity();
      mat.translate(vec3(-11, 0, 0));
      m_bridge[0] = app_scene_->add_shape(mat, new mesh_box(vec3(1, 1, 50)), green, false);
      rb_bridge[0] = m_bridge[0]->get_node()->get_rigid_body();
      
      for (int i = 1; i < no_planks-1; i++) {
        mat.loadIdentity();
        mat.translate(vec3(-8+(i*2), 0, 0));
        m_bridge[i] = app_scene_->add_shape(mat, new mesh_box(vec3(0.8f, 0.1f, 10)), orange, true);
        rb_bridge[i] = m_bridge[i]->get_node()->get_rigid_body();
    }
    
      mat.loadIdentity();
      mat.translate(vec3(11, 0, 0));
      m_bridge[no_planks-1] = app_scene_->add_shape(mat, new mesh_box(vec3(1, 1, 50)), green, false);
      rb_bridge[no_planks-1] = m_bridge[no_planks-1]->get_node()->get_rigid_body();


     // btRigidBody *rBa;
      /*
      mat.loadIdentity();
      mat.translate(vec3(-6, 0, 0));
      //material *locMat = new material(_col);
      m_bridge1 = app_scene_->add_shape(mat, new mesh_box(vec3(0.8f, 0.1f, 10)), red, true);
      rb_bridge1 = m_bridge1->get_node()->get_rigid_body();

     // btRigidBody *rBb;
      
      mat.loadIdentity();
      mat.translate(vec3(-4, 0, 0));
      //material *locMat = new material(_col);
      m_bridge2 = app_scene_->add_shape(mat, new mesh_box(vec3(0.8f, 0.1f, 10)), red, false);
      rb_bridge2 = m_bridge2->get_node()->get_rigid_body();
      */
      

     // btPoint2PointConstraint pToPCon = btPoint2PointConstraint(*rBa, *rBb, btVector3(1,0,0),btVector3(-1,0,0));
     //btHingeConstraint* hinge = new btHingeConstraint( 0.0f);
      
     // btHingeConstraint hinge = btHingeConstraint(rBa, rBb,new btTransform(0,btVector3(0,1,0)),btTransform(0,btVector3(0,1,0)));
     // btHingeConstraint hinge =  new btHingeConstraint(rBa, rBb, btTransform(), btTransform(),false);
    
   
     
      //ctWorldTransform.setOrigin(pivot);
      //ctWorldTransform.setRotation(rot);
     
     
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
     // drawBox(mat, vec3(-10, 0, 0), vec3(3, 1, 50), vec4(0.2f, 1, 0.2f, 1), false); //left
     // drawBox(mat, vec3(10, 0, 0), vec3(3, 1, 50), vec4(0.2f, 1, 0.2f, 1), false); //right

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
      //printf("%f %f %f\n", ballForce.x(), ballForce.y(), ballForce.z());

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

    void applyHinge() {
      //runs thru the array of the bridge to apply the hinge to each object 
      for (int i = 0; i < no_planks-1; i++) {
        app_scene_->applyHinge(rb_bridge[i], rb_bridge[i + 1], btVector3(1, 0, 0), btVector3(-1, 0, 0), btVector3(0, 0, 1), btVector3(0, 0, 1));
      }

      printf("End of Hinge Function \n");
    }

    // this is called to draw the world
    void draw_world(int x, int y, int w, int h) {
     
      move_Ball();
      applyHinge();

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
