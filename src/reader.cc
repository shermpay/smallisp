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
  Symbol = '[a-zA-Z\+\-\*\/!]+'
  Int = '[0-9]+'
*/

namespace sl {

const std::string reader::Error::Str(void) const {
  std::stringstream sstream;
  sstream << linum();
  sstream.put(':');
  sstream << colnum();
  sstream << " - ";
  sstream << msg_;
  return sstream.str();
}

enum Delim : char { kLParen = '(', kRParen = ')' };

const long Reader::kStartLinum = 1;
const int Reader::kStartColnum = 1;

const Error *Reader::Failed(const reader::Error &err) {
  this->error_ = &err;
  return new Error(err.msg());
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

std::string Reader::Next(void) {
  std::string token;
  char c;
  ReadWhitespace();
  while (!isspace(c = PeekChar()) && c != Delim::kLParen &&
         c != Delim::kRParen && c != EOF) {
    GetChar();
    token.push_back(c);
  }
  return token;
}

const std::string Reader::Str(void) const {
  std::stringstream sstream;
  sstream << "Line: " << linum() << ", Col: " << colnum() << std::endl;
  if (error_ != nullptr) sstream << "Error: " << error()->Str() << std::endl;
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
const Object *Reader::ReadInt(void) {
  std::string int_str;
  char c = PeekChar();
  if (isdigit(c) || c == '-') {
    GetChar();
    int_str.push_back(c);
  } else {
    return Failed(
        reader::Error(linum(), colnum(), "Unable to read int " + int_str));
  }
  while (isdigit(c = PeekChar())) {
    GetChar();
    int_str.push_back(c);
  }
  return ReadInt(int_str);
}

// TODO: Optimize this to one pass. This requires two passes.
const Object *Reader::ReadInt(const std::string &int_str) {
  size_t ptr;
  int base = 10;
  long value = std::stol(int_str, &ptr, base);
  if (ptr < int_str.size()) {
    return Failed(
        reader::Error(linum(), colnum(), "Unable to read int " + int_str));
  } else {
    return Int::Get(value);
  }
}

// SymbolStart = '[a-zA-Z\+\-\*\/!]'
// Symbol = '([0-9]|SymbolStart)*'
inline static bool IsSymbolStartChar(char c) {
  return isalpha(c) || c == '+' || c == '-' || c == '*' || c == '/' || c == '!';
}

inline static bool IsSymbolChar(char c) {
  return IsSymbolStartChar(c) || isdigit(c);
}

const Object *Reader::ReadSymbol(void) {
  std::string symbol_name;
  char c = PeekChar();
  if (IsSymbolStartChar(c)) {
    GetChar();
    symbol_name.push_back(c);
    while (IsSymbolChar(c = PeekChar())) {
      GetChar();
      symbol_name.push_back(c);
    }
    return Symbol::Get(symbol_name);
  } else {
    return Failed(reader::Error(
        linum(), colnum(), "Symbol must start with alphabet character or '-'"));
  }
}

const Object *Reader::ReadSymbol(const std::string &symbol_name) {
  for (const char &c : symbol_name) {
    if (!IsSymbolChar(c)) {
      return Failed(reader::Error(
          linum(), colnum(),
          "Symbol must contain only alphanumeric characters or '-'"));
    }
  }
  return Symbol::Get(symbol_name);
}

static const List *ReadSexpHelper(Reader *reader) {
  const Object *obj = reader->ReadExpr();
  reader->ReadWhitespace();
  if (reader->PeekChar() == Delim::kRParen) {
    reader->GetChar();
    return Cons(obj, kNil);
  } else if (reader->PeekChar() == EOF) {
    return nullptr;
  } else {
    const List *rest = ReadSexpHelper(reader);
    return (rest ? Cons(obj, rest) : nullptr);
  }
}

const Object *Reader::ReadSexp(void) {
  GetChar();
  ReadWhitespace();
  const List *list = ReadSexpHelper(this);
  if (!list)
    return Failed(
        reader::Error(linum(), colnum(), "Unable to read S-Expression."));
  return list;
}

const Object *Reader::ReadExpr(void) {
  char c = PeekChar();
  switch (c) {
    case EOF:
      return nullptr;  // This must be handled in all cases
    case Delim::kLParen: {
      return ReadSexp();
    }
    case Delim::kRParen: {
      return Failed(reader::Error(linum(), colnum(), "Unmatched ')'"));
    }
    default: {
      std::string token = Next();
      char start = token[0];
      if (isalpha(start) || (start == '-' && token.size() == 1) ||
          (start == '-' && isalpha(token[1]))) {
        return ReadSymbol(token);
      } else if (isdigit(start) ||
                 (start == '-' && token.size() > 1 && isdigit(token[1]))) {
        return ReadInt(token);
      } else {
        // Get rid of whitespace?
        ReadWhitespace();
        return ReadExpr();
      }
    }
  }
  std::string unknown_char("Unknown character: ");
  unknown_char.push_back('\'');
  unknown_char.push_back(c);
  unknown_char.push_back('\'');
  return Failed(reader::Error(linum(), colnum(), unknown_char));
}

const std::vector<const Object *> Reader::ReadExprList(void) {
  std::vector<const Object *> vec;
  ReadWhitespace();
  while (PeekChar() != EOF) {
    const Object *obj = ReadExpr();
    vec.push_back(obj);
    ReadWhitespace();
  }
  return vec;
}

const Object *Read(std::istream &input_stream) {
  Reader reader(input_stream);
  return reader.ReadExpr();
}

const Object *Read(void) { return Read(std::cin); }

}  // namespace sl
