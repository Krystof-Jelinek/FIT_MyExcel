#ifndef __PROGTEST__
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cctype>
#include <climits>
#include <cfloat>
#include <cassert>
#include <cmath>
#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <string>
#include <array>
#include <vector>
#include <list>
#include <set>
#include <map>
#include <stack>
#include <queue>
#include <unordered_set>
#include <unordered_map>
#include <memory>
#include <algorithm>
#include <functional>
#include <iterator>
#include <stdexcept>
#include <variant>
#include <optional>
#include <compare>
#include <charconv>
#include <span>
#include <utility>
#include "expression.h"
using namespace std::literals;
using CValue = std::variant<std::monostate, double, std::string>;

constexpr unsigned                     SPREADSHEET_CYCLIC_DEPS                 = 0x01;
constexpr unsigned                     SPREADSHEET_FUNCTIONS                   = 0x02;
constexpr unsigned                     SPREADSHEET_FILE_IO                     = 0x04;
constexpr unsigned                     SPREADSHEET_SPEED                       = 0x08;
constexpr unsigned                     SPREADSHEET_PARSER                      = 0x10;
#endif /* __PROGTEST__ */

//helper debugging function
std::ostream& operator<<(std::ostream& os, const CValue& value){
  if(value.index() == 0){
    os << "monostate";
    return os;
  }
  if(value.index() == 1){
    os << std::get<double> ( value );
    return os;
  }
  if(value.index() == 2){
    os << std::get<std::string> ( value );
    return os;
  }
  throw std::logic_error("CValue << operator fail");
}

//------------------------------------------OPERATORS---------------------------------------------------------

CValue operator + (const CValue & first, const CValue & second){
  if((first.index() == 0)||(second.index() == 0)){
    return std::monostate{};
  }
  if(first.index() == 2 && second.index() == 2){
    return std::get<std::string> ( first ) + std::get<std::string> ( second );
  }
  if(first.index() == 1 && second.index() == 1){
    return std::get<double> ( first ) + std::get<double> ( second );
  }
  if(first.index() == 1 && second.index() == 2){
    return std::to_string(std::get<double> ( first )) + std::get<std::string> ( second );
  }
  if(first.index() == 2 && second.index() == 1){
    return std::get<std::string> ( first ) + std::to_string(std::get<double> ( second ));
  }
  throw std::logic_error("something went wrong in + operator for CValue");
} 

CValue operator - (const CValue & first, const CValue & second){
  if((first.index() != second.index())){
    return std::monostate{};
  }
  if((first.index() == 0) || second.index() == 2){
    return std::monostate{};
  }
  return std::get<double> ( first ) - std::get<double> ( second );
}

CValue operator * (const CValue & first, const CValue & second){
  if((first.index() != second.index())){
    return std::monostate{};
  }
  if((first.index() == 0) || second.index() == 2){
    return std::monostate{};
  }
  return std::get<double> ( first ) * std::get<double> ( second );
}

CValue operator / (const CValue & first, const CValue & second){
  if((first.index() != second.index())){
    return std::monostate{};
  }
  if((first.index() == 0) || second.index() == 2){
    return std::monostate{};
  }
  if(std::get<double> ( second ) == 0){
    return std::monostate{};
  }
  return std::get<double> ( first ) / std::get<double> ( second );
}

CValue operator == (const CValue & first, const CValue & second){
  if((first.index() != second.index())){
    return std::monostate{};
  }
  if((first.index() == 2) || second.index() == 2){
    if(std::get<std::string> ( first ) == std::get<std::string> ( second )){
      return double(1);
    }
    return double(0);
  }
  if(std::get<double> ( first ) == std::get<double> ( second )){
    return double(1);
  }
  return double(0);
}

CValue operator != (const CValue & first, const CValue & second){
  CValue tmp = (first == second);
  if(tmp.index() == 0){
    return std::monostate{};
  }
  if(std::get<double> ( tmp ) == 1){
    return double(0);
  }
  return double(1);
}

CValue operator < (const CValue & first, const CValue & second){
  if((first.index() != second.index())){
    return std::monostate{};
  }
  if((first.index() == 2) || second.index() == 2){
    if(std::get<std::string> ( first ) < std::get<std::string> ( second )){
      return double(1);
    }
    return double(0);
  }
  if(std::get<double> ( first ) < std::get<double> ( second )){
    return double(1);
  }
  return double(0);
}

CValue operator <= (const CValue & first, const CValue & second){
  if((first.index() != second.index())){
    return std::monostate{};
  }
  if((first.index() == 2) || second.index() == 2){
    if(std::get<std::string> ( first ) <= std::get<std::string> ( second )){
      return double(1);
    }
    return double(0);
  }
  if(std::get<double> ( first ) <= std::get<double> ( second )){
    return double(1);
  }
  return double(0);
}

CValue operator > (const CValue & first, const CValue & second){
  if((first.index() != second.index())){
    return std::monostate{};
  }
  if((first.index() == 2) || second.index() == 2){
    if(std::get<std::string> ( first ) > std::get<std::string> ( second )){
      return double(1);
    }
    return double(0);
  }
  if(std::get<double> ( first ) > std::get<double> ( second )){
    return double(1);
  }
  return double(0);
}

CValue operator >= (const CValue & first, const CValue & second){
  if((first.index() != second.index())){
    return std::monostate{};
  }
  if((first.index() == 2) || second.index() == 2){
    if(std::get<std::string> ( first ) >= std::get<std::string> ( second )){
      return double(1);
    }
    return double(0);
  }
  if(std::get<double> ( first ) >= std::get<double> ( second )){
    return double(1);
  }
  return double(0);
}

CValue my_pow(const CValue & first, const CValue & second){
  if((first.index() != second.index())){
    return std::monostate{};
  }
  if((first.index() == 0) || second.index() == 2){
    return std::monostate{};
  }
  return std::pow(std::get<double> ( first ),std::get<double> ( second ));
}

CValue my_neg(const CValue & first){
  if((first.index() == 0)||first.index() == 2){
    return std::monostate{};
  }
  return -std::get<double> ( first );
}

class CCell;

//------------------------------------------CPOS---------------------------------------------------------


//Class that represents coordinates of  cells inside sheet
class CPos
{
  public:
    CPos(){
      first_cord = "";
      second_cord = -1;
    }
    
    CPos(std::string_view str){
        size_t i = 0;
        second_cord = 0;
        bool invalid = true;
        //letters
        while (i < str.size() && std::isalpha(str[i])) {
            first_cord += std::toupper(str[i]);
            i++;
        }
        //numbers
        while (i < str.size() && std::isdigit(str[i])) {
            second_cord = second_cord * 10 + (str[i] - '0');
            i++;
            invalid = false;
        }
        // Check if the entire string was consumed correctly
        if (i != str.size() || first_cord.empty() || second_cord < 0 || invalid == true) {
            throw std::invalid_argument("Invalid CPos format");
        }
    }
    
    CPos & operator =(const CPos & in){
      this->first_cord = in.first_cord;
      this->second_cord = in.second_cord;
      return *this;
    }

    void changeSecondCord(int i){
      second_cord = second_cord + i;
      if(second_cord < 0){
        throw std::invalid_argument("Invalid CPos second coordinate");
      }
    }
    
