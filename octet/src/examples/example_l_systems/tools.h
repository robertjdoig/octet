#ifndef tools_h

#define tools_h


namespace octet {
  static class tools {

  public:

    static float pie() {
      return atan(1) * 4;
    }

    static float degToRad(float degrees_) {
      return degrees_ * (pie() / 180);
    }

    static void polarToCart( float radius, float angle, vec2& cart) {
      cart.x() -= radius * sin(degToRad(angle));
      cart.y() += radius * cos(degToRad(angle));      
    }

  };

}

#endif // !tools_h
