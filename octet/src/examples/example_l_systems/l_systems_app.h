////////////////////////////////////////////////////////////////////////////////
//
// (C) Andy Thomason 2012-2014
//
// Modular Framework for OpenGLES2 rendering on multiple platforms.
//
// invaderer example: simple game with sprites and sounds
//
// Level: 1
//
// Demonstrates:
//   Basic framework app
//   Shaders
//   Basic Matrices
//   Simple game mechanics
//   Texture loaded from GIF file
//   Audio
//
#include <vector>

#include "tree.h"
#include "rule.h"
#include "tools.h"

namespace octet {

  class l_systems_app : public octet::app {

    // Matrix to transform points in our camera space to the world.
    // This lets us move our camera
    mat4t cameraToWorld;
    mat4t textToWorld;

    tree tree_;
    rule rule_;


    int rule_state = 0;
    int itterations = 1;
    float angle = 22.7f;

    vec3 pan = vec3(0, 3, 3);


    robs_texture_shader texture_shader_;
    GLuint font_texture;
    bitmap_font font;

    void draw_text(robs_texture_shader &shader, float x, float y, float scale, const char *text) {
      mat4t modelToWorld;
      modelToWorld.loadIdentity();
      modelToWorld.translate(x, y, 0);
      modelToWorld.scale(scale, scale, 0);
      mat4t modelToProjection = mat4t::build_projection_matrix(modelToWorld, textToWorld);

      enum { max_quads = 512 };
      bitmap_font::vertex vertices[max_quads * 4];
      uint32_t indices[max_quads * 6];
      aabb bb(vec3(0, 0, 0), vec3(256, 256, 0));

      unsigned num_quads = font.build_mesh(bb, vertices, indices, max_quads, text, 0);
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, font_texture);

      float colourArray[4] = { 1,1,1,1 };
      shader.render(modelToProjection, 0, colourArray);

      glVertexAttribPointer(attribute_pos, 3, GL_FLOAT, GL_FALSE, sizeof(bitmap_font::vertex), (void*)&vertices[0].x);
      glEnableVertexAttribArray(attribute_pos);
      glVertexAttribPointer(attribute_uv, 3, GL_FLOAT, GL_FALSE, sizeof(bitmap_font::vertex), (void*)&vertices[0].u);
      glEnableVertexAttribArray(attribute_uv);

      glDrawElements(GL_TRIANGLES, num_quads * 6, GL_UNSIGNED_INT, indices);
    }

  public:

    // this is called when we construct the class
    l_systems_app(int argc, char **argv) : app(argc, argv), font(512, 256, "assets/big.fnt") {
    }

    // this is called once OpenGL is initialized
    void app_init() {
      texture_shader_.init();
      font_texture = resource_dict::get_texture_handle(GL_RGBA, "assets/big_0.gif");

      tree_.setup();

      rule_.readfile();

      std::string axiom = rule_.compute_rule(itterations);

      tree_.create_tree(axiom, rule_.get_angle(), itterations);
    }

    void reset_tree() {

      tree_.clear_buffer();

      // rule_.set_axiom(rule_state);
      std::string axiom = rule_.compute_rule(itterations);

      tree_.create_tree(axiom, angle, itterations);
    }

    void input() {

      // 1
      if (is_key_going_up(0x31)) {
        if (rule_.get_axiom_state() > 0) {
          rule_.set_axiom_state(-1);
          // rule_state--;
          reset_tree();
          angle = rule_.get_angle();
        }
      }
      // 2
      if (is_key_going_up(0x32)) {
        if (rule_.get_axiom_state() < rule_.get_rule_buffer() - 1) {
          //rule_state++;
          rule_.set_axiom_state(1);
          reset_tree();
          angle = rule_.get_angle();
        }
      }

      // 3
      if (is_key_going_up(0x33)) {
        if (itterations > 0) {
          itterations--;
          reset_tree();
        }
      }

      // 4
      if (is_key_going_up(0x34)) {
        // change itterations
        if (itterations < 7) {
          itterations++;
          reset_tree();

        }
      }


      // 5
      if (is_key_down(0x35)) {
        if (angle > 0) {
          angle--;
          reset_tree();
        }
      }
      // 6
      if (is_key_down(0x36)) {
        if (angle < 360) {
          angle++;
          reset_tree();
        }
      }

      // 7
      if (is_key_down(0x37)) {
        if (tree_.get_shrink_scale() > 0.8f) {

          tree_.set_shrink_scale(-0.001f);
          reset_tree();
        }
      }
      // 8
      if (is_key_down(0x38)) {
        if (tree_.get_shrink_scale() < 1.1f) {

          tree_.set_shrink_scale(+0.001f);
          reset_tree();
        }
      }

      // 9
      if (is_key_down(0x39)) {
        if (rule_.get_probability_ratio() > 0) {

          rule_.set_probability_ratio(-1);
          reset_tree();
        }
      }
      // 0
      if (is_key_down(0x30)) {
        if (rule_.get_probability_ratio() < 100) {

          rule_.set_probability_ratio(1);
          reset_tree();
        }
      }
      // R
      if (is_key_going_up(0x52)) {
        reset_tree();
      }

      //zoom in Minus Key
      if (is_key_down(0xBD)) {
        pan.z() += 0.1f;
      }
      //zoom out Plus KEy
      else if (is_key_down(0xBB)) {
        pan.z() -= 0.1f;
      }

      // Arrow Up
      if (is_key_down(key_up)) {
        pan.y() -= 0.1f;
      }
      else if (is_key_down(key_down)) {
        pan.y() += 0.1f;
      }
      if (is_key_down(key_left)) {
        pan.x() += 0.1f;
      }
      else if (is_key_down(key_right)) {
        pan.x() -= 0.1f;
      }

    }

    void draw_hud() {
      char score_text[512];

      const char* text = "Loops: %d \n Rules: %d \n Angle: %f \n Shrink: %d percent \n Probability: %d percent \n";
      sprintf(score_text, text, itterations + 1, rule_.get_axiom_state() + 1, angle, (int)(tree_.get_shrink_scale() * 100), rule_.get_probability_ratio());
      draw_text(texture_shader_, -2, 5, 0.5f / 256, score_text);
    }


    // this is called to draw the world
    void draw_world(int x, int y, int w, int h) {

      // set a viewport - includes whole window area
      glViewport(x, y, w, h);

      // clear the background to black
      glClearColor(0, 0, 0, 1);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      // don't allow Z buffer depth testing (closer objects are always drawn in front of far ones)
      glDisable(GL_DEPTH_TEST);

      // allow alpha blend (transparency when alpha channel is 0)
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


      textToWorld.loadIdentity();
      textToWorld.translate(0, 3, 3);

      draw_hud();

      input();

      cameraToWorld.loadIdentity();
      cameraToWorld.translate(pan);

      tree_.render(cameraToWorld);

      // move the listener with the camera
      vec4 &cpos = cameraToWorld.w();
      alListener3f(AL_POSITION, cpos.x(), cpos.y(), cpos.z());

    }
  };
}

