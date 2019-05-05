/* A Bison parser, made by GNU Bison 2.5.  */

/* Bison implementation for Yacc-like parsers in C
   
      Copyright (C) 1984, 1989-1990, 2000-2011 Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.5"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Copy the first part of user declarations.  */

/* Line 268 of yacc.c  */
#line 31 "cfparse.y"

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/queue.h>
#include <sys/time.h>

#include <netinet/in.h>

#include <arpa/inet.h>

#include <stdlib.h>
#include <string.h>

#include "dhcp6.h"
#include "config.h"
#include "common.h"

extern int lineno;
extern int cfdebug;

extern void yywarn __P((char *, ...))
	__attribute__((__format__(__printf__, 1, 2)));
extern void yyerror __P((char *, ...))
	__attribute__((__format__(__printf__, 1, 2)));

#define MAKE_NAMELIST(l, n, p) do { \
	(l) = (struct cf_namelist *)malloc(sizeof(*(l))); \
	if ((l) == NULL) { \
		yywarn("can't allocate memory"); \
		if (p) cleanup_cflist(p); \
		return (-1); \
	} \
	memset((l), 0, sizeof(*(l))); \
	l->line = lineno; \
	l->name = (n); \
	l->params = (p); \
	} while (0)

#define MAKE_CFLIST(l, t, pp, pl) do { \
	(l) = (struct cf_list *)malloc(sizeof(*(l))); \
	if ((l) == NULL) { \
		yywarn("can't allocate memory"); \
		if (pp) free(pp); \
		if (pl) cleanup_cflist(pl); \
		return (-1); \
	} \
	memset((l), 0, sizeof(*(l))); \
	l->line = lineno; \
	l->type = (t); \
	l->ptr = (pp); \
	l->list = (pl); \
	l->tail = (l); \
	} while (0)

static struct cf_namelist *iflist_head, *hostlist_head, *iapdlist_head;
static struct cf_namelist *profilelist_head;
static struct cf_namelist *addrpoollist_head;
static struct cf_namelist *authinfolist_head, *keylist_head;
static struct cf_namelist *ianalist_head;
struct cf_list *cf_dns_list, *cf_dns_name_list, *cf_ntp_list;
struct cf_list *cf_sip_list, *cf_sip_name_list;
struct cf_list *cf_nis_list, *cf_nis_name_list;
struct cf_list *cf_nisp_list, *cf_nisp_name_list;
struct cf_list *cf_bcmcs_list, *cf_bcmcs_name_list;
long long cf_refreshtime = -1;

extern int yylex __P((void));
extern int cfswitch_buffer __P((char *));
static int add_namelist __P((struct cf_namelist *, struct cf_namelist **));
static void cleanup __P((void));
static void cleanup_namelist __P((struct cf_namelist *));
static void cleanup_cflist __P((struct cf_list *));


/* Line 268 of yacc.c  */
#line 146 "y.tab.c"

/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     INTERFACE = 258,
     IFNAME = 259,
     PROFILE = 260,
     PROFILENAME = 261,
     PREFIX_INTERFACE = 262,
     SLA_ID = 263,
     SLA_LEN = 264,
     IFID = 265,
     DUID_ID = 266,
     ID_ASSOC = 267,
     IA_PD = 268,
     IAID = 269,
     IA_NA = 270,
     T1 = 271,
     T2 = 272,
     SUGGEST_T = 273,
     ADDRESS = 274,
     REQUEST = 275,
     SEND = 276,
     ALLOW = 277,
     PREFERENCE = 278,
     HOST = 279,
     HOSTNAME = 280,
     DUID = 281,
     OPTION = 282,
     RAPID_COMMIT = 283,
     RECONFIGURE_ACCEPT = 284,
     DNS_SERVERS = 285,
     DNS_NAME = 286,
     NTP_SERVERS = 287,
     REFRESHTIME = 288,
     SIP_SERVERS = 289,
     SIP_NAME = 290,
     NIS_SERVERS = 291,
     NIS_NAME = 292,
     NISP_SERVERS = 293,
     NISP_NAME = 294,
     BCMCS_SERVERS = 295,
     BCMCS_NAME = 296,
     AFTR_SERVERS = 297,
     INFO_ONLY = 298,
     SCRIPT = 299,
     DELAYEDKEY = 300,
     AUTHENTICATION = 301,
     PROTOCOL = 302,
     ALGORITHM = 303,
     DELAYED = 304,
     RECONFIG = 305,
     HMACMD5 = 306,
     MONOCOUNTER = 307,
     AUTHNAME = 308,
     RDM = 309,
     KEY = 310,
     KEYINFO = 311,
     REALM = 312,
     KEYID = 313,
     SECRET = 314,
     KEYNAME = 315,
     EXPIRE = 316,
     ADDRPOOL = 317,
     POOLNAME = 318,
     RANGE = 319,
     TO = 320,
     ADDRESS_POOL = 321,
     INCLUDE = 322,
     NUMBER = 323,
     SLASH = 324,
     EOS = 325,
     BCL = 326,
     ECL = 327,
     STRING = 328,
     QSTRING = 329,
     PREFIX = 330,
     INFINITY = 331,
     COMMA = 332
   };
#endif
/* Tokens.  */
#define INTERFACE 258
#define IFNAME 259
#define PROFILE 260
#define PROFILENAME 261
#define PREFIX_INTERFACE 262
#define SLA_ID 263
#define SLA_LEN 264
#define IFID 265
#define DUID_ID 266
#define ID_ASSOC 267
#define IA_PD 268
#define IAID 269
#define IA_NA 270
#define T1 271
#define T2 272
#define SUGGEST_T 273
#define ADDRESS 274
#define REQUEST 275
#define SEND 276
#define ALLOW 277
#define PREFERENCE 278
#define HOST 279
#define HOSTNAME 280
#define DUID 281
#define OPTION 282
#define RAPID_COMMIT 283
#define RECONFIGURE_ACCEPT 284
#define DNS_SERVERS 285
#define DNS_NAME 286
#define NTP_SERVERS 287
#define REFRESHTIME 288
#define SIP_SERVERS 289
#define SIP_NAME 290
#define NIS_SERVERS 291
#define NIS_NAME 292
#define NISP_SERVERS 293
#define NISP_NAME 294
#define BCMCS_SERVERS 295
#define BCMCS_NAME 296
#define AFTR_SERVERS 297
#define INFO_ONLY 298
#define SCRIPT 299
#define DELAYEDKEY 300
#define AUTHENTICATION 301
#define PROTOCOL 302
#define ALGORITHM 303
#define DELAYED 304
#define RECONFIG 305
#define HMACMD5 306
#define MONOCOUNTER 307
#define AUTHNAME 308
#define RDM 309
#define KEY 310
#define KEYINFO 311
#define REALM 312
#define KEYID 313
#define SECRET 314
#define KEYNAME 315
#define EXPIRE 316
#define ADDRPOOL 317
#define POOLNAME 318
#define RANGE 319
#define TO 320
#define ADDRESS_POOL 321
#define INCLUDE 322
#define NUMBER 323
#define SLASH 324
#define EOS 325
#define BCL 326
#define ECL 327
#define STRING 328
#define QSTRING 329
#define PREFIX 330
#define INFINITY 331
#define COMMA 332




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 293 of yacc.c  */
#line 130 "cfparse.y"

	long long num;
	char* str;
	struct cf_list *list;
	struct dhcp6_prefix *prefix;
	struct dhcp6_range *range;
	struct dhcp6_poolspec *pool;