    // function that can change the first coordinate(letters) in i direction (works for negative i values as well)
    void changeFirstCord(int i){
      std::string uppercaseFirstCord = first_cord;

      // Iterate through each character of the string in reverse order
      for (; i != 0; i += (i > 0) ? -1 : 1) {
          for (int j = uppercaseFirstCord.size() - 1; j >= 0; --j) {
              if (i > 0) {
                  // If the character is 'Z', change it to 'A' and continue iterating to the previous character
                  if (uppercaseFirstCord[j] == 'Z') {
                      uppercaseFirstCord[j] = 'A';
                  } else { // If the character is not 'Z', increment it and break the loop
                      ++uppercaseFirstCord[j];
                      break;
                  }
                  // If we reach the beginning of the string and it's 'Z', add 'A' at the beginning
                  if (j == 0 && uppercaseFirstCord[j] == 'A') {
                      uppercaseFirstCord = 'A' + uppercaseFirstCord;
                  }
              } else { // Decrementing
                  // If the character is 'A', change it to 'Z' and continue iterating to the previous character
                  if (uppercaseFirstCord[j] == 'A') {
                      uppercaseFirstCord[j] = 'Z';
                  } else { // If the character is not 'A', decrement it and break the loop
                      --uppercaseFirstCord[j];
                      break;
                  }
                  // If we reach the beginning of the string, remove the first character ('A')
                  if (j == 0 && uppercaseFirstCord[j] == 'Z') {
                      uppercaseFirstCord = uppercaseFirstCord.substr(1);
                  }
              }
          }
      }
      first_cord = uppercaseFirstCord;
    }
  

    std::string first_cord;
    int second_cord;
};

//------------------------------------------CNODES---------------------------------------------------------


//abstract class from which all operations will derive this class will be the nodes of AST
class CNode{
  public:
    //this function evaluates the node it self it will mostly likely evaluate all its arguments(buble down the AST) then use overloaded operators for computing the result
    virtual CValue evaluate() = 0;
    //this function will return all references of the AST in form of strings
    virtual void getReferences(std::set<std::string> & references) = 0;
    //this function basically does bfs on the graph to find cyclic dependencies
    virtual void checkcyclic(std::set<CPos> in_set, bool & flag_valid) = 0;
    virtual ~CNode(){}
};

//this class is a bit redundant serves only for a better code readability
class CAst{
  public:  
    std::shared_ptr<CNode> root;
};

class CCell{
  private:
    CAst eval_tree; // stores AST for this cell
    bool already_parsed = false;
  public:
    std::string expresion;
    CPos position;
    std::shared_ptr<std::map<CPos, CCell>> table_ptr;

    CCell(std::shared_ptr<std::map<CPos, CCell>> in);
    CValue getValue();
    bool setValue(const std::string & input);
    void getReferences(std::set<std::string> & references);
    CCell& operator=(const CCell& other);
    //this function will shift all references by x and y coordinates x and y both can be negative
    void moveReferences(int x_off, int y_off);
    void checkcyclic(std::set<CPos> in_set, bool & flag_valid);
};

class CValNode : public CNode{
  public:
  CValNode(CValue in){
    value = in;
  }
  CValNode(double in){
    value = in;
  }
  CValNode(std::string in){
    value = in;
  }
  virtual ~CValNode() override{}
  
  virtual CValue evaluate() override{
    return value;
  }

  virtual void checkcyclic(std::set<CPos> in_set, bool & flag_valid){
    return;
  }

  virtual void getReferences(std::set<std::string> & references){
    return;
  }

  private:
    CValue value;
};

class CReference : public CNode{
  public:
  CReference(const std::string & in_pos, std::shared_ptr<std::map<CPos, CCell>> in_ptr){
    std::string str = in_pos;
    reference_string = in_pos;
    str.erase(std::remove_if(str.begin(), str.end(), [](char c) {
        return c == '$';
    }), str.end());
    
    position = CPos(str);
    table_ptr = in_ptr;
  }
  
  virtual CValue evaluate() override{
    //this cells needs to acces the sheet so it can determine the value that its referencing too
    if(table_ptr->find(position) != table_ptr->end()){
      return table_ptr->at(position).getValue();
    }
    CValue tmp;
    return tmp;
  }

  //this is the only class that is adding something to the set other will only be passing it down the AST
  virtual void getReferences(std::set<std::string> & references){
    references.insert(reference_string);
  }

  virtual void checkcyclic(std::set<CPos> in_set, bool & flag_valid){
    //if we found that we are dependant on ourself thats a cyclic dependency
    if(in_set.find(position) != in_set.end()){
      flag_valid = false;
      return;
    }
    //else we continue the bfs and add ourself as dependency that we cant meet again when searching the graph
    if(table_ptr->find(position) != table_ptr->end()){
      table_ptr->at(position).checkcyclic(in_set, flag_valid);
    }
  }

  private:
    CPos position;
    std::string reference_string;
    std::shared_ptr<std::map<CPos, CCell>> table_ptr;
};

class CPlus : public CNode{
  public:
  CPlus(std::shared_ptr<CNode> first, std::shared_ptr<CNode> second){
    first_arg = first;
    second_arg = second;
  }

  virtual CValue evaluate() override{
    return first_arg->evaluate() + second_arg->evaluate();
  }

  virtual void getReferences(std::set<std::string> & references){
    first_arg->getReferences(references);
    second_arg->getReferences(references);
  }

  virtual void checkcyclic(std::set<CPos> in_set, bool & flag_valid){
    if(!flag_valid){
      return;
    }
    first_arg->checkcyclic(in_set, flag_valid);
    second_arg->checkcyclic(in_set, flag_valid);
  }

  private:
    std::shared_ptr<CNode> first_arg;
    std::shared_ptr<CNode> second_arg;
};

class CMinus : public CNode{
  public:
  CMinus(std::shared_ptr<CNode> first, std::shared_ptr<CNode> second){
    first_arg = first;
    second_arg = second;
  }

  virtual CValue evaluate() override{
    return first_arg->evaluate() - second_arg->evaluate();
  }

  virtual void getReferences(std::set<std::string> & references){
    first_arg->getReferences(references);
    second_arg->getReferences(references);
  }

  virtual void checkcyclic(std::set<CPos> in_set, bool & flag_valid){
    if(!flag_valid){
      return;
    }
    first_arg->checkcyclic(in_set, flag_valid);
    second_arg->checkcyclic(in_set, flag_valid);
  }

  private:
    std::shared_ptr<CNode> first_arg;
    std::shared_ptr<CNode> second_arg;
};

class CMul : public CNode{
  public:
  CMul(std::shared_ptr<CNode> first, std::shared_ptr<CNode> second){
    first_arg = first;
    second_arg = second;
  }

  virtual CValue evaluate() override{
    return first_arg->evaluate() * second_arg->evaluate();
  }

  virtual void getReferences(std::set<std::string> & references){
    first_arg->getReferences(references);
    second_arg->getReferences(references);
  }

  virtual void checkcyclic(std::set<CPos> in_set, bool & flag_valid){
    if(!flag_valid){
      return;
    }
    first_arg->checkcyclic(in_set, flag_valid);
    second_arg->checkcyclic(in_set, flag_valid);
  }

  private:
    std::shared_ptr<CNode> first_arg;
    std::shared_ptr<CNode> second_arg;
};

class CDiv : public CNode{
  public:
  CDiv(std::shared_ptr<CNode> first, std::shared_ptr<CNode> second){
    first_arg = first;
    second_arg = second;
  }

  virtual CValue evaluate() override{
    return first_arg->evaluate() / second_arg->evaluate();
  }

  virtual void getReferences(std::set<std::string> & references){
    first_arg->getReferences(references);
    second_arg->getReferences(references);
  }

  virtual void checkcyclic(std::set<CPos> in_set, bool & flag_valid){
    if(!flag_valid){
      return;
    }
    first_arg->checkcyclic(in_set, flag_valid);
    second_arg->checkcyclic(in_set, flag_valid);
  }

  private:
    std::shared_ptr<CNode> first_arg;
    std::shared_ptr<CNode> second_arg;
};

class CPow : public CNode{
  public:
  CPow(std::shared_ptr<CNode> first, std::shared_ptr<CNode> second){
    first_arg = first;
    second_arg = second;
  }

  virtual CValue evaluate() override{
    return my_pow(first_arg->evaluate(), second_arg->evaluate());
  }

  virtual void getReferences(std::set<std::string> & references){
    first_arg->getReferences(references);
    second_arg->getReferences(references);
  }

