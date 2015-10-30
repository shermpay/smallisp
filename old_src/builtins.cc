#include "builtins.h"
namespace builtins {
const Symbol *DefSymbol = Symbol::get("def");
const Symbol *DefFnSymbol = Symbol::get("deffn");
const Symbol *IfSymbol = Symbol::get("if");

const std::unordered_map<const Symbol *, SlMacro *> SpecialForms = {
    {DefSymbol, new SlMacro(2)},
    {DefFnSymbol, new SlMacro(3)},
    {IfSymbol, new SlMacro(2)},
};

const Symbol *AddSymbol = Symbol::get("add");
const Symbol *SubSymbol = Symbol::get("sub");
const Symbol *MulSymbol = Symbol::get("mul");

const std::unordered_map<const Symbol *, SlFunction *> Defs = {
    {AddSymbol, new SlFunction(2)},
    {SubSymbol, new SlFunction(2)},
    {MulSymbol, new SlFunction(2)},
};

} // namespace Builtins
