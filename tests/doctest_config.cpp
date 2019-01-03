/*******************************************************************************
module:   doctest_config
author:   digimokan
date:     08 AUG 2017 (created)
purpose:  main on/off switch for using the doctest testing submodule
*******************************************************************************/

/*******************************************************************************
 * DOCTEST MAIN ON/OFF SWITCH
 ******************************************************************************/

// create command-line "doctest" executable that runs all test cases
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

/*******************************************************************************
 * DOCTEST CONFIG OPTIONS
 ******************************************************************************/

// undefine normal CHECK_EQ/WARN_GT/etc macros

#define DOCTEST_CONFIG_NO_SHORT_MACRO_NAMES

// make fast-compile "FAST_CHECK" macros compile even faster
// SUPER_FAST side effects: debuggers may see extra stack layers

#define DOCTEST_CONFIG_SUPER_FAST_ASSERTS

// alias the FAST macros to use the normal macro names
// FAST side effects: when one expr fails, the whole test case ends

#define CHECK_UNARY_FALSE   DOCTEST_FAST_CHECK_UNARY_FALSE
#define CHECK_UNARY         DOCTEST_FAST_CHECK_UNARY
#define CHECK_EQ            DOCTEST_FAST_CHECK_EQ
#define CHECK_NE            DOCTEST_FAST_CHECK_NE
#define CHECK_GT            DOCTEST_FAST_CHECK_GT
#define CHECK_GE            DOCTEST_FAST_CHECK_GE
#define CHECK_LT            DOCTEST_FAST_CHECK_LT
#define CHECK_LE            DOCTEST_FAST_CHECK_LE

#define REQUIRE_UNARY_FALSE DOCTEST_FAST_REQUIRE_UNARY_FALSE
#define REQUIRE_UNARY       DOCTEST_FAST_REQUIRE_UNARY
#define REQUIRE_EQ          DOCTEST_FAST_REQUIRE_EQ
#define REQUIRE_NE          DOCTEST_FAST_REQUIRE_NE
#define REQUIRE_GT          DOCTEST_FAST_REQUIRE_GT
#define REQUIRE_GE          DOCTEST_FAST_REQUIRE_GE
#define REQUIRE_LT          DOCTEST_FAST_REQUIRE_LT
#define REQUIRE_LE          DOCTEST_FAST_REQUIRE_LE

#define WARN_UNARY_FALSE    DOCTEST_FAST_WARN_UNARY_FALSE
#define WARN_UNARY          DOCTEST_FAST_WARN_UNARY
#define WARN_EQ             DOCTEST_FAST_WARN_EQ
#define WARN_NE             DOCTEST_FAST_WARN_NE
#define WARN_GT             DOCTEST_FAST_WARN_GT
#define WARN_GE             DOCTEST_FAST_WARN_GE
#define WARN_LT             DOCTEST_FAST_WARN_LT
#define WARN_LE             DOCTEST_FAST_WARN_LE

/*******************************************************************************
 * DOCTEST SINGLE-HEADER LIB (MUST GO LAST)
 ******************************************************************************/

#include "doctest.h"

