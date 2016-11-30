#ifndef rule_h

#define rule_h

namespace octet {

  class rule {

  private:

    struct axiom {
      std::string node;
      std::string f_rule;
      std::string x_rule;
      std::string r_rule;
      float angle; 
    };

    std::vector<axiom> rules;

    axiom current_axiom;
  public:

    rule::rule() {

    }

    // Reads the file to store the rules in a vector
    void readfile() {
      axiom each_rule;

      std::ifstream rule_file("axioms.txt");
      while (rule_file >> each_rule.node >> each_rule.f_rule >> each_rule.x_rule >> each_rule.r_rule >> each_rule.angle) {
        rules.push_back(each_rule);
      }

     current_axiom = rules[0];
    }

    // Switches the rules 
    void switch_axiom(int state) {
        current_axiom = rules[state];
    }

    float get_angle() {
      return current_axiom.angle;
    }

    std::string get_f_rule() {
      return current_axiom.f_rule;
    }

    std::string get_x_rule() {
      return current_axiom.x_rule;
    }

    // Itterates through the rule and changes the node depending on the rules
    std::string compute_rule(int itteration_) {
      for (int i = 0; i != itteration_; i++) {
        std::string tempString;
        for (char& c : current_axiom.node) {
          switch (c) {
          case 'f':
            tempString += current_axiom.f_rule;
            break;
          case 'x':
            tempString += current_axiom.x_rule;
            break;
          case 'r': {
            int probability = rand() % 100;
            std::cout << probability << "\n";
            if (probability > 50)
              tempString += current_axiom.r_rule;
            break; }
          default: 
           tempString += c;
          }
        }
        current_axiom.node = tempString;
      }
      return current_axiom.node;
    }

  };
}

#endif // !rule_h
