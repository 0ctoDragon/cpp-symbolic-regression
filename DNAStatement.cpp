#include "StdAfx.h"
#include ".\dnastatement.h"

void StatementParameters::setTreeDensity(int TD){
		if((TD>30)&&(TD<=100))
			TreeDensity=TD;
		else
			TreeDensity = DEFAULTREEDENSITY;
		}
int StatementParameters::getTreeDensity() const{return TreeDensity;}
void StatementParameters::setMaxDepth(int MD){
			Maxdepth=MD;
		}
int StatementParameters::getMaxDepth() const{return Maxdepth;}
void StatementParameters::setMutationProb(double Prob){
    if(Prob<0.0f) MutationPbblty = 0.0f;
    else if(Prob>1.0f) MutationPbblty = 1.0f;
    else MutationPbblty = Prob;
}
double StatementParameters::getMutationProb( )const {return MutationPbblty;}


void CDNAStatement::setTreeDensity(int TD){
		if((TD>30)&&(TD<=100))
			TreeDensity=TD;
		else
			TreeDensity = DEFAULTREEDENSITY;
		}
int CDNAStatement::getTreeDensity() const{return TreeDensity;}
void CDNAStatement::setMaxDepth(int MD){
			Maxdepth=MD;
		}
int CDNAStatement::getMaxDepth() const{return Maxdepth;}
void CDNAStatement::setMutationProb(double Prob){
    if(Prob<0.0f) MutationPbblty = 0.0f;
    else if(Prob>1.0f) MutationPbblty = 1.0f;
    else MutationPbblty = Prob;
}

double CDNAStatement::getMutationProb( )const {return MutationPbblty;}
double CDNAStatement::getFitness( )const {return this->Fitness;}
void CDNAStatement::setFitness( double Fit){this->Fitness = Fit;} ;



CDNAStatement::CDNAStatement(GENEStatementType S,  COUNTER MaxDepthX):
Type(S), Fitness(0.0f), CrossOverMaxDepth(MaxDepthX){

	MemTrace(1);      

	this->setMaxDepth((rand()%4)+6);
	this->setTreeDensity((rand()%20)+70);
	this->setMutationProb((((double)(rand()%50))+100.0) / 1000.0);
    try{
		
        arity = CFunctionSet::Arity(S); 
		CompleteConstruction();
	}
    catch(CString Pblm){
        throw Pblm;
    }

};

/*******************************
Admin methods
*******************************/
void CDNAStatement::destroy(){
    
	arity				= 0; 
	Type				= UNDEF;
	MutationPbblty		= 0.0f;
	CrossOverMaxDepth	= 0;
            
        for(unsigned int i=0; i<SubStatements.size(); i++)
            delete  SubStatements[i];

	Fitness = 0.0f; 
	SubStatements.clear();
}
void CDNAStatement::copy(const CDNAStatement& S){
	this->destroy();
	arity   = S.arity; 
        Type    = S.Type;
		TreeDensity = S.TreeDensity;
		Maxdepth =  S.Maxdepth;
        Fitness = S.Fitness;
        MutationPbblty   =  S.MutationPbblty;   
        CrossOverMaxDepth = S.CrossOverMaxDepth;
        
        for(unsigned int j=0; j<S.SubStatements.size(); j++)
            SubStatements.push_back(new CDNAStatement(*(S.SubStatements[j])));
}



const CDNAStatement& CDNAStatement::operator=(const CDNAStatement& S){
    copy(S);
    return *this;
}


void CDNAStatement::CompleteConstruction(){
    while(SubStatements.size() < arity)
        SubStatements.push_back(new CDNAStatement(UNDEF));
}



CDNAStatement::CDNAStatement(const CDNAStatement& S):
Fitness(NULL)
{
    copy(S);
    MemTrace(1);
}


CDNAStatement::~CDNAStatement(void)
{
    destroy();
}

/*************************
Set methods
**************************/
void CDNAStatement::setCrossOverMaxDepth(int Depth){
        if(CrossOverMaxDepth > 0){
            CrossOverMaxDepth = Depth;
            return; 
        }
        throw CString(_T("Illegal value for CrossOverMaxDepth at CDNAStatement::setCrossOverMaxDepth"));
}



void CDNAStatement::addBranch(unsigned int branchNum, const CDNAStatement& S){
	SubStatements[branchNum] = new CDNAStatement(S);
}

bool CDNAStatement::removeBranch(unsigned int branchNum){


        if(branchNum >= this->arity) return false;
        
        if(!(!(*SubStatements[branchNum]))) {
                delete SubStatements[branchNum];
                SubStatements[branchNum] = new CDNAStatement(UNDEF);
        }
        return true;
}


