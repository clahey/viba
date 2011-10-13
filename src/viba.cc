/*
 ============================================================================
 Name        : viba.cpp
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C++,
 ============================================================================
 */

#include <iostream>
#include "tune.hh"

using namespace std;

int main(int argc, char* argv[]) {
  Tune().Parse(argv[1]);
  return 0;
}
