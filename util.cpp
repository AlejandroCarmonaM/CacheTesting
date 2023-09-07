#include <cstdlib>
#include <cstring>
#include <iostream>

using namespace std;

bool parse_size_arg(const char* arg, const char* name, size_t& var) {
  auto len = strlen(name);
  if (arg[0] == '-' && arg[1] == '-' && !strncmp(&arg[2], name, len) && arg[len + 2] == '=') {
    errno = 0;
    char *p;
    var = strtol(&arg[3 + len], &p, 10);
    if (errno != 0 || p == &arg[3 + len] || *p != '\0') {
      cerr << "El valor de --" << name << " debe ser entero: " << &arg[3+len] << endl;
      return false;
    } else {
      return true;
    }
  } else {
    return false;
  }
}

bool parse_double_arg(const char* arg, const char* name, double& var) {
  auto len = strlen(name);
  if (arg[0] == '-' && arg[1] == '-' && !strncmp(&arg[2], name, len) && arg[len + 2] == '=') {
    errno = 0;
    char *p;
    var = strtof(&arg[3 + len], &p);
    if (errno != 0 || p == &arg[3 + len] || *p != '\0') {
      cerr << "El valor de --" << name << " debe ser un número: " << &arg[3+len] << endl;
      return false;
    } else {
      return true;
    }      
  } else {
    return false;
  }
}

bool parse_bool_arg(const char* arg, const char* name, bool& var) {
  auto len = strlen(name);
  if (arg[0] == '-' && arg[1] == '-' && !strncmp(&arg[2], name, len) && (arg[len + 2] == '=' || arg[len + 2] == '\0')) {
    if (arg[len + 2] == '\0' || !strcmp(&arg[2 + len], "=true")) {
      var = true;
      return true;
    } else if (!strcmp(&arg[2 + len], "=false")) {
      var = false;
      return true;
    } else {
      cerr << "El valor de --" << name << " debe ser un true o false: " << &arg[3+len] << endl;
      return false;
    }
  } else {
    return false;
  }
}

bool parse_string_arg(const char* arg, const char* name, string& var) {
  auto len = strlen(name);
  if (arg[0] == '-' && arg[1] == '-' && !strncmp(&arg[2], name, len) && arg[len + 2] == '=') {
    var = string(&arg[3 + len]);
    return true;
  } else {
    return false;
  }
}

