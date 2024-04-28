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

    void incrementSecondCord(int i){
      second_cord = second_cord + i;
    }
    void incrementFirstCord(int i){
      // Convert first_cord to uppercase for consistency
      std::string uppercaseFirstCord = first_cord;

      // Iterate through each character of the string in reverse order
      for(;i > 0;i--){
        for (int j = uppercaseFirstCord.size() - 1; j >= 0; --j) {
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
        }
      }
      first_cord = uppercaseFirstCord;
    }
  

    std::string first_cord;
    int second_cord;
};

//abstract class from which all operations will derive
class CNode{
  public:
    virtual CValue evaluate() = 0;
    virtual ~CNode(){}
};

class CAst{
  public:  
    std::shared_ptr<CNode> root;
};

class CCell{
  private:
    CAst eval_tree;
    bool already_parsed;
  public:
    std::string expresion;
    CPos position;
    std::shared_ptr<std::map<CPos, CCell>> table_ptr;
    CCell(std::shared_ptr<std::map<CPos, CCell>> in);
    CValue getValue();
    bool setValue(const std::string & input);
    CCell& operator=(const CCell& other);
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

  private:
    CValue value;
};

class CReference : public CNode{
  public:
  CReference(const std::string & in_pos, std::shared_ptr<std::map<CPos, CCell>> in_ptr){
    std::string str = in_pos;
    str.erase(std::remove_if(str.begin(), str.end(), [](char c) {
        return c == '$';
    }), str.end());
    
    position = CPos(str);
    table_ptr = in_ptr;
  }
  
  virtual CValue evaluate() override{
    if(table_ptr->find(position) != table_ptr->end()){
      return table_ptr->at(position).getValue();
    }
    CValue tmp;
    return tmp;
  }

  private:
    CPos position;
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

  private:
    std::shared_ptr<CNode> first_arg;
    std::shared_ptr<CNode> second_arg;
};

class CMyBuilder : public CExprBuilder{
  public:
    virtual void  opAdd(){
      //first i take out the SECOND argument of the function cuz thats how zasobnik works
      //! this part computes the result
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

    void storeCValue(const CValue & in){
      CValNode tmp(in);
      my_stack.push(std::make_shared<CValNode>(tmp));
    }
};

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

CValue CCell::getValue(){
  if(already_parsed){
    return eval_tree.root->evaluate();
  }
  CMyBuilder my_builder;
  my_builder.setTablePtr(table_ptr);
  parseExpression(expresion,my_builder);
  already_parsed = true;
  eval_tree = my_builder.getTree();
  return my_builder.getResult();
}

bool CCell::setValue(const std::string & input){
  //TODO osetrit ze je spravny vstup
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
          this->already_parsed = false;
          this->table_ptr = nullptr;
        }
        return *this;
    }

class CSpreadsheet
{
  public:
    static unsigned capabilities()
    {
      return SPREADSHEET_CYCLIC_DEPS | SPREADSHEET_FUNCTIONS | SPREADSHEET_FILE_IO | SPREADSHEET_SPEED | SPREADSHEET_PARSER;
    }

    CSpreadsheet(){
      table = std::make_shared<std::map<CPos, CCell>>();
    }
    ~CSpreadsheet(){
      table->clear();
    }

    CSpreadsheet& operator=(const CSpreadsheet& other)
    {
        if (this != &other) // Check for self-assignment
        {
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
        try{
          CPos position(part_before);
          this->setCell(position, part_after);
        }catch(std::invalid_argument & e){
          return false;
        }
      }
      return true;
    }

    bool save(std::ostream & os) const{
      for(auto itr = table->begin(); itr != table->end(); itr++){
        os << itr->second.position.first_cord << itr->second.position.second_cord;
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
      //TODO nejaka kontrola jestli contents je validni
      //TODO this could use some optimization -- i can use the parser and store valid AST into the cell so its doesnt need to be parsed again next time
      if(contents[0] == '='){
        CMyBuilder my_builder;
        //this fails when i dont set the pointer and i dont know why at all
        my_builder.setTablePtr(table);
        try{
          parseExpression(contents, my_builder);
        }catch(std::invalid_argument & e){
          //this means the things inside contents wasnt parasble therefore wrong input
          std::cout << "toto nebyl teda hezký vstup" << std::endl;
          return false;
        }
      }

      if(table->find(pos) != table->end()){
        return table->at(pos).setValue(contents);
      }
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
        if(table->find(pos) != table->end()){
            return table->at(pos).getValue();
        }
        CValue tmp;
        return tmp;
    }
    
    void copyRect(CPos dst, CPos src, int w = 1, int h = 1){
      std::vector<CCell> copied;
      CPos src_original = src;
      for(int i = w; i > 0; i--){
        for(int j = h; j > 0; j--){
          if(table->find(src) != table->end()){
            copied.push_back(table->at(src));
          }
          src.incrementSecondCord(1);
        }
        src.second_cord = src_original.second_cord;
        src.incrementFirstCord(1);
      }
    }
  
  private:
    std::shared_ptr<std::map<CPos, CCell>> table;
};

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
  tmp_pos.incrementFirstCord(x_move);
  tmp_pos.incrementSecondCord(y_move);
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
    tmp.incrementFirstCord(1);
    assert(tmp.first_cord == "AB" && tmp.second_cord == 158);
    tmp.incrementSecondCord(10);
    assert(tmp.first_cord == "AB" && tmp.second_cord == 168);
  }
  {
    CPos tmp("Y1");
    assert(tmp.first_cord == "Y" && tmp.second_cord == 1);
    tmp.incrementFirstCord(1);
    assert(tmp.first_cord == "Z" && tmp.second_cord == 1);
    tmp.incrementFirstCord(1);
    assert(tmp.first_cord == "AA" && tmp.second_cord == 1);
    tmp.incrementFirstCord(25);
    assert(tmp.first_cord == "AZ" && tmp.second_cord == 1);
    tmp.incrementFirstCord(1);
    assert(tmp.first_cord == "BA" && tmp.second_cord == 1);
  }
  {
    CPos tmp("ZZZ1");
    tmp.incrementFirstCord(2);
    assert(tmp.first_cord == "AAAB" && tmp.second_cord == 1);
  }
  {
    std::string tmp = "$AA$11";
    assert(getMovedPosition(tmp, 25, 10) == "$AZ$21");
    assert(getMovedPosition(tmp, 26, 10) == "$BA$21");
  }
  {
    std::string tmp = "AA$11";
    assert(getMovedPosition(tmp, 25, 10) == "AZ$21");
    assert(getMovedPosition(tmp, 26, 10) == "BA$21");
  }
  {
    std::string tmp = "$AA11";
    assert(getMovedPosition(tmp, 25, 10) == "$AZ21");
    assert(getMovedPosition(tmp, 26, 10) == "$BA21");
  }
  {
    std::string tmp = "AA11";
    assert(getMovedPosition(tmp, 25, 10) == "AZ21");
    assert(getMovedPosition(tmp, 26, 10) == "BA21");
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
  /*
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
  */
  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */



//TODO zkusit fake builder na kontrolu setcell ze by mohlo byt rychlejsi ale idk no