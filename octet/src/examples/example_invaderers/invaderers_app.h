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

#include <fstream>
#include <iostream>
#include <string>
#include <cstring>
#include <sstream>

#include "sprite.h"


namespace octet {
 
  class invaderers_app : public octet::app {
       
    // Matrix to transform points in our camera space to the world.
    // This lets us move our camera
    mat4t cameraToWorld;

    // shader to draw a textured triangle
    texture_shader texture_shader_;

    enum {
      num_sound_sources = 8,
      num_rows = 20,
      num_cols = 20,
      num_missiles = 2,
      num_bombs = 2,
      num_borders = 4,
      num_blocks = num_rows * num_cols,
      num_invaderers = num_rows * num_cols,

      // sprite definitions
      
      game_over_sprite = 0,

      first_explosion_sprite,
      last_explosion_sprite = first_explosion_sprite + num_invaderers - 1,

      first_path_sprite,
      last_path_sprite = first_path_sprite + num_invaderers - 1,

      first_invaderer_sprite,
      last_invaderer_sprite = first_invaderer_sprite + num_invaderers - 1,

      first_block_sprite,
      last_block_sprite = first_block_sprite + num_blocks - 1,

      first_missile_sprite,
      last_missile_sprite = first_missile_sprite + num_missiles - 1,

      first_bomb_sprite,
      last_bomb_sprite = first_bomb_sprite + num_bombs - 1,

      first_border_sprite,
      last_border_sprite = first_border_sprite + num_borders - 1,

      ship_sprite,

      num_sprites,
    };

    // timers for missiles and bombs
    int missiles_disabled;
    int bombs_disabled;

    // accounting for bad guys
    int live_invaderers;
    bool invaderHit[num_invaderers];
    int num_lives;

    // game state
    bool game_over;
    int score;

    // speed of enemy
    float invader_velocity;

    //speed of ship
    float ship_velocity = 0.05f;

    // sounds
    ALuint whoosh;
    ALuint bang;
    unsigned cur_source;
    ALuint sources[num_sound_sources];

    // big array of sprites
    sprite sprites[num_sprites];

    // random number generator
    class random randomizer;

    // a texture for our text
    GLuint font_texture;

    // information for our text
    bitmap_font font;

    ALuint get_sound_source() {
      return sources[cur_source++ % num_sound_sources];
    }



    char read_file(int arrayPos) {

      //*
      //http://stackoverflow.com/questions/13035674/how-to-read-line-by-line-or-a-whole-text-file-at-once
      std::ifstream myfile("pos.txt");
      std::string text; 
      std::string com_line;
      while (std::getline(myfile, text)){
        com_line += text;
      }
      
      // http://www.cplusplus.com/forum/general/100714/ 
      char *chars = new char[com_line.length()];
      std::strcpy(chars, com_line.c_str());
      //std::cout << chars;
      /*/

      std::ifstream myfile("pos.txt");
      char chars[num_invaderers+1];
      myfile.get(chars, num_invaderers+1);
      //*/
      return   chars[arrayPos];
     // delete [] chars; 
    }




    // called when we hit an enemy
    void on_hit_invaderer() {
      ALuint source = get_sound_source();
      alSourcei(source, AL_BUFFER, bang);
      alSourcePlay(source);

      live_invaderers--;
      score++;
      if (live_invaderers == 4) {
        invader_velocity *= 4;
      }
      else if (live_invaderers == 0) {
        game_over = true;
        sprites[game_over_sprite].translate(-20, 0, 0);
      }
    }

    // called when we are hit
    void on_hit_ship() {
      ALuint source = get_sound_source();
      alSourcei(source, AL_BUFFER, bang);
      alSourcePlay(source);

      if (--num_lives == 0) {
        game_over = true;
        sprites[game_over_sprite].translate(-20, 0, 0);
      }
    }

