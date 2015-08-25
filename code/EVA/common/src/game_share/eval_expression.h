#ifndef GAMESHARE_EVAL_EXPRESSION_H
#define GAMESHARE_EVAL_EXPRESSION_H

#include <game_share/game_def.h>
#include <nel/misc/eval_num_expr.h>
#include <nel/misc/sstring.h>

class CEvalExpr
{
public:

    double  Eval( NLMISC::CSString& expression );
    double  Eval( NLMISC::CSString& expression, double param );
    double  Eval( NLMISC::CSString& expression, double param0, double param1 );
    double  Eval( NLMISC::CSString& expression, double param0, double param1 , double param2 );
    double  Eval( NLMISC::CSString& expression, double param0, double param1 , double param2 , double param3 , double param4 , double param5 , double param6 , double param7 , double param8 );

    static CEvalExpr &getInstance()
    {
        if(!Instance)
        {
            Instance = new CEvalExpr();
        }
        return *Instance;
    }
private:

    enum
    {
        ParamMaxLength = 256,
    };

    char                        m_StrParam[ParamMaxLength];
    NLMISC::CSString            m_Expr;
    NLMISC::CEvalNumExpr        m_EvalExpr;

    static CEvalExpr *Instance;
};

#define EvalExpr CEvalExpr::getInstance()

#endif
