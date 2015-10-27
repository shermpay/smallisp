#include "reader.h"

#include <cctype>

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

/*
  Smallisp Grammar
  ----------------
  Prog = Expr*
  Expr = Sexp | Symbol | Int
  Sexp = '(' Symbol Expr* ')'
  Symbol = '[a-zA-Z\-]+'
  Int = '[0-9]+'
*/

namespace sl {

const std::string reader::Error::Str(void) const {
  auto line = std::to_string(linum());
  auto col = std::to_string(colnum());
  std::string str;
  str.append(line);
  str.push_back(':');
  str.append(col);
  str.append(" - ");
  str.append(msg_);
  return str;
}

enum Delim : char { kLParen = '(', kRParen = ')' };

const long Reader::kStartLinum = 1;
const int Reader::kStartColnum = 1;

void *Reader::Failed(const reader::Error &err) {
  this->error_ = &err;
  fprintf(stderr, "ReaderError: %s\n", Str().c_str());
  return nullptr;
}

char Reader::PeekChar(void) { return stream().peek(); }

char Reader::GetChar(void) {
  char c = stream().get();
  curr_line_push_back(c);
  switch (c) {
    case '\n':
      inc_linum();
      reset_colnum();
      clear_curr_line();
      break;
    case ' ':
    case '\t':
    default:
      inc_colnum();
      break;
  }
  return c;
}

const std::string Reader::Str(void) const {
  std::stringstream sstream;
  sstream << "Line: " << linum() << ", Col: " << colnum() << std::endl;
  sstream << "Error: " << (error() ? error()->Str() : "None") << std::endl;
  sstream << '\t' << curr_line() << std::endl;
  sstream << '\t';
  for (int i = 0; i < colnum() - 1; ++i) {
    sstream << ' ';
  }
  sstream << '^' << std::endl;
  return sstream.str();
}

void Reader::ReadWhitespace(void) {
  while (isspace(PeekChar())) {
    GetChar();
  }
}

// TODO: Optimize this to one pass. This requires two passes.
const Int *Reader::ReadInt(void) {
  std::string int_str;
  char c = PeekChar();
  if (isdigit(c) || c == '-') {
    GetChar();
    int_str.push_back(c);
  } else {
    Failed(reader::Error(linum(), colnum(), ""));
    return nullptr;
  }
  while (isdigit(c = PeekChar())) {
    GetChar();
    int_str.push_back(c);
  }
  size_t ptr;
  int base = 10;
  long value = std::stol(int_str, &ptr, base);
  if (ptr < int_str.size()) {
    Failed(reader::Error(linum(), colnum(), ""));
    return nullptr;
  } else {
    return Int::Get(value);
  }
}

inline static bool IsSymbolChar(char c) { return isalnum(c) || c == '-'; }

const Symbol *Reader::ReadSymbol(void) {
  std::string symbol_name;
  char c = PeekChar();
  if (isalpha(c) || c == '-') {
    GetChar();
    symbol_name.push_back(c);
    while (IsSymbolChar(c = PeekChar())) {
      if (isalnum(c) || c == '-') {
        GetChar();
        symbol_name.push_back(c);
      } else {
        Failed(reader::Error(
            linum(), colnum(),
            "Symbol must contain only alphanumeric characters or '-'"));
        break;
      }
    }
    return Symbol::Get(symbol_name);
  } else {
    Failed(reader::Error(linum(), colnum(),
                         "Symbol must start with alphabet character or '-'"));
    return nullptr;
  }
}

const Object *Reader::ReadSexp(void) {
  ReadWhitespace();
  const Symbol *symbol = ReadSymbol();
  ReadWhitespace();
  return new List(new ConsC(symbol, kNil));
}
const Object *Reader::ReadExpr(void) {
  char c = PeekChar();
  switch (c) {
    case Delim::kLParen:
      ReadSexp();
      break;
    case Delim::kRParen:
      std::string error_msg{""};
      Failed(reader::Error(linum(), colnum(), error_msg));
      break;
  }
}

const Object *Read(std::istream &input_stream) {
  Reader reader(input_stream);
  return reader.ReadExpr();
}

const Object *Read(void) { return Read(std::cin); }

}  // namespace sl