    // use the keyboard to move the ship
    void move_ship(float _ship_velocity) {
      //const float ship_speed = 1.0f;
      // left and right arrows
      if (is_key_down(key_left)) {
        sprites[ship_sprite].translate(-_ship_velocity, 0, 0);
        if (sprites[ship_sprite].collides_with(sprites[first_border_sprite + 2])) {
          sprites[ship_sprite].translate(+_ship_velocity, 0, 0);
        }
        for (int i = 0; i < num_blocks; i++) {
          if (sprites[ship_sprite].collides_with(sprites[first_block_sprite + i])) {
            sprites[ship_sprite].translate(+_ship_velocity, 0, 0);
          }
        }
      }
      else if (is_key_down(key_right)) {
        sprites[ship_sprite].translate(+_ship_velocity, 0, 0);
        if (sprites[ship_sprite].collides_with(sprites[first_border_sprite + 3])) {
          sprites[ship_sprite].translate(-_ship_velocity, 0, 0);
        }
        for (int i = 0; i < num_blocks; i++) {
          if (sprites[ship_sprite].collides_with(sprites[first_block_sprite + i])) {
            sprites[ship_sprite].translate(-_ship_velocity, 0, 0);
          }
        }
      }
      if (is_key_down(key_down)) {
        sprites[ship_sprite].translate(0, -_ship_velocity, 0);
       // if (sprites[ship_sprite].collides_with(sprites[first_border_sprite + 4])) {
       //   sprites[ship_sprite].translate(0, +_ship_velocity, 0);
       // }
        for (int i = 0; i < num_blocks; i++) {
          if (sprites[ship_sprite].collides_with(sprites[first_block_sprite + i])) {
            sprites[ship_sprite].translate(0,+_ship_velocity, 0);
          }
        }
      }
      else if (is_key_down(key_up)) {
        sprites[ship_sprite].translate(0, +_ship_velocity, 0);
       // if (sprites[ship_sprite].collides_with(sprites[first_border_sprite + 5])) {
       //   sprites[ship_sprite].translate(0, -_ship_velocity, 0);
       // }
        for (int i = 0; i < num_blocks; i++) {
          if (sprites[ship_sprite].collides_with(sprites[first_block_sprite + i])) {
            sprites[ship_sprite].translate(0,-_ship_velocity, 0);
          }
        }
      }
  
      //'s' key
      if (is_key_down(0x57)) {
        sprites[ship_sprite].translate(0, 0, -_ship_velocity);
        if (sprites[ship_sprite].collides_with(sprites[first_border_sprite + 2])) {
          sprites[ship_sprite].translate(0, 0, +_ship_velocity);
        }
      }
      //'w' key
      else if (is_key_down(0x53)) {
        sprites[ship_sprite].translate(0, 0, +_ship_velocity);
        if (sprites[ship_sprite].collides_with(sprites[first_border_sprite + 3])) {
          sprites[ship_sprite].translate(0, 0, -_ship_velocity);
        }
      }
    }
    
    // fire button (space)
    void fire_missiles() {
      if (missiles_disabled) {
        --missiles_disabled;
      }
      else if (is_key_going_down(' ')) {
        // find a missile
        for (int i = 0; i != num_missiles; ++i) {
          if (!sprites[first_missile_sprite + i].is_enabled()) {
            sprites[first_missile_sprite + i].set_relative(sprites[ship_sprite], 0, 0, 0);
            sprites[first_missile_sprite + i].is_enabled() = true;
            missiles_disabled = 5;
            ALuint source = get_sound_source();
            alSourcei(source, AL_BUFFER, whoosh);
            alSourcePlay(source);
            break;
          }
        }
      }
    }