/* Line 293 of yacc.c  */
#line 347 "y.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 343 of yacc.c  */
#line 359 "y.tab.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  2
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   265

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  78
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  40
/* YYNRULES -- Number of rules.  */
#define YYNRULES  117
/* YYNRULES -- Number of states.  */
#define YYNSTATES  257

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   332

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     4,     7,     9,    11,    13,    15,    17,
      19,    21,    23,    25,    32,    39,    46,    51,    56,    61,
      66,    71,    76,    81,    86,    91,    96,   101,   106,   114,
     121,   129,   136,   143,   150,   154,   161,   162,   165,   167,
     168,   171,   175,   179,   182,   186,   190,   194,   198,   202,
     206,   210,   214,   218,   220,   224,   226,   229,   231,   234,
     237,   239,   241,   243,   245,   247,   249,   251,   253,   255,
     257,   259,   261,   263,   267,   270,   274,   279,   285,   288,
     292,   294,   296,   297,   300,   302,   304,   308,   314,   315,
     318,   322,   326,   333,   334,   337,   341,   345,   349,   350,
     353,   355,   359,   360,   363,   367,   371,   375,   379,   381,
     383,   385,   387,   388,   391,   395,   399,   403
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      79,     0,    -1,    -1,    79,    80,    -1,    81,    -1,    82,
      -1,    83,    -1,    84,    -1,    85,    -1,    86,    -1,    87,
      -1,    89,    -1,    88,    -1,     3,     4,    71,    92,    72,
      70,    -1,     5,     6,    71,    92,    72,    70,    -1,    24,
      25,    71,    92,    72,    70,    -1,    27,    30,    90,    70,
      -1,    27,    31,    74,    70,    -1,    27,    32,    90,    70,
      -1,    27,    34,    90,    70,    -1,    27,    35,    74,    70,
      -1,    27,    36,    90,    70,    -1,    27,    37,    74,    70,
      -1,    27,    38,    90,    70,    -1,    27,    39,    74,    70,
      -1,    27,    40,    90,    70,    -1,    27,    41,    74,    70,
      -1,    27,    33,    68,    70,    -1,    12,    13,    14,    71,
     101,    72,    70,    -1,    12,    13,    71,   101,    72,    70,
      -1,    12,    15,    14,    71,   109,    72,    70,    -1,    12,
      15,    71,   109,    72,    70,    -1,    46,    53,    71,   111,
      72,    70,    -1,    56,    60,    71,   116,    72,    70,    -1,
      67,    74,    70,    -1,    62,    63,    71,    92,    72,    70,
      -1,    -1,    90,    91,    -1,    73,    -1,    -1,    92,    93,
      -1,    21,    94,    70,    -1,    20,    94,    70,    -1,    43,
      70,    -1,    22,    95,    70,    -1,    26,    11,    70,    -1,
      19,    97,    70,    -1,    75,    98,    70,    -1,    23,    68,
      70,    -1,    44,    74,    70,    -1,    45,    73,    70,    -1,
      64,    96,    70,    -1,    66,    99,    70,    -1,    95,    -1,
      95,    77,    94,    -1,    28,    -1,    46,    53,    -1,    29,
      -1,    13,    68,    -1,    15,    68,    -1,    34,    -1,    35,
      -1,    30,    -1,    31,    -1,    32,    -1,    33,    -1,    36,
      -1,    37,    -1,    38,    -1,    39,    -1,    40,    -1,    41,
      -1,    42,    -1,    73,    65,    73,    -1,    73,   100,    -1,
      73,   100,   100,    -1,    73,    69,    68,   100,    -1,    73,
      69,    68,   100,   100,    -1,    73,   100,    -1,    73,   100,
     100,    -1,    76,    -1,    68,    -1,    -1,   101,   102,    -1,
     103,    -1,   106,    -1,    75,    98,    70,    -1,    18,    71,
     104,    72,    70,    -1,    -1,   104,   105,    -1,    16,    68,
      70,    -1,    17,    68,    70,    -1,     7,     4,    71,   107,
      72,    70,    -1,    -1,   107,   108,    -1,     8,    68,    70,
      -1,     9,    68,    70,    -1,    10,    68,    70,    -1,    -1,
     109,   110,    -1,   103,    -1,    19,    97,    70,    -1,    -1,
     111,   112,    -1,    47,   113,    70,    -1,    48,   114,    70,
      -1,    54,   115,    70,    -1,    55,    73,    70,    -1,    49,
      -1,    50,    -1,    51,    -1,    52,    -1,    -1,   116,   117,
      -1,    57,    74,    70,    -1,    58,    68,    70,    -1,    59,
      74,    70,    -1,    61,    74,    70,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   154,   154,   156,   160,   161,   162,   163,   164,   165,
     166,   167,   168,   172,   184,   196,   208,   217,   232,   241,
     250,   265,   274,   289,   298,   313,   322,   337,   366,   375,
     389,   398,   415,   427,   439,   450,   462,   463,   481,   504,
     505,   523,   531,   539,   547,   555,   563,   571,   579,   588,
     596,   604,   612,   623,   627,   637,   645,   653,   660,   668,
     676,   684,   692,   700,   708,   716,   724,   732,   740,   748,
     756,   764,   772,   783,   814,   841,   874,   901,   934,   959,
     990,   994,  1001,  1002,  1020,  1021,  1022,  1033,  1043,  1044,
    1062,  1070,  1081,  1091,  1092,  1110,  1118,  1126,  1137,  1138,
    1156,  1157,  1168,  1169,  1187,  1195,  1203,  1211,  1222,  1223,
    1227,  1231,  1235,  1236,  1254,  1262,  1270,  1278
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "INTERFACE", "IFNAME", "PROFILE",
  "PROFILENAME", "PREFIX_INTERFACE", "SLA_ID", "SLA_LEN", "IFID",
  "DUID_ID", "ID_ASSOC", "IA_PD", "IAID", "IA_NA", "T1", "T2", "SUGGEST_T",
  "ADDRESS", "REQUEST", "SEND", "ALLOW", "PREFERENCE", "HOST", "HOSTNAME",
  "DUID", "OPTION", "RAPID_COMMIT", "RECONFIGURE_ACCEPT", "DNS_SERVERS",
  "DNS_NAME", "NTP_SERVERS", "REFRESHTIME", "SIP_SERVERS", "SIP_NAME",
  "NIS_SERVERS", "NIS_NAME", "NISP_SERVERS", "NISP_NAME", "BCMCS_SERVERS",
  "BCMCS_NAME", "AFTR_SERVERS", "INFO_ONLY", "SCRIPT", "DELAYEDKEY",
  "AUTHENTICATION", "PROTOCOL", "ALGORITHM", "DELAYED", "RECONFIG",
  "HMACMD5", "MONOCOUNTER", "AUTHNAME", "RDM", "KEY", "KEYINFO", "REALM",
  "KEYID", "SECRET", "KEYNAME", "EXPIRE", "ADDRPOOL", "POOLNAME", "RANGE",
  "TO", "ADDRESS_POOL", "INCLUDE", "NUMBER", "SLASH", "EOS", "BCL", "ECL",
  "STRING", "QSTRING", "PREFIX", "INFINITY", "COMMA", "$accept",
  "statements", "statement", "interface_statement", "profile_statement",
  "host_statement", "option_statement", "ia_statement",
  "authentication_statement", "key_statement", "include_statement",
  "addrpool_statement", "address_list", "address_list_ent", "declarations",
  "declaration", "dhcpoption_list", "dhcpoption", "rangeparam",
  "addressparam", "prefixparam", "poolparam", "duration", "iapdconf_list",
  "iapdconf", "suggest_t", "iats", "iat", "prefix_interface", "ifparams",
  "ifparam", "ianaconf_list", "ianaconf", "authparam_list", "authparam",
  "authproto", "authalg", "authrdm", "keyparam_list", "keyparam", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    78,    79,    79,    80,    80,    80,    80,    80,    80,
      80,    80,    80,    81,    82,    83,    84,    84,    84,    84,
      84,    84,    84,    84,    84,    84,    84,    84,    85,    85,
      85,    85,    86,    87,    88,    89,    90,    90,    91,    92,
      92,    93,    93,    93,    93,    93,    93,    93,    93,    93,
      93,    93,    93,    94,    94,    95,    95,    95,    95,    95,
      95,    95,    95,    95,    95,    95,    95,    95,    95,    95,
      95,    95,    95,    96,    97,    97,    98,    98,    99,    99,
     100,   100,   101,   101,   102,   102,   102,   103,   104,   104,
     105,   105,   106,   107,   107,   108,   108,   108,   109,   109,
     110,   110,   111,   111,   112,   112,   112,   112,   113,   113,
     114,   115,   116,   116,   117,   117,   117,   117
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     2,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     6,     6,     6,     4,     4,     4,     4,
       4,     4,     4,     4,     4,     4,     4,     4,     7,     6,
       7,     6,     6,     6,     3,     6,     0,     2,     1,     0,
       2,     3,     3,     2,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     1,     3,     1,     2,     1,     2,     2,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     3,     2,     3,     4,     5,     2,     3,
       1,     1,     0,     2,     1,     1,     3,     5,     0,     2,
       3,     3,     6,     0,     2,     3,     3,     3,     0,     2,
       1,     3,     0,     2,     3,     3,     3,     3,     1,     1,
       1,     1,     0,     2,     3,     3,     3,     3
};

