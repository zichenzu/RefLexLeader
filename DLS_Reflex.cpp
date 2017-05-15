 
#include <gecode/driver.hh> 
#include <gecode/int.hh>              
#include <gecode/minimodel.hh>      
#include <gecode/Reflex/reflex.hpp>  
using namespace Gecode;     

int s_n,s_nn;    
      
int soln=0; 
int sol;  
IntArgs PerA; 
 
    
//**************************************************// 
//**************************************************//
//**************************************************//
//**************************************************//
//**************************************************//
//**************************************************//

   

 
int pn(int a)
{
	if(a==1) return 1; 
	else return a*pn(a-1);
}

  
class Perm : public Space { 
	protected: 
	   IntVarArray q; 
	public: 
	   Perm(void) :q(*this,s_n,1,s_n) { 
		  distinct(*this,q);
		  branch(*this, q, INT_VAR_NONE(), INT_VAL_MIN()); 
	   } 
	   // Search support 
	   Perm(bool share, Perm& s) : Space(share, s) {
		  q.update(*this, share, s.q); 
	   }   
	   virtual Space* copy(bool share) {
		  return new Perm(share,*this); 
	   } 
	   // Print solution 
	   void print(void) const {
		  for(int i=0;i<q.size();i++)
			PerA[sol*s_n+i]=q[i].val();
		  sol++;
	   } 
	   
}; 

class DLS : public Script { 
private:
  /// Size of magic square
  const int n;
  /// Fields of square  
  IntVarArray x;

public:
  /// Post constraints
  DLS(const SizeOptions& opt)
    : n(opt.size()), x(*this,n*n,1,n) { 
 
 
	IntVarArgs H1((n*n-n)/2);  
	IntVarArgs H2((n*n-n)/2);  
	int ss=0; 
	for(int i=0;i<n;i++)
		for(int j=i+1;j<n;j++)   
		{           
			H1[ss]=x[i*n+j];
			H2[ss++]=x[j*n+i];      
			   
		}     
	reflex(*this, H1, H2);   
  
	//value precedence 
	IntArgs c(n);
	for(int i=1;i<=n;i++)
		c[i-1]=i; 
	precede(*this,x,c);  
 
	//symmetry preservation constraints
	for(int i=0;i<opt.size();i++)
		rel(*this,x[i*opt.size()+i],IRT_EQ,i+1); 
	rel(*this,x[0*opt.size()+opt.size()-1],IRT_LQ,x[(opt.size()-1)*opt.size()]); 
 
  
    Perm* ms = new Perm;    
	DFS<Perm> e(ms);        
	delete ms; 
	// Use the search engine to find all solutions  
	while (Perm*sf =e.next()) { 
		sf->print(); delete sf;
	}
	std::cout<<"the sol is "<<sol<<"\n";
	  
	branch(*this, x, INT_VAR_NONE(), INT_VAL_MIN());
  }        
  
  /// Constructor for cloning \a s
  DLS(bool share, DLS& s) : Script(share,s), n(s.n) {   
    x.update(*this, share, s.x);
  }
 
  /// Copy during cloning
  virtual Space* 
  copy(bool share) {
    return new DLS(share,*this);  
  }
  /// Print solution
  virtual void
  print(std::ostream& os) const { 
 } 

};

/** \brief Main-function
 *  \relates DLS
 */
int
main(int argc, char* argv[]) {
  SizeOptions opt("DLS");
  opt.iterations(1);
  opt.size(3);
  opt.solutions(0);
  opt.c_d(1);
  opt.parse(argc,argv);
  s_n=opt.size();
  Script::run<DLS,DFS,SizeOptions>(opt);
  std::cout<<soln<<"\n";
  return 0;
}

 

