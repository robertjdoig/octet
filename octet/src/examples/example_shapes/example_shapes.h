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
    ref<visual_scene> app_scene;

  public:
    example_shapes(int argc, char **argv) : app(argc, argv) {
    }

    ~example_shapes() {
    }

	vec3 ballPos =  vec3(-10, -5, 0);

	float rotAngle = 45;

    /// this is called once OpenGL is initialized
    void app_init() {
      app_scene =  new visual_scene();
      app_scene->create_default_camera_and_lights();
      app_scene->get_camera_instance(0)->get_node()->translate(vec3(0, 4, 0));

      material *red = new material(vec4(1, 0.5, 0, 1));
      material *green = new material(vec4(0.5, 1, 0, 1));
      material *blue = new material(vec4(0, 0, 1, 1));
	  material *black = new material(vec4(0, 0, 0, 1));

     mat4t mat;
    // mat.translate(ballPos);
    // app_scene->add_shape(mat, new mesh_sphere(vec3(1, 1, 1), 1), red, true);

	 drawSphere(mat, ballPos, vec3(1,1,1),0.5f, vec4(1, 1, 0, 1), true);


	// for (int i = 0; i < 10; i++) {
		// drawCylinder(mat, vec3(-10, -7, 0), vec3(3, 1, 4), vec4(0.2f, 1, 0.2f, 1), false);
	// }


		 mat.loadIdentity();
		
		 mat.translate(-10, 0, 0);
		 for (int i = 0; i < 10; i++) {
			 mat.translate(vec3(i/2, 0, 0));
			 app_scene->add_shape(mat, new mesh_cylinder(vec3(1, 1, 1), 1), red, false);
		 }

	 //draw bridge
	 drawBridge(mat, vec3(0, 5, 0), rotAngle, 0, 0, 1, vec3(12, 0.1, 5), vec4(0.2f, 1, 0.2f, 1), false);
	 
	 // ground
	 drawBox(mat, vec3(-10,-7,0), vec3(3, 1, 50), vec4(0.2f, 1, 0.2f, 1), false); //left
	 drawBox(mat, vec3(10,-7,0), vec3(3,1,50), vec4(0.2f, 1, 0.2f, 1), false); //right

    }

	void drawBox(mat4t _mat, vec3 _pos, vec3 _size, vec4 _col, bool isMovable) {
		_mat.loadIdentity();
		_mat.translate(_pos);
		material *locMat = new material(_col);
		app_scene->add_shape(_mat, new mesh_box(_size),locMat , isMovable);
		//app_scene->apply_central_force(_pos);
	
	}

	void drawSphere(mat4t _mat, vec3 _pos, vec3 _size,float _radius, vec4 _col, bool isMovable) {
		_mat.loadIdentity();
		_mat.translate(_pos);
		material *locMat = new material(_col);
		app_scene->add_shape(_mat, new mesh_sphere(_size, _radius),locMat, isMovable);
	}

	void drawBridge(mat4t _mat, vec3 _pos,float _rotAngle, float _rotPointX,float _rotPointY,float _rotPointZ, vec3 _size, vec4 _col, bool isMovable) {
		_mat.loadIdentity();
		_mat.translate(_pos);
		_mat.rotate(_rotAngle, _rotPointX, _rotPointY, _rotPointZ);
		
		material *locMat = new material(_col);
		app_scene->add_shape(_mat, new mesh_box(_size), locMat, isMovable);

	}

	void drawCylinder(mat4t _mat, vec3 _pos, vec3 _size, vec4 _col, bool isMovable) {
		_mat.loadIdentity();
		_mat.translate(_pos);

		material *locMat = new material(_col);
		app_scene->add_shape(_mat, new mesh_cylinder(_size,15), locMat, isMovable);

	}

    // this is called to draw the world
    void draw_world(int x, int y, int w, int h) {
	
		int vx = 0, vy = 0;
      get_viewport_size(vx, vy);
      app_scene->begin_render(vx, vy);

      // update matrices. assume 30 fps.
      app_scene->update(1.0f/30);
      // draw the scene
      app_scene->render((float)vx / vy);

    }
  };
}
