#ifndef tree_h

#define tree_h

#include "sprite.h"
#include "tools.h"

namespace octet {

  class tree {
  private:

    robs_texture_shader texture_shader_;

    GLuint texture;
    vec4 colour = vec4(1, 1, 1, 1);

    std::vector <sprite> sprites;

    struct position {
      vec2 pos;
      float angle;
      float branch_length;
    };

    std::vector<position> stack_position;

    position turtle;

    float shrink_scale = 0.98f;

  public:

    tree::tree() {}

    void setup() {
      texture_shader_.init();
      //texture = resource_dict::get_texture_handle(GL_RGBA, "#792cd6");
      texture = resource_dict::get_texture_handle(GL_RGBA, "assets/invaderers/tree_texture.gif");
      turtle.pos = vec2(0.0f, 0.0f);
      turtle.angle = 0.0f;
      turtle.branch_length = 0.25f;
    }

    void create_tree(std::string rule_, float angle, int itterations) {

      char* chars = new char[rule_.length()];
      std::strcpy(chars, rule_.c_str());

      sprite locSprite;
      for (int i = 0; i != rule_.length(); i++) {
        switch (chars[i]) {
        case 'f':
        case 'r':
          // Creates the branch at the new location defined in previous rules and adds it to the branch buffer
          locSprite.init(texture, turtle.pos, 0.05f, turtle.branch_length, colour);
          locSprite.rotate(turtle.angle, 0, 0, 1);
          sprites.push_back(locSprite);

          // Shrinking the new branch
          turtle.branch_length *= shrink_scale;
          colour.x() *= 0.7f; // Changes the next branch's colour

                              // Adding the polar coord to the previous cart coord location
          tools::polarToCart(turtle.branch_length, turtle.angle, turtle.pos);
          break;
        case '-':
          // Turning the branches right;
          turtle.angle -= angle;
          break;
        case '+':
          // Turning the branches Left
          turtle.angle += angle;
          break;
        case '[': {
          stack_position.push_back(turtle); // Saves the turtle to the stack
          break; }
        case ']':
          colour.x() = 1.0f; // Resets the colour for the next branch

                             // Gets the last turtle on the stack
          turtle = stack_position.at(stack_position.size() - 1);
          stack_position.pop_back();
          break;
        }
      }
    }

    // Resets all tree variables
    void clear_buffer() {
      // Clears the draw and position buffers
      sprites.clear();
      stack_position.clear();

      // Reseting the turtle to the first draw position
      turtle.pos = vec2(0.0f, 0.0f);
      turtle.angle = 0.0f;
      turtle.branch_length = 0.25f;

      //Reseting the Start Colour
      colour = vec4(1, 1, 1, 1);
    }

    // Renders all branches in the sprite buffer
    void render(mat4t& cameraToWorld_) {
      int size = sprites.size();
      for (int i = 0; i != size; i++) {
        sprites[i].render(texture_shader_, cameraToWorld_);
      }
    }

    void set_shrink_scale(float percent) {
      shrink_scale += percent;
    }

    float get_shrink_scale() {
      return shrink_scale;
    }

    // Passes the turtle position 
    vec2 get_turtle_position() {
      return turtle.pos;
    }
  };

}


#endif // !tree_h
