
#pragma once

// Substitution for server side pg_type.h
// until it's clear what the fuzz is about!

#ifndef	OIDOID

#define BOOLOID		    16
#define BYTEAOID	    17
#define INT2OID		    21
#define INT4OID		    23
#define TEXTOID		    25
#define OIDOID	    	26

#define CHAROID         18
#define NAMEOID         19
#define INT8OID		    20
#define INT2VECTOROID	22
#define REGPROCOID	    24
#define TIDOID		    27
#define XIDOID		    28
#define CIDOID		    29
#define OIDVECTOROID	30
#define PGTYPEOID	    71
#define PGATTRIBUTEOID	75
#define PGPROCOID	    81
#define PGCLASSOID      83

// These are not in pg_type.h but are needed for the OID values
// Could be in pg_type.h but that would require including it in dbparam.hpp which is not ideal
// Compiled from pg_type.h in PostgreSQL 15.5 (although we have PostgreSQL 14 installed, these OIDs are the same in both versions)
// Copilot is not able to find these in pg_type.h but they are needed for the OID values, so they are added here as a workaround
// Copilot is suggesting a lot  of other OIDs that are not needed, so they are not included here
// Copilot is suggesting a lot of strange text, but the pg_class OID is actually 83, not 78, so that is corrected here as well
//#define PGCLASSOID    78

#define JSONOID		    114
#define XMLOID		    142

#define VARCHAROID	    1043
#define BPCHAROID	    1042
#define FLOAT4OID	    700
#define FLOATOID        700
#define FLOAT8OID	    701
#define DOUBLEOID       701
#define TIMESTAMPTZOID	1184
#define TIMESTAMPOID	1114
#define TIMETZOID	    1266
#define TIMEOID		    1083
#define DATEOID		    1082

#define DECIMALOID	    1700
#define NUMERICOID	    1700
#define JSONBOID	    3802
#define UUIDOID		    2950

#endif