  virtual void checkcyclic(std::set<CPos> in_set, bool & flag_valid){
    if(!flag_valid){
      return;
    }
    first_arg->checkcyclic(in_set, flag_valid);
    second_arg->checkcyclic(in_set, flag_valid);
  }

  private:
    std::shared_ptr<CNode> first_arg;
    std::shared_ptr<CNode> second_arg;
};

class CNeg : public CNode{
  public:
  CNeg(std::shared_ptr<CNode> in){
    arg = in;
  }

  virtual CValue evaluate() override{
    return my_neg(arg->evaluate());
  }

  virtual void getReferences(std::set<std::string> & references){
    arg->getReferences(references);
  }

  virtual void checkcyclic(std::set<CPos> in_set, bool & flag_valid){
    if(!flag_valid){
      return;
    }
    arg->checkcyclic(in_set, flag_valid);
  }

  private:
    std::shared_ptr<CNode> arg;
};

class CEq : public CNode{
  public:
  CEq(std::shared_ptr<CNode> first, std::shared_ptr<CNode> second){
    first_arg = first;
    second_arg = second;
  }

  virtual CValue evaluate() override{
    return first_arg->evaluate() == second_arg->evaluate();
  }

  virtual void getReferences(std::set<std::string> & references){
    first_arg->getReferences(references);
    second_arg->getReferences(references);
  }

  virtual void checkcyclic(std::set<CPos> in_set, bool & flag_valid){
    if(!flag_valid){
      return;
    }
    first_arg->checkcyclic(in_set, flag_valid);
    second_arg->checkcyclic(in_set, flag_valid);
  }

  private:
    std::shared_ptr<CNode> first_arg;
    std::shared_ptr<CNode> second_arg;
};

class CNeq : public CNode{
  public:
  CNeq(std::shared_ptr<CNode> first, std::shared_ptr<CNode> second){
    first_arg = first;
    second_arg = second;
  }

  virtual CValue evaluate() override{
    return first_arg->evaluate() != second_arg->evaluate();
  }

  virtual void getReferences(std::set<std::string> & references){
    first_arg->getReferences(references);
    second_arg->getReferences(references);
  }

  virtual void checkcyclic(std::set<CPos> in_set, bool & flag_valid){
    if(!flag_valid){
      return;
    }
    first_arg->checkcyclic(in_set, flag_valid);
    second_arg->checkcyclic(in_set, flag_valid);
  }

  private:
    std::shared_ptr<CNode> first_arg;
    std::shared_ptr<CNode> second_arg;
};

class CLt : public CNode{
  public:
  CLt(std::shared_ptr<CNode> first, std::shared_ptr<CNode> second){
    first_arg = first;
    second_arg = second;
  }

  virtual CValue evaluate() override{
    return first_arg->evaluate() < second_arg->evaluate();
  }

  virtual void getReferences(std::set<std::string> & references){
    first_arg->getReferences(references);
    second_arg->getReferences(references);
  }

  virtual void checkcyclic(std::set<CPos> in_set, bool & flag_valid){
    if(!flag_valid){
      return;
    }
    first_arg->checkcyclic(in_set, flag_valid);
    second_arg->checkcyclic(in_set, flag_valid);
  }

  private:
    std::shared_ptr<CNode> first_arg;
    std::shared_ptr<CNode> second_arg;
};

class CLe : public CNode{
  public:
  CLe(std::shared_ptr<CNode> first, std::shared_ptr<CNode> second){
    first_arg = first;
    second_arg = second;
  }

  virtual CValue evaluate() override{
    return first_arg->evaluate() <= second_arg->evaluate();
  }

  virtual void getReferences(std::set<std::string> & references){
    first_arg->getReferences(references);
    second_arg->getReferences(references);
  }

  virtual void checkcyclic(std::set<CPos> in_set, bool & flag_valid){
    if(!flag_valid){
      return;
    }
    first_arg->checkcyclic(in_set, flag_valid);
    second_arg->checkcyclic(in_set, flag_valid);
  }

  private:
    std::shared_ptr<CNode> first_arg;
    std::shared_ptr<CNode> second_arg;
};

class CGt : public CNode{
  public:
  CGt(std::shared_ptr<CNode> first, std::shared_ptr<CNode> second){
    first_arg = first;
    second_arg = second;
  }

  virtual CValue evaluate() override{
    return first_arg->evaluate() > second_arg->evaluate();
  }

  virtual void getReferences(std::set<std::string> & references){
    first_arg->getReferences(references);
    second_arg->getReferences(references);
  }

  virtual void checkcyclic(std::set<CPos> in_set, bool & flag_valid){
    if(!flag_valid){
      return;
    }
    first_arg->checkcyclic(in_set, flag_valid);
    second_arg->checkcyclic(in_set, flag_valid);
  }

  private:
    std::shared_ptr<CNode> first_arg;
    std::shared_ptr<CNode> second_arg;
};

class CGe : public CNode{
  public:
  CGe(std::shared_ptr<CNode> first, std::shared_ptr<CNode> second){
    first_arg = first;
    second_arg = second;
  }

  virtual CValue evaluate() override{
    return first_arg->evaluate() >= second_arg->evaluate();
  }

  virtual void getReferences(std::set<std::string> & references){
    first_arg->getReferences(references);
    second_arg->getReferences(references);
  }

  virtual void checkcyclic(std::set<CPos> in_set, bool & flag_valid){
    if(!flag_valid){
      return;
    }
    first_arg->checkcyclic(in_set, flag_valid);
    second_arg->checkcyclic(in_set, flag_valid);
  }

  private:
    std::shared_ptr<CNode> first_arg;
    std::shared_ptr<CNode> second_arg;
};

//------------------------------------------BUILDER---------------------------------------------------------


class CMyBuilder : public CExprBuilder{
  public:
    CMyBuilder(){}

    //this servers a purpos of not computing the result, if this flag is set it wont try to create result it will only create
    // the AST so it can be searched in for dependencies which will then be check for cycle
    CMyBuilder(bool onlyCyclic){
      cyclicCheckeronly = onlyCyclic;
    }