    // pick and invader and fire a bomb
    void fire_bombs() {
      if (bombs_disabled) {
        --bombs_disabled;
      }
      else {
        // find an invaderer
        sprite &ship = sprites[ship_sprite];
        for (int j = randomizer.get(0, num_invaderers); j < num_invaderers; ++j) {
          sprite &invaderer = sprites[first_invaderer_sprite + j];
          if (invaderer.is_enabled() && invaderer.is_above(ship, 0.3f)) {
            // find a bomb
            for (int i = 0; i != num_bombs; ++i) {
              if (!sprites[first_bomb_sprite + i].is_enabled()) {
                sprites[first_bomb_sprite + i].set_relative(invaderer, 0, -0.25f, 0);
                sprites[first_bomb_sprite + i].is_enabled() = true;
                bombs_disabled = 30;
                ALuint source = get_sound_source();
                alSourcei(source, AL_BUFFER, whoosh);
                alSourcePlay(source);
                return;
              }
            }
            return;
          }
        }
      }
    }

    // animate the missiles
    void bomb_timer() {
      const float missile_speed = 0.1f;
      for (int i = 0; i != num_missiles; ++i) {
        sprite &missile = sprites[first_missile_sprite + i];
        if (missile.is_enabled()) {
          missile.translate(0, missile_speed, 0);
          for (int j = 0; j != num_invaderers; ++j) {
            sprite &invaderer = sprites[first_invaderer_sprite + j];
            if (invaderer.is_enabled() && missile.collides_with(invaderer)) {
              invaderer.is_enabled() = false;
              invaderer.translate(20, 0, 0);
              missile.is_enabled() = false;
              missile.translate(20, 0, 0);
              invaderHit[j] = true;
              on_hit_invaderer();

              goto next_missile;
            }
          }
          for (int j = 0; j != num_blocks; ++j) {
            sprite &block = sprites[first_block_sprite + j];
            if (missile.collides_with(block)) {
             // block.is_enabled() = false;
             // block.translate(20, 0, 0);
              missile.is_enabled() = false;
              missile.translate(20, 0, 0);
              
            

              goto next_missile;
            }
          }
          if (missile.collides_with(sprites[first_border_sprite + 1])) {
            missile.is_enabled() = false;
            missile.translate(20, 0, 0);
          }
        }
      next_missile:;
      }
    }

    // animate the bombs
    void move_bombs() {
      const float bomb_speed = 0.2f;
      for (int i = 0; i != num_bombs; ++i) {
        sprite &bomb = sprites[first_bomb_sprite + i];
        if (bomb.is_enabled()) {
          bomb.translate(0, -bomb_speed, 0);
          if (bomb.collides_with(sprites[ship_sprite])) {
            bomb.is_enabled() = false;
            bomb.translate(20, 0, 0);
            bombs_disabled = 50;
            on_hit_ship();
            goto next_bomb;
          }
          if (bomb.collides_with(sprites[first_border_sprite + 0])) {
            bomb.is_enabled() = false;
            bomb.translate(20, 0, 0);
          }
        }
      next_bomb:;
      }
    }

    // move the array of enemies
    void move_invaders(float dx, float dy, float dz) {
      for (int j = 0; j != num_invaderers; ++j) {
        sprite &invaderer = sprites[first_invaderer_sprite + j];
        if (invaderer.is_enabled()) {
          invaderer.translate(dx, dy, dz);
        }
      }
    }

    // check if any invaders hit the sides.
    bool invaders_collide(sprite &border) {
      for (int j = 0; j != num_invaderers; ++j) {
        sprite &invaderer = sprites[first_invaderer_sprite + j];
        if (invaderer.is_enabled() && invaderer.collides_with(border)) {
          return true;
        }
      }
      return false;
    }



