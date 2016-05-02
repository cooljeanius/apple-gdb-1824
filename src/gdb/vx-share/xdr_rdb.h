/* vx-share/xdr_rdb.h - xdr for additional rdb structures */

/* Copyright 1992 Free Software Foundation, Inc.

   This code was donated by Wind River Systems, Inc. */

/*
modification history
--------------------
01c,25may91,maf  rolled RDBVERS for VxGDB 1.0 version.
01b,12apr91,maf  rolled RDBVERS.
01a,09jan91,maf  created using v1a of xdr_rdb.h for 4.0.2 VxWorks 68k.
		 added struct SOURCE_STEP.
*/

#ifndef INCxdrrdbh
#define INCxdrrdbh

enum arg_type {
        T_UNKNOWN = 0,
        T_BYTE = 1,
        T_WORD = 2,
        T_INT = 3,
        T_FLOAT = 4,
        T_DOUBLE = 5
};
typedef enum arg_type arg_type;
bool_t xdr_arg_type(XDR *xdrs, arg_type *objp);


struct arg_value {
        arg_type type;
        union {
                char v_byte;
                short v_word;
                int v_int;
                float v_fp;
                double v_dp;
        } arg_value_u;
};
typedef struct arg_value arg_value;
bool_t xdr_arg_value(XDR *xdrs, arg_value *objp);

struct func_call {
        int func_addr;
        struct {
                u_int args_len;
                arg_value *args_val;
        } args;
};
typedef struct func_call func_call;
bool_t xdr_func_call(XDR *xdrs, func_call *objp);


typedef char *arg_one;
bool_t xdr_arg_one(XDR *xdrs, arg_one *objp);


typedef struct {
        u_int arg_array_len;
        arg_one *arg_array_val;
} arg_array;
bool_t xdr_arg_array(XDR *xdrs, arg_array *objp);


/*
 * Structures used to pass structures required for
 * process control but not part of the standard ptrace interface
 */

/*
 * arg_info is used to pass arguments into process start
 */
struct arg_info {
	int rargc;
	char **rargv;
};
typedef struct arg_info Arg_info;


enum EVENT_TYPE {
        EVENT_BREAK = 0,
        EVENT_STOP = 1,
        EVENT_EXIT = 2,
        EVENT_BUS_ERR = 3,
        EVENT_SUSPEND = 4,
        EVENT_ZERO_DIV = 5,
        EVENT_SIGNAL = 6,
        EVENT_START = 7
};
typedef enum EVENT_TYPE EVENT_TYPE;


struct RDB_EVENT {
	int status;
	int taskId;
        EVENT_TYPE eventType;
        int sigType;
};
typedef struct RDB_EVENT RDB_EVENT;


struct TASK_START {
        int status;
        int pid;
};
typedef struct TASK_START TASK_START;


struct SYMBOL_ADDR {
        int status;
        u_int addr;
};
typedef struct SYMBOL_ADDR SYMBOL_ADDR;

struct SOURCE_STEP {
	int taskId;
	u_int startAddr;
	u_int endAddr;
};
typedef struct SOURCE_STEP SOURCE_STEP;

#define MAX_ARG_CNT 10
#define MAX_FUNC_ARGS 100
#define MAX_ARG_LEN   100


bool_t xdr_arg_info(void);
bool_t xdr_EVENT_TYPE(XDR *xdrs, EVENT_TYPE *objp);
bool_t xdr_RDB_EVENT(XDR *xdrs, RDB_EVENT *objp);
bool_t xdr_TASK_START(XDR *xdrs, TASK_START *objp);
bool_t xdr_SYMBOL_ADDR(XDR *xdrs, SYMBOL_ADDR *objp);
bool_t xdr_SOURCE_STEP(XDR *xdrs, SOURCE_STEP *objp);

#define RDBPROG (u_long)0x44444444
#define RDBVERS (u_long)3UL
#endif	/* INCxdrrdbh */

/* EOF */