bool CDNAStatement::replaceBranch(unsigned int branchNum, const CDNAStatement& S){
	if(removeBranch(branchNum)) {
		addBranch(branchNum, S);    
		return true;
	}
	return false;
}


/*************************
Get methods
**************************/
unsigned int CDNAStatement::getSize()  const{
    //Number of nodes in the tree
    COUNTER Total = 0;
    for(COUNTER i=0; i<this->SubStatements.size(); i++)
        Total += this->SubStatements[i]->getSize();
    return Total+1;
}  

unsigned int CDNAStatement::getDepth() const{

    if(Type == UNDEF) return (unsigned int)0;
    if(!arity) return (unsigned int)1;
    
    unsigned int res = 0;
    for(unsigned int i=0; i < SubStatements.size(); i++){
        unsigned int d = SubStatements[i]->getDepth();
        if(d > res) res = d;
    }
    
    return res +1 ;
}

/*****************************
Utility operators/Methods
******************************/
bool CDNAStatement::operator==(const CDNAStatement& S) const{
        if((arity-S.arity)||((int)Type-(int)S.Type)) return false;            
        if (!arity) return true;
        
        for(unsigned int i=0; i<SubStatements.size();i++)
            if(!(*SubStatements[i] == *S.SubStatements[i]))
                return false;
        
        return true;
}

const CDNAStatement& CDNAStatement::operator[] (unsigned int i){

    if(i >= arity)
        throw CString(_T("out of bound at CDNAStatement::operator[]"));
    return *(SubStatements[i]);
}


 void  CDNAStatement::toTreeCtrl(CTreeCtrl* Tctrl, HTREEITEM branch){
                
	if(!branch) branch = Tctrl->InsertItem((LPCTSTR)" ");
	Tctrl->SetItemText(branch, CFunctionSet::TreeTag(Type));
         for(unsigned int i=0; i<SubStatements.size(); i++)
		if(SubStatements[i]){
			HTREEITEM Root =  Tctrl->InsertItem(CFunctionSet::TreeTag(Type), branch); 
			SubStatements[i]->toTreeCtrl(Tctrl, Root);
		}
}
CString CDNAStatement::toString(){

	CString Res;
	Res = _T("(");
			
	try{
		Res += CFunctionSet::TreeTag(Type);
	}
	catch(CString ErrStatement){
				Res += ErrStatement;	
	}

	for(COUNTER i=0; i<this->SubStatements.size(); i++)
		Res += this->SubStatements[i]->toString();
	
	Res += _T(")");
	return Res;
}

/*****************************
Random Creation Methos
******************************/

void CDNAStatement::simplify(){

	bool goAgain = false;
	for(COUNTER i=0; i<SubStatements.size(); i++)
		this->SubStatements[i]->simplify();

	switch(Type){
			case DIV:{
				if(SubStatements[1]->Type == N_1){
						CDNAStatement M(*SubStatements[0]);
						copy(M);
						goAgain = true;
				}
				else
					if(*SubStatements[0] == *SubStatements[1]){
						this->copy(CDNAStatement(N_1));
						goAgain = true;
					}
					else
						if(SubStatements[0]->Type == N_0){
							this->copy(CDNAStatement(N_0));
							goAgain = true;
						}
				break;
			 }
			case MULT:{
				if(SubStatements[0]->Type == N_1){
						CDNAStatement M(*SubStatements[1]);
						copy(M);
						goAgain = true;
				}
				else
					if(SubStatements[1]->Type == N_1){
						CDNAStatement M(*SubStatements[0]);
						copy(M);
						goAgain = true;
					}else
						if((SubStatements[0]->Type == N_0)||(SubStatements[1]->Type == N_0)){
							copy(CDNAStatement(N_0));
							goAgain = true;
						}
				break;	
			}
			case PLUS:{
				if(SubStatements[0]->Type == N_0){
					CDNAStatement M(*SubStatements[1]);
					copy(M);
					goAgain = true;
				}
				else if(SubStatements[1]->Type == N_0){
					CDNAStatement M(*SubStatements[0]);
					copy(M);
					goAgain = true;
				}

				break;
			}
			case MINUS:{	
				if(*SubStatements[0] == *SubStatements[1]){
						this->copy(CDNAStatement(N_0));
						goAgain = true;
				}
				else if(SubStatements[1]->Type == N_0){
						CDNAStatement M(*SubStatements[0]);
						copy(M);
						goAgain = true;
				}
				break;
			}
	}
	if(goAgain) simplify();
}



