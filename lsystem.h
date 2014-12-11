////////////////////////////////////////////////////////////////////////////////
//
// (C) Juanmihd
//
////////////////////////////////////////////////////////////////////////////////

#ifndef LSYSTEM_INCLUDED
#define LSYSTEM_INCLUDED

namespace octet{

  /// This is the lsystem class. It will create an object that will control the l_system
  class lsystem : public resource{
    enum { DEBUG_LS_PARSER = 0, DEBUG_LS_GEN = 0, DEBUG_LS_DRAW = 0 };
    /// This struct contain the information of a dupla (left word, with a size, and a list of right words, with a size) 
    struct dupla : public resource{
      char *left;
      int size_left;
      dynarray<char *> right;
      dynarray<int> size_right;
    };

    /// This struct contain the information for a rule (left side, right side, and in which position of the left side is the symbol to be substituted)
    struct rule : public resource{
      dynarray<char> right;
      dynarray<char> context;
      int pos_in_context;
      int iteration;
      float probability;
    };

    /// This is an array with all the words obtained by the LSystem
    dynarray<dynarray<char>> words;
    /// This is a set of rules, it's just an array of rules with all the rules
    dynarray<dynarray<ref<rule>>> rules;
    /// This is a dictionary of symbols, and asign them a number
    dictionary<int> alphabet;
    /// Contains the current iteration of the LSystem
    int cur_iteration;
    /// Max iteration calculated
    int max_iteration;
    /// Initial iteration;
    int ini_iteration;
    /// Array of all the angles
    dynarray<float> ls_angle;
    /// Array of all the distances
    dynarray<float> ls_distance;
    //Things to handle the file
    dynarray<uint8_t> buffer;
    char * currentChar;
    int restBuffer;
    //Special boopleans for special rules
    bool probability_rule;
    bool iteration_rule;
    bool contextual_rule;
    //For random rules
    random rand;

    /// Get next char and reduce the size of the rest of the buffer
    void next_char(){
      ++currentChar; 
      --restBuffer;
    }
    
    /// Copies the rupla contained into the dupla into the new_rule
    void copy_rule(rule *new_rule, const dupla &dupla){
      //Every rule is a "simple rule", SYMBOL:RULE
      //Copy right part
      int size_rule = dupla.size_right[0];
      for (int i = 0; i < size_rule; ++i){
        new_rule->right.push_back(dupla.right[0][i]);
      }
      new_rule->pos_in_context = -1;
      new_rule->iteration = -1;
      new_rule->probability = 1.0f;
      
      //But it may also be a "complex rule", SYMBOL:RULE PROBABILITY ITERATION CONTEXT POS_CONTEXT  
      if (dupla.right.size() > 1){
        //To fill later
        new_rule->probability = get_float(dupla.right[1], dupla.size_right[1]);
      }
    }

    //Get the int value from a word
    int get_int(char *word, int size_word){
      int number = 0;

      for (int i = 0; i != size_word; ++i){
        number = 10 * number + (*word - 0x30);
        ++word;
      }

      return number;
    }

    /// Get the float value from a word
    float get_float(char *word, int size_word){
      float number = 0;
      int decimal = -1;
      int current_pos;
      for (current_pos = 0; current_pos < size_word && decimal == -1; ++current_pos){
        if (*word == '.'){
          decimal = 0;
        }
        else{
          number = 10 * number + (*word - 0x30);
        }
        ++word;
      }
      if (decimal == 0){
        for (; current_pos < size_word; ++current_pos){
          number = 10 * number + (*word - 0x30);
          ++decimal;
          ++word;
        }

        for (int i = 0; i < decimal; ++i){
          number = number / 10;
        }
      }
      return number;
    }

    /// This function check if a char is the left part of the dupla is equal to a given char
    ///   It receives a dupla and also the word it like to be compared into, with the size of the word, and it returns true if equal or false if different
    bool left_side_is(const dupla &new_dupla, char *word2, int size){
      bool equal = new_dupla.size_left == size;
      for (int i = 0; i < size && equal; ++i){
        equal = new_dupla.left[i] == word2[i];
      }
      return equal;
    }

    ///@brief Get the new "dupla" with the left side and the right side
    ///@param new_dupla The dupla where it will read
    ///@return  True if it went well or false if it went father that the rest of the buffer
    bool get_new_dupla_line(dupla &new_dupla){
      new_dupla.left = currentChar;
      new_dupla.size_left = 0;
      new_dupla.right.reset();
      new_dupla.size_right.reset();
      while (*currentChar != ':' && restBuffer > 0){
        ++new_dupla.size_left;
        next_char();
      }
      next_char();
      char *new_right = currentChar;
      int size_new_right = 0;
      while (*currentChar != 0x0D && restBuffer > 0){
        if (*currentChar == 0x20){
          while (*currentChar == 0x20)
            next_char();
          new_dupla.right.push_back(new_right);
          new_dupla.size_right.push_back(size_new_right);
          new_right = currentChar;
          size_new_right = 1;
        }
        else{
          ++size_new_right;
        }
        next_char();
      }

      if (size_new_right != 0){
        new_dupla.right.push_back(new_right);
        new_dupla.size_right.push_back(size_new_right);
      }
      next_char();
      if (*currentChar == 0x0A)
        next_char();
      if(DEBUG_LS_PARSER) printf_dupla(new_dupla);
      return restBuffer > 0;
    }

