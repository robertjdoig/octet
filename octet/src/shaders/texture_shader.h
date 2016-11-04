////////////////////////////////////////////////////////////////////////////////
//
// (C) Andy Thomason 2012-2014
//
// Modular Framework for OpenGLES2 rendering on multiple platforms.
//
// Single texture shader with no lighting

namespace octet { namespace shaders {
  class texture_shader : public shader {
    // indices to use with glUniform*()

    // index for model space to projection space matrix
    GLuint modelToProjectionIndex_;

    // index for texture sampler
    GLuint samplerIndex_;
    
    // Index for the sprite colour
    GLuint colourIndex_;
    
    // Index for the time variable
    GLuint timeIndex_;

  public:
    void init() {
      // this is the vertex shader.
      // it is called for each corner of each triangle
      // it inputs pos and uv from each corner
      // it outputs gl_Position and uv_ to the rasterizer
      const char vertex_shader[] = SHADER_STR(
        varying vec2 uv_;

        attribute vec4 pos;
        attribute vec2 uv;

        uniform mat4 modelToProjection;

        void main() { gl_Position = modelToProjection * pos; uv_ = uv; }
      );

      // this is the fragment shader
      // after the rasterizer breaks the triangle into fragments
      // this is called for every fragment
      // it outputs gl_FragColor, the color of the pixel and inputs uv_
      const char fragment_shader[] = SHADER_STR(
        varying vec2 uv_;
        uniform sampler2D sampler;
        uniform vec4 colour = vec4(1,1,1,1);
        uniform int time = 0;

        //http://glslsandbox.com/e#36439.0 The digit camo code which is added to the texture sampler. 

        void main() { 
          
          float r = fract(sin(length(floor(gl_FragCoord.xy / 5.0) + time))*1e6);
          float g = fract(cos(length(floor(gl_FragCoord.xy / 3.0) + time))*1e6);
          float b = fract(tan(length(floor(gl_FragCoord.xy / 1.0) + time))*1e6);

          gl_FragColor = (texture2D(sampler, uv_) * vec4(r, g, b, 1))*colour;
        }
      );
    
      // use the common shader code to compile and link the shaders
      // the result is a shader program
      shader::init(vertex_shader, fragment_shader);

      // extract the indices of the uniforms to use later
      modelToProjectionIndex_ = glGetUniformLocation(program(), "modelToProjection");
      samplerIndex_ = glGetUniformLocation(program(), "sampler");
      colourIndex_ = glGetUniformLocation(program(), "colour");
      timeIndex_ = glGetUniformLocation(program(), "time");
    }

    void render(const mat4t &modelToProjection, int sampler, float colourArray[4], int time) {
      // tell openGL to use the program
      shader::render();

      // customize the program with uniforms
      glUniform1i(samplerIndex_, sampler);
      glUniformMatrix4fv(modelToProjectionIndex_, 1, GL_FALSE, modelToProjection.get());
      glUniform4fv(colourIndex_, 1,colourArray);
      glUniform1i(timeIndex_, time);
    }
  };
}}
