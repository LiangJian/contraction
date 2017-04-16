/*
   type.h
   April 2017
*/
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
    void test(){};
    unsigned int get_size(){return size;}
    void load(std::string filename){};
    void assign(std::vector<std::complex<T> >&data){
      if(data.size() != size*size) {std::cout<<"size mismatch"<<std::endl;exit(-1);}
      for(int i=0;i<size;i++)
      for(int j=0;j<size;j++)
	matrix(i,j) = data.at(i*size+j);
    };

    Eigen::Matrix< std::complex<T> , Eigen::Dynamic , Eigen::Dynamic>get_matrix(int order, int count1=0,int count2=0){
	//std::cout<<matrix<<std::endl;
      Eigen::Matrix< std::complex<T> , Eigen::Dynamic , Eigen::Dynamic>val;
      int outersize = 1;
      for(int i=order+1;i<meta.size();i++)outersize*=meta[i].length;
      int innersize = 1;
      for(int i=order-1;i>=0;i--)innersize*=meta[i].length;
      if(count1 > outersize*innersize) std::cerr<<"size mismatch"<<std::endl;
      if(count2 > outersize*innersize) std::cerr<<"size mismatch"<<std::endl;
      int i_outer1 = count1/innersize;
      int i_inner1 = count1%innersize;
      int i_outer2 = count2/innersize;
      int i_inner2 = count2%innersize;
      val = Eigen::Map<Eigen::Matrix< std::complex<T> , Eigen::Dynamic , Eigen::Dynamic>, 0, Eigen::Stride<Eigen::Dynamic,Eigen::Dynamic> >
	(matrix.data()+(i_outer2*innersize*meta[order].length+i_inner2)*size+i_outer1*innersize*meta[order].length+i_inner1, 
	 meta[order].length,meta[order].length, Eigen::Stride<Eigen::Dynamic,Eigen::Dynamic>(matrix.outerStride()*innersize,matrix.innerStride()*innersize));
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
    void test(){};
    unsigned int get_size(){return size;}
    void load(std::string filename){};
    void assign(std::vector<std::complex<T> >&data){
      if(data.size() != size) {std::cout<<"size mismatch"<<std::endl;exit(-1);}
      for(int i=0;i<size;i++) vector[i]=data.at(i);
    };
    void copy_meta(Vectors<T>& src){
      meta=src.meta;
      size=src.size;
    }
    Eigen::Matrix< std::complex<T> , Eigen::Dynamic , 1> get_vector(int order, int count=0){
      Eigen::Matrix< std::complex<T> , Eigen::Dynamic , 1>val;
      int outersize = 1;
      for(int i=order+1;i<meta.size();i++)outersize*=meta[i].length;
      int innersize = 1;
      for(int i=order-1;i>=0;i--)innersize*=meta[i].length;
      //std::cout<<outersize<<'\t'<<innersize<<std::endl;
      if(count > outersize*innersize) std::cerr<<"size mismatch"<<std::endl;
      int i_outer = count/innersize;
      int i_inner = count%innersize;
      val = Eigen::Map<Eigen::Matrix< std::complex<T> , Eigen::Dynamic , 1>, 0, Eigen::InnerStride<Eigen::Dynamic> >
	(vector.data()+i_outer*innersize*meta[order].length+i_inner, meta[order].length, 1, Eigen::InnerStride<>(innersize));
	//.block(0,0,2,1);//pos pos size size
      return val;
    }
    Vectors<T> get_sub_vector(int order, int count=0){
      Eigen::Matrix< std::complex<T> , Eigen::Dynamic , 1>tmp;
      int outersize = 1;
      for(int i=order+1;i<meta.size();i++)outersize*=meta[i].length;
      int innersize = 1;
      for(int i=order-1;i>=0;i--)innersize*=meta[i].length;
      //std::cout<<outersize<<'\t'<<innersize<<std::endl;
      if(count > outersize*innersize) std::cerr<<"size mismatch"<<std::endl;
      int i_outer = count/innersize;
      int i_inner = count%innersize;
      tmp = Eigen::Map<Eigen::Matrix< std::complex<T> , Eigen::Dynamic , 1>, 0, Eigen::InnerStride<Eigen::Dynamic> >
	(vector.data()+i_outer*innersize*meta[order].length+i_inner, meta[order].length, 1, Eigen::InnerStride<>(innersize));
	//.block(0,0,2,1);//pos pos size size
      Vectors<T>val;
      val.add(meta[order].name,meta[order].size);
      val.ini();
      val.vector = tmp;
      return val;
    }
    Vectors<T> friend operator *(Matrices<T>& M,Vectors<T>& src){
      Vectors<T>val;
      val.copy_meta(src);
      val.ini();
      if(M.size == src.size ){
	  val.vector=M.matrix*src.vector;
      }else{
	  std::cerr<<"size mismatch"<<std::endl;
	  exit(-1);
      }
      return val;
    };
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
