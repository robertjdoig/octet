# Space Invaders Bomber Man


# Brief 
To change the basic space invaders program to a different game.  

# [Video link](https://www.youtube.com/watch?v=YU7D2hr5v8s&feature=youtu.be "Video Link")

# Introduction 

This game is BomberMan, it focuses on a few technical areas.  


# Shader Programming 

The shader program was adapted to include 4 variables. It passes in a matrix, texture, colour and a frame counter. These variables are used to create the visual effect on all the sprites in the scene. 

The variables are input to the shader program via the render function. This function is called at runtime to draw the sprites in the game. The inputs are passed to the local variables. The 'glUniform' function connects the main variable to the shader variable, which allows it to be used in the rest of the shader program. 

    void render(const mat4t &modelToProjection, int sampler, float colourArray[4], int time) {
      // tell openGL to use the program
      shader::render();

      // customize the program with uniforms
      glUniform1i(samplerIndex_, sampler);
      glUniformMatrix4fv(modelToProjectionIndex_, 1, GL_FALSE, modelToProjection.get());
      glUniform4fv(colourIndex_, 1,colourArray);
      glUniform1i(timeIndex_, time);
    }

In the main function of the fragment shader, the red, green, blue channels are calculated by the position on the screen over time. This creates a pixelated effect over the existing sprites which bound to time. The RGB channels are then multiplied by the colour and the texture of the sprite. This is happening for all the pixels of the sprites. 

    void main() {      
      float r = fract(sin(length(floor(gl_FragCoord.xy / 5.0) + time))*1e6);
      float g = fract(cos(length(floor(gl_FragCoord.xy / 3.0) + time))*1e6);
      float b = fract(tan(length(floor(gl_FragCoord.xy / 1.0) + time))*1e6);

      gl_FragColor = (texture2D(sampler, uv_) * vec4(r, g, b, 1))*colour;
    }

# TXT level generation 


The level is designed and drawn from a .TXT file. This gives more flexibility to tweak the level design without hard coding it. 

The function 'read_file' handles all of the file data. The input is an index to the position of the array needed, which returns the character in the level file. This is then put through a switch to choose the tile that should be drawn at that location. 

The level generation is implemented using the 'std::ifstream' and the 'std::string' variables. The ifstream reads the file "pos.txt". The file is then added line by line to a static std::string. Static is used in this case to read and store the data once, instead of rewriting the data for every cell. A while loop is also used so that the .TXT file length can be changed. The reason for using carriage returns in the .TXT file instead of writing it all on one line is so that the level design is more visible when looking at the text file.  The code used to perform this function was posted on the Stackoverflow forum. The link is below.    

      //http://stackoverflow.com/questions/13035674/how-to-read-line-by-line-or-a-whole-text-file-at-once
      std::ifstream myfile("pos.txt");
      std::string text; 
      static std::string com_line;
      while (std::getline(myfile, text)){
        com_line += text;
      }

The combined lines (com_line) is then reformatted into a character array so that it can be indexed from the switch statement. The std::strcpy functions breaks up the string into characters and adds them to the character array with the same length as the combined line string. This conversion was posted on a Cplusplus forum, with the link below. 
      
      // http://www.cplusplus.com/forum/general/100714/ 
      char *chars = new char[com_line.length()];
      std::strcpy(chars, com_line.c_str());
   
The read_file function returns a character with the index that is pasted to it from the switch statement. The arrayPos is a position in the level matrix which matches up to the position of characters in the text file "pos.txt". 

      char read_file(int arrayPos) {
      
          //READING THE FILE CODE
	
          return   chars[arrayPos];
      }

Then finally to draw the sprites, the switch statement will go through the conditions and draw the sprite defined in the character that has been past to it. So for each of the characters a different case is being activated. For example when a full stop is passed to the switch statement a path sprite will be drawn at that cell. 
 
    //This code has been shortened and is missing the sprite draw calls. 

    for (int j = 0; j != num_rows; j++) {
        for (int i = 0; i != num_cols; i++) {

          switch (read_file(i + j*num_cols)) {
          case '.':
            //Draw Path
            break;

          case 'x':
            //draw path
            //draw Invader
            break;

          case 'b':
            //draw block 
            break;

          default:
            std::cout << "not reading the level file";
          }
        }
     }

# Gameplay 


The gameplay is very simple. The user can move on both the X and Y axis to avoid the space invaders. The gameplay was not finished so only the ship movement and the collisions between the walls, bullets and ship were implemented.

The input from the keyboard allows the user to move up, down, left and right from the respected arrow keys. This is handled by the 'is_key_down' function within octet. When the key is being pressed a translation is added to the direction of the arrow. Within this IF statement is nested an second IF statement. This is triggered when the ship is colliding with the block sprite. This negates the translation of the first movement by adding an second translation in the opposite direction. 

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

When the player has fired a bullet at the invaders is will collide with either a block, border or invader. When this happens the bullet is translated off screen and disabled so that it can be reused. This also happens to the invaders when they are killed. However, the blocks and borders are unchanged when hit otherwise you would be able to destroy the level. 


# Joint work

Matthew Duddington - worked on the CSV Reader function and level generation switch. 

Louis Bennette - He showed me how to pass a variable to a shader. 