/* YYDEFACT[STATE-NAME] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       2,     0,     1,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     3,     4,     5,     6,     7,     8,     9,    10,
      12,    11,     0,     0,     0,     0,     0,    36,     0,    36,
       0,    36,     0,    36,     0,    36,     0,    36,     0,     0,
       0,     0,     0,    39,    39,     0,    82,     0,    98,    39,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   102,   112,    39,    34,     0,     0,    82,     0,
      98,     0,     0,    16,    38,    37,    17,    18,    27,    19,
      20,    21,    22,    23,    24,    25,    26,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    40,     0,     0,     0,     0,     0,     0,
      83,    84,    85,     0,     0,     0,   100,    99,     0,     0,
       0,     0,     0,     0,   103,     0,     0,     0,     0,     0,
     113,     0,     0,     0,     0,     0,    55,    57,    62,    63,
      64,    65,    60,    61,    66,    67,    68,    69,    70,    71,
      72,     0,     0,    53,     0,     0,     0,     0,    43,     0,
       0,     0,     0,     0,     0,    13,     0,     0,    14,     0,
       0,    88,    29,     0,     0,     0,    31,    15,   108,   109,
       0,   110,     0,   111,     0,     0,    32,     0,     0,     0,
       0,    33,    35,    81,    80,    74,    46,    58,    59,    56,
      42,     0,    41,    44,    48,    45,    49,    50,     0,    51,
      78,    52,     0,    47,    28,    93,     0,    86,    30,   101,
     104,   105,   106,   107,   114,   115,   116,   117,    75,    54,
      73,    79,     0,     0,     0,     0,     0,    89,    76,     0,
       0,     0,     0,    94,     0,     0,    87,    77,     0,     0,
       0,    92,    90,    91,    95,    96,    97
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    50,    75,    66,   103,   152,   153,   162,   133,
     167,   164,   195,    69,   110,   111,   216,   237,   112,   233,
     243,    71,   117,    87,   124,   180,   182,   184,    88,   130
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -163
static const yytype_int16 yypact[] =
{
    -163,    17,  -163,     9,     0,    32,    -6,   200,   -35,    -2,
     -10,   -18,  -163,  -163,  -163,  -163,  -163,  -163,  -163,  -163,
    -163,  -163,    -5,     4,   -12,    -9,     7,  -163,    18,  -163,
      -1,  -163,    27,  -163,    28,  -163,    30,  -163,    31,    10,
      24,    35,    13,  -163,  -163,    36,  -163,    37,  -163,  -163,
     -45,    43,   -31,    44,   -27,    45,   -13,    46,    12,    47,
      21,    48,  -163,  -163,  -163,  -163,    67,    77,  -163,    -4,
    -163,     5,   125,  -163,  -163,  -163,  -163,  -163,  -163,  -163,
    -163,  -163,  -163,  -163,  -163,  -163,  -163,   -17,    -7,   135,
      50,   183,   183,   183,    51,   114,    57,    55,    61,    62,
      63,    68,    64,  -163,    70,    -3,   146,    82,    89,    64,
    -163,  -163,  -163,     8,    50,    90,  -163,  -163,    92,   -15,
     112,   113,    91,    96,  -163,    93,   103,    98,    99,   104,
    -163,   105,   -36,   106,   109,   115,  -163,  -163,  -163,  -163,
    -163,  -163,  -163,  -163,  -163,  -163,  -163,  -163,  -163,  -163,
    -163,   128,   116,   -16,   117,   118,   120,   122,  -163,   123,
     124,   119,   132,   -36,   133,  -163,   126,   134,  -163,   136,
     111,  -163,  -163,   138,   139,   156,  -163,  -163,  -163,  -163,
     157,  -163,   158,  -163,   172,   173,  -163,   174,   175,   176,
     177,  -163,  -163,  -163,  -163,   -36,  -163,  -163,  -163,  -163,
    -163,   183,  -163,  -163,  -163,  -163,  -163,  -163,   178,  -163,
     -36,  -163,   137,  -163,  -163,  -163,    -8,  -163,  -163,  -163,
    -163,  -163,  -163,  -163,  -163,  -163,  -163,  -163,  -163,  -163,
    -163,  -163,   -36,     2,   180,   181,   182,  -163,   -36,   185,
     186,   187,   188,  -163,   189,   190,  -163,  -163,   191,   192,
     193,  -163,  -163,  -163,  -163,  -163,  -163
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -163,  -163,  -163,  -163,  -163,  -163,  -163,  -163,  -163,  -163,
    -163,  -163,    95,  -163,   -28,  -163,   -92,   163,  -163,    71,
     141,  -163,  -162,   196,  -163,   -64,  -163,  -163,  -163,  -163,
    -163,   195,  -163,  -163,  -163,  -163,  -163,  -163,  -163,  -163
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const yytype_uint16 yytable[] =
{
     154,   210,    45,   106,   106,    47,    23,   116,   234,   235,
     239,   240,   241,    22,   107,   107,    67,     2,    39,    26,
       3,    72,     4,   107,   114,    73,   107,   114,    74,     5,
     119,   120,   193,   228,   178,   179,    89,   121,   122,    77,
     194,     6,    74,    79,     7,    24,    74,    25,   231,   116,
     125,   126,   127,    41,   128,   123,    42,    81,    40,    46,
      74,   201,    48,     8,   236,   129,    43,    53,   108,   169,
     238,   109,   109,     9,   242,    44,   247,   115,    49,    10,
     174,    62,    83,    65,    11,    74,    90,    91,    92,    93,
      94,    85,    51,    95,    74,    63,    90,    91,    92,    93,
      94,    55,    57,    95,    59,    61,    64,    68,    70,   229,
      96,    97,    98,    76,    78,    80,    82,    84,    86,   156,
      96,    97,    98,   132,    52,   157,    54,   158,    56,   159,
      58,    99,    60,   100,   160,   161,   163,   166,   165,   101,
     168,    99,   102,   100,    90,    91,    92,    93,    94,   104,
     170,    95,   102,   171,    90,    91,    92,    93,    94,   172,
     176,    95,   177,   181,   185,   183,   186,   187,    96,    97,
      98,   188,   189,   190,   191,   192,   196,   197,    96,    97,
      98,   199,   215,   198,   208,   175,   200,   202,   203,    99,
     204,   100,   205,   206,   207,   212,   134,   118,   135,    99,
     102,   100,   209,   211,   213,   232,   214,   131,   217,   218,
     102,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   219,   220,   221,   151,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      37,    38,   222,   223,   224,   225,   226,   227,   244,   245,
     173,   230,   246,   248,   249,   250,   155,     0,   251,   252,
     253,   254,   255,   256,   105,   113
};

#define yypact_value_is_default(yystate) \
  ((yystate) == (-163))

#define yytable_value_is_error(yytable_value) \
  YYID (0)

static const yytype_int16 yycheck[] =
{
      92,   163,    14,     7,     7,    14,     6,    71,    16,    17,
       8,     9,    10,     4,    18,    18,    44,     0,    53,    25,
       3,    49,     5,    18,    19,    70,    18,    19,    73,    12,
      47,    48,    68,   195,    49,    50,    64,    54,    55,    70,
      76,    24,    73,    70,    27,    13,    73,    15,   210,   113,
      57,    58,    59,    63,    61,    72,    74,    70,    60,    71,
      73,    77,    71,    46,    72,    72,    71,    68,    72,    72,
     232,    75,    75,    56,    72,    71,   238,    72,    71,    62,
      72,    71,    70,    70,    67,    73,    19,    20,    21,    22,
      23,    70,    74,    26,    73,    71,    19,    20,    21,    22,
      23,    74,    74,    26,    74,    74,    71,    71,    71,   201,
      43,    44,    45,    70,    70,    70,    70,    70,    70,    68,
      43,    44,    45,    73,    29,    11,    31,    70,    33,    74,
      35,    64,    37,    66,    73,    73,    73,    73,    70,    72,
      70,    64,    75,    66,    19,    20,    21,    22,    23,    72,
       4,    26,    75,    71,    19,    20,    21,    22,    23,    70,
      70,    26,    70,    51,    73,    52,    70,    74,    43,    44,
      45,    68,    74,    74,    70,    70,    70,    68,    43,    44,
      45,    53,    71,    68,    65,   114,    70,    70,    70,    64,
      70,    66,    70,    70,    70,    69,    13,    72,    15,    64,
      75,    66,    70,    70,    70,    68,    70,    72,    70,    70,
      75,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    42,    70,    70,    70,    46,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    70,    70,    70,    70,    70,    70,    68,    68,
     109,    73,    70,    68,    68,    68,    93,    -1,    70,    70,
      70,    70,    70,    70,    68,    70
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    79,     0,     3,     5,    12,    24,    27,    46,    56,
      62,    67,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,     4,     6,    13,    15,    25,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    53,
      60,    63,    74,    71,    71,    14,    71,    14,    71,    71,
      90,    74,    90,    68,    90,    74,    90,    74,    90,    74,
      90,    74,    71,    71,    71,    70,    92,    92,    71,   101,
      71,   109,    92,    70,    73,    91,    70,    70,    70,    70,
      70,    70,    70,    70,    70,    70,    70,   111,   116,    92,
      19,    20,    21,    22,    23,    26,    43,    44,    45,    64,
      66,    72,    75,    93,    72,   101,     7,    18,    72,    75,
     102,   103,   106,   109,    19,    72,   103,   110,    72,    47,
      48,    54,    55,    72,   112,    57,    58,    59,    61,    72,
     117,    72,    73,    97,    13,    15,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    46,    94,    95,    94,    95,    68,    11,    70,    74,
      73,    73,    96,    73,    99,    70,    73,    98,    70,    72,
       4,    71,    70,    98,    72,    97,    70,    70,    49,    50,
     113,    51,   114,    52,   115,    73,    70,    74,    68,    74,
      74,    70,    70,    68,    76,   100,    70,    68,    68,    53,
      70,    77,    70,    70,    70,    70,    70,    70,    65,    70,
     100,    70,    69,    70,    70,    71,   104,    70,    70,    70,
      70,    70,    70,    70,    70,    70,    70,    70,   100,    94,
      73,   100,    68,   107,    16,    17,    72,   105,   100,     8,
       9,    10,    72,   108,    68,    68,    70,   100,    68,    68,
      68,    70,    70,    70,    70,    70,    70
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  However,
   YYFAIL appears to be in use.  Nevertheless, it is formally deprecated
   in Bison 2.4.2's NEWS entry, where a plan to phase it out is
   discussed.  */

