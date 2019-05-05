/* A Bison parser, made by GNU Bison 2.5.  */

/* Bison interface for Yacc-like parsers in C
   
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

/* Line 2068 of yacc.c  */
#line 130 "cfparse.y"

	long long num;
	char* str;
	struct cf_list *list;
	struct dhcp6_prefix *prefix;
	struct dhcp6_range *range;
	struct dhcp6_poolspec *pool;



/* Line 2068 of yacc.c  */
#line 215 "y.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


