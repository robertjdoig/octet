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

    int probability_ratio = 50;
    int state = 0;
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
    void set_axiom_state(int change) {
      state += change;
    }

    int get_axiom_state() {
      return state;
    }

    float get_angle() {
      return current_axiom.angle;
    }

    // Itterates through the rule and changes the node depending on the rules
    std::string compute_rule(int itteration_) {
      current_axiom = rules[state];
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
            if (probability > probability_ratio) {
              tempString += current_axiom.r_rule;
            }
            else {
              tempString += c;
            }
            break; }
          default:
            tempString += c;
          }
        }
        current_axiom.node = tempString;
      }
      return current_axiom.node;
    }

    void set_probability_ratio(int value_) {
      probability_ratio += value_;
    }

    int get_probability_ratio() {
      return probability_ratio;
    }

    int get_rule_buffer() {
      return rules.size();
    }

  };
}

#endif // !rule_h