    void draw_text(texture_shader &shader, float x, float y, float scale, const char *text) {
      mat4t modelToWorld;
      modelToWorld.loadIdentity();
      modelToWorld.translate(x, y, 0);
      modelToWorld.scale(scale, scale, 0);
      mat4t modelToProjection = mat4t::build_projection_matrix(modelToWorld, cameraToWorld);

      /*mat4t tmp;
      glLoadIdentity();
      glTranslatef(x, y, 0);
      glGetFloatv(GL_MODELVIEW_MATRIX, (float*)&tmp);
      glScalef(scale, scale, 1);
      glGetFloatv(GL_MODELVIEW_MATRIX, (float*)&tmp);*/

      enum { max_quads = 32 };
      bitmap_font::vertex vertices[max_quads * 4];
      uint32_t indices[max_quads * 6];
      aabb bb(vec3(0, 0, 0), vec3(256, 256, 0));

      unsigned num_quads = font.build_mesh(bb, vertices, indices, max_quads, text, 0);
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, font_texture);

      float colourArray[4] = { 1,0.5f,1,1 };
      shader.render(modelToProjection, 0,colourArray );

      glVertexAttribPointer(attribute_pos, 3, GL_FLOAT, GL_FALSE, sizeof(bitmap_font::vertex), (void*)&vertices[0].x);
      glEnableVertexAttribArray(attribute_pos);
      glVertexAttribPointer(attribute_uv, 3, GL_FLOAT, GL_FALSE, sizeof(bitmap_font::vertex), (void*)&vertices[0].u);
      glEnableVertexAttribArray(attribute_uv);

      glDrawElements(GL_TRIANGLES, num_quads * 6, GL_UNSIGNED_INT, indices);
    }

  public:

    // this is called when we construct the class
    invaderers_app(int argc, char **argv) : app(argc, argv), font(512, 256, "assets/big.fnt") {
    }

