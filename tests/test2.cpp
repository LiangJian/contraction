#include <iostream>
#include <Eigen/Dense>
#include "../includes/type.h"
#include <vector>

using Eigen::MatrixXd;
int main()
{
  std::vector<std::complex<float> > data;

  contraction::Vectors<float> vector;
  vector.add("ix",4);
  vector.add("is",3);
  vector.add("ic",2);
  vector.ini();
  data.resize(vector.size);
  for(int i=0;i<data.size();++i)data.at(i) = i;
  vector.assign(data);

  contraction::Matrices<float> matrix;
  matrix.add("ic",4);
  matrix.add("is",2);
  matrix.add("ix",4);
  matrix.ini();
  data.resize(matrix.size*matrix.size);
  for(int i=0;i<data.size();++i)data.at(i) = i;
  matrix.assign(data);

  try{
    std::cout<<(matrix.get_sub_matrix(0,0,0)*vector.get_sub_vector(0,0)).vector<<std::endl;
  }catch (const char* msg){
    std::cerr << msg << std::endl;
  }
  try{
  std::cout<<(matrix.get_sub_matrix(1,0,0)*vector.get_sub_vector(1,0)).vector<<std::endl;
  }catch (const char* msg){
    std::cerr << msg << std::endl;
  }
  try{
  std::cout<<(matrix.get_sub_matrix(2,0,0)*vector.get_sub_vector(0,0)).vector<<std::endl;
  }catch (const char* msg){
    std::cerr << msg << std::endl;
  }

  return 0;
}