    virtual void  opAdd(){
      //first i take out the SECOND argument of the function cuz thats how zasobnik works
      //! this part computes the result
      if(!cyclicCheckeronly)
      {
        std::shared_ptr<CNode> second = my_stack.top();
        my_stack.pop();
        std::shared_ptr<CNode> first = my_stack.top();
        my_stack.pop();
        CPlus in(first,second);
        CValue tmp = in.evaluate();
        storeCValue(tmp);
      }

      //! this part builds the three
      {
        std::shared_ptr<CNode> second = build_stack.top();
        build_stack.pop();
        std::shared_ptr<CNode> first = build_stack.top();
        build_stack.pop();
        CPlus in(first,second);
        std::shared_ptr<CNode> to_be_pushed = std::make_shared<CPlus>(in);
        build_stack.push(to_be_pushed);
      }
    }
    virtual void  opSub(){
      //first i take out the SECOND argument of the function cuz thats how zasobnik works
      //! this part computes the result
      if(!cyclicCheckeronly)
      {
        std::shared_ptr<CNode> second = my_stack.top();
        my_stack.pop();
        std::shared_ptr<CNode> first = my_stack.top();
        my_stack.pop();
        CMinus in(first,second);
        CValue tmp = in.evaluate();
        storeCValue(tmp);
      }

      //! this part builds the three
      {
        std::shared_ptr<CNode> second = build_stack.top();
        build_stack.pop();
        std::shared_ptr<CNode> first = build_stack.top();
        build_stack.pop();
        CMinus in(first,second);
        std::shared_ptr<CNode> to_be_pushed = std::make_shared<CMinus>(in);
        build_stack.push(to_be_pushed);
      }
    }
    virtual void  opMul() {
      //first i take out the SECOND argument of the function cuz thats how zasobnik works
      //! this part computes the result
      if(!cyclicCheckeronly)
      {
        std::shared_ptr<CNode> second = my_stack.top();
        my_stack.pop();
        std::shared_ptr<CNode> first = my_stack.top();
        my_stack.pop();
        CMul in(first,second);
        CValue tmp = in.evaluate();
        storeCValue(tmp);
      }

      //! this part builds the three
      {
        std::shared_ptr<CNode> second = build_stack.top();
        build_stack.pop();
        std::shared_ptr<CNode> first = build_stack.top();
        build_stack.pop();
        CMul in(first,second);
        std::shared_ptr<CNode> to_be_pushed = std::make_shared<CMul>(in);
        build_stack.push(to_be_pushed);
      }
    }
    virtual void  opDiv() {
      //first i take out the SECOND argument of the function cuz thats how zasobnik works
      //! this part computes the result
      if(!cyclicCheckeronly)
      {
        std::shared_ptr<CNode> second = my_stack.top();
        my_stack.pop();
        std::shared_ptr<CNode> first = my_stack.top();
        my_stack.pop();
        CDiv in(first,second);
        CValue tmp = in.evaluate();
        storeCValue(tmp);
      }

      //! this part builds the three
      {
        std::shared_ptr<CNode> second = build_stack.top();
        build_stack.pop();
        std::shared_ptr<CNode> first = build_stack.top();
        build_stack.pop();
        CDiv in(first,second);
        std::shared_ptr<CNode> to_be_pushed = std::make_shared<CDiv>(in);
        build_stack.push(to_be_pushed);
      }
    }
    virtual void  opPow() {
      //first i take out the SECOND argument of the function cuz thats how zasobnik works
      //! this part computes the result
      if(!cyclicCheckeronly)
      {
        std::shared_ptr<CNode> second = my_stack.top();
        my_stack.pop();
        std::shared_ptr<CNode> first = my_stack.top();
        my_stack.pop();
        CPow in(first,second);
        CValue tmp = in.evaluate();
        storeCValue(tmp);
      }

      //! this part builds the three
      {
        std::shared_ptr<CNode> second = build_stack.top();
        build_stack.pop();
        std::shared_ptr<CNode> first = build_stack.top();
        build_stack.pop();
        CPow in(first,second);
        std::shared_ptr<CNode> to_be_pushed = std::make_shared<CPow>(in);
        build_stack.push(to_be_pushed);
      }
    }
    virtual void  opNeg() {
      //first i take out the SECOND argument of the function cuz thats how zasobnik works
      //! this part computes the result
      if(!cyclicCheckeronly)
      {
        std::shared_ptr<CNode> first = my_stack.top();
        my_stack.pop();
        CNeg in(first);
        CValue tmp = in.evaluate();
        storeCValue(tmp);
      }

      //! this part builds the three
      {
        std::shared_ptr<CNode> first = build_stack.top();
        build_stack.pop();
        CNeg in(first);
        std::shared_ptr<CNode> to_be_pushed = std::make_shared<CNeg>(in);
        build_stack.push(to_be_pushed);
      }
    }
    virtual void  opEq() {
      //first i take out the SECOND argument of the function cuz thats how zasobnik works
      //! this part computes the result
      if(!cyclicCheckeronly)
      {
        std::shared_ptr<CNode> second = my_stack.top();
        my_stack.pop();
        std::shared_ptr<CNode> first = my_stack.top();
        my_stack.pop();
        CEq in(first,second);
        CValue tmp = in.evaluate();
        storeCValue(tmp);
      }

      //! this part builds the three
      {
        std::shared_ptr<CNode> second = build_stack.top();
        build_stack.pop();
        std::shared_ptr<CNode> first = build_stack.top();
        build_stack.pop();
        CEq in(first,second);
        std::shared_ptr<CNode> to_be_pushed = std::make_shared<CEq>(in);
        build_stack.push(to_be_pushed);
      }
    }
    virtual void  opNe() {
      //first i take out the SECOND argument of the function cuz thats how zasobnik works
      //! this part computes the result
      if(!cyclicCheckeronly)
      {
        std::shared_ptr<CNode> second = my_stack.top();
        my_stack.pop();
        std::shared_ptr<CNode> first = my_stack.top();
        my_stack.pop();
        CNeq in(first,second);
        CValue tmp = in.evaluate();
        storeCValue(tmp);
      }

      //! this part builds the three
      {
        std::shared_ptr<CNode> second = build_stack.top();
        build_stack.pop();
        std::shared_ptr<CNode> first = build_stack.top();
        build_stack.pop();
        CNeq in(first,second);
        std::shared_ptr<CNode> to_be_pushed = std::make_shared<CNeq>(in);
        build_stack.push(to_be_pushed);
      }
    }
    virtual void  opLt() {
      //first i take out the SECOND argument of the function cuz thats how zasobnik works
      //! this part computes the result
      if(!cyclicCheckeronly)
      {
        std::shared_ptr<CNode> second = my_stack.top();
        my_stack.pop();
        std::shared_ptr<CNode> first = my_stack.top();
        my_stack.pop();
        CLt in(first,second);
        CValue tmp = in.evaluate();
        storeCValue(tmp);
      }

      //! this part builds the three
      {
        std::shared_ptr<CNode> second = build_stack.top();
        build_stack.pop();
        std::shared_ptr<CNode> first = build_stack.top();
        build_stack.pop();
        CLt in(first,second);
        std::shared_ptr<CNode> to_be_pushed = std::make_shared<CLt>(in);
        build_stack.push(to_be_pushed);
      }
    }
    virtual void  opLe() {
      //first i take out the SECOND argument of the function cuz thats how zasobnik works
      //! this part computes the result
      if(!cyclicCheckeronly)
      {
        std::shared_ptr<CNode> second = my_stack.top();
        my_stack.pop();
        std::shared_ptr<CNode> first = my_stack.top();
        my_stack.pop();
        CLe in(first,second);
        CValue tmp = in.evaluate();
        storeCValue(tmp);
      }

      //! this part builds the three
      {
        std::shared_ptr<CNode> second = build_stack.top();
        build_stack.pop();
        std::shared_ptr<CNode> first = build_stack.top();
        build_stack.pop();
        CLe in(first,second);
        std::shared_ptr<CNode> to_be_pushed = std::make_shared<CLe>(in);
        build_stack.push(to_be_pushed);
      }
    }
    virtual void  opGt() {
      //first i take out the SECOND argument of the function cuz thats how zasobnik works
      //! this part computes the result
      if(!cyclicCheckeronly)
      {
        std::shared_ptr<CNode> second = my_stack.top();
        my_stack.pop();
        std::shared_ptr<CNode> first = my_stack.top();
        my_stack.pop();
        CGt in(first,second);
        CValue tmp = in.evaluate();
        storeCValue(tmp);
      }

      //! this part builds the three
      {
        std::shared_ptr<CNode> second = build_stack.top();
        build_stack.pop();
        std::shared_ptr<CNode> first = build_stack.top();
        build_stack.pop();
        CGt in(first,second);
        std::shared_ptr<CNode> to_be_pushed = std::make_shared<CGt>(in);
        build_stack.push(to_be_pushed);
      }
    }
    virtual void  opGe() {
      //first i take out the SECOND argument of the function cuz thats how zasobnik works
      //! this part computes the result
      if(!cyclicCheckeronly)
      {
        std::shared_ptr<CNode> second = my_stack.top();
        my_stack.pop();
        std::shared_ptr<CNode> first = my_stack.top();
        my_stack.pop();
        CGe in(first,second);
        CValue tmp = in.evaluate();
        storeCValue(tmp);
      }

      //! this part builds the three
      {
        std::shared_ptr<CNode> second = build_stack.top();
        build_stack.pop();
        std::shared_ptr<CNode> first = build_stack.top();
        build_stack.pop();
        CGe in(first,second);
        std::shared_ptr<CNode> to_be_pushed = std::make_shared<CGe>(in);
        build_stack.push(to_be_pushed);
      }
    }
    
    virtual void  valNumber(double val) {
      CValNode tmp(val);
      std::shared_ptr<CNode> in = std::make_shared<CValNode>(tmp);
      my_stack.push(in);
      build_stack.push(in);
    }
    
