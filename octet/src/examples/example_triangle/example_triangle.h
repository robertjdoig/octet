////////////////////////////////////////////////////////////////////////////////
//
// (C) Andy Thomason 2012-2014
//
// Modular Framework for OpenGLES2 rendering on multiple platforms.
//
namespace octet {
  /// Drawing a triangle without a scene
  class example_triangle : public app {
    // a very basic color shader
    ref<color_shader> shader;
    GLuint vertices;
  public:
    /// this is called when we construct the class before everything is initialised.
    example_triangle(int argc, char **argv) : app(argc, argv) {
    }

    float branch_len = 0.2f;

    /// this is called once OpenGL is initialized
    void app_init() {
      shader = new color_shader();
      /*
      glGenBuffers(1, &vertices);
      glBindBuffer(GL_ARRAY_BUFFER, vertices);

      // corners (vertices) of the triangle
      static const float vertex_data[] = {
       -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f,
         0.4f, 0.7f, 0.0f,
      };

      glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data, GL_STATIC_DRAW);

      */
    }

    void branch(float _len) {


      draw_Quad(0.0f, -1.0f, 0.1f,_len);




    }

    void draw_Quad(float _x, float _y, float _halve_width, float _halve_height ) {

      glGenBuffers(1, &vertices);
      glBindBuffer(GL_ARRAY_BUFFER, vertices);

      static const float vertex_data[] = {
        _x - _halve_width, _y, 0.0f,
        _x - _halve_width, _y + _halve_height, 0.0f,
        _x + _halve_width, _y + _halve_height, 0.0f,
        _x + _halve_width, _y, 0.0f,
      };

      glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data, GL_STATIC_DRAW);

      glEnableVertexAttribArray(0);

      // use the buffer we made earlier.
      glBindBuffer(GL_ARRAY_BUFFER, vertices);

      // tell OpenGL what kind of vertices we have
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), NULL);

      // draw a triangle
      // glDrawArrays(GL_TRIANGLES, 0, 3);
      glDrawArrays(GL_QUADS, 0, 4);
    }



    /// this is called to draw the world
    void draw_world(int x, int y, int w, int h) {
      // the framework gives us the dimensions of the window.
      int vx = 0, vy = 0;
      get_viewport_size(vx, vy);

      /// set a viewport - includes whole window area
      glViewport(0, 0, vx, vy);

      /// clear the background and the depth buffer
      glClearColor(0.1f, 0.1f, 0.1f, 1);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      /// allow Z buffer depth testing (closer objects are always drawn in front of far ones)
      glEnable(GL_DEPTH_TEST);

      // we use a unit matrix will not change the (-1..1, -1..1, -1..1) xyz space of OpenGL
      mat4t modelToProjection;

      // we use a simple solid color shader.
      vec4 emissive_color(1, 1, 0, 1);
      shader->render(modelToProjection, emissive_color);

      branch(branch_len);


      /*
      // use vertex attribute 0 for our vertices (we could use 1, 2, 3 etc for other things)
      glEnableVertexAttribArray(0);

      // use the buffer we made earlier.
      glBindBuffer(GL_ARRAY_BUFFER, vertices);

      // tell OpenGL what kind of vertices we have
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 4*sizeof(float), NULL);

      // draw a triangle
     // glDrawArrays(GL_TRIANGLES, 0, 3);
      glDrawArrays(GL_QUADS, 0, 4);
      */
    }
  };
}