void CDNAStatement::grow(){
    try{
		for(unsigned int i = 0; i < SubStatements.size(); i++){
			SubStatements[i]->setMaxDepth(this->getMaxDepth()-1);
			SubStatements[i]->growCreate();       
			}
    }
    catch(CString Ecx){
        CString R(_T(" at CDNAStatement::grow-->\r\n"));
        R = Ecx + R;
        throw R;
    }
}

void CDNAStatement::growCreate(){
	//If at the end, we need a terminal.
    //If not at the end, we might get a Terminal 
    //or a function. So i flip a coin	
	
	double TempProb = this->getMutationProb();
	int TempMaxDepth = this->getMaxDepth();
	int  TempTreeDensity = this->getTreeDensity();
    
	
	if((this->Maxdepth + 1 <= 0)||((rand()%100 >= this->TreeDensity))){
		copy(CDNAStatement(CFunctionSet::getRandTerminal()));
		this->setMutationProb(TempProb);
		this->setMaxDepth(TempMaxDepth);
		this->setTreeDensity(TempTreeDensity);
		return;
	}


	this->copy(CDNAStatement (CFunctionSet::getRandFunction()));
	this->setMutationProb(TempProb);
	this->setMaxDepth(TempMaxDepth);
	this->setTreeDensity(TempTreeDensity);
	
	for(COUNTER i = 0; i<this->SubStatements.size(); i++){
		try{
			SubStatements[i]->setMaxDepth(this->getMaxDepth()-1);
			SubStatements[i]->setTreeDensity(this->getTreeDensity());
			SubStatements[i]->setMutationProb(this->getMutationProb());
			this->SubStatements[i]->growCreate();
		}
		catch(CString Err){
			CString R(_T(" at CDNAStatement::growCreate\r\n"));
			R = Err + R;
			throw R;
		}
	}
}

/*****************************
Evaluation Methods
******************************/
F<double> CDNAStatement::FromConst(GENEStatementType C){
    unsigned char Num = (CString(CFunctionSet::TreeTag(C)).Right(1))[0];
    if((Num>=48)&&(Num<=57))
        return (F<double>)(Num-48);
     
    throw (CString)(_T("Non Numerical constant at CDNAStatement::FromConst"));   
}



F<double> CDNAStatement::Eval(F<double> val){

	
	try{
		switch (Type){
	            
			case UNDEF:
				throw CString(_T("UNDEF"));
		                
			case PLUS:
				return  SubStatements[0]->Eval(val) + SubStatements[1]->Eval(val);
		                
			case MINUS:
				return  SubStatements[0]->Eval(val) - SubStatements[1]->Eval(val);
		                
			case DIV:{
					F<double> Den = SubStatements[1]->Eval(val); 
					if( Den == 0.0f) return 1.0f;
					return  SubStatements[0]->Eval(val) / Den;
				}
			case MULT:
				return  SubStatements[0]->Eval(val) * SubStatements[1]->Eval(val);
		               
			case N_M1: return -1.0f;
			case N_0: return 0.0f;
			case N_0P25: return 0.25f;
			case N_1: //return 1.0;
			case N_2: //return -1.0;
			case N_3:
			case N_5:
				return FromConst(Type);
		            
			case X_1:
				return val;
		}
		CString Xcept;
		Xcept.Format("Unknown statement [%d] at CDNAStatement::Eval", (COUNTER)Type);
		throw Xcept;
	}
	catch(CString Exc){
		if(Exc == CString(_T("UNDEF")))
			throw Exc;
		else{
			CString Mess;
			Mess.Format("Impossible to evaluate function at value [%f] CDNAStatement::Eval -->\r\n", val);
			Mess = Exc + Mess;    
			throw Mess;
		}
	} 
}
   
/*****************************
Breeding Methods
******************************/

CDNAStatement& CDNAStatement::operator*(CDNAStatement& Dad) {

        CDNAStatement* Kid = new CDNAStatement(*this);
        Kid->setMutationProb((Dad.getMutationProb()+this->getMutationProb())/2.0);
		Kid->setTreeDensity((this->TreeDensity+Dad.getTreeDensity())/2);
		Kid->setMaxDepth((this->getMaxDepth() + Dad.getMaxDepth())/2);

        unsigned int DadDepth = Dad.getDepth();
        unsigned int MomDepth = getDepth();
        int CrossDepthKid = (rand()%this->CrossOverMaxDepth)%
                            ((MomDepth < DadDepth)? MomDepth:DadDepth);
        CrossDepthKid++;
        if(CrossDepthKid < 2) CrossDepthKid = 2;
        
	
	CDNAStatement* DadPart = Dad.getBranchRandomType(CrossOverMaxDepth, FUNC);
        if(!DadPart)
            DadPart = Dad.getBranchRandomType(CrossOverMaxDepth, TERMINAL);
	
        if(DadPart){
            //figure out how deep we can cut
            int KidCrossDepth = Kid->getDepth() - DadPart->getDepth();

            if(KidCrossDepth > 1){
            //find a suitable cross in that range
                CDNAStatement* KidCross = 
                    Kid->getBranchRandomType(KidCrossDepth, 
                        CFunctionSet::GetTypeClass(DadPart->Type));
                    if(KidCross)
                        *KidCross = *DadPart;
            }
        }
        
	
	Kid->mutate();
	Kid->Fitness = 0.0f;
        return *Kid;

 }
