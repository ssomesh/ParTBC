#include <iostream>
#include <stdio.h>
#include <cuda_runtime.h>
#include "utils/timer.h"
#include "graph.h"
#include "approx.h"

// Conditional Compilation for timing the code using #ifdef TIMER. 
// Set the macro using -DTIMER from the command line 

using namespace std;

void write_output_to_file(const char* filename, double* nodeBC, unsigned n) {
  printf("Writing output to %s\n", filename);
  FILE *o = fopen(filename, "w");

  for(unsigned s = 0; s < n; s++) {
    fprintf(o, "%d %.2lf\n", s, nodeBC[s]);
  }

  fclose(o);
}

int main(int argc, char** argv) {

  if(argc < 3) {
    cerr << "Usage: " << argv[0] << " " << "<input graph> k"  << endl; 
    exit(1);
  }
  ifstream in (argv[1]);
  Graph G;
  string input_file = string(argv[1]);
   cudaSetDevice(1);

#ifdef TIMER
  CPUTimer cputimer;
  cputimer.Start();
#endif

  G.readCSR(input_file);  
  //G.printGraph();

  // allocating space for vertex BC, both on CPU and GPU
  double*  nodeBC = (double*) malloc(sizeof(double) * G.nnodes); // the node attribute in this case is node BC
  




#ifdef TIMER
  cputimer.Stop();
  cout << "time elapsed in reading the graph =  " << cputimer.Elapsed()  << " second" << endl;
#endif


#ifdef TIMER
  cputimer.Start();
#endif
  bc_exact(G, nodeBC);
#ifdef TIMER
  cputimer.Stop();
  cout << "time elapsed in bc computation =  " << cputimer.Elapsed()  << " second" << endl;
#endif


#if 1
/* writing the BC of each node to a file */

 // extracting only the file name from the path
 string output_file = "";
 bool flag = false; // to tell that a '.' is encountered
 for(const char* cp = argv[1]; *cp; ++cp) {
   if(flag && *cp == 'e') // a ".e" is encountered, which is potentially the beginning of the extension ".edges"
     break; // break from the for loop
   switch(*cp) {
     case '/' : 
       output_file = "";
       break;
     case '.' :
       flag = true;
       break;
     default: 
         output_file += *cp;
   }
 }


  string output_file1 = "bc_exact_" + output_file + ".txt" ; // creating the desired output filename
 
  write_output_to_file(output_file1.c_str(), nodeBC, G.nnodes);

#endif

  return 0;
}