    virtual void  valString(std::string val) {
      CValNode tmp(val);
      std::shared_ptr<CNode> in = std::make_shared<CValNode>(tmp);
      my_stack.push(in);
      build_stack.push(in);
    }
    
    virtual void  valReference(std::string val) {
        CReference tmp(val, table_ptr);
        std::shared_ptr<CNode> in = std::make_shared<CReference>(tmp);
        my_stack.push(in);
        build_stack.push(in);
    }

    virtual void  valRange(std::string val) {}
    virtual void  funcCall(std::string fnName, int paramCount) {}

    //here we can neatly just evaluate the root of the tree which is basically the last operation so in order to make the result it has to evaluate all
    // the before operations therefore creating the correct result
    CValue getResult(){
      CValue tmp = my_stack.top()->evaluate();
      tree.root = build_stack.top();
      return tmp;
    }

    CAst getTree(){
        tree.root = build_stack.top();
        return tree;
    }

    void setTablePtr(std::shared_ptr<std::map<CPos, CCell>> in){
        table_ptr = in;
    }

  private:
    
    CAst tree;
    std::shared_ptr<std::map<CPos, CCell>> table_ptr;
    std::stack<std::shared_ptr<CNode>> my_stack;
    std::stack<std::shared_ptr<CNode>> build_stack;
    bool cyclicCheckeronly = false;

    void storeCValue(const CValue & in){
      CValNode tmp(in);
      my_stack.push(std::make_shared<CValNode>(tmp));
    }
};

//helper operators for testing the CPos class
bool operator == (const CPos & first, const CPos & second){
  return ((first.first_cord == second.first_cord) && (first.second_cord == second.second_cord));
}

bool operator != (const CPos & first, const CPos & second){
  return !(first == second);
}

bool operator < (const CPos & first, const CPos & second){
  if(first.second_cord != second.second_cord){
    return first.second_cord < second.second_cord;
  }
  return first.first_cord < second.first_cord;
}

//------------------------------------------CCELL IMPL---------------------------------------------------------


CValue CCell::getValue(){
  //no need to parse the whole expresion when we already have the AST for it
  if(already_parsed){
    return eval_tree.root->evaluate();
  }
  //else we create the tree and set it up for next evaluation
  CMyBuilder my_builder;
  my_builder.setTablePtr(table_ptr);
  parseExpression(expresion,my_builder);
  already_parsed = true;
  eval_tree = my_builder.getTree();
  return my_builder.getResult();
}

bool CCell::setValue(const std::string & input){
  expresion = input;
  already_parsed = false;
  return true;
}

CCell::CCell(std::shared_ptr<std::map<CPos, CCell>> in){
  table_ptr = in;
}

CCell& CCell::operator=(const CCell& other)
    {
        if (this != &other) // Check for self-assignment
        {
          this->position = other.position;
          this->expresion = other.expresion;
          this->eval_tree = CAst{};
          //these are set to null so we can correctly create a deep copy and it wont be refering into the other table
          this->already_parsed = false;
          this->table_ptr = nullptr;
        }
        return *this;
    }

void CCell::getReferences(std::set<std::string> & references){
  //we cant get references if we dont have the AST so in that case we must create the AST
  if(!already_parsed){
    getValue();
  }
  eval_tree.root->getReferences(references);
} 

void CCell::checkcyclic(std::set<CPos> in_set, bool & flag_valid){
  //when the AST is not present we need to create it in order to traverse it
  if(!already_parsed){
    CMyBuilder my_builder(true);
    my_builder.setTablePtr(table_ptr);
    parseExpression(expresion,my_builder);
    already_parsed = true;
    eval_tree = my_builder.getTree();
  }
  if(in_set.find(position) != in_set.end()){
    flag_valid = false;
    return;
  }
  in_set.insert(position);
  eval_tree.root->checkcyclic(in_set,flag_valid);
}

//this function will shift in_pos by x and y and return correct new position for example A0 x = 1, y = 5 --> will return B5 
std::string getMovedPosition(std::string in_pos, int x_move, int y_move);

//this function iterates over all references and shifts them by x and y using the function above
void CCell::moveReferences(int x_off, int y_off){
  std::set<std::string> refs;
  getReferences(refs);
  int im_at = 0;
  while(im_at <= int(expresion.size()-1)){
    for(auto str : refs){
      if(expresion.substr(im_at, str.size()) == str){
        std::string new_ref = getMovedPosition(str, x_off, y_off);
        expresion.erase(im_at, im_at + str.size()-1);
        expresion.insert(im_at, new_ref);
        im_at = im_at + new_ref.size();
      }
    }
    im_at++;
  }
  already_parsed = false;
}

//this function returns the difference in coordinates of dst and src
std::pair<int, int> countOffset(CPos dst, CPos src);

//------------------------------------------SPREADSHEET---------------------------------------------------------


class CSpreadsheet
{
  public:
    static unsigned capabilities()
    {
      return SPREADSHEET_CYCLIC_DEPS;
      //return SPREADSHEET_CYCLIC_DEPS | SPREADSHEET_FUNCTIONS | SPREADSHEET_FILE_IO | SPREADSHEET_SPEED | SPREADSHEET_PARSER;
    }

    CSpreadsheet(){
      table = std::make_shared<std::map<CPos, CCell>>();
    }
    ~CSpreadsheet(){
      //need to manualy clear the table otherwise it will create a cyclic dependency with shared_ptrs --> therefore never dealocating the memory
      table->clear();
    }

    CSpreadsheet& operator=(const CSpreadsheet& other)
    {
        if (this != &other) // Check for self-assignment
        {
            //creating a deep copy since we are working with shared_ptr
            table = std::make_shared<std::map<CPos, CCell>>();
            for(auto itr = other.table->begin(); itr != other.table->end(); itr++){
              CCell tmp(nullptr);
              tmp = itr->second;
              tmp.table_ptr = table;
              table->insert({tmp.position,tmp});
            }
        }
        return *this;
    }

    //function loads the isteam parsing it by set delimiters and then validating the values that shoudl be set in each cell
    bool load(std::istream & is){
      std::ostringstream oss;
      oss << is.rdbuf();
      std::string content = oss.str();

      table->clear();

      if(content == ""){
        *this = CSpreadsheet{};
        return true;
      }

      std::vector<std::string> parts;
      std::string delimiter = ";!end!;";
      size_t start = 0;
      size_t end = content.find(delimiter);
      while (end != std::string::npos) {
          parts.push_back(content.substr(start, end - start));
          start = end + delimiter.length();
          end = content.find(delimiter, start);
      }

      for(std::string & part : parts){
        size_t pos = part.find(";!pos!;");
        if (pos == std::string::npos){
          return false;
        }
        std::string part_before = part.substr(0, pos);
        std::string part_after = part.substr(pos + 7);
        //using try catch block - if the content is not valid the setCell will throw error then resulting in return false since the input was not valid
        try{
          CPos position(part_before);
          this->setCell(position, part_after);
        }catch(std::invalid_argument & e){
          return false;
        }
      }
      return true;
    }

    //funtion saves the whole content of sheet into osteam
    bool save(std::ostream & os) const{
      for(auto itr = table->begin(); itr != table->end(); itr++){
        os << itr->second.position.first_cord << itr->second.position.second_cord;
        //using multichar delimiters for simplicity -- i would forbid users to use these two strings inside a value of cell
        os << ";!pos!;";
        os << itr->second.expresion;
        os << ";!end!;";
        if(!os){
          return false;
        }
      }
      return true;
    }

