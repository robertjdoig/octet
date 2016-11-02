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

    texture_shader texture_shader_;

  public:
    example_shapes(int argc, char **argv) : app(argc, argv) {
    }

    ~example_shapes() {
    }

    float rotAngle = 45;
    btVector3 ballForce = btVector3(0, 0, 0);
    float ballMass = 1; 

    mesh_instance *m_ball; 
    btRigidBody *rb_ball; 

    btRigidBody *rb_swing;


    btTransform ctWorldTransform;
   
    int no_planks = 11;
    btRigidBody *rb_bridge[11];


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

      //Draw Ball
      drawSphere(mat, vec3(0, 8, 0), vec3(1, 1, 1), 0.5f, vec4(1, 1, 0, 1), true);
  
      //Left Bank
      rb_bridge[0] = drawBox(mat, vec3(-11, 3, 0), vec3(1, 1, 50), vec4(0.5, 1, 0, 1), false);
      
      //Run through and draw planks
      for (int i = 1; i < no_planks-1; i++)
        rb_bridge[i] = drawBox(mat, vec3(-8 + (i * 2), 3, 0), vec3(0.8f, 0.1f, 10), vec4(1, 0.5f, 0, 1), true);
      
      //Right Bank
      rb_bridge[no_planks-1] = drawBox(mat, vec3(11, 3, 0), vec3(1, 1, 50), vec4(0.5, 1, 0, 1), false);

      //Apply the spring to the rb_bridge array populated above
      applySpring();
   

      rb_swing = drawBox(mat, vec3(0, -2, 0), vec3(1, 1, 1), vec4(0, 1, 1, 1), true);
  
      app_scene_->applyHinge(rb_swing, btVector3(0, 6, 0), btVector3(0, 0, 1));
     // rb_swing->applyTorque(btVector3(0, 0, 10));


      //draw the water under the bridge
      drawBox(mat, vec3(0, -7, 0), vec3(50, 1, 50), vec4(0.2f, 0.2f, 1.0f, 1), false); //water

    }

    btRigidBody* drawBox(mat4t _mat, vec3 _pos, vec3 _size, vec4 _col, bool isMovable) {
      _mat.loadIdentity();
      _mat.translate(_pos);
      material *locMat = new material(_col);
      mesh_instance *m = app_scene_->add_shape(_mat, new mesh_box(_size), locMat, isMovable);
      btRigidBody *rb = m->get_node()->get_rigid_body();

      return rb; 
    }

    void drawSphere(mat4t _mat, vec3 _pos, vec3 _size, float _radius, vec4 _col, bool isMovable) {
      _mat.loadIdentity();
      _mat.translate(_pos);
      material *locMat = new material(_col);
      m_ball = app_scene_->add_shape(_mat, new mesh_sphere(_size, _radius), locMat, isMovable, ballMass);
      rb_ball = m_ball->get_node()->get_rigid_body();
    }

    void move_Ball() {
     // printf("%f %f %f\n", ballForce.x(), ballForce.y(), ballForce.z());

        // left and right arrows, moving along the X axis
        if (is_key_down(key_left)) {
          ballForce += btVector3(-0.3f, 0, 0);
        }
        else if (is_key_down(key_right)) {
          ballForce += btVector3(0.3f, 0, 0);
        }
        //Up and Down Arrows, moving along the Z axis
        else if (is_key_down(key_up)) {
          ballForce += btVector3(0, 0, -0.1f);
        }
        else if (is_key_down(key_down)) {
          ballForce += btVector3(0, 0, 0.1f);
        }
        else {
          ballForce = btVector3(0, 0, 0);
        }
        rb_ball->applyForce(ballForce, btVector3(0, 0, 0));
    }

    void applySpring() {
      //This applys a spring inbetween the ends of each plank. This is done so that it mimics a real rope bridge
      for (int i = 0; i < no_planks - 1; i++) {
        app_scene_->applySpring(rb_bridge[i], rb_bridge[i + 1], 0.8f, -8.0f);
        app_scene_->applySpring(rb_bridge[i], rb_bridge[i + 1], 0.8f, 8.0f);
      }
    }

    void spawnBall() {
      mat4t _mat;
      if(is_key_down(0x57))
      drawSphere(_mat, vec3(0, 8, 0), vec3(1, 1, 1), 0.5f, vec4(1, 1, 0, 1), true);
    }

    void moveSwing() {
      //rb_swing->applyTorque(btVector3(0, 0, 30));
      if (is_key_down(0x44))
        rb_swing->applyTorqueImpulse(btVector3(0, 0, 30));
      else if(is_key_down(0x41))
        rb_swing->applyTorqueImpulse(btVector3(0, 0, -30));
    }

    // this is called to draw the world
    void draw_world(int x, int y, int w, int h) {
     
      //applys a force to the ball to move it around
      move_Ball();
      //change_mass();
      spawnBall();
      moveSwing();
    
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