    /// This funcion receives a dupla and prints it out
    void printf_dupla(const dupla &dupla_){
      for (int i = 0; i < dupla_.size_left; ++i){
        printf("%c", dupla_.left[i]);
      }
      printf(":");
      for (unsigned i = 0; i != dupla_.right.size(); ++i){
        printf("%i ", dupla_.size_right[i]); 
        for (unsigned j = 0; j != dupla_.size_right[i]; ++j){
          printf("%c", dupla_.right[i][j]);
        }
      }
      printf("\n");
    }

    /// Print the initial word
    void printf_word(int num){
      for (int i = 0; i < words[num].size(); ++i){
        printf("%c", words[num][i]);
      }
      printf("\n");
    }

    /// Print the set of rules
    void printf_rules(){
      for (int i = 0; i < rules.size(); ++i){
        for (int j = 0; j < rules[i].size(); ++j){
          printf("%s", rules[i][j]->right.data());
        }
        printf("\n");
      }
      printf("End rules\n");
    }

    /// This will be the whole process of lexer, and parser the LS file
    bool decode_file(){
      int size_rules;
      dupla new_dupla;
      //Check if the first word is "redefine"
      get_new_dupla_line(new_dupla);
      if (left_side_is(new_dupla,"redefine",8)){
        //process redefine (TODO!!!)
        get_new_dupla_line(new_dupla);
      }
      //Process angles
      if (left_side_is(new_dupla, "angle", 5)){
        //process angle
        int sizeAngles = new_dupla.right.size();

        for (int i = 0; i < sizeAngles; ++i){
          ls_angle.push_back(get_float(new_dupla.right[i], new_dupla.size_right[i]));
        }
        get_new_dupla_line(new_dupla);
      }
      //Process distances
      if (left_side_is(new_dupla, "distance", 8)){
        //process distance
        int sizeDistances = new_dupla.right.size();
        for (int i = 0; i < sizeDistances; ++i){
          ls_distance.push_back(get_float(new_dupla.right[i], new_dupla.size_right[i]));
        }
        get_new_dupla_line(new_dupla);
      }
      //Process num iteration
      if (left_side_is(new_dupla, "iteration", 9)){
        //Process num
        ini_iteration = (int) get_float(new_dupla.right[0], new_dupla.size_right[0]);
        get_new_dupla_line(new_dupla);
      }
      //Read symbols
      if (left_side_is(new_dupla, "symbols", 7)){
        dynarray<char> symbol;
        symbol.resize(2);
        int num_symbols = (int)get_float(new_dupla.right[0], new_dupla.size_right[0]);
        for (int i = 0; i < num_symbols; ++i){
          get_new_dupla_line(new_dupla);
          symbol[0] = new_dupla.left[0];
          symbol[1] = '\0';
          alphabet[symbol.data()] = i;
        }
        get_new_dupla_line(new_dupla);
      }
      //Read initial axiom
      if (left_side_is(new_dupla, "initial", 7)){
        words.resize(ini_iteration+5);
        words[0].reserve(5);
        for (int i = 0; i < new_dupla.size_right[0]; ++i){
          words[0].push_back(new_dupla.right[0][i]);
        }
        get_new_dupla_line(new_dupla);
      }
      //Read number and type of rules
      if (left_side_is(new_dupla, "rules", 5)){
        int numInfo = new_dupla.right.size();
        printf("Size right current dupla: %i\n", numInfo);
        size_rules = (int) get_float(new_dupla.right[0], new_dupla.size_right[0]);
        for (int i = 1; i < numInfo; ++i){
          //Check what type of info i'm adding to the system, and set it up
          // { _PROB, _ITER, _CONT, _POS_CON };
          if (new_dupla.right[i][0] == 'P'){
            printf("P!!!!!\n");
            probability_rule = true;
          }
          else if (new_dupla.right[i][0] == 'I'){
            printf("I!!!!!\n");
            iteration_rule = true;
          }
          else if (new_dupla.right[i][0] == 'C'){
            printf("C!!!!!\n");
            contextual_rule = true;
          }
        }
        //Read rules
        dynarray<char> symbol;
        symbol.resize(2);
        rules.resize(alphabet.get_size());
        while (size_rules > 0){
          get_new_dupla_line(new_dupla);
          --size_rules;
          //Create the rule!
          rule *new_rule = new rule;
          copy_rule(new_rule, new_dupla);
          //Storage the rule!
          symbol[0] = new_dupla.left[0];
          symbol[1] = '\0';
          int aux = alphabet[symbol.data()];
          rules[aux].push_back(new_rule);
          if (rules[aux].size() > 1){
            printf("JAJA!");
          }
        }
      }
      return true;
    }

