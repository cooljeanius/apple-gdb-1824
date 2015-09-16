/*
 * MW_TclAppleScriptHeader.h
 */

#if __POWERPC__
# include "MW_TclAppleScriptHeaderPPC"
#elif __CFM68K__
# include "MW_TclAppleScriptHeaderCFM68K"
#else
# include "MW_TclAppleScriptHeader68K"
#endif /* __POWERPC__ || __CFM68K__ */

/* EOF */
