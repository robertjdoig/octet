#ifndef text_h

#define text_h

namespace octet {
  //class my_funky_class : public octet::app {}

  class text : public octet::app {
  private:
    GLuint font_texture;
    bitmap_font font;

  public:

    void setup(GLuint font_texture_) {
      font_texture = font_texture_;
    }
    
    void draw_text(texture_shader &shader, mat4t& cameraToWorld_, float x, float y, float scale, const char *text) {
      mat4t modelToWorld;
      modelToWorld.loadIdentity();
      modelToWorld.translate(x, y, 0);
      modelToWorld.scale(scale, scale, 0);
      mat4t modelToProjection = mat4t::build_projection_matrix(modelToWorld, cameraToWorld_);

      enum { max_quads = 32 };
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
  };





}

#endif // !text_h
