/*
   type.h
   April 2017
*/
#ifndef __TYPE__
#define __TYPE__
#include <Eigen/Dense>
#include <vector>
#include <iostream>

namespace contraction{

  struct Param{
    std::string name;
    unsigned int length;
    unsigned int order;
    Param(std::string name_, unsigned length_, unsigned order_){
      name = name_;
      length = length_;
      order = order_;
    }
  }; 

  template<typename T>
  class Vectors;

  template<typename T>
  class Matrices{
  public:
    unsigned int size;
    Eigen::Matrix< std::complex<T> , Eigen::Dynamic , Eigen::Dynamic>matrix;
    std::vector <Param> meta;
    Matrices(){size=1;};
    ~Matrices(){};
    void add(std::string name, unsigned length){
      Param tmp(name,length,meta.size());
      meta.push_back(tmp);
      size *= length;
    }
    void ini(){
      if(size>1)
	matrix.resize(size,size);
      else
	std::cout<<"nothing in the matrix"<<std::endl;
    }

    void load(std::string filename){}

    void assign(std::vector<std::complex<T> >&data){
      if(data.size() != size*size) {throw("size mismatch");}
      for(int i=0;i<size;i++)
      for(int j=0;j<size;j++)
	matrix(i,j) = data.at(i*size+j);
    }
    void copy_meta(Matrices<T>const& src){
      meta=src.meta;
      size=src.size;
    }

    Matrices<T>get_sub_matrix(int order, int count1=0,int count2=0){
      Eigen::Matrix< std::complex<T> , Eigen::Dynamic , Eigen::Dynamic>tmp;
      int outersize = 1;
      for(int i=order+1;i<meta.size();i++)outersize*=meta[i].length;
      int innersize = 1;
      for(int i=order-1;i>=0;i--)innersize*=meta[i].length;
      if(count1 > outersize*innersize) throw("size mismatch");
      if(count2 > outersize*innersize) throw("size mismatch");
      int i_outer1 = count1/innersize;
      int i_inner1 = count1%innersize;
      int i_outer2 = count2/innersize;
      int i_inner2 = count2%innersize;
      tmp = Eigen::Map<Eigen::Matrix< std::complex<T> , Eigen::Dynamic , Eigen::Dynamic>, 0, Eigen::Stride<Eigen::Dynamic,Eigen::Dynamic> >
	(matrix.data()+(i_outer2*innersize*meta[order].length+i_inner2)*size+i_outer1*innersize*meta[order].length+i_inner1, 
	 meta[order].length,meta[order].length, Eigen::Stride<Eigen::Dynamic,Eigen::Dynamic>(matrix.outerStride()*innersize,matrix.innerStride()*innersize));
      Matrices<T>val;
      val.add(meta[order].name,meta[order].length);
      val.ini();
      val.matrix = tmp;
      return val;
    }
    Vectors<T> operator *(Vectors<T>const& V){
      Vectors<T>val;
      val.copy_meta(V);
      val.ini();
      if(size != V.size ){
	  throw("size mismatch");
      }else if(meta[0].name != V.meta[0].name){
	  throw("dimension name mismatch");
      }else{
	  val.vector=matrix*V.vector;
      }
      return val;
    }

  };

  template<typename T>
  class Vectors{
  public:
    unsigned int size;
    Eigen::Matrix< std::complex<T> , Eigen::Dynamic , 1>vector;
    std::vector <contraction::Param> meta;
    Vectors(){size=1;};
    ~Vectors(){};
    void add(std::string name, unsigned length){
      Param tmp(name,length,meta.size());
      meta.push_back(tmp);
      size *= length;
    }
    void ini(){
      if(size>1)
	vector.resize(size);
      else
	std::cout<<"nothing in the vector"<<std::endl;
    }
    void load(std::string filename){};

    void assign(std::vector<std::complex<T> >&data){
      if(data.size() != size) {throw("size mismatch");}
      for(int i=0;i<size;i++) vector[i]=data.at(i);
    };
    void copy_meta(Vectors<T>const& src){
      meta=src.meta;
      size=src.size;
    }

    Vectors<T> get_sub_vector(int order, int count=0){
      Eigen::Matrix< std::complex<T> , Eigen::Dynamic , 1>tmp;
      int outersize = 1;
      for(int i=order+1;i<meta.size();i++)outersize*=meta[i].length;
      int innersize = 1;
      for(int i=order-1;i>=0;i--)innersize*=meta[i].length;
      //std::cout<<outersize<<'\t'<<innersize<<std::endl;
      if(count > outersize*innersize) throw("size mismatch");
      int i_outer = count/innersize;
      int i_inner = count%innersize;
      tmp = Eigen::Map<Eigen::Matrix< std::complex<T> , Eigen::Dynamic , 1>, 0, Eigen::InnerStride<Eigen::Dynamic> >
	(vector.data()+i_outer*innersize*meta[order].length+i_inner, meta[order].length, 1, Eigen::InnerStride<>(innersize));
	//.block(0,0,2,1);//pos pos size size
      Vectors<T>val;
      val.add(meta[order].name,meta[order].length);
      val.ini();
      val.vector = tmp;
      return val;
    }
  };

  template<typename T>
  class Peram_sink: public Vectors<T>
  {
  public:
    Peram_sink(int Ns, int Nt, int NeV){
      this->add("is",Ns);
      this->add("it",Nt);
      this->add("ieV",NeV);
      this->ini();
    }
  };

  template<typename T>
  class Peram_source: public Vectors<T>
  {
  public:
    Peram_source(int Nr, int NdlS, int NdlT, int NdlV){
      this->add("ir",Nr);
      this->add("idlS",NdlS);
      this->add("idlT",NdlT);
      this->add("idlV",NdlV);
      this->ini();
    }
  };

  template<typename K, typename V>
  class Stochastic_peram
  {
  public:
    void test(){
	std::cout<<(sizeof(K))<<'\t'<<sizeof(V)<<std::endl;
    }
    Stochastic_peram(){};
    //void test(void);
  };
}
#endif