    /// Generates the next iteration of the l_system given a input
    void produce_next_word(dynarray<char> &output, const dynarray<char> &input){
      int size_input = input.size();
      int size_output = size_input;
      output.reserve(size_output);
      for (int i = 0; i < size_input; ++i){
        char symbol[2];
        symbol[0] = input[i];
        symbol[1] = '\0';
        if (!alphabet.contains(symbol)){
          output.push_back(input[i]);
          --size_output;
        }
        else{
          int value_symbol = alphabet[symbol];
          int num_rules = rules[value_symbol].size();
          //Obtain the rule to be applied
          rule * cur_rule;
          if (num_rules > 1){
            if (probability_rule){
              float rand_value = rand.get(0.0f, 1.0f);
              //printf("%f\n", rand_value);
              int i_rule;
              bool rule_find = false;
              for (i_rule = 0; i_rule < num_rules && !rule_find; ++i_rule){
                rule_find = (rules[value_symbol][i_rule]->probability > rand_value);
              }
              cur_rule = rules[value_symbol][i_rule - 1];
            }
            else{
              cur_rule = rules[value_symbol][0];
            }
          } else{
            cur_rule = rules[value_symbol][0];
          }
          int size_rule = cur_rule->right.size();
          if (output.capacity() - output.size() > 10000){
            printf("what?");
            output.reserve(output.size());
            size_output = 0;
          }
          if (size_output <= size_rule){
            output.reserve(output.size() + size_rule * 2);
            size_output += size_rule;
          }
          else
            size_output -= size_rule;
          for (int i_rule = 0; i_rule < size_rule; ++i_rule){
            if (size_output == 0){
              size_output = 1;
              output.reserve(output.capacity() + 2);
            }
            output.push_back(cur_rule->right[i_rule]);
            --size_output;
          }
        }
      }
    }

    ///Will generate the tree to the initial generation. The starting point is max_iteration = 0, and cur_iteration = 0, with ini_iteration the objective
    void ini_generation(){
      for (int i = 1; i <= ini_iteration; ++i){
        produce_next_word(words[i], words[i - 1]);
        if (DEBUG_LS_GEN) printf_word(i);
      }
      cur_iteration = ini_iteration;
      max_iteration = ini_iteration;
    }

    /// Obtains next word
    void generate_new_word(){
      if (words.size() <= cur_iteration)
        words.resize(cur_iteration + 5);
      //printf("L_system_word\n");
      produce_next_word(words[cur_iteration], words[max_iteration]);
      //printf("L_system_word DONE\n");
      ++max_iteration;
    }

  public:
    lsystem(){}

    /// Load the file given by the name to the current l_system. It will initialize it to the current representation
    bool load_file(char * file_name){
      app_utils::get_url(buffer, file_name);
      restBuffer = buffer.size();
      ini_iteration = 0;
      cur_iteration = 0;
      max_iteration = 0;
      probability_rule = false;
      iteration_rule = false;
      contextual_rule = false;
      currentChar = (char*)buffer.data();
      bool no_error = decode_file();
      ini_generation();
      return no_error;
    }

    void recalculate(){
      for (int i = 0; i < words.size(); ++i)
        words[i].reset();
      words.reset();
      for (int i = 0; i < rules.size(); ++i)
        rules[i].reset();
      rules.reset();
      alphabet.reset();
      ls_angle.reset();
      ls_distance.reset();
      restBuffer = buffer.size();
      ini_iteration = 0;
      cur_iteration = 0;
      max_iteration = 0;
      probability_rule = false;
      iteration_rule = false;
      contextual_rule = false;
      currentChar = (char*)buffer.data();
      bool no_error = decode_file();
      ini_generation();
    }

    /// Go to a given iteration of the l_system
    void go_to(int obj_iteration){
      while (cur_iteration < obj_iteration)
        next();
      while (cur_iteration > obj_iteration)
        previous();
    }
    
    /// Go to the next iteration of the l_system
    void next(){
      ++cur_iteration;
      if (cur_iteration > max_iteration){
        generate_new_word();
        //printf("Generating new word\n");
      }
    }

    /// Go to the previous iteration of the l_system
    void previous(){
      if (cur_iteration > 0)
        --cur_iteration;
      else
        printf("Cannot go before the iteration number 0!\n");
    }

    /// Returns the current iteration
    int get_iteration(){
      return cur_iteration;
    }

    ///Return the current word
    char *get_word(){
      return words[cur_iteration].data();
    }

    int get_size_word(){
      return words[cur_iteration].size();
    }

    float get_angle(int i = 0){
      return ls_angle[i];
    }

    float get_distance(int i = 0){
      return ls_distance[i];
    }

    float set_angle(float angle_, int i = 0){
      ls_angle[i] = angle_;
    }

    float set_distance(float distance_, int i = 0){
      ls_distance[i] = distance_;
    }

    /// Print the current iteration
    void print(){
      printf("Current iteration %i\n", cur_iteration);
      printf_word(cur_iteration);
      printf("\n");
    }
  };

}

#endif