void CDNAStatement::mutate(){
    
	int MutProb = (int)(MutationPbblty *1000.0f);
	
	while(rand()%1000 < MutProb){
		this->setMutationProb(this->getMutationProb() + ((rand()%10)?0:0.005));
		this->setTreeDensity(this->getTreeDensity()+((rand()%10)?0:1));
		this->setMaxDepth(this->getMaxDepth()+((rand()%10)?0:1));
		int MutDepthKid = (rand()%getDepth())+1;
		CDNAStatement* MutPart = NULL;
		switch(rand()%2){
	
			case 0:
				MutPart = this->getBranchRandomType(MutDepthKid, TERMINAL);
				break;
		
			case 1:
				MutPart = this->getBranchRandomType(MutDepthKid, FUNC);
				break;

		}

		if(MutPart){
			CDNAStatement T;
				switch(CFunctionSet::GetTypeClass(MutPart->Type)){
        
					case(TERMINAL):
						T = CDNAStatement(CFunctionSet::getRandTerminal());
						break;


					case (FUNC):
						T = CDNAStatement(CFunctionSet::getRandFunction());
						switch(rand()%2){
							case 0:
								T.setMaxDepth(MutPart->getDepth());
								T.grow();		
								break;
							case 1:{
								int index = rand()%T.SubStatements.size();
								T.SubStatements[index]->copy(*MutPart);
								for(COUNTER i=0; i<T.SubStatements.size();i++)
									if(i != index){
										T.SubStatements[i]->setMaxDepth(MutPart->getDepth());
										T.SubStatements[i]->growCreate();	
										}
								break;
							       }
						}		
						break;
				}
			*MutPart = T;
			MutProb /= 2;
		}
	}

	if(rand()%3 == 0) simplify();
}
 
 CDNAStatement* CDNAStatement::getBranchRandomType(COUNTER MaxDepth, FUNCTIONTYPECLASS BranchType){

        vector<CDNAStatement*> candidates;
        FUNCTIONTYPECLASS m_ClassType = CFunctionSet::GetTypeClass(this->Type);

        if  ((this->getDepth() <= MaxDepth) &&
            ( (m_ClassType == BranchType ))) 
                candidates.push_back(this);
        
                
        
        for(COUNTER i=0; i<this->SubStatements.size(); i++){
            CDNAStatement* candidate = SubStatements[i]->getBranchRandomType(MaxDepth, BranchType);
            if(candidate) candidates.push_back(candidate);
        }

        if(candidates.size())
            return candidates[rand()%candidates.size()];
        else 
            return ((CDNAStatement*)0);
}



void CDNAStatement::draw(COUNTER FitCaseNum, double RangeMin, double RangeMax){

		double IntervalSize = (RangeMax-RangeMin)/(double)FitCaseNum;
		try{

			double x1 = RangeMin;
			double y1 = Eval((F<double>)RangeMin).x();

			double x2;
			double y2;

			for(COUNTER i=1; i<FitCaseNum-1; i++){
			
				x2 = RangeMin + (i*IntervalSize);	//Pick a point 
				int somewhere = rand()%100;		//in the interval,
				if (somewhere) x2+= (1.0f/double(somewhere))*IntervalSize;	//somewhere in the interval.
				y2 = Eval((F<double>)x2).x();

				glBegin(GL_LINES);
			
					glVertex3f((GLfloat) x1, (GLfloat) y1, 0.0f);
					glVertex3f((GLfloat) x2, (GLfloat) y2, 0.0f);

				glEnd();

				x1 = x2;
				y1 = y2;
			}

			glBegin(GL_LINES);
				glVertex3f((GLfloat) x1, (GLfloat) y1, 0.0f);
				glVertex3f((GLfloat) RangeMax, (GLfloat) Eval((F<double>)RangeMax).x(), 0.0f);
			glEnd();
		}
		catch(CString Err){
			
				throw Err;
		}
}










