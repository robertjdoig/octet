////////////////////////////////////////////////////////////////////////////////
//
// (C) Andy Thomason 2012-2014
//
// Modular Framework for OpenGLES2 rendering on multiple platforms.
//

#include <vector>

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

    std::string axiom = "a";
    char* chars;
    //static const float vertex_data[]; 

    std::vector<float> v_vertex_data;

    /// this is called once OpenGL is initialized
    void app_init() {
      shader = new color_shader();


      //rule(10);

      std::string c = rule(2);
      printf("%d \n", c.length());
      chars = new char[c.length()];
      std::strcpy(chars, c.c_str());


    }


    std::string rule(int _iterations) {
      for (int i = 0; i < _iterations; i++) {

        std::string tempString;
        for (char& c : axiom) {
          //rule for A
          if (c == 'a') {
            tempString += "ab";
          }
          //Rule for B
          else if (c == 'b') {
            tempString += "a";
          }
        }
        axiom = tempString;

      }

      std::cout << axiom << std::endl;

      return axiom;
    }

    void branch(float _len) {

      draw_Quad(_len, 0, 0.1f, _len);

      _len *= 0.98f;
      if (_len > 0.01f) {
        glPushMatrix();
        branch(_len);
        glPopMatrix();
      }

    }

    void draw_Quad(float x, float y, float _halve_width, float _halve_height) {



      static const float vertex_data[] = {
        x + _halve_width, y, 0.0f,
        x - _halve_width, y, 0.0f,
        x + _halve_width, y + _halve_height, 0.0f,
        x - _halve_width, y + _halve_height, 0.0f,
      };

      glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data, GL_STATIC_DRAW);

      glEnableVertexAttribArray(0);




      ////////////////////////////////////////////////////
      //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
      //http://glslsandbox.com/e#36217.0
      //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
      /////////////////////////////////////////////////////
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

      //branch(branch_len);

      glGenBuffers(1, &vertices);
      glBindBuffer(GL_ARRAY_BUFFER, vertices);

      draw_Quad(-0.1f, 0, 0.1f, 0.2f);
      draw_Quad(0.1f, 0, 0.1f, 0.2f);

      // use the buffer we made earlier.
      //  glBindBuffer(GL_ARRAY_BUFFER, vertices);

      // tell OpenGL what kind of vertices we have
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), NULL);

      // draw a triangle
      // glDrawArrays(GL_TRIANGLES, 0, 3);
      glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

      /*
      printf("%d \n", sizeof(chars));

      for (int i = 0; i < sizeof(chars); i++) {
      switch (chars[i])
      {
      case 'a':
      branch(branch_len);
      break;
      case 'b':
      branch(1.0f);
      default:
      break;
      }
      }
      */
    }
  };
}