    bool setCell(CPos pos, std::string contents){
      //validate input if its an expresion (all other things are strings -- therefore valid)
      if(contents[0] == '='){
        CMyBuilder my_builder(true);
        my_builder.setTablePtr(table);
        try{
          parseExpression(contents, my_builder);
        }catch(std::invalid_argument & e){
          //this means the things inside contents wasnt parasble therefore wrong input
          std::cout << "toto nebyl teda hezký vstup" << std::endl;
          return false;
        }
      }
      // if this cell already exists then only set new value
      if(table->find(pos) != table->end()){
        return table->at(pos).setValue(contents);
      }
      //otherwise create a new cell
      else{
          bool res;
          CCell tmp(table);
          res = tmp.setValue(contents);
          tmp.position = pos;
          if(!res){
              return false;
          }
          table->insert({pos, tmp});
          return true;
      }
    }
    
    CValue getValue(CPos pos){
      //needs to check if this cell was inicialized and check for cycle dependencies
      if(table->find(pos) != table->end()){
        std::set<CPos> in_set;
        bool valid = true;
        table->at(pos).checkcyclic(in_set, valid);
        if(valid){
          return table->at(pos).getValue();
        }
      }
      //if not valid then return CValue()
      CValue tmp;
      return tmp;
    }

    void copyRect(CPos dst, CPos src, int w = 1, int h = 1){
      std::map<CPos,CCell> copied;
      std::pair<int,int> offset = countOffset(dst,src);
      CPos src_original = src;
      //iterating over the source rectangle shifting all references and storing it inside a temporary container
      //needs to be copied cuz the rectangles can be overlaping
      for(int i = 0; i < w; i++){
        for(int j = 0; j < h; j++){
          CPos new_pos = dst;
          new_pos.changeFirstCord(i);
          new_pos.changeSecondCord(j);
          if(table->find(src) != table->end()){
            CCell new_cell = table->at(src);
            new_cell.position = new_pos;
            new_cell.table_ptr = table;
            new_cell.moveReferences(offset.first, offset.second);
            copied.insert({new_pos,new_cell});
          }
          src.changeSecondCord(1);
        }
        src.second_cord = src_original.second_cord;
        src.changeFirstCord(1);
      }
      //copying the src rectange into its destination
      CPos dst_original = dst;
      for(int i = 0; i < w; i++){
        for(int j = 0; j < h; j++){
          if(copied.find(dst) != copied.end()){
            CCell insert_cell = copied.at(dst);
            table->insert_or_assign(dst, insert_cell);
            //cuz copy constructor doesnt copy the pointer
            table->at(dst).table_ptr = table;
          }
          else{
            table->erase(dst);
          }
          dst.changeSecondCord(1);
        }
        dst.second_cord = dst_original.second_cord;
        dst.changeFirstCord(1);
      }
    }
  
    std::shared_ptr<std::map<CPos, CCell>> getTable(){
      return table;
    }

  private:
    std::shared_ptr<std::map<CPos, CCell>> table;
};

//------------------------------------------HELPER FUNCTIONS---------------------------------------------------------

std::pair<int, int> countOffset(CPos dst, CPos src){
  int colofset = 0;
  int rowofset = 0;

  rowofset = dst.second_cord - src.second_cord;
  //function simply adds 1 to the smaller coordiante -- its not efficient but efficiency wasnt the main concern here
  if(dst.first_cord.size() == src.first_cord.size()){
    if(dst.first_cord > src.first_cord){
      while(src.first_cord != dst.first_cord){
        src.changeFirstCord(1);
        colofset++;
      }
    }
    else{
      while(src.first_cord != dst.first_cord){
        dst.changeFirstCord(1);
        colofset--;
      }
    }
  }
  if(dst.first_cord.size() > src.first_cord.size()){
    while(src.first_cord != dst.first_cord){
      src.changeFirstCord(1);
      colofset++;
    }
  }
  else{
    while(src.first_cord != dst.first_cord){
        dst.changeFirstCord(1);
        colofset--;
      }
  }
  return std::pair<int,int>(colofset,rowofset);
}

int countOccurrences(const std::string& str, char target) {
    int count = 0;
    for (char ch : str) {
        if (ch == target) {
            count++;
        }
    }
    return count;
}

std::string getMovedPosition(std::string in_pos, int x_move, int y_move){
  
  int dolars = countOccurrences(in_pos, '$');
  bool first_dolar = false;
  bool second_dolar = false;
  if(in_pos[0] == '$'){
    first_dolar = true;
  }
  if(dolars == 2){
    second_dolar = true;
  }
  if(dolars == 1 && (!first_dolar)){
    second_dolar = true;
  }

  in_pos.erase(std::remove_if(in_pos.begin(), in_pos.end(), [](char c) {
        return c == '$';
    }), in_pos.end());

  CPos tmp_pos(in_pos);
  if(!first_dolar){
    tmp_pos.changeFirstCord(x_move);
  }
  if(!second_dolar){
    tmp_pos.changeSecondCord(y_move);
  }
  std::string res;
  if(first_dolar){
    res += '$';
  }
  res += tmp_pos.first_cord;
  if(second_dolar){
    res += '$';
  }
  res += std::to_string(tmp_pos.second_cord);

  return res;
}


#ifndef __PROGTEST__

bool                                   valueMatch                              ( const CValue                        & r,
                                                                                 const CValue                        & s )

{
  if ( r . index () != s . index () )
    return false;
  if ( r . index () == 0 )
    return true;
  if ( r . index () == 2 )
    return std::get<std::string> ( r ) == std::get<std::string> ( s );
  if ( std::isnan ( std::get<double> ( r ) ) && std::isnan ( std::get<double> ( s ) ) )
    return true;
  if ( std::isinf ( std::get<double> ( r ) ) && std::isinf ( std::get<double> ( s ) ) )
    return ( std::get<double> ( r ) < 0 && std::get<double> ( s ) < 0 )
           || ( std::get<double> ( r ) > 0 && std::get<double> ( s ) > 0 );
  return fabs ( std::get<double> ( r ) - std::get<double> ( s ) ) <= 1e8 * DBL_EPSILON * fabs ( std::get<double> ( r ) );
}

//------------------------------------------ASSERTS---------------------------------------------------------


