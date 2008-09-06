#include "StdAfx.h"
#include ".\functionset.h"


LPCTSTR CFunctionSet::TreeTag(GENEStatementType S){

	TOSTRING(S, UNDEF);

	if(S == N_0P25) return "0.25";
	if(S == N_M1) return "-1";
	TOSTRING(S, X_1);
	TOSTRING(S, N_0);
	if(S == N_1) return "1";
	if(S == N_2) return "2";
	TOSTRING(S, N_3);
	TOSTRING(S, N_5);
	    
	TOSTRING(S, PLUS);
	TOSTRING(S, MINUS);
	TOSTRING(S, DIV);
	TOSTRING(S, MULT);
	    
	TOSTRING(S, INTERVAL);
	CString RMsg;
	RMsg.Format("Unknown Statement Formation [%d] at CFunctionSet::TreeTag", (COUNTER) S);	
	throw RMsg;
}


unsigned int CFunctionSet::Arity(GENEStatementType S){

    switch(S){
	case UNDEF:
	case X_1:
	case N_M1:
	case N_0:
	case N_0P25:
	case N_1:
	case N_2:
	case N_3:
	case N_5:
	case INTERVAL:
            return 0;
            
        case  PLUS:    
       case  MINUS:    
        case  DIV:    
        case  MULT:    

            return 2;
            
	

   }         
   throw CString(_T("Unknown instruction ["))+CString(TreeTag(S))+CString(_T("] in CFunctionSet::Arity")) ;
}

CFunctionSet::CFunctionSet(void)
{
}

CFunctionSet::~CFunctionSet(void)
{
}
