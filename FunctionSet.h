#pragma once



enum GENEStatementType{ 
    
    UNDEF = 0,

    //Terminals
    X_1,
    N_1,
    N_2,
    N_3,
    N_5,
   
    //Functions
    PLUS,
    MINUS,
    DIV,
    MULT,
};


#define BEGTERM (unsigned int) X_1
#define ENDTERM (unsigned int) N_5

#define BEGFUNC (unsigned int) PLUS
#define ENDFUNC (unsigned int) MULT

#define FUNCTIONTYPECLASS unsigned char
#define ANYTYPE	(FUNCTIONTYPECLASS)0
#define TERMINAL	(FUNCTIONTYPECLASS)1
#define BOOLFUNC	(FUNCTIONTYPECLASS)2
#define FUNC		(FUNCTIONTYPECLASS)3
#define NOTYPE		(FUNCTIONTYPECLASS)4


#define TOSTRING(var, statement) \
    if(var == statement) return _T(#statement)

#define FROMSTRING(str, statement) \
    if (str == CString(_T(#statement)))\
                return statement



class CFunctionSet{                

    public:


        static bool isFunction(GENEStatementType T){
                    return (((unsigned int)T>=BEGFUNC)
                            &&((unsigned int)T<=ENDFUNC));              
              };
        static bool isTerminal(GENEStatementType T){
                    return (((unsigned int)T>=BEGTERM)
                    &&((unsigned int)T<=ENDTERM));              
              };
         
        static GENEStatementType getRandTerminal(){
                    unsigned int FuncNums = (ENDTERM + 1) - BEGTERM;
                    return (GENEStatementType)((rand()%FuncNums)+ BEGTERM);
              };
              
        static GENEStatementType getRandFunction(){
                    unsigned int FuncNums = (ENDFUNC + 1) - BEGFUNC;
                    return (GENEStatementType)((rand()%FuncNums)+ BEGFUNC);    
              };

        static bool isSameFunctionTypeClass(GENEStatementType T1, GENEStatementType T2){
                
                if (((isFunction(T1))&&(isFunction(T2)))||
                        ((isTerminal(T1))&&(isTerminal(T2)))) return true;
                return ((T1==UNDEF)&&(T2==UNDEF));
        }

        static FUNCTIONTYPECLASS GetTypeClass(GENEStatementType T2){
		 if(isTerminal(T2))      return TERMINAL;
		 if(isFunction(T2))      return FUNC;
		return NOTYPE;
        }

    static LPCTSTR TreeTag(GENEStatementType S);       
    static GENEStatementType fromString (const CString& Str);
    static unsigned int Arity(GENEStatementType S);


public:
    CFunctionSet(void);
    ~CFunctionSet(void);
};