int main ()
{
  try{
    CPos tmp("aa");
    throw std::logic_error("problem in asserts");
  }catch(std::invalid_argument & e){std::cout << "Caught exception" << std::endl;}
  try{
    CPos tmp("");
    throw std::logic_error("problem in asserts");
  }catch(std::invalid_argument & e){std::cout << "Caught exception" << std::endl;}
  try{
    CPos tmp("aa-5");
    throw std::logic_error("problem in asserts");
  }catch(std::invalid_argument & e){std::cout << "Caught exception" << std::endl;}
  try{
    CPos tmp("aa5B");
    throw std::logic_error("problem in asserts");
  }catch(std::invalid_argument & e){std::cout << "Caught exception" << std::endl;}
  try{
    CPos tmp("55A");
    throw std::logic_error("problem in asserts");
  }catch(std::invalid_argument & e){std::cout << "Caught exception" << std::endl;}
  {
    CPos tmp("aA0");
    assert(tmp.first_cord == "AA" && tmp.second_cord == 0);
  }
  {
    CPos tmp("aXA0");
    assert(tmp.first_cord == "AXA" && tmp.second_cord == 0);
  }
  {
    CPos tmp("aA01");
    assert(tmp.first_cord == "AA" && tmp.second_cord == 1);
  }
  {
    CPos tmp("aA158");
    assert(tmp.first_cord == "AA" && tmp.second_cord == 158);
  }
  {
    CPos tmp("aA158");
    assert(tmp.first_cord == "AA" && tmp.second_cord == 158);
    tmp.changeFirstCord(1);
    assert(tmp.first_cord == "AB" && tmp.second_cord == 158);
    tmp.changeSecondCord(10);
    assert(tmp.first_cord == "AB" && tmp.second_cord == 168);
  }
  {
    CPos tmp("Y1");
    assert(tmp.first_cord == "Y" && tmp.second_cord == 1);
    tmp.changeFirstCord(1);
    assert(tmp.first_cord == "Z" && tmp.second_cord == 1);
    tmp.changeFirstCord(1);
    assert(tmp.first_cord == "AA" && tmp.second_cord == 1);
    tmp.changeFirstCord(25);
    assert(tmp.first_cord == "AZ" && tmp.second_cord == 1);
    tmp.changeFirstCord(1);
    assert(tmp.first_cord == "BA" && tmp.second_cord == 1);
  }
  {
    CPos tmp("ZZZ1");
    tmp.changeFirstCord(2);
    assert(tmp.first_cord == "AAAB" && tmp.second_cord == 1);
  }
  {
    CPos tmp("AA1");
    tmp.changeFirstCord(-1);
    tmp.changeSecondCord(-1);
    assert(tmp.first_cord == "Z" && tmp.second_cord == 0);
  }
  {
    CPos tmp("B1");
    tmp.changeFirstCord(-1);
    tmp.changeSecondCord(-1);
    assert(tmp.first_cord == "A" && tmp.second_cord == 0);
  }
  {
    CPos src("B1");
    CPos dst("B2");
    std::pair<int, int> expected = std::make_pair(0, 1);
    std::pair<int, int> result = countOffset(dst, src);
    assert(expected.first == result.first && expected.second == result.second);
  }
  {
    CPos src("B1");
    CPos dst("B20");
    std::pair<int, int> expected = std::make_pair(0, 19);
    std::pair<int, int> result = countOffset(dst, src);
    assert(expected.first == result.first && expected.second == result.second);
  }
  {
    CPos src("B10");
    CPos dst("B2");
    std::pair<int, int> expected = std::make_pair(0, -8);
    std::pair<int, int> result = countOffset(dst, src);
    assert(expected.first == result.first && expected.second == result.second);
  }
  {
    CPos src("A10");
    CPos dst("Z2");
    std::pair<int, int> expected = std::make_pair(25, -8);
    std::pair<int, int> result = countOffset(dst, src);
    assert(expected.first == result.first && expected.second == result.second);
  }
  {
    CPos src("A10");
    CPos dst("AA2");
    std::pair<int, int> expected = std::make_pair(26, -8);
    std::pair<int, int> result = countOffset(dst, src);
    assert(expected.first == result.first && expected.second == result.second);
  }
  {
    CPos src("AA10");
    CPos dst("A2");
    std::pair<int, int> expected = std::make_pair(-26, -8);
    std::pair<int, int> result = countOffset(dst, src);
    assert(expected.first == result.first && expected.second == result.second);
  }
  {
    CPos src("Z10");
    CPos dst("A2");
    std::pair<int, int> expected = std::make_pair(-25, -8);
    std::pair<int, int> result = countOffset(dst, src);
    assert(expected.first == result.first && expected.second == result.second);
  }
  {
    CPos tmp("AAAA1");
    tmp.changeFirstCord(-1);
    tmp.changeSecondCord(-1);
    assert(tmp.first_cord == "ZZZ" && tmp.second_cord == 0);
    tmp.changeFirstCord(-1);
    assert(tmp.first_cord == "ZZY" && tmp.second_cord == 0);
  }
  {
    std::string tmp = "$AA$11";
    assert(getMovedPosition(tmp, 25, 10) == "$AA$11");
    assert(getMovedPosition(tmp, 26, 10) == "$AA$11");
  }
  {
    std::string tmp = "AA$11";
    assert(getMovedPosition(tmp, 25, 10) == "AZ$11");
    assert(getMovedPosition(tmp, 26, 10) == "BA$11");
  }
  {
    std::string tmp = "$AA11";
    assert(getMovedPosition(tmp, 25, 10) == "$AA21");
    assert(getMovedPosition(tmp, 26, 10) == "$AA21");
  }
  {
    std::string tmp = "AA11";
    assert(getMovedPosition(tmp, 25, 10) == "AZ21");
    assert(getMovedPosition(tmp, 26, 10) == "BA21");
  }
  {
    CSpreadsheet sheet;
    CCell tmp(sheet.getTable());
    tmp.expresion = "=A1 + B1";
    tmp.moveReferences(1,1);
    assert(tmp.expresion == "=B2 + C2");
  }
  {
    CSpreadsheet sheet;
    CCell tmp(sheet.getTable());
    tmp.expresion = "=$A1 + $B$1";
    tmp.moveReferences(1,1);
    assert(tmp.expresion == "=$A2 + $B$1");
    tmp.moveReferences(-1,-1);
    assert(tmp.expresion == "=$A1 + $B$1");
    tmp.expresion = "=A$1 + B$1";
    tmp.moveReferences(26,26);
    assert(tmp.expresion == "=AA$1 + AB$1");
    tmp.moveReferences(-26,-26);
    assert(tmp.expresion == "=A$1 + B$1");
    tmp.moveReferences(25,25);
    assert(tmp.expresion == "=Z$1 + AA$1");

  }

  CSpreadsheet tmp;
  CPos pos("A0");
  tmp.setCell(pos, "=\"COE\"+ 10");

  std::cout << tmp.getValue(pos) << std::endl;
  //std::cout << tmp.getValue(pos) << std::endl;

  CSpreadsheet x0, x1;
  std::ostringstream oss;
  std::istringstream iss;
  std::string data;
  assert ( x0 . setCell ( CPos ( "A1" ), "10" ) );
  assert ( x0 . setCell ( CPos ( "A2" ), "20.5" ) );
  assert ( x0 . setCell ( CPos ( "A3" ), "3e1" ) );
  assert ( x0 . setCell ( CPos ( "A4" ), "=40" ) );
  assert ( x0 . setCell ( CPos ( "A5" ), "=5e+1" ) );
  assert ( x0 . setCell ( CPos ( "A6" ), "raw text with any characters, including a quote \" or a newline\n" ) );
  assert ( x0 . setCell ( CPos ( "A7" ), "=\"quoted string, quotes must be doubled: \"\". Moreover, backslashes are needed for C++.\"" ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "A1" ) ), CValue ( 10.0 ) ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "A2" ) ), CValue ( 20.5 ) ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "A3" ) ), CValue ( 30.0 ) ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "A4" ) ), CValue ( 40.0 ) ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "A5" ) ), CValue ( 50.0 ) ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "A6" ) ), CValue ( "raw text with any characters, including a quote \" or a newline\n" ) ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "A7" ) ), CValue ( "quoted string, quotes must be doubled: \". Moreover, backslashes are needed for C++." ) ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "A8" ) ), CValue() ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "AAAA9999" ) ), CValue() ) );
  assert ( x0 . setCell ( CPos ( "A8" ), "=$A$1 + 10" ) );
  std::cout << x0 . getValue ( CPos ( "A8" ) ) << std::endl;
  assert ( valueMatch ( x0 . getValue ( CPos ( "A8" ) ), CValue ( 20.0 ) ) );
  assert ( x0 . setCell ( CPos ( "B1" ), "=A1+A2*A3" ) );
  assert ( x0 . setCell ( CPos ( "B2" ), "= -A1 ^ 2 - A2 / 2   " ) );
  assert ( x0 . setCell ( CPos ( "B3" ), "= 2 ^ $A$1" ) );
  assert ( x0 . setCell ( CPos ( "B4" ), "=($A1+A$2)^2" ) );
  assert ( x0 . setCell ( CPos ( "B5" ), "=B1+B2+B3+B4" ) );
  assert ( x0 . setCell ( CPos ( "B6" ), "=B1+B2+B3+B4+B5" ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "B1" ) ), CValue ( 625.0 ) ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "B2" ) ), CValue ( -110.25 ) ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "B3" ) ), CValue ( 1024.0 ) ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "B4" ) ), CValue ( 930.25 ) ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "B5" ) ), CValue ( 2469.0 ) ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "B6" ) ), CValue ( 4938.0 ) ) );
  assert ( x0 . setCell ( CPos ( "A1" ), "12" ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "B1" ) ), CValue ( 627.0 ) ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "B2" ) ), CValue ( -154.25 ) ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "B3" ) ), CValue ( 4096.0 ) ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "B4" ) ), CValue ( 1056.25 ) ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "B5" ) ), CValue ( 5625.0 ) ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "B6" ) ), CValue ( 11250.0 ) ) );
  x1 = x0;
  assert ( valueMatch ( x0 . getValue ( CPos ( "B1" ) ), CValue ( 627.0 ) ) );
  assert ( valueMatch ( x1 . getValue ( CPos ( "B1" ) ), CValue ( 627.0 ) ) );
  assert ( x0 . setCell ( CPos ( "A2" ), "100" ) );
  assert ( x1 . setCell ( CPos ( "A2" ), "=A3+A5+A4" ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "B1" ) ), CValue ( 3012.0 ) ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "B2" ) ), CValue ( -194.0 ) ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "B3" ) ), CValue ( 4096.0 ) ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "B4" ) ), CValue ( 12544.0 ) ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "B5" ) ), CValue ( 19458.0 ) ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "B6" ) ), CValue ( 38916.0 ) ) );
  assert ( valueMatch ( x1 . getValue ( CPos ( "B1" ) ), CValue ( 3612.0 ) ) );
  assert ( valueMatch ( x1 . getValue ( CPos ( "B2" ) ), CValue ( -204.0 ) ) );
  assert ( valueMatch ( x1 . getValue ( CPos ( "B3" ) ), CValue ( 4096.0 ) ) );
  assert ( valueMatch ( x1 . getValue ( CPos ( "B4" ) ), CValue ( 17424.0 ) ) );
  assert ( valueMatch ( x1 . getValue ( CPos ( "B5" ) ), CValue ( 24928.0 ) ) );
  assert ( valueMatch ( x1 . getValue ( CPos ( "B6" ) ), CValue ( 49856.0 ) ) );
  oss . clear ();
  oss . str ( "" );
  assert ( x0 . save ( oss ) );
  data = oss . str ();
  iss . clear ();
  iss . str ( data );
  assert ( x1 . load ( iss ) );
  assert ( valueMatch ( x1 . getValue ( CPos ( "B1" ) ), CValue ( 3012.0 ) ) );
  assert ( valueMatch ( x1 . getValue ( CPos ( "B2" ) ), CValue ( -194.0 ) ) );
  assert ( valueMatch ( x1 . getValue ( CPos ( "B3" ) ), CValue ( 4096.0 ) ) );
  assert ( valueMatch ( x1 . getValue ( CPos ( "B4" ) ), CValue ( 12544.0 ) ) );
  assert ( valueMatch ( x1 . getValue ( CPos ( "B5" ) ), CValue ( 19458.0 ) ) );
  assert ( valueMatch ( x1 . getValue ( CPos ( "B6" ) ), CValue ( 38916.0 ) ) );
  assert ( x0 . setCell ( CPos ( "A3" ), "4e1" ) );
  assert ( valueMatch ( x1 . getValue ( CPos ( "B1" ) ), CValue ( 3012.0 ) ) );
  assert ( valueMatch ( x1 . getValue ( CPos ( "B2" ) ), CValue ( -194.0 ) ) );
  assert ( valueMatch ( x1 . getValue ( CPos ( "B3" ) ), CValue ( 4096.0 ) ) );
  assert ( valueMatch ( x1 . getValue ( CPos ( "B4" ) ), CValue ( 12544.0 ) ) );
  assert ( valueMatch ( x1 . getValue ( CPos ( "B5" ) ), CValue ( 19458.0 ) ) );
  assert ( valueMatch ( x1 . getValue ( CPos ( "B6" ) ), CValue ( 38916.0 ) ) );
  oss . clear ();
  oss . str ( "" );
  assert ( x0 . save ( oss ) );
  data = oss . str ();
  for ( size_t i = 0; i < std::min<size_t> ( data . length (), 10 ); i ++ )
    data[i] ^=0x5a;
  iss . clear ();
  iss . str ( data );
  assert ( ! x1 . load ( iss ) );
  assert ( x0 . setCell ( CPos ( "D0" ), "10" ) );
  assert ( x0 . setCell ( CPos ( "D1" ), "20" ) );
  assert ( x0 . setCell ( CPos ( "D2" ), "30" ) );
  assert ( x0 . setCell ( CPos ( "D3" ), "40" ) );
  assert ( x0 . setCell ( CPos ( "D4" ), "50" ) );
  assert ( x0 . setCell ( CPos ( "E0" ), "60" ) );
  assert ( x0 . setCell ( CPos ( "E1" ), "70" ) );
  assert ( x0 . setCell ( CPos ( "E2" ), "80" ) );
  assert ( x0 . setCell ( CPos ( "E3" ), "90" ) );
  assert ( x0 . setCell ( CPos ( "E4" ), "100" ) );
  assert ( x0 . setCell ( CPos ( "F10" ), "=D0+5" ) );
  assert ( x0 . setCell ( CPos ( "F11" ), "=$D0+5" ) );
  assert ( x0 . setCell ( CPos ( "F12" ), "=D$0+5" ) );
  assert ( x0 . setCell ( CPos ( "F13" ), "=$D$0+5" ) );
  x0 . copyRect ( CPos ( "G11" ), CPos ( "F10" ), 1, 4 );
  assert ( valueMatch ( x0 . getValue ( CPos ( "F10" ) ), CValue ( 15.0 ) ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "F11" ) ), CValue ( 15.0 ) ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "F12" ) ), CValue ( 15.0 ) ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "F13" ) ), CValue ( 15.0 ) ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "F14" ) ), CValue() ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "G10" ) ), CValue() ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "G11" ) ), CValue ( 75.0 ) ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "G12" ) ), CValue ( 25.0 ) ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "G13" ) ), CValue ( 65.0 ) ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "G14" ) ), CValue ( 15.0 ) ) );
  x0 . copyRect ( CPos ( "G11" ), CPos ( "F10" ), 2, 4 );
  assert ( valueMatch ( x0 . getValue ( CPos ( "F10" ) ), CValue ( 15.0 ) ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "F11" ) ), CValue ( 15.0 ) ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "F12" ) ), CValue ( 15.0 ) ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "F13" ) ), CValue ( 15.0 ) ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "F14" ) ), CValue() ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "G10" ) ), CValue() ) );
  oss . clear ();
  oss . str ( "" );
  assert ( x0 . save ( oss ) );
  data = oss . str ();
  assert ( valueMatch ( x0 . getValue ( CPos ( "G11" ) ), CValue ( 75.0 ) ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "G12" ) ), CValue ( 25.0 ) ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "G13" ) ), CValue ( 65.0 ) ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "G14" ) ), CValue ( 15.0 ) ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "H10" ) ), CValue() ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "H11" ) ), CValue() ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "H12" ) ), CValue() ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "H13" ) ), CValue ( 35.0 ) ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "H14" ) ), CValue() ) );
  assert ( x0 . setCell ( CPos ( "F0" ), "-27" ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "H14" ) ), CValue ( -22.0 ) ) );
  x0 . copyRect ( CPos ( "H12" ), CPos ( "H13" ), 1, 2 );
  assert ( valueMatch ( x0 . getValue ( CPos ( "H12" ) ), CValue ( 25.0 ) ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "H13" ) ), CValue ( -22.0 ) ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "H14" ) ), CValue ( -22.0 ) ) );

  CSpreadsheet x2;
  assert ( x2 . setCell ( CPos ( "A1" ), "10" ) );
  assert ( x2 . setCell ( CPos ( "A2" ), "20.5" ) );
  assert ( x2 . setCell ( CPos ( "A3" ), "=A4 + A6 + 20 * 60 - 10" ) );
  assert ( x2 . setCell ( CPos ( "A4" ), "=A6 + A6 / 5" ) );
  assert ( x2 . setCell ( CPos ( "A6" ), "=A3 + A3" ) );


  assert ( valueMatch ( x2 . getValue ( CPos ( "A1" ) ), CValue ( 10.0 ) ) );
  assert ( valueMatch ( x2 . getValue ( CPos ( "A3" ) ), CValue ( ) ) );


  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */