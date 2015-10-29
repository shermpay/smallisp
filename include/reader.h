//===- smallisp/reader.h - Smallisp reader definition --*- mode: C++ -*-=== //

#include <iostream>

#include "list.h"
#include "objects.h"

namespace sl {

namespace reader {

class Error {
 public:
  Error(long linum, int colnum, const std::string &msg) : msg_(msg) {
    this->linum_ = linum;
    this->colnum_ = colnum;
  };

  Error &operator=(const Error &error) {
    linum_ = error.linum();
    colnum_ = error.colnum();
    msg_ = error.msg();
    return *this;
  };

  inline long linum(void) const { return linum_; };
  inline int colnum(void) const { return colnum_; };
  inline std::string msg(void) const { return msg_; };

  const std::string Str(void) const;

 private:
  long linum_;
  int colnum_;
  std::string msg_;
};

}  // namespace reader

// Reader is a complete smallisp reader that reads smallisp expressions
// and produces smallisp objects.
// TODO: Consider implementing Reader using a stream interface(use operator>>).
class Reader {
 public:
  static const long kStartLinum;
  static const int kStartColnum;
  Reader(std::istream &input_stream)
      : linum_(kStartLinum),
        colnum_(kStartColnum),
        stream_(input_stream),
        error_(nullptr) {}

  // Getters
  inline long linum(void) const { return linum_; }
  inline int colnum(void) const { return colnum_; }
  inline std::istream &stream(void) { return stream_; }
  inline const std::string &curr_line(void) const { return curr_line_; }
  inline const reader::Error *error(void) const { return error_; }

  inline void inc_linum(void) { ++linum_; }
  inline void inc_colnum(void) { ++colnum_; }
  inline void dec_linum(void) { --linum_; }
  inline void dec_colnum(void) { --colnum_; }
  inline void reset_linum(void) { linum_ = kStartLinum; }
  inline void reset_colnum(void) { colnum_ = kStartColnum; }
  inline void curr_line_push_back(char c) { curr_line_.push_back(c); }
  inline void clear_curr_line(void) { curr_line_.clear(); }

  // Declare that the reader has failed and stores the error
  void *Failed(const reader::Error &err);
  // Return the next character in the stream
  char PeekChar(void);
  // Get the next character in the stream
  // Update the state of the reader accordingly
  char GetChar(void);
  // Read the next token seperated by whitespace
  std::string Next(void);

  // Output a string representation of the current state of the reader
  const std::string Str(void) const;
  // Reading
  void ReadWhitespace(void);
  const Int *ReadInt(void);
  const Int *ReadInt(const std::string &);
  const Symbol *ReadSymbol(void);
  const Symbol *ReadSymbol(const std::string &);
  const List *ReadSexp(void);
  const Object *ReadExpr(void);

 private:
  long linum_;
  int colnum_;
  std::istream &stream_;
  std::string curr_line_;
  const reader::Error *error_;
};

// Read is the standard interface to reading smallisp expressions
const Object *Read(std::istream &input_stream);

// Read with stdin as the input_stream
const Object *Read(void);
};
