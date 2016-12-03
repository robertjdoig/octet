#ifndef sprite_h

#define sprite_h

namespace octet {
  
  class sprite {
    // where is our sprite (overkill for a 2D game!)
    mat4t modelToWorld;

    // half the width of the sprite
    float halfWidth;

    // half the height of the sprite
    float halfHeight;

    // what texture is on our sprite
    int texture;

    // true if this sprite is enabled.
    bool enabled;

    float colour[4] = { 1,1,1,1 };

  public:
    sprite() {
      texture = 0;
      enabled = true;
    }

    void init(int _texture, vec2 pos, float w, float h, vec4 colour_) {
      modelToWorld.loadIdentity();
      modelToWorld.translate(pos.x(), pos.y(), 0.0f);
      halfWidth = w * h;
      halfHeight = h;
      texture = _texture;
      enabled = true;
      
      colour[0] = colour_.x();
      colour[1] = colour_.y();
      colour[2] = colour_.z();
      colour[3] = colour_.w();
    }

    void render(robs_texture_shader &shader, mat4t &cameraToWorld) {
      // invisible sprite... used for gameplay.
      if (!texture) return;

      // build a projection matrix: model -> world -> camera -> projection
      // the projection space is the cube -1 <= x/w, y/w, z/w <= 1
      mat4t modelToProjection = mat4t::build_projection_matrix(modelToWorld, cameraToWorld);

      // set up opengl to draw textured triangles using sampler 0 (GL_TEXTURE0)
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, texture);

      // use "old skool" rendering
      //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
      //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      
     
      shader.render(modelToProjection, 0, colour);

      // this is an array of the positions of the corners of the sprite in 3D
      // a straight "float" here means this array is being generated here at runtime.
      float vertices[] = {
        -halfWidth - 0.013f, 0, 0,
        halfWidth + 0.013f, 0, 0,
        halfWidth + 0.01f,  halfHeight, 0,
        -halfWidth - 0.01,  halfHeight, 0,
      };

      // attribute_pos (=0) is position of each corner
      // each corner has 3 floats (x, y, z)
      // there is no gap between the 3 floats and hence the stride is 3*sizeof(float)
      glVertexAttribPointer(attribute_pos, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)vertices);
      glEnableVertexAttribArray(attribute_pos);

      // this is an array of the positions of the corners of the texture in 2D
      static const float uvs[] = {
        0,  0,
        1,  0,
        1,  1,
        0,  1,
      };

      // attribute_uv is position in the texture of each corner
      // each corner (vertex) has 2 floats (x, y)
      // there is no gap between the 2 floats and hence the stride is 2*sizeof(float)
      glVertexAttribPointer(attribute_uv, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)uvs);
      glEnableVertexAttribArray(attribute_uv);

      // finally, draw the sprite (4 vertices)
      glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    }

    // move the object
    void translate(float x, float y, float z) {
      modelToWorld.translate(x, y, z);
    }

    void rotate(float angle, float x, float y, float z) {
      modelToWorld.rotate(angle, x, y, z);
    }

    mat4t getMatrix() {
      return modelToWorld;
    }
  };

}

#endif // sprite_h
