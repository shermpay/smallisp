/* -*- mode: C++ -*- */
#ifndef _SYMBOL_DEF
#define _SYMBOL_DEF

#include <string>
#include <sstream>
#include <unordered_map>

struct Object;
class Symbol;

class Symbol {
public:
  const std::string name() const { return this->Name; }
  Object *object() const { return this->Obj; }
  static Symbol *get(const std::string &Name) {
    Symbol *Sym;
    auto Found = Pool.find(Name);
    if (Found == Pool.end()) {
      Sym = new Symbol(Name);
      Pool.insert({Name, Sym});
    } else {
      Sym = Found->second;
    }
    return Sym;
  };

  static std::string dumpPool() {
    std::ostringstream SStream;
    SStream << "POOL:\n";
    for (auto Pair : Pool) {
      SStream << "\t" << Pair.first << "\n";
    }
    return SStream.str();
  };

private:
  static std::unordered_map<std::string, Symbol *> Pool;
  const std::string Name;
  Object *Obj;
  Symbol(const std::string &Name) : Name(Name) {}
};

#endif
