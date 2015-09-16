#ifndef MUTATING_GUILE_IFACE_H_GUARD
#define MUTATING_GUILE_IFACE_H_GUARD 1

#if (GUILE_VERSION >= 200000) && (GUILE_VERSION <= 200003)
# error AutoGen does not work with this version of Guile
  choke me.
#endif

#if   GUILE_VERSION < 107000
# define AG_SCM_BOOL_P(_b)            SCM_BOOLP(_b)
# define AG_SCM_CHAR(_c)              gh_scm2char(_c)
# define AG_SCM_CHARS(_s)             SCM_CHARS(_s)
# define AG_SCM_FALSEP(_r)            SCM_FALSEP(_r)
# define AG_SCM_FROM_LONG(_l)         gh_long2scm(_l)
# define AG_SCM_INT2SCM(_i)           gh_int2scm(_i)
# define AG_SCM_IS_PROC(_p)           SCM_NFALSEP( scm_procedure_p(_p))
# define AG_SCM_LIST_P(_l)            SCM_NFALSEP( scm_list_p(_l))
# define AG_SCM_LISTOFNULL()          scm_listofnull
# define AG_SCM_LONG2SCM(_i)          gh_long2scm(_i)
# define AG_SCM_NFALSEP(_r)           SCM_NFALSEP(_r)
# define AG_SCM_NULLP(_m)             SCM_NULLP(_m)
# define AG_SCM_NUM_P(_n)             SCM_NUMBERP(_n)
# define AG_SCM_PAIR_P(_p)            SCM_NFALSEP( scm_pair_p(_p))
# define AG_SCM_STR02SCM(_s)          scm_makfrom0str(_s)
# define AG_SCM_STR2SCM(_st,_sz)      scm_mem2string(_st,_sz)
# define AG_SCM_STRING_P(_s)          SCM_STRINGP(_s)
# define AG_SCM_STRLEN(_s)            SCM_STRING_LENGTH(_s)
# define AG_SCM_SYM_P(_s)             SCM_SYMBOLP(_s)
# define AG_SCM_TO_INT(_i)            gh_scm2int(_i)
# define AG_SCM_TO_LONG(_v)           gh_scm2long(_v)
# define AG_SCM_TO_NEWSTR(_s)         gh_scm2newstr(_s, NULL)
# define AG_SCM_TO_ULONG(_v)          gh_scm2ulong(_v)
# define AG_SCM_VEC_P(_v)             SCM_VECTORP(_v)

#elif GUILE_VERSION < 200000
# define AG_SCM_BOOL_P(_b)            scm_is_bool(_b)
# define AG_SCM_CHAR(_c)              SCM_CHAR(_c)
# define AG_SCM_CHARS(_s)             scm_i_string_chars(_s)
# define AG_SCM_FALSEP(_r)            scm_is_false(_r)
# define AG_SCM_FROM_LONG(_l)         scm_from_long(_l)
# define AG_SCM_INT2SCM(_i)           scm_from_int(_i)
# define AG_SCM_IS_PROC(_p)           scm_is_true( scm_procedure_p(_p))
# define AG_SCM_LIST_P(_l)            scm_is_true( scm_list_p(_l))
# define AG_SCM_LISTOFNULL()          scm_list_1(SCM_EOL)
# define AG_SCM_LONG2SCM(_i)          scm_from_long(_i)
# define AG_SCM_NFALSEP(_r)           scm_is_true(_r)
# define AG_SCM_NULLP(_m)             scm_is_null(_m)
# define AG_SCM_NUM_P(_n)             scm_is_number(_n)
# define AG_SCM_PAIR_P(_p)            scm_is_true( scm_pair_p(_p))
# define AG_SCM_STR02SCM(_s)          scm_from_locale_string(_s)
# define AG_SCM_STR2SCM(_st,_sz)      scm_from_locale_stringn(_st,_sz)
# define AG_SCM_STRING_P(_s)          scm_is_string(_s)
# define AG_SCM_STRLEN(_s)            scm_c_string_length(_s)
# define AG_SCM_SYM_P(_s)             scm_is_symbol(_s)
# define AG_SCM_TO_INT(_i)            scm_to_int(_i)
# define AG_SCM_TO_LONG(_v)           scm_to_long(_v)
# define AG_SCM_TO_NEWSTR(_s)         scm_to_locale_string(_s)
# define AG_SCM_TO_ULONG(_v)          scm_to_ulong(_v)
# define AG_SCM_VEC_P(_v)             scm_is_vector(_v)

#elif GUILE_VERSION < 201000
# define AG_SCM_BOOL_P(_b)            scm_is_bool(_b)
# define AG_SCM_CHAR(_c)              SCM_CHAR(_c)
# define AG_SCM_CHARS(_s)             scm_i_string_chars(_s)
# define AG_SCM_FALSEP(_r)            scm_is_false(_r)
# define AG_SCM_FROM_LONG(_l)         scm_from_long(_l)
# define AG_SCM_INT2SCM(_i)           scm_from_int(_i)
# define AG_SCM_IS_PROC(_p)           scm_is_true( scm_procedure_p(_p))
# define AG_SCM_LIST_P(_l)            scm_is_true( scm_list_p(_l))
# define AG_SCM_LISTOFNULL()          scm_list_1(SCM_EOL)
# define AG_SCM_LONG2SCM(_i)          scm_from_long(_i)
# define AG_SCM_NFALSEP(_r)           scm_is_true(_r)
# define AG_SCM_NULLP(_m)             scm_is_null(_m)
# define AG_SCM_NUM_P(_n)             scm_is_number(_n)
# define AG_SCM_PAIR_P(_p)            scm_is_true( scm_pair_p(_p))
# define AG_SCM_STR02SCM(_s)          scm_from_utf8_string(_s)
# define AG_SCM_STR2SCM(_st,_sz)      scm_from_utf8_stringn(_st,_sz)
# define AG_SCM_STRING_P(_s)          scm_is_string(_s)
# define AG_SCM_STRLEN(_s)            scm_c_string_length(_s)
# define AG_SCM_SYM_P(_s)             scm_is_symbol(_s)
# define AG_SCM_TO_INT(_i)            scm_to_int(_i)
# define AG_SCM_TO_LONG(_v)           scm_to_long(_v)
# define AG_SCM_TO_NEWSTR(_s)         scm_to_locale_string(_s)
# define AG_SCM_TO_ULONG(_v)          scm_to_ulong(_v)
# define AG_SCM_VEC_P(_v)             scm_is_vector(_v)

#else
# error unknown GUILE_VERSION
  choke me.
#endif

#endif /* MUTATING_GUILE_IFACE_H_GUARD */
