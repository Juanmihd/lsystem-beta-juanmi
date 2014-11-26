////////////////////////////////////////////////////////////////////////////////
//
// (C) Juanmihd
//
////////////////////////////////////////////////////////////////////////////////

#ifndef LSYSTEM_INCLUDED
#define LSYSTEM_INCLUDED

namespace octet{

  /// @brief This is the lsystem class. It will create an object that will control the l_system
  class lsystem{

    struct rule{
      int posSymbol;
      char *substitution;
    };

    struct dupla{
      char *left;
      int size_left;
      dynarray<char *> right;
      dynarray<int> size_right;
    };

    dynarray<char> initial_word;
    dynarray<dynarray<char>> word;
    dynarray<dynarray<char>> set_rules;
    int num_rules;
    dynarray<int> symbols;
    dynarray<int> symbols_rev;
    int iteration;
    dynarray<float> ls_angle;
    dynarray<float> ls_distance;
    //Things to handle the file
    dynarray<uint8_t> buffer;
    char * currentChar;
    int restBuffer;

    /// Get next char and reduce the size of the rest of the buffer
    void next_char(){
      ++currentChar; 
      --restBuffer;
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
          number = 10 * number + *word;
        }
        ++word;
      }
      if (decimal == 0){
        for (; current_pos < size_word; ++current_pos){
          number = 10 * number + *word;
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
    bool left_side_is(dupla new_dupla, char *word2, int size){
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
      while (*currentChar != '\n' && restBuffer > 0){
        if (*currentChar == 0x20){
          next_char();
          new_dupla.right.push_back(new_right);
          new_dupla.size_right.push_back(size_new_right);
          new_right = currentChar;
          size_new_right = 0;
        }
        else
          ++size_new_right;
        next_char();
      }
      next_char();
      return restBuffer > 0;
    }

    ///@brief This will be the whole process of lexer, and parser the LS file
    bool decode_file(){
      dupla new_dupla;
      //Check if the first word is "redefine"
      get_new_dupla_line(new_dupla);
      if (left_side_is(new_dupla,"redefine",8)){
        //process redefine
        get_new_dupla_line(new_dupla);
      }
      //Process angles
      if (left_side_is(new_dupla, "angle", 5)){
        //process angle
        int sizeAngles = new_dupla.right.size();
        for (int i = 0; i < sizeAngles; ++i){
          ls_angle.push_back(get_float(new_dupla.right[i],new_dupla.size_right[i]));
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
      //Read initial axiom
      if (left_side_is(new_dupla, "initial", 7)){
        for (int i = 0; i < new_dupla.size_left; ++i){
          initial_word.push_back(new_dupla.left[i]);
        }
        get_new_dupla_line(new_dupla);
      }
      //Read number and type of rules
      if (left_side_is(new_dupla, "rules", 5)){
        int numInfo = new_dupla.right.size();
        num_rules = get_float(new_dupla.right[0], new_dupla.size_right[0]);
        for (int i = 1; i < numInfo; ++i){
          //Check what type of info i'm adding to the system, and set it up
        }
      }
      //Read rules
      set_rules.resize(num_rules);
      while (num_rules > 0){
        get_new_dupla_line(new_dupla);
        --num_rules;

      }
      return true;
    }

  public:
    lsystem(){}

    bool load_file(char * file_name){
      dynarray<uint8_t> buffer;
      restBuffer = buffer.size();
      currentChar = (char*)buffer.data();
      app_utils::get_url(buffer, file_name);
      return decode_file();
    }
  };

}

#endif