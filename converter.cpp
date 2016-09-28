#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <strstream> 
using namespace std;
#include <time.h>
#include <direct.h>
#include < iomanip >
#include "memory.h"
#include <vector>
/* Declarations */


// global variables



int main() {

  // local variables
  double eps = 1e-7;
  double dist = 0;
  int count = 0;
  bool D2skip = false;
  std::stringstream in;
  in.clear();
  string inputfile;
  string in_file;
  string out_file;
  long i, j, nnodes, node, nele, ele, null, nn;
  long nnodes_at, nele_at;
  char str1[1000];

  double  x, y, z;
  vector <double > anode;

  vector <vector <double> > nodedata;
  vector <vector <long> > eledata;
  vector <vector <double> > nodedata_at;
  vector <vector <long> > eledata_at;
  vector <long > anele;
  long mat, e1, e2, e3, e4, e5, e6, e7, e8;
  string dummy, pcs;
  int zones, k;

  vector <double > alayer;
  vector <long > mats;
  vector <long > mats_at;

  vector <bool> del;
  vector <long> newidx;

  vector <vector <long >> Tomats;
  vector <string > eletyp;
  vector <string > eletyp_at;

  string geo;
  char zeile[1000];
  double fummy;
  int iummy;
  double coord = -9999;
  long idx = 0;
  string plane;
  string skip;

  cout << "Mesh-Merge" << endl;
  // read inputfile
  inputfile = "merge.input";
  ifstream input(inputfile.data(), ios::in);
  if (!input) {
    cout << "Error opening inputfile " << inputfile << "\n";
    cout << '\n' << "Quitting";
    return 0;
  }
  cout << "Reading input file " << inputfile << endl;

  input >> geo;
  if (geo.find("#INPUT") != string::npos){
    while (0 == 0){
      input.getline(zeile, 1000);
      input >> geo; //= string(zeile);
      if (geo.find("$PLANE") != string::npos){
        input.getline(zeile, 1000);
        input >> plane; //= string(zeile);
      }
      if (geo.find("$COORDINATE") != string::npos){
        input.getline(zeile, 1000);
        input >> coord; //= string(zeile);
      }
      if (geo.find("$EPSILON") != string::npos){
        input.getline(zeile, 1000);
        input >> eps; //= string(zeile);
      }
      if (geo.find("$SKIP") != string::npos){
        input.getline(zeile, 1000);
        input >> skip; //= string(zeile);
        if (skip.find("2D") != string::npos)
          D2skip = true;
      }
      if (geo.find("#STOP") != string::npos){
        break;
      }
    }
  }
  // close input file
  input.close();
  cout << "Merging along " << plane << " plane at " << plane << " = " << coord << endl;
  cout << " Epsilon = " << eps << endl;
  if(D2skip == true)
    cout << " Skipping 2D elements from attach.msh " << endl;
  if (plane.compare("x") == 0)
    idx = 0;
  if (plane.compare("y") == 0)
    idx = 1;
  if (plane.compare("z") == 0)
    idx = 2;
  
  // first mesh
  in_file = "base.msh";
  ifstream ein (in_file.data(),ios::in);
  if (!ein.good()){
    cout << " Input file " << in_file  << " not found " << endl;
	  exit(0);
  }

  cout << " Reading  " << in_file << endl;
  // skip header mesh file
  ein.getline(str1, 1000); 
  ein.getline(str1, 1000); 
  ein.getline(str1, 1000); 
  in.str((string ) str1);
  in >> pcs;
  in.clear();
  ein.getline(str1, 1000); 
  // get no nodes
  ein.getline(str1, 1000); 
  in.str((string ) str1);
  in >> nnodes;
  in.clear();
  cout << "Nodes: " << nnodes << endl;
  // read nodedata
  for(i=0;i<nnodes;i++){
    ein.getline(str1, 1000); 
    in.str((string ) str1);
    in >> node >> x >> y >> z;
    anode.push_back(x);
    anode.push_back(y);
    anode.push_back(z);
    nodedata.push_back(anode);
    anode.clear();
    in.clear();
  }

  // skip header mesh file
  ein.getline(str1, 1000); 
  // get no ele
  ein.getline(str1, 1000); 
  in.str((string ) str1);
  in >> nele;
  in.clear();
  cout << "Elements: " << nele << endl;
  // read eledata
  for (i = 0; i < nele; i++){
    ein.getline(str1, 1000);
    in.str((string)str1);
    in >> ele >> mat >> dummy;
    anele.push_back(mat);

    if (dummy.find("tri") != string::npos){
      in >> e1 >> e2 >> e3;
      anele.push_back(e1);
      anele.push_back(e2);
      anele.push_back(e3);
    }
    if (dummy.find("quad") != string::npos){
      in >> e1 >> e2 >> e3 >> e4;
      anele.push_back(e1);
      anele.push_back(e2);
      anele.push_back(e3);
      anele.push_back(e4);
    }
    if (dummy.find("tet") != string::npos){
      in >> e1 >> e2 >> e3 >> e4;
      anele.push_back(e1);
      anele.push_back(e2);
      anele.push_back(e3);
      anele.push_back(e4);
    }
    if (dummy.find("pyr") != string::npos){
      in >> e1 >> e2 >> e3 >> e4 >> e5;
      anele.push_back(e1);
      anele.push_back(e2);
      anele.push_back(e3);
      anele.push_back(e4);
      anele.push_back(e5);
    }
    if (dummy.find("pri") != string::npos){
      in >> e1 >> e2 >> e3 >> e4 >> e5 >> e6;
      anele.push_back(e1);
      anele.push_back(e2);
      anele.push_back(e3);
      anele.push_back(e4);
      anele.push_back(e5);
      anele.push_back(e6);
    }
    if (dummy.find("hex") != string::npos){
      in >> e1 >> e2 >> e3 >> e4 >> e5 >> e6 >> e7 >> e8;
      anele.push_back(e1);
      anele.push_back(e2);
      anele.push_back(e3);
      anele.push_back(e4);
      anele.push_back(e5);
      anele.push_back(e6);
      anele.push_back(e7);
      anele.push_back(e8);
    }
    eletyp.push_back(dummy);
    eledata.push_back(anele);
    anele.clear();
    in.clear();
  }
  ein.close();


  // second mesh
  in_file = "attach.msh";
  ein.open(in_file.data(), ios::in);
  if (!ein.good()){
    cout << " Input file " << in_file << " not found " << endl;
    exit(0);
  }

  cout << " Reading  " << in_file << endl;
  // skip header mesh file
  ein.getline(str1, 1000);
  ein.getline(str1, 1000);
  ein.getline(str1, 1000);
  in.str((string)str1);
  in >> pcs;
  in.clear();
  ein.getline(str1, 1000);
  // get no nodes
  ein.getline(str1, 1000);
  in.str((string)str1);
  in >> nnodes_at;
  in.clear();
  cout << "Nodes: " << nnodes_at << endl;
  // read nodedata
  for (i = 0; i<nnodes_at; i++){
    ein.getline(str1, 1000);
    in.str((string)str1);
    in >> node >> x >> y >> z;
    anode.push_back(x);
    anode.push_back(y);
    anode.push_back(z);
    nodedata_at.push_back(anode);
    anode.clear();
    in.clear();
    del.push_back(false);
    newidx.push_back(-1);
  }

  // skip header mesh file
  ein.getline(str1, 1000);
  // get no ele
  ein.getline(str1, 1000);
  in.str((string)str1);
  in >> nele_at;
  in.clear();
  cout << "Elements: " << nele_at << endl;
  // read eledata
  for (i = 0; i<nele_at; i++){
    ein.getline(str1, 1000);
    in.str((string)str1);
    in >> ele >> mat >> dummy;
    anele.push_back(mat);

    if (!D2skip){
      if (dummy.find("tri") != string::npos){
        in >> e1 >> e2 >> e3;
        anele.push_back(e1);
        anele.push_back(e2);
        anele.push_back(e3);
      }
      if (dummy.find("quad") != string::npos){
        in >> e1 >> e2 >> e3 >> e4;
        anele.push_back(e1);
        anele.push_back(e2);
        anele.push_back(e3);
        anele.push_back(e4);
      }
    }
    if (dummy.find("tet") != string::npos){
      in >> e1 >> e2 >> e3 >> e4;
      anele.push_back(e1);
      anele.push_back(e2);
      anele.push_back(e3);
      anele.push_back(e4);
    }
    if (dummy.find("pyr") != string::npos){
      in >> e1 >> e2 >> e3 >> e4 >> e5;
      anele.push_back(e1);
      anele.push_back(e2);
      anele.push_back(e3);
      anele.push_back(e4);
      anele.push_back(e5);
    }
    if (dummy.find("pri") != string::npos){
      in >> e1 >> e2 >> e3 >> e4 >> e5 >> e6;
      anele.push_back(e1);
      anele.push_back(e2);
      anele.push_back(e3);
      anele.push_back(e4);
      anele.push_back(e5);
      anele.push_back(e6);
    }
    if (dummy.find("hex") != string::npos){
      in >> e1 >> e2 >> e3 >> e4 >> e5 >> e6 >> e7 >> e8;
      anele.push_back(e1);
      anele.push_back(e2);
      anele.push_back(e3);
      anele.push_back(e4);
      anele.push_back(e5);
      anele.push_back(e6);
      anele.push_back(e7);
      anele.push_back(e8);
    }
    eletyp_at.push_back(dummy);
    eledata_at.push_back(anele);
    anele.clear();
    in.clear();
  }
  nele_at = eledata_at.size();
  ein.close();


  // check for each node of the attached mesh
  for (i = 0; i < nnodes_at; i++){
    // is node on attachment surface?
    if (nodedata_at[i][idx] != coord){
      // no
      newidx[i] = nnodes + count;
      count++;
      continue; 
    }
    //yes: search double node
    for (j = 0; j < nnodes; j++){
      // is node on attachment surface?
      if (nodedata[i][idx] != coord)
        continue;
      // check distance
      dist =  (nodedata_at[i][0] - nodedata[j][0])*(nodedata_at[i][0] - nodedata[j][0]);
      dist += (nodedata_at[i][1] - nodedata[j][1])*(nodedata_at[i][1] - nodedata[j][1]);
      dist += (nodedata_at[i][2] - nodedata[j][2])*(nodedata_at[i][2] - nodedata[j][2]);
      dist = pow(dist, 0.5);
      if (dist < eps){ // found
        del[i] = true; // remove this node
        newidx[i] = j; // replace by base mesh node index
        cout << " duplicate node found with indices: " << j << " : " << i << endl;
        break;
      }
    }
  }


  out_file = "output.msh";
  ofstream aus(out_file.data(), ios::out);
  if (!aus.good()){
    cout << " Output file " << out_file  << " not opened " << endl;
    exit(0);
  }

  cout << " Writing  " << out_file << endl;
  // header nodes
  aus << "#FEM_MSH" << endl;
  aus << " $PCS_TYPE" << endl;
  aus << pcs << endl;
  aus << " $NODES" << endl;
  aus << nnodes+count << endl;

  int ncount = 0;
  // base mesh nodes, as read in
  for (i = 0; i<nnodes; i++){
    aus << i << " ";
    aus << setprecision(12) << nodedata[i][0] << " ";
    aus << setprecision(12) << nodedata[i][1] << " ";
    aus << setprecision(12) << nodedata[i][2] << " ";
    aus << endl;
  }
  // new nodes, skip double ones
  for (i = 0; i < nnodes_at; i++){
    if (del[i] == true)
      continue;
    aus << newidx[i] << " ";
    aus << setprecision(12) << nodedata_at[i][0] << " ";
    aus << setprecision(12) << nodedata_at[i][1] << " ";
    aus << setprecision(12) << nodedata_at[i][2] << " ";
    aus << endl;
    ncount++;
  }

  // Now the elements
  aus << " $ELEMENTS" << endl;
  aus << nele+nele_at<< endl;

  for (i = 0; i < nele; i++){
      aus << i << " "; // idx
      aus << eledata[i][0] << " "; // mg
      aus <<  eletyp[i] << " ";  // typ
      for (j = 1; j < eledata[i].size(); j++) // nodes
        aus << eledata[i][j] << " ";
      aus << endl;
    }
    
  for (i = 0; i < nele_at; i++){
    aus << nele + i << " "; // idx
    aus << eledata_at[i][0] << " "; // mg
    aus << eletyp_at[i] << " ";  // typ
    for (j = 1; j < eledata[i].size(); j++) {// nodes
      idx = eledata_at[i][j];
      aus << newidx[idx] << " ";
    }
    aus << endl;
  }
  aus << "#STOP" ;

  cout << "Nodes: " << nnodes + ncount << endl;
  cout << "Elements: " << nele + nele_at << endl;



  aus.close();
  cout << "Output file: output.msh - done!" << endl;

  nodedata.clear();
  anode.clear();
  eledata.clear();
  anele.clear();
  alayer.clear();
  mats.clear();
  Tomats.clear();
  eletyp.clear();

  system("PAUSE");

  return 0;
}