    // this is called once OpenGL is initialized
    void app_init() {

      // set up the shader
      texture_shader_.init();

      // set up the matrices with a camera 5 units from the origin
      cameraToWorld.loadIdentity();
      cameraToWorld.translate(0, 0, 3);

      float white[4] = { 1,1,1,1 };
      float orange[4] = { 1,0.5f,0,1 };
      float yellow[4] = { 1,1,0,1 };
      float green[4] = { 0,1,0,1 };

      font_texture = resource_dict::get_texture_handle(GL_RGBA, "assets/big_0.gif");

      GLuint ship = resource_dict::get_texture_handle(GL_RGBA, "assets/invaderers/ship.gif");

     
      sprites[ship_sprite].init(ship, 0, -2.75f, 0, 0.15f, 0.15f, white);

      GLuint GameOver = resource_dict::get_texture_handle(GL_RGBA, "assets/invaderers/GameOver.gif");
      sprites[game_over_sprite].init(GameOver, 20, 0, 0, 3, 1.5f, white);

      GLuint invaderer = resource_dict::get_texture_handle(GL_RGBA, "assets/invaderers/invaderer.gif");
      GLuint brick = resource_dict::get_texture_handle(GL_RGBA, "assets/invaderers/brick_sprite.gif");
      GLuint path = resource_dict::get_texture_handle(GL_RGBA, "assets/invaderers/sand_sprite.gif");

      GLuint invaderExplosion = resource_dict::get_texture_handle(GL_RGBA, "assets/invaderers/InvadersExplosion.gif");

      for (int j = 0; j != num_rows; j++) {
        for (int i = 0; i != num_cols; i++) {

          //assert(first_invaderer_sprite + i + j*num_cols <= last_invaderer_sprite);

          // sprites[first_invaderer_sprite + i + j*num_cols].init(
           //  invaderer, ((float)i - num_cols * 0.5f) * 0.5f, 2.50f - ((float)j * 0.5f), 0.25f, 0.25f);

           // Draws the sprite that has been selected
          float tile_size = 0.25f; 
          float x = ((float)i - num_cols * 0.5f) * tile_size;
          float y = 2.50f - ((float)j * tile_size);

          switch (read_file(i + j*num_cols)) {
          case '.':
            //Draw Path
            sprites[first_path_sprite + i + j*num_cols].init(path, x, y, 0.0f, tile_size, tile_size, yellow);

            break;
          case 'x':
            //draw invader
            sprites[first_path_sprite + i + j*num_cols].init(path, x, y, 0.0f, tile_size, tile_size, yellow);

            sprites[first_invaderer_sprite + i + j*num_cols].init( invaderer, x, y, 0.0f, tile_size, tile_size, green);

            //invaderer, ((float)i - num_cols * 0.01f) * 0.5f, 0.0f, 3.0f - ((float)j * 0.25f), 0.1f, 0.1f);
            break;
          case 'b':
            //draw block 
            sprites[first_block_sprite + i + j*num_cols].init(brick, x, y, 0.0f, tile_size, tile_size, orange);
            break;
          default:
            std::cout << "not reading the level file";
          }
        }
      }
      
      // set the border to white for clarity
      GLuint col_white = resource_dict::get_texture_handle(GL_RGB, "#ffffff");  
      sprites[first_border_sprite + 0].init(col_white, 0, -3, 0, 6, 0.2f, white);
      sprites[first_border_sprite + 1].init(col_white, 0, 3, 0, 6, 0.2f, white);
      sprites[first_border_sprite + 2].init(col_white, -3, 0, 0, 0.2f, 6, white);
      sprites[first_border_sprite + 3].init(col_white, 3, 0, 0, 0.2f, 6, white);
      

      // use the missile texture
      GLuint missile = resource_dict::get_texture_handle(GL_RGBA, "assets/invaderers/bomb_circle.gif");
      //missile = resource_dict::get_texture_handle(GL_RGBA, "#fff200");
      for (int i = 0; i != num_missiles; ++i) {
        // create missiles off-screen
        sprites[first_missile_sprite + i].init(missile, 20, 0, 0, 0.15f, 0.15f, white);
        sprites[first_missile_sprite + i].is_enabled() = false;
      }

      // use the bomb texture
      GLuint bomb = resource_dict::get_texture_handle(GL_RGBA, "assets/invaderers/bomb.gif");
      for (int i = 0; i != num_bombs; ++i) {
        // create bombs off-screen
        sprites[first_bomb_sprite + i].init(bomb, 20, 0, 0, 0.0625f, 0.25f, white);
        sprites[first_bomb_sprite + i].is_enabled() = false;
      }

      // sounds
      whoosh = resource_dict::get_sound_handle(AL_FORMAT_MONO16, "assets/invaderers/whoosh.wav");
      bang = resource_dict::get_sound_handle(AL_FORMAT_MONO16, "assets/invaderers/bang.wav");
      cur_source = 0;
      alGenSources(num_sound_sources, sources);

      // sundry counters and game state.
      missiles_disabled = 0;
      bombs_disabled = 50;
      invader_velocity = 0.01f;
      live_invaderers = num_invaderers;
      num_lives = 3;
      game_over = false;
      score = 0;
    }

    // called every frame to move things
    void simulate() {

      if (game_over) {
        return;
      }

      move_ship(ship_velocity);

      fire_missiles();

     // fire_bombs();

      bomb_timer();

      move_bombs();

     // move_invaders(invader_velocity, 0, 0);


      sprite &border = sprites[first_border_sprite + (invader_velocity < 0 ? 2 : 3)];
      if (invaders_collide(border)) {
        invader_velocity = -invader_velocity;
        move_invaders(invader_velocity, -0.1f, 0);
      }

    
    }

    // this is called to draw the world
    void draw_world(int x, int y, int w, int h) {
      simulate();

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

      // draw all the sprites
      for (int i = 0; i != num_sprites; ++i) {
        sprites[i].render(texture_shader_, cameraToWorld);
      }

      char score_text[32];
      sprintf(score_text, "score: %d   lives: %d\n", score, num_lives);
      draw_text(texture_shader_, -1.75f, 2, 1.0f / 256, score_text);

      // move the listener with the camera
      vec4 &cpos = cameraToWorld.w();
      alListener3f(AL_POSITION, cpos.x(), cpos.y(), cpos.z());
    }
  };
}
