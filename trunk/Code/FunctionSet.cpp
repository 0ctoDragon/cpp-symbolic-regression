#include "StdAfx.h"
#include ".\functionset.h"


LPCTSTR CFunctionSet::TreeTag(GENEStatementType S){

	TOSTRING(S, UNDEF);

	TOSTRING(S, X_1);
	TOSTRING(S, N_1);
	TOSTRING(S, N_2);
	TOSTRING(S, N_3);
	TOSTRING(S, N_5);
	    
	TOSTRING(S, PLUS);
	TOSTRING(S, MINUS);
	TOSTRING(S, DIV);
	TOSTRING(S, MULT);
	    
	CString RMsg;
	RMsg.Format("Unknown Statement Formation [%d] at CFunctionSet::TreeTag", (COUNTER) S);	
	throw RMsg;
}

GENEStatementType CFunctionSet::fromString (const CString& Str){

	FROMSTRING(Str,   UNDEF);
	
	FROMSTRING(Str,   PLUS);             
	FROMSTRING(Str,   MINUS);            
	FROMSTRING(Str,   DIV);              
	FROMSTRING(Str,   MULT);             
		
	throw CString(_T("Unknown String Formation [+"))+ Str + CString(_T("] at CFunctionSet::fromString"));
}


unsigned int CFunctionSet::Arity(GENEStatementType S){

    switch(S){
	case UNDEF:
	case X_1:
	case N_1:
	case N_2:
	case N_3:
	case N_5:
   
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
