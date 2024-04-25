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
    return std::to_string(std::get<double> ( second )) + std::get<std::string> ( first );
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

class CPos;

class CNode;

class CAst{
  private:
    CNode * root;
    std::map<CPos, CCell> * table_ptr;
};

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
    std::string first_cord;
    int second_cord;
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

class CCell{
  private:
    CPos position;
    CValue value;
    CAst eval_tree;
    bool already_parsed;
    std::map<CPos, CCell> * table_ptr;

  public:
    CValue getValue() const{
      return value;
    }
    void setValue(const std::string & input);
    
};

//abstract class from which all operations will derive
class CNode{
  public:
    virtual CValue evaluate() = 0;
};

class CMonostate : public CNode{
  public:
  CMonostate(CValue in){
    value = in;
  }
  
  virtual CValue evaluate() override{
    return value;
  }
  private:
    CValue value;
};

class CNumber : public CNode{
  public:
  CNumber(double in){
    value = in;
  }
  
  virtual CValue evaluate() override{
    return CValue(value);
  }
  private:
    double value;
};

class CString : public CNode{
  public:
  CString(std::string in){
    value = in;
  }
  
  virtual CValue evaluate() override{
    return CValue(value);
  }
  private:
    std::string value;
};

class CReference : public CNode{
  public:
  CReference(std::string in_pos){
    position = CPos(in_pos);
  }
  
  virtual CValue evaluate() override{
    return table_ptr->at(position).getValue();
  }
  private:
    CPos position;
    std::map<CPos, CCell> * table_ptr;
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
      std::shared_ptr<CNode> first = my_stack.top();
      my_stack.pop();
      std::shared_ptr<CNode> second = my_stack.top();
      my_stack.pop();
      CValue tmp = CPlus(first,second).evaluate();
      storeCValue(tmp);
    }
    virtual void  opSub() = 0;
    virtual void  opMul() = 0;
    virtual void  opDiv() = 0;
    virtual void  opPow() = 0;
    virtual void  opNeg() = 0;
    virtual void  opEq() = 0;
    virtual void  opNe() = 0;
    virtual void  opLt() = 0;
    virtual void  opLe() = 0;
    virtual void  opGt() = 0;
    virtual void  opGe() = 0;
    virtual void  valNumber(double val) = 0;
    virtual void  valString(std::string val) = 0;
    virtual void  valReference(std::string val) = 0;
    virtual void  valRange(std::string val) = 0;
    virtual void  funcCall(std::string fnName, int paramCount) = 0;

  private:
    void storeCValue(CValue & in){
      if(in.index() == 0){
        CMonostate store(in);
        my_stack.push(std::make_shared<CMonostate>(store));
      }
      if(in.index() == 1){
        CNumber store(std::get<double>(in));
        my_stack.push(std::make_shared<CNumber>(store));
      }
      if(in.index() == 2){
        CString store(std::get<std::string>(in));
        my_stack.push(std::make_shared<CString>(store));
      }
    }
    
    CAst tree;
    std::map<CPos, CCell> * table_ptr;
    std::stack<std::shared_ptr<CNode>> my_stack;
};

class CSpreadsheet
{
  public:
    static unsigned capabilities()
    {
      return SPREADSHEET_CYCLIC_DEPS | SPREADSHEET_FUNCTIONS | SPREADSHEET_FILE_IO | SPREADSHEET_SPEED | SPREADSHEET_PARSER;
    }

    CSpreadsheet();
    bool load(std::istream & is);
    bool save(std::ostream & os) const;
    bool setCell(CPos pos, std::string contents);
    CValue getValue(CPos pos);
    void copyRect(CPos dst, CPos src, int w = 1, int h = 1);
  
  private:
    std::map<CPos, CCell> table;
};

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

  CString a("A");
  CNumber b(20);
  std::shared_ptr<CNode> p= std::make_shared<CString>(a);
  std::shared_ptr<CNode> q= std::make_shared<CNumber>(b);
  CPlus res(p,q);
  CValue i = res.evaluate();
  std::cout << i << std::endl;

  
  /*
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