#define YYFAIL		goto yyerrlab
#if defined YYFAIL
  /* This is here to suppress warnings from the GCC cpp's
     -Wunused-macros.  Normally we don't worry about that warning, but
     some users do, and we want to make it easy for users to remove
     YYFAIL uses, which will produce warnings from Bison 2.5.  */
#endif

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* This macro is provided for backward compatibility. */

#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (0, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  YYSIZE_T yysize1;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = 0;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - Assume YYFAIL is not used.  It's too flawed to consider.  See
       <http://lists.gnu.org/archive/html/bison-patches/2009-12/msg00024.html>
       for details.  YYERROR is fine as it does not invoke this
       function.
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                yysize1 = yysize + yytnamerr (0, yytname[yyx]);
                if (! (yysize <= yysize1
                       && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                  return 2;
                yysize = yysize1;
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  yysize1 = yysize + yystrlen (yyformat);
  if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
    return 2;
  yysize = yysize1;

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */
#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */


/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.

       Refer to the stacks thru separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yytoken = 0;
  yyss = yyssa;
  yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */
  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss_alloc, yyss);
	YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 13:

/* Line 1806 of yacc.c  */
#line 173 "cfparse.y"
    {
		struct cf_namelist *ifl;

		MAKE_NAMELIST(ifl, (yyvsp[(2) - (6)].str), (yyvsp[(4) - (6)].list));

		if (add_namelist(ifl, &iflist_head))
			return (-1);
	}
    break;

  case 14:

/* Line 1806 of yacc.c  */
#line 185 "cfparse.y"
    {
		struct cf_namelist *profilelist;

		MAKE_NAMELIST(profilelist, (yyvsp[(2) - (6)].str), (yyvsp[(4) - (6)].list));

		if (add_namelist(profilelist, &profilelist_head))
			return (-1);
	}
    break;

  case 15:

/* Line 1806 of yacc.c  */
#line 197 "cfparse.y"
    {
		struct cf_namelist *host;

		MAKE_NAMELIST(host, (yyvsp[(2) - (6)].str), (yyvsp[(4) - (6)].list));

		if (add_namelist(host, &hostlist_head))
			return (-1);
	}
    break;

  case 16:

/* Line 1806 of yacc.c  */
#line 209 "cfparse.y"
    {
			if (cf_dns_list == NULL)
				cf_dns_list = (yyvsp[(3) - (4)].list);
			else {
				cf_dns_list->tail->next = (yyvsp[(3) - (4)].list);
				cf_dns_list->tail = (yyvsp[(3) - (4)].list)->tail;
			}
		}
    break;

  case 17:

/* Line 1806 of yacc.c  */
#line 218 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, CFLISTENT_GENERIC, (yyvsp[(3) - (4)].str), NULL);

			if (cf_dns_name_list == NULL) {
				cf_dns_name_list = l;
				cf_dns_name_list->tail = l;
				cf_dns_name_list->next = NULL;
			} else {
				cf_dns_name_list->tail->next = l;
				cf_dns_name_list->tail = l->tail;
			}
		}
    break;

  case 18:

/* Line 1806 of yacc.c  */
#line 233 "cfparse.y"
    {
			if (cf_ntp_list == NULL)
				cf_ntp_list = (yyvsp[(3) - (4)].list);
			else {
				cf_ntp_list->tail->next = (yyvsp[(3) - (4)].list);
				cf_ntp_list->tail = (yyvsp[(3) - (4)].list)->tail;
			}
		}
    break;

  case 19:

/* Line 1806 of yacc.c  */
#line 242 "cfparse.y"
    {
			if (cf_sip_list == NULL)
				cf_sip_list = (yyvsp[(3) - (4)].list);
			else {
				cf_sip_list->tail->next = (yyvsp[(3) - (4)].list);
				cf_sip_list->tail = (yyvsp[(3) - (4)].list)->tail;
			}
		}
    break;

  case 20:

/* Line 1806 of yacc.c  */
#line 251 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, CFLISTENT_GENERIC, (yyvsp[(3) - (4)].str), NULL);

			if (cf_sip_name_list == NULL) {
				cf_sip_name_list = l;
				cf_sip_name_list->tail = l;
				cf_sip_name_list->next = NULL;
			} else {
				cf_sip_name_list->tail->next = l;
				cf_sip_name_list->tail = l->tail;
			}
		}
    break;

  case 21:

/* Line 1806 of yacc.c  */
#line 266 "cfparse.y"
    {
			if (cf_nis_list == NULL)
				cf_nis_list = (yyvsp[(3) - (4)].list);
			else {
				cf_nis_list->tail->next = (yyvsp[(3) - (4)].list);
				cf_nis_list->tail = (yyvsp[(3) - (4)].list)->tail;
			}
		}
    break;

  case 22:

/* Line 1806 of yacc.c  */
#line 275 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, CFLISTENT_GENERIC, (yyvsp[(3) - (4)].str), NULL);

			if (cf_nis_name_list == NULL) {
				cf_nis_name_list = l;
				cf_nis_name_list->tail = l;
				cf_nis_name_list->next = NULL;
			} else {
				cf_nis_name_list->tail->next = l;
				cf_nis_name_list->tail = l->tail;
			}
		}
    break;

  case 23:

/* Line 1806 of yacc.c  */
#line 290 "cfparse.y"
    {
			if (cf_nisp_list == NULL)
				cf_nisp_list = (yyvsp[(3) - (4)].list);
			else {
				cf_nisp_list->tail->next = (yyvsp[(3) - (4)].list);
				cf_nisp_list->tail = (yyvsp[(3) - (4)].list)->tail;
			}
		}
    break;

  case 24:

/* Line 1806 of yacc.c  */
#line 299 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, CFLISTENT_GENERIC, (yyvsp[(3) - (4)].str), NULL);

			if (cf_nisp_name_list == NULL) {
				cf_nisp_name_list = l;
				cf_nisp_name_list->tail = l;
				cf_nisp_name_list->next = NULL;
			} else {
				cf_nisp_name_list->tail->next = l;
				cf_nisp_name_list->tail = l->tail;
			}
		}
    break;

  case 25:

/* Line 1806 of yacc.c  */
#line 314 "cfparse.y"
    {
			if (cf_bcmcs_list == NULL)
				cf_bcmcs_list = (yyvsp[(3) - (4)].list);
			else {
				cf_bcmcs_list->tail->next = (yyvsp[(3) - (4)].list);
				cf_bcmcs_list->tail = (yyvsp[(3) - (4)].list)->tail;
			}
		}
    break;

  case 26:

/* Line 1806 of yacc.c  */
#line 323 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, CFLISTENT_GENERIC, (yyvsp[(3) - (4)].str), NULL);

			if (cf_bcmcs_name_list == NULL) {
				cf_bcmcs_name_list = l;
				cf_bcmcs_name_list->tail = l;
				cf_bcmcs_name_list->next = NULL;
			} else {
				cf_bcmcs_name_list->tail->next = l;
				cf_bcmcs_name_list->tail = l->tail;
			}
		}
    break;

  case 27:

/* Line 1806 of yacc.c  */
#line 338 "cfparse.y"
    {
			if (cf_refreshtime == -1) {
				cf_refreshtime = (yyvsp[(3) - (4)].num);
				if (cf_refreshtime < -1 ||
				    cf_refreshtime > 0xffffffff) {
					/*
					 * refresh time should not be negative
					 * according to the lex definition,
					 * but check it for safety.
					 */
					yyerror("refresh time is out of range");
				}
				if (cf_refreshtime < DHCP6_IRT_MINIMUM) {
					/*
					 * the value MUST NOT be smaller than
					 * IRT_MINIMUM.
					 */
					yyerror("refresh time is too small "
					    "(must not be smaller than %d)",
					    DHCP6_IRT_MINIMUM);
				}
			} else {
				yywarn("multiple refresh times (ignored)");
			}
		}
    break;

  case 28:

/* Line 1806 of yacc.c  */
#line 367 "cfparse.y"
    {
			struct cf_namelist *iapd;

			MAKE_NAMELIST(iapd, (yyvsp[(3) - (7)].str), (yyvsp[(5) - (7)].list));

			if (add_namelist(iapd, &iapdlist_head))
				return (-1);
		}
    break;

  case 29:

/* Line 1806 of yacc.c  */
#line 376 "cfparse.y"
    {
			struct cf_namelist *iapd;
			char *zero;

			if ((zero = strdup("0")) == NULL) {
				yywarn("can't allocate memory");
				return (-1);
			}
			MAKE_NAMELIST(iapd, zero, (yyvsp[(4) - (6)].list));

			if (add_namelist(iapd, &iapdlist_head))
				return (-1);
		}
    break;

  case 30:

/* Line 1806 of yacc.c  */
#line 390 "cfparse.y"
    {
			struct cf_namelist *iana;

			MAKE_NAMELIST(iana, (yyvsp[(3) - (7)].str), (yyvsp[(5) - (7)].list));

			if (add_namelist(iana, &ianalist_head))
				return (-1);
		}
    break;

  case 31:

/* Line 1806 of yacc.c  */
#line 399 "cfparse.y"
    {
			struct cf_namelist *iana;
			char *zero;

			if ((zero = strdup("0")) == NULL) {
				yywarn("can't allocate memory");
				return (-1);
			}
			MAKE_NAMELIST(iana, zero, (yyvsp[(4) - (6)].list));

			if (add_namelist(iana, &ianalist_head))
				return (-1);
		}
    break;

  case 32:

/* Line 1806 of yacc.c  */
#line 416 "cfparse.y"
    {
		struct cf_namelist *authinfo;

		MAKE_NAMELIST(authinfo, (yyvsp[(2) - (6)].str), (yyvsp[(4) - (6)].list));

		if (add_namelist(authinfo, &authinfolist_head))
			return (-1);
	}
    break;

  case 33:

/* Line 1806 of yacc.c  */
#line 428 "cfparse.y"
    {
		struct cf_namelist *key;

		MAKE_NAMELIST(key, (yyvsp[(2) - (6)].str), (yyvsp[(4) - (6)].list));

		if (add_namelist(key, &keylist_head))
			return (-1);
	}
    break;

  case 34:

/* Line 1806 of yacc.c  */
#line 440 "cfparse.y"
    {
		if (cfswitch_buffer((yyvsp[(2) - (3)].str))) {
			free((yyvsp[(2) - (3)].str));
			return (-1);
		}
		free((yyvsp[(2) - (3)].str));
	}
    break;

  case 35:

/* Line 1806 of yacc.c  */
#line 451 "cfparse.y"
    {
		struct cf_namelist *pool;

		MAKE_NAMELIST(pool, (yyvsp[(2) - (6)].str), (yyvsp[(4) - (6)].list));

		if (add_namelist(pool, &addrpoollist_head))
			return (-1);
	}
    break;

  case 36:

/* Line 1806 of yacc.c  */
#line 462 "cfparse.y"
    { (yyval.list) = NULL; }
    break;

  case 37:

/* Line 1806 of yacc.c  */
#line 464 "cfparse.y"
    {
			struct cf_list *head;

			if ((head = (yyvsp[(1) - (2)].list)) == NULL) {
				(yyvsp[(2) - (2)].list)->next = NULL;
				(yyvsp[(2) - (2)].list)->tail = (yyvsp[(2) - (2)].list);
				head = (yyvsp[(2) - (2)].list);
			} else {
				head->tail->next = (yyvsp[(2) - (2)].list);
				head->tail = (yyvsp[(2) - (2)].list)->tail;
			}

			(yyval.list) = head;
		}
    break;

  case 38:

/* Line 1806 of yacc.c  */
#line 482 "cfparse.y"
    {
		struct cf_list *l;
		struct in6_addr a0, *a;

		if (inet_pton(AF_INET6, (yyvsp[(1) - (1)].str), &a0) != 1) {
			yywarn("invalid IPv6 address: %s", (yyvsp[(1) - (1)].str));
			free((yyvsp[(1) - (1)].str));
			return (-1);
		}
		if ((a = malloc(sizeof(*a))) == NULL) {
			yywarn("can't allocate memory");
			return (-1);
		}
		*a = a0;

		MAKE_CFLIST(l, CFLISTENT_GENERIC, a, NULL);

		(yyval.list) = l;
	}
    break;

  case 39:

/* Line 1806 of yacc.c  */
#line 504 "cfparse.y"
    { (yyval.list) = NULL; }
    break;

  case 40:

/* Line 1806 of yacc.c  */
#line 506 "cfparse.y"
    {
			struct cf_list *head;

			if ((head = (yyvsp[(1) - (2)].list)) == NULL) {
				(yyvsp[(2) - (2)].list)->next = NULL;
				(yyvsp[(2) - (2)].list)->tail = (yyvsp[(2) - (2)].list);
				head = (yyvsp[(2) - (2)].list);
			} else {
				head->tail->next = (yyvsp[(2) - (2)].list);
				head->tail = (yyvsp[(2) - (2)].list)->tail;
			}

			(yyval.list) = head;
		}
    break;

  case 41:

/* Line 1806 of yacc.c  */
#line 524 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, DECL_SEND, NULL, (yyvsp[(2) - (3)].list));

			(yyval.list) = l;
		}
    break;

  case 42:

/* Line 1806 of yacc.c  */
#line 532 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, DECL_REQUEST, NULL, (yyvsp[(2) - (3)].list));

			(yyval.list) = l;
		}
    break;

  case 43:

/* Line 1806 of yacc.c  */
#line 540 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, DECL_INFO_ONLY, NULL, NULL);
			/* no value */
			(yyval.list) = l;
		}
    break;

  case 44:

/* Line 1806 of yacc.c  */
#line 548 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, DECL_ALLOW, NULL, (yyvsp[(2) - (3)].list));

			(yyval.list) = l;
		}
    break;

  case 45:

/* Line 1806 of yacc.c  */
#line 556 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, DECL_DUID, (yyvsp[(2) - (3)].str), NULL);

			(yyval.list) = l;
		}
    break;

  case 46:

/* Line 1806 of yacc.c  */
#line 564 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, DECL_ADDRESS, (yyvsp[(2) - (3)].prefix),NULL);

			(yyval.list) = l;
		}
    break;

  case 47:

/* Line 1806 of yacc.c  */
#line 572 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, DECL_PREFIX, (yyvsp[(2) - (3)].prefix), NULL);

			(yyval.list) = l;
		}
    break;

  case 48:

/* Line 1806 of yacc.c  */
#line 580 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, DECL_PREFERENCE, NULL, NULL);
			l->num = (yyvsp[(2) - (3)].num);

			(yyval.list) = l;
		}
    break;

  case 49:

/* Line 1806 of yacc.c  */
#line 589 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, DECL_SCRIPT, (yyvsp[(2) - (3)].str), NULL);

			(yyval.list) = l;
		}
    break;

  case 50:

/* Line 1806 of yacc.c  */
#line 597 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, DECL_DELAYEDKEY, (yyvsp[(2) - (3)].str), NULL);

			(yyval.list) = l;
		}
    break;

  case 51:

/* Line 1806 of yacc.c  */
#line 605 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, DECL_RANGE, (yyvsp[(2) - (3)].range), NULL);

			(yyval.list) = l;
		}
    break;

  case 52:

/* Line 1806 of yacc.c  */
#line 613 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, DECL_ADDRESSPOOL, (yyvsp[(2) - (3)].pool), NULL);

			(yyval.list) = l;
		}
    break;

  case 53:

/* Line 1806 of yacc.c  */
#line 624 "cfparse.y"
    {
			(yyval.list) = (yyvsp[(1) - (1)].list);
		}
    break;

  case 54:

/* Line 1806 of yacc.c  */
#line 628 "cfparse.y"
    {
			(yyvsp[(1) - (3)].list)->next = (yyvsp[(3) - (3)].list);
			(yyvsp[(1) - (3)].list)->tail = (yyvsp[(3) - (3)].list)->tail;

			(yyval.list) = (yyvsp[(1) - (3)].list);
		}
    break;

  case 55:

/* Line 1806 of yacc.c  */
#line 638 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, DHCPOPT_RAPID_COMMIT, NULL, NULL);
			/* no value */
			(yyval.list) = l;
		}
    break;

  case 56:

/* Line 1806 of yacc.c  */
#line 646 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, DHCPOPT_AUTHINFO, NULL, NULL);
			l->ptr = (yyvsp[(2) - (2)].str);
			(yyval.list) = l;
		}
    break;

  case 57:

/* Line 1806 of yacc.c  */
#line 654 "cfparse.y"
    {
			struct cf_list *l;
			MAKE_CFLIST(l, DHCPOPT_RECONFIGURE_ACCEPT, NULL, NULL);
			/* no value */
			(yyval.list) = l;	
		}
    break;

  case 58:

/* Line 1806 of yacc.c  */
#line 661 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, DHCPOPT_IA_PD, NULL, NULL);
			l->num = (yyvsp[(2) - (2)].num);
			(yyval.list) = l;
		}
    break;

  case 59:

/* Line 1806 of yacc.c  */
#line 669 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, DHCPOPT_IA_NA, NULL, NULL);
			l->num = (yyvsp[(2) - (2)].num);
			(yyval.list) = l;
		}
    break;

  case 60:

/* Line 1806 of yacc.c  */
#line 677 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, DHCPOPT_SIP, NULL, NULL);
			/* currently no value */
			(yyval.list) = l;
		}
    break;

  case 61:

/* Line 1806 of yacc.c  */
#line 685 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, DHCPOPT_SIPNAME, NULL, NULL);
			/* currently no value */
			(yyval.list) = l;
		}
    break;

  case 62:

/* Line 1806 of yacc.c  */
#line 693 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, DHCPOPT_DNS, NULL, NULL);
			/* currently no value */
			(yyval.list) = l;
		}
    break;

  case 63:

/* Line 1806 of yacc.c  */
#line 701 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, DHCPOPT_DNSNAME, NULL, NULL);
			/* currently no value */
			(yyval.list) = l;
		}
    break;

  case 64:

/* Line 1806 of yacc.c  */
#line 709 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, DHCPOPT_NTP, NULL, NULL);
			/* currently no value */
			(yyval.list) = l;
		}
    break;

  case 65:

/* Line 1806 of yacc.c  */
#line 717 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, DHCPOPT_REFRESHTIME, NULL, NULL);
			/* currently no value */
			(yyval.list) = l;
		}
    break;

  case 66:

/* Line 1806 of yacc.c  */
#line 725 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, DHCPOPT_NIS, NULL, NULL);
			/* currently no value */
			(yyval.list) = l;
		}
    break;

  case 67:

/* Line 1806 of yacc.c  */
#line 733 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, DHCPOPT_NISNAME, NULL, NULL);
			/* currently no value */
			(yyval.list) = l;
		}
    break;

  case 68:

/* Line 1806 of yacc.c  */
#line 741 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, DHCPOPT_NISP, NULL, NULL);
			/* currently no value */
			(yyval.list) = l;
		}
    break;

  case 69:

/* Line 1806 of yacc.c  */
#line 749 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, DHCPOPT_NISPNAME, NULL, NULL);
			/* currently no value */
			(yyval.list) = l;
		}
    break;

  case 70:

/* Line 1806 of yacc.c  */
#line 757 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, DHCPOPT_BCMCS, NULL, NULL);
			/* currently no value */
			(yyval.list) = l;
		}
    break;

  case 71:

/* Line 1806 of yacc.c  */
#line 765 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, DHCPOPT_BCMCSNAME, NULL, NULL);
			/* currently no value */
			(yyval.list) = l;
		}
    break;

  case 72:

/* Line 1806 of yacc.c  */
#line 773 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, DHCPOPT_AFTR, NULL, NULL);
			/* currently no value */
			(yyval.list) = l;
		}
    break;

  case 73:

/* Line 1806 of yacc.c  */
#line 784 "cfparse.y"
    {
			struct dhcp6_range range0, *range;		

			memset(&range0, 0, sizeof(range0));
			if (inet_pton(AF_INET6, (yyvsp[(1) - (3)].str), &range0.min) != 1) {
				yywarn("invalid IPv6 address: %s", (yyvsp[(1) - (3)].str));
				free((yyvsp[(1) - (3)].str));
				free((yyvsp[(3) - (3)].str));
				return (-1);
			}
			if (inet_pton(AF_INET6, (yyvsp[(3) - (3)].str), &range0.max) != 1) {
				yywarn("invalid IPv6 address: %s", (yyvsp[(3) - (3)].str));
				free((yyvsp[(1) - (3)].str));
				free((yyvsp[(3) - (3)].str));
				return (-1);
			}
			free((yyvsp[(1) - (3)].str));
			free((yyvsp[(3) - (3)].str));

			if ((range = malloc(sizeof(*range))) == NULL) {
				yywarn("can't allocate memory");
				return (-1);
			}
			*range = range0;

			(yyval.range) = range;
		}
    break;

  case 74:

/* Line 1806 of yacc.c  */
#line 815 "cfparse.y"
    {
			struct dhcp6_prefix pconf0, *pconf;		

			memset(&pconf0, 0, sizeof(pconf0));
			if (inet_pton(AF_INET6, (yyvsp[(1) - (2)].str), &pconf0.addr) != 1) {
				yywarn("invalid IPv6 address: %s", (yyvsp[(1) - (2)].str));
				free((yyvsp[(1) - (2)].str));
				return (-1);
			}
			free((yyvsp[(1) - (2)].str));
			/* validate other parameters later */
			pconf0.plen = 128; /* XXX this field is ignored */
			if ((yyvsp[(2) - (2)].num) < 0)
				pconf0.pltime = DHCP6_DURATION_INFINITE;
			else
				pconf0.pltime = (u_int32_t)(yyvsp[(2) - (2)].num);
			pconf0.vltime = pconf0.pltime;

			if ((pconf = malloc(sizeof(*pconf))) == NULL) {
				yywarn("can't allocate memory");
				return (-1);
			}
			*pconf = pconf0;

			(yyval.prefix) = pconf;
		}
    break;

  case 75:

/* Line 1806 of yacc.c  */
#line 842 "cfparse.y"
    {
			struct dhcp6_prefix pconf0, *pconf;		

			memset(&pconf0, 0, sizeof(pconf0));
			if (inet_pton(AF_INET6, (yyvsp[(1) - (3)].str), &pconf0.addr) != 1) {
				yywarn("invalid IPv6 address: %s", (yyvsp[(1) - (3)].str));
				free((yyvsp[(1) - (3)].str));
				return (-1);
			}
			free((yyvsp[(1) - (3)].str));
			/* validate other parameters later */
			pconf0.plen = 128; /* XXX */
			if ((yyvsp[(2) - (3)].num) < 0)
				pconf0.pltime = DHCP6_DURATION_INFINITE;
			else
				pconf0.pltime = (u_int32_t)(yyvsp[(2) - (3)].num);
			if ((yyvsp[(3) - (3)].num) < 0)
				pconf0.vltime = DHCP6_DURATION_INFINITE;
			else
				pconf0.vltime = (u_int32_t)(yyvsp[(3) - (3)].num);

			if ((pconf = malloc(sizeof(*pconf))) == NULL) {
				yywarn("can't allocate memory");
				return (-1);
			}
			*pconf = pconf0;

			(yyval.prefix) = pconf;
		}
    break;

  case 76:

/* Line 1806 of yacc.c  */
#line 875 "cfparse.y"
    {
			struct dhcp6_prefix pconf0, *pconf;		

			memset(&pconf0, 0, sizeof(pconf0));
			if (inet_pton(AF_INET6, (yyvsp[(1) - (4)].str), &pconf0.addr) != 1) {
				yywarn("invalid IPv6 address: %s", (yyvsp[(1) - (4)].str));
				free((yyvsp[(1) - (4)].str));
				return (-1);
			}
			free((yyvsp[(1) - (4)].str));
			/* validate other parameters later */
			pconf0.plen = (yyvsp[(3) - (4)].num);
			if ((yyvsp[(4) - (4)].num) < 0)
				pconf0.pltime = DHCP6_DURATION_INFINITE;
			else
				pconf0.pltime = (u_int32_t)(yyvsp[(4) - (4)].num);
			pconf0.vltime = pconf0.pltime;

			if ((pconf = malloc(sizeof(*pconf))) == NULL) {
				yywarn("can't allocate memory");
				return (-1);
			}
			*pconf = pconf0;

			(yyval.prefix) = pconf;
		}
    break;

  case 77:

/* Line 1806 of yacc.c  */
#line 902 "cfparse.y"
    {
			struct dhcp6_prefix pconf0, *pconf;		

			memset(&pconf0, 0, sizeof(pconf0));
			if (inet_pton(AF_INET6, (yyvsp[(1) - (5)].str), &pconf0.addr) != 1) {
				yywarn("invalid IPv6 address: %s", (yyvsp[(1) - (5)].str));
				free((yyvsp[(1) - (5)].str));
				return (-1);
			}
			free((yyvsp[(1) - (5)].str));
			/* validate other parameters later */
			pconf0.plen = (yyvsp[(3) - (5)].num);
			if ((yyvsp[(4) - (5)].num) < 0)
				pconf0.pltime = DHCP6_DURATION_INFINITE;
			else
				pconf0.pltime = (u_int32_t)(yyvsp[(4) - (5)].num);
			if ((yyvsp[(5) - (5)].num) < 0)
				pconf0.vltime = DHCP6_DURATION_INFINITE;
			else
				pconf0.vltime = (u_int32_t)(yyvsp[(5) - (5)].num);

			if ((pconf = malloc(sizeof(*pconf))) == NULL) {
				yywarn("can't allocate memory");
				return (-1);
			}
			*pconf = pconf0;

			(yyval.prefix) = pconf;
		}
    break;

  case 78:

/* Line 1806 of yacc.c  */
#line 935 "cfparse.y"
    {
			struct dhcp6_poolspec* pool;		

			if ((pool = malloc(sizeof(*pool))) == NULL) {
				yywarn("can't allocate memory");
				free((yyvsp[(1) - (2)].str));
				return (-1);
			}
			if ((pool->name = strdup((yyvsp[(1) - (2)].str))) == NULL) {
				yywarn("can't allocate memory");
				free((yyvsp[(1) - (2)].str));
				return (-1);
			}
			free((yyvsp[(1) - (2)].str));

			/* validate other parameters later */
			if ((yyvsp[(2) - (2)].num) < 0)
				pool->pltime = DHCP6_DURATION_INFINITE;
			else
				pool->pltime = (u_int32_t)(yyvsp[(2) - (2)].num);
			pool->vltime = pool->pltime;

			(yyval.pool) = pool;
		}
    break;

  case 79:

/* Line 1806 of yacc.c  */
#line 960 "cfparse.y"
    {
			struct dhcp6_poolspec* pool;		

			if ((pool = malloc(sizeof(*pool))) == NULL) {
				yywarn("can't allocate memory");
				free((yyvsp[(1) - (3)].str));
				return (-1);
			}
			if ((pool->name = strdup((yyvsp[(1) - (3)].str))) == NULL) {
				yywarn("can't allocate memory");
				free((yyvsp[(1) - (3)].str));
				return (-1);
			}
			free((yyvsp[(1) - (3)].str));

			/* validate other parameters later */
			if ((yyvsp[(2) - (3)].num) < 0)
				pool->pltime = DHCP6_DURATION_INFINITE;
			else
				pool->pltime = (u_int32_t)(yyvsp[(2) - (3)].num);
			if ((yyvsp[(3) - (3)].num) < 0)
				pool->vltime = DHCP6_DURATION_INFINITE;
			else
				pool->vltime = (u_int32_t)(yyvsp[(3) - (3)].num);

			(yyval.pool) = pool;
		}
    break;

  case 80:

/* Line 1806 of yacc.c  */
#line 991 "cfparse.y"
    {
			(yyval.num) = -1;
		}
    break;

  case 81:

/* Line 1806 of yacc.c  */
#line 995 "cfparse.y"
    {
			(yyval.num) = (yyvsp[(1) - (1)].num);
		}
    break;

  case 82:

/* Line 1806 of yacc.c  */
#line 1001 "cfparse.y"
    { (yyval.list) = NULL; }
    break;

  case 83:

/* Line 1806 of yacc.c  */
#line 1003 "cfparse.y"
    {
			struct cf_list *head;

			if ((head = (yyvsp[(1) - (2)].list)) == NULL) {
				(yyvsp[(2) - (2)].list)->next = NULL;
				(yyvsp[(2) - (2)].list)->tail = (yyvsp[(2) - (2)].list);
				head = (yyvsp[(2) - (2)].list);
			} else {
				head->tail->next = (yyvsp[(2) - (2)].list);
				head->tail = (yyvsp[(2) - (2)].list)->tail;
			}

			(yyval.list) = head;
		}
    break;

  case 84:

/* Line 1806 of yacc.c  */
#line 1020 "cfparse.y"
    { (yyval.list) = (yyvsp[(1) - (1)].list); }
    break;

  case 85:

/* Line 1806 of yacc.c  */
#line 1021 "cfparse.y"
    { (yyval.list) = (yyvsp[(1) - (1)].list); }
    break;

  case 86:

/* Line 1806 of yacc.c  */
#line 1023 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, IACONF_PREFIX, (yyvsp[(2) - (3)].prefix), NULL);

			(yyval.list) = l;
		}
    break;

  case 87:

/* Line 1806 of yacc.c  */
#line 1034 "cfparse.y"
    {
		struct cf_list *ifl;

		MAKE_CFLIST(ifl, IASUGGEST_T, NULL, (yyvsp[(3) - (5)].list));
		(yyval.list) = ifl;
	}
    break;

  case 88:

/* Line 1806 of yacc.c  */
#line 1043 "cfparse.y"
    { (yyval.list) = NULL; }
    break;

  case 89:

/* Line 1806 of yacc.c  */
#line 1045 "cfparse.y"
    {
			struct cf_list *head;

			if ((head = (yyvsp[(1) - (2)].list)) == NULL) {
				(yyvsp[(2) - (2)].list)->next = NULL;
				(yyvsp[(2) - (2)].list)->tail = (yyvsp[(2) - (2)].list);
				head = (yyvsp[(2) - (2)].list);
			} else {
				head->tail->next = (yyvsp[(2) - (2)].list);
				head->tail = (yyvsp[(2) - (2)].list)->tail;
			}

			(yyval.list) = head;
		}
    break;

  case 90:

/* Line 1806 of yacc.c  */
#line 1063 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, IAPARAM_T1, NULL, NULL);
			l->num = (yyvsp[(2) - (3)].num);
			(yyval.list) = l;
		}
    break;

  case 91:

/* Line 1806 of yacc.c  */
#line 1071 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, IAPARAM_T2, NULL, NULL);
			l->num = (yyvsp[(2) - (3)].num);
			(yyval.list) = l;
		}
    break;

  case 92:

/* Line 1806 of yacc.c  */
#line 1082 "cfparse.y"
    {
		struct cf_list *ifl;

		MAKE_CFLIST(ifl, IACONF_PIF, (yyvsp[(2) - (6)].str), (yyvsp[(4) - (6)].list));
		(yyval.list) = ifl;
	}
    break;

  case 93:

/* Line 1806 of yacc.c  */
#line 1091 "cfparse.y"
    { (yyval.list) = NULL; }
    break;

  case 94:

/* Line 1806 of yacc.c  */
#line 1093 "cfparse.y"
    {
			struct cf_list *head;

			if ((head = (yyvsp[(1) - (2)].list)) == NULL) {
				(yyvsp[(2) - (2)].list)->next = NULL;
				(yyvsp[(2) - (2)].list)->tail = (yyvsp[(2) - (2)].list);
				head = (yyvsp[(2) - (2)].list);
			} else {
				head->tail->next = (yyvsp[(2) - (2)].list);
				head->tail = (yyvsp[(2) - (2)].list)->tail;
			}

			(yyval.list) = head;
		}
    break;

  case 95:

/* Line 1806 of yacc.c  */
#line 1111 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, IFPARAM_SLA_ID, NULL, NULL);
			l->num = (yyvsp[(2) - (3)].num);
			(yyval.list) = l;
		}
    break;

  case 96:

/* Line 1806 of yacc.c  */
#line 1119 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, IFPARAM_SLA_LEN, NULL, NULL);
			l->num = (yyvsp[(2) - (3)].num);
			(yyval.list) = l;
		}
    break;

  case 97:

/* Line 1806 of yacc.c  */
#line 1127 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, IFPARAM_IFID, NULL, NULL);
			l->num = (u_int64_t)(yyvsp[(2) - (3)].num);
			(yyval.list) = l;
		}
    break;

  case 98:

/* Line 1806 of yacc.c  */
#line 1137 "cfparse.y"
    { (yyval.list) = NULL; }
    break;

  case 99:

/* Line 1806 of yacc.c  */
#line 1139 "cfparse.y"
    {
			struct cf_list *head;

			if ((head = (yyvsp[(1) - (2)].list)) == NULL) {
				(yyvsp[(2) - (2)].list)->next = NULL;
				(yyvsp[(2) - (2)].list)->tail = (yyvsp[(2) - (2)].list);
				head = (yyvsp[(2) - (2)].list);
			} else {
				head->tail->next = (yyvsp[(2) - (2)].list);
				head->tail = (yyvsp[(2) - (2)].list)->tail;
			}

			(yyval.list) = head;
		}
    break;

  case 100:

/* Line 1806 of yacc.c  */
#line 1156 "cfparse.y"
    { (yyval.list) = (yyvsp[(1) - (1)].list); }
    break;

  case 101:

/* Line 1806 of yacc.c  */
#line 1158 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, IACONF_ADDR, (yyvsp[(2) - (3)].prefix), NULL);

			(yyval.list) = l;
		}
    break;

  case 102:

/* Line 1806 of yacc.c  */
#line 1168 "cfparse.y"
    { (yyval.list) = NULL; }
    break;

  case 103:

/* Line 1806 of yacc.c  */
#line 1170 "cfparse.y"
    {
			struct cf_list *head;

			if ((head = (yyvsp[(1) - (2)].list)) == NULL) {
				(yyvsp[(2) - (2)].list)->next = NULL;
				(yyvsp[(2) - (2)].list)->tail = (yyvsp[(2) - (2)].list);
				head = (yyvsp[(2) - (2)].list);
			} else {
				head->tail->next = (yyvsp[(2) - (2)].list);
				head->tail = (yyvsp[(2) - (2)].list)->tail;
			}

			(yyval.list) = head;
		}
    break;

  case 104:

/* Line 1806 of yacc.c  */
#line 1188 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, AUTHPARAM_PROTO, NULL, NULL);
			l->num = (yyvsp[(2) - (3)].num);
			(yyval.list) = l;
		}
    break;

  case 105:

/* Line 1806 of yacc.c  */
#line 1196 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, AUTHPARAM_ALG, NULL, NULL);
			l->num = (yyvsp[(2) - (3)].num);
			(yyval.list) = l;
		}
    break;

  case 106:

/* Line 1806 of yacc.c  */
#line 1204 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, AUTHPARAM_RDM, NULL, NULL);
			l->num = (yyvsp[(2) - (3)].num);
			(yyval.list) = l;
		}
    break;

  case 107:

/* Line 1806 of yacc.c  */
#line 1212 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, AUTHPARAM_KEY, NULL, NULL);
			l->ptr = (yyvsp[(2) - (3)].str);
			(yyval.list) = l;
		}
    break;

  case 108:

/* Line 1806 of yacc.c  */
#line 1222 "cfparse.y"
    { (yyval.num) = DHCP6_AUTHPROTO_DELAYED; }
    break;

  case 109:

/* Line 1806 of yacc.c  */
#line 1223 "cfparse.y"
    { (yyval.num) = DHCP6_AUTHPROTO_RECONFIG; }
    break;

  case 110:

/* Line 1806 of yacc.c  */
#line 1227 "cfparse.y"
    { (yyval.num) = DHCP6_AUTHALG_HMACMD5; }
    break;

  case 111:

/* Line 1806 of yacc.c  */
#line 1231 "cfparse.y"
    { (yyval.num) = DHCP6_AUTHRDM_MONOCOUNTER; }
    break;

  case 112:

/* Line 1806 of yacc.c  */
#line 1235 "cfparse.y"
    { (yyval.list) = NULL; }
    break;

  case 113:

/* Line 1806 of yacc.c  */
#line 1237 "cfparse.y"
    {
			struct cf_list *head;

			if ((head = (yyvsp[(1) - (2)].list)) == NULL) {
				(yyvsp[(2) - (2)].list)->next = NULL;
				(yyvsp[(2) - (2)].list)->tail = (yyvsp[(2) - (2)].list);
				head = (yyvsp[(2) - (2)].list);
			} else {
				head->tail->next = (yyvsp[(2) - (2)].list);
				head->tail = (yyvsp[(2) - (2)].list)->tail;
			}

			(yyval.list) = head;
		}
    break;

  case 114:

/* Line 1806 of yacc.c  */
#line 1255 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, KEYPARAM_REALM, NULL, NULL);
			l->ptr = (yyvsp[(2) - (3)].str);
			(yyval.list) = l;
		}
    break;

  case 115:

/* Line 1806 of yacc.c  */
#line 1263 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, KEYPARAM_KEYID, NULL, NULL);
			l->num = (yyvsp[(2) - (3)].num);
			(yyval.list) = l;
		}
    break;

  case 116:

/* Line 1806 of yacc.c  */
#line 1271 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, KEYPARAM_SECRET, NULL, NULL);
			l->ptr = (yyvsp[(2) - (3)].str);
			(yyval.list) = l;
		}
    break;

  case 117:

/* Line 1806 of yacc.c  */
#line 1279 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, KEYPARAM_EXPIRE, NULL, NULL);
			l->ptr = (yyvsp[(2) - (3)].str);
			(yyval.list) = l;
		}
    break;



/* Line 1806 of yacc.c  */
#line 3366 "y.tab.c"
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined(yyoverflow) || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}



/* Line 2067 of yacc.c  */
#line 1288 "cfparse.y"

/* supplement routines for configuration */
static int
add_namelist(new, headp)
	struct cf_namelist *new, **headp;
{
	struct cf_namelist *n;
	
	/* check for duplicated configuration */
	for (n = *headp; n; n = n->next) {
		if (strcmp(n->name, new->name) == 0) {
			yywarn("duplicated name: %s (ignored)",
			       new->name);
			cleanup_namelist(new);
			return (0);
		}
	}

	new->next = *headp;
	*headp = new;

	return (0);
}

/* free temporary resources */
static void
cleanup()
{
	cleanup_namelist(iflist_head);
	iflist_head = NULL;
	cleanup_namelist(profilelist_head);
	profilelist_head = NULL;
	cleanup_namelist(hostlist_head);
	hostlist_head = NULL;
	cleanup_namelist(iapdlist_head);
	iapdlist_head = NULL;
	cleanup_namelist(ianalist_head);
	ianalist_head = NULL;
	cleanup_namelist(authinfolist_head);
	authinfolist_head = NULL;
	cleanup_namelist(keylist_head);
	keylist_head = NULL;
	cleanup_namelist(addrpoollist_head);
	addrpoollist_head = NULL;

	cleanup_cflist(cf_sip_list);
	cf_sip_list = NULL;
	cleanup_cflist(cf_sip_name_list);
	cf_sip_name_list = NULL;
	cleanup_cflist(cf_dns_list);
	cf_dns_list = NULL;
	cleanup_cflist(cf_dns_name_list);
	cf_dns_name_list = NULL;
	cleanup_cflist(cf_ntp_list);
	cf_ntp_list = NULL;
	cleanup_cflist(cf_nis_list);
	cf_nis_list = NULL;
	cleanup_cflist(cf_nis_name_list);
	cf_nis_name_list = NULL;
	cleanup_cflist(cf_nisp_list);
	cf_nisp_list = NULL;
	cleanup_cflist(cf_nisp_name_list);
	cf_nisp_name_list = NULL;
	cleanup_cflist(cf_bcmcs_list);
	cf_bcmcs_list = NULL;
	cleanup_cflist(cf_bcmcs_name_list);
	cf_bcmcs_name_list = NULL;
}

static void
cleanup_namelist(head)
	struct cf_namelist *head;
{
	struct cf_namelist *ifp, *ifp_next;

	for (ifp = head; ifp; ifp = ifp_next) {
		ifp_next = ifp->next;
		cleanup_cflist(ifp->params);
		free(ifp->name);
		free(ifp);
	}
}

static void
cleanup_cflist(p)
	struct cf_list *p;
{
	struct cf_list *n;

	if (p == NULL)
		return;

	n = p->next;
	if (p->type == DECL_ADDRESSPOOL) {
		free(((struct dhcp6_poolspec *)p->ptr)->name);
	}
	if (p->ptr)
		free(p->ptr);
	if (p->list)
		cleanup_cflist(p->list);
	free(p);

	cleanup_cflist(n);
}

#define config_fail() \
	do { cleanup(); configure_cleanup(); return (-1); } while(0)

int
cf_post_config()
{
	if (configure_keys(keylist_head))
		config_fail();

	if (configure_authinfo(authinfolist_head))
		config_fail();

	if (configure_ia(iapdlist_head, IATYPE_PD))
		config_fail();

	if (configure_ia(ianalist_head, IATYPE_NA))
		config_fail();

	if (configure_pool(addrpoollist_head))
		config_fail();

	if (configure_profile(profilelist_head))
		config_fail();

	if (configure_interface(iflist_head))
		config_fail();

	if (configure_host(hostlist_head))
		config_fail();

	if (configure_global_option())
		config_fail();

	configure_commit();
	cleanup();
	return (0);
}
#undef config_fail

void
cf_init()
{
	iflist_head = NULL;
	profilelist_head = NULL;
}

