#include "utils.h"

#include <execinfo.h>
#include <cstdio>
#include <cstdlib>

#include <string>
#include <vector>

#include "objects.h"

namespace sl {

namespace debug {

static bool Enabled = false;

void Enable(bool enable) { Enabled = enable; }

static const size_t kStackTraceSize = 8;
static const std::string kAddr2LineBin = "/bin/addr2line";

// Parse the filename from backtrace string that has the following format:
// FILENAME(FUNCTIONNAME)[ADDRESS]
// Example:
// bin/list_test(_ZNK2sl3Nil4RestEv+0x68) [0x446258]
static const std::string ParseFilename(char *str) {
  std::string filename;
  for (char *c = str; *c != '\0' && *c != '('; ++c) {
    filename.push_back(*c);
  }
  return filename;
}

// Execute addr2line given a filename and an address
static bool ExecAddr2Line(const std::string &filename, void *addr) {
  char exec[256];
  sprintf(exec, "%s -C -e %s %p", kAddr2LineBin.c_str(), filename.c_str(),
          addr);
  system(exec);
  return true;
}

const Error &ErrorWithTrace(const std::string &msg) {
  if (Enabled) {
    fprintf(stderr, "Error: %s\n", msg.c_str());
    void *backtrace_buf[kStackTraceSize];
    int nptrs = backtrace(backtrace_buf, kStackTraceSize);
    fprintf(stderr, "Stack Trace(%d addresses): \n", nptrs);
    char **trace_strs = backtrace_symbols(backtrace_buf, nptrs);
    if (trace_strs) {
      // Skip the first one, it is just this call.
      for (int i = 1; i < nptrs; ++i) {
        const std::string filename = ParseFilename(trace_strs[i]);
        fprintf(stderr, "  %d %s \n    ", i, trace_strs[i]);
        ExecAddr2Line(filename, backtrace_buf[i]);
      }
      delete trace_strs;
    }
  }
  return Error::Val(msg);
}
}  // namespace debug
}  // namespace sl
