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

  for(int i=0;i<6;i++){
    std::cout<<i<<std::endl;
    std::cout<<vector.get_sub_vector(0,i).vector<<std::endl;
  }
  std::cout<<"===="<<std::endl;
  for(int i=0;i<8;i++){
    std::cout<<i<<std::endl;
    std::cout<<vector.get_sub_vector(1,i).vector<<std::endl;
  }
  std::cout<<"===="<<std::endl;
  for(int i=0;i<12;i++){
    std::cout<<i<<std::endl;
    std::cout<<vector.get_sub_vector(2,i).vector<<std::endl;
  }
 
  std::cout<<"===="<<std::endl;
  std::cout<<"===="<<std::endl;
  contraction::Matrices<float> matrix;
  matrix.add("ic",3);
  matrix.add("is",2);
  matrix.add("ix",1);
  matrix.ini();
  data.resize(matrix.size*matrix.size);
  for(int i=0;i<data.size();++i)data.at(i) = i;
  matrix.assign(data);
  for(int i=0;i<2;i++)
  for(int j=0;j<2;j++){
    std::cout<<i<<'\t'<<j<<std::endl;
    std::cout<<matrix.get_sub_matrix(0,i,j).matrix<<std::endl;
  }
  std::cout<<"===="<<std::endl;
  for(int i=0;i<3;i++)
  for(int j=0;j<3;j++){
    std::cout<<i<<'\t'<<j<<std::endl;
    std::cout<<matrix.get_sub_matrix(1,i,j).matrix<<std::endl;
  }
  std::cout<<"===="<<std::endl;
  for(int i=0;i<6;i++)
  for(int j=0;j<6;j++){
    std::cout<<i<<'\t'<<j<<std::endl;
    std::cout<<matrix.get_sub_matrix(2,i,j).matrix<<std::endl;
  }

  std::cout<<"===="<<std::endl;
  std::cout<<"===="<<std::endl;
  Eigen::MatrixXf M1 = Eigen::MatrixXf::Random(8,8);
  std::cout << "Column major input:" << std::endl << M1 << "\n";

  Eigen::Map<Eigen::MatrixXf,0,Eigen::OuterStride<> > M2(M1.data(), M1.rows(), (M1.cols()/2), Eigen::OuterStride<>(M1.outerStride()*2));
  std::cout << "1 column over 3:" << std::endl << M2 << "\n";

  Eigen::Map<Eigen::MatrixXf,0,Eigen::InnerStride<> > M3(M1.data(), M1.rows(), (M1.cols()), Eigen::InnerStride<>(M1.innerStride()*2));
  std::cout << "1 column over 3:" << std::endl << M3.block(0,0,4,8) << "\n";

  Eigen::Map<Eigen::MatrixXf,0,Eigen::Stride<Eigen::Dynamic,Eigen::Dynamic> > M4(M1.data(), M1.rows()/2, (M1.cols()/2),  
										 Eigen::Stride<Eigen::Dynamic,Eigen::Dynamic>(M1.outerStride()*2,M1.innerStride()*2));
  //std::cout << "1 column over 3:" << std::endl << M4.block(0,0,4,4) << "\n";
  std::cout << "1 column over 3:" << std::endl << M4 << "\n";

  //contraction::Vectors<float> vector2;
  //vector2 = matrix*vector;

  return 0;
}
