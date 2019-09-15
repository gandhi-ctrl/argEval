#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "argEval.h"


struct config_t{
	char*  outFileName;
	char*  defaultValue;
	int    iValue;
	float  fValue;
	double dValue;
	int    verbose;
} _config = {
	.outFileName = NULL,
	.defaultValue = "no Value",
	.iValue = 200,
	.fValue = M_PI,
	.dValue = M_E,
	.verbose = 0,
};

int setOutFileFunction(int argc, char** argv);
int extraArgumentFunction(int argc, char** argv);

 /* The ArgumentDefinition_t structure
 ************************************
 *
 * 1: String for the short option (eg: program -v [...])
 * 2: String for the long option (eg: program --verbose [...])
 * 3: Help string - a string that describes what the option does (maybe even includes the default value)
 * 4: Number of parameters that follow the option
 * 5: Type of the parameters that follow the option. This can be one of the following:
 *         vType_none       - This is for the case when no parameter are following the option or any parameter should get ignored
 *         vType_callback   - This is for the case when the programmer wants to make it's own parameter evaluation routine. Must be followed by a pointer to a routine of typedef int (*argCallback_t)(int argc, char** argv).
 *         vType_string     - This is for the case when the option expects a string. Must be followed by a pointer to a string variable.
 *         vType_integer    - This is for the case when the option expects an integer. Must be followed by a pointer to an integer variable.
 *         vType_float      - This is for the case when the option expects a float. Must be followed by a pointer to an float variable.
 *         vType_double     - This is for the case when the option expects a double. Must be followed by a pointer to an double variable.
 * 6: The pointer specified by the last field. Can be NULL in case of vType_none
 * 7: This field expects a pointer to an integer variable which will count how many times the option has been put in the command line. This can be NULL if not used.
 */

static ArgumentDefinition_t _argArray[] = {
/*	  1 |  2       |  3                                            | 4| 5             | 6                    | 7      */
/*	------------------------------------------------------------------------------------------------------------------*/
	{"h" , "help"   , "shows this screen"                           , 0, vType_none,     NULL,                  NULL},
	{"i" , "inFile" , "specifies an input file"                     , 1, vType_string,   NULL,                  NULL},
	{"o" , "outFile", "specifies an output file"                    , 1, vType_callback, setOutFileFunction,    NULL},
	{"d" , "default", "default value in help screen (default: {})"  , 1, vType_string,   &_config.defaultValue, NULL},
	{NULL, "integer", "a generic interger value (default: {})"      , 1, vType_integer,  &_config.iValue,       NULL},
	{NULL, "float",   "value for PI (default: {})"                  , 1, vType_float,    &_config.fValue,       NULL},
	{NULL, "double",  "value for e (default: {})"                   , 1, vType_double,   &_config.dValue,       NULL},
	{"v" , "verbose", "enables debug messages"                      , 0, vType_none,     NULL,                  &_config.verbose},
};

int main(int argc, char** argv)
{
	char* inFileName = NULL;

	_argArray[1].variablePointer = &inFileName;

	argEval_enableHelpWithoutArguments();
	argEval_registerCallbackForExtraArguments(extraArgumentFunction);
	argEval_registerArguments(_argArray, sizeof (_argArray) / sizeof (ArgumentDefinition_t), &_argArray[0]);
	argEval_Parse(argc, argv, stderr);

	printf("Input File = %s\n", inFileName);
	printf("Output File= %s\n", _config.outFileName);
	if(_config.verbose != 0){
		printf("Debug Messages enabled...\n");
	}

	return (EXIT_SUCCESS);
}

int setOutFileFunction(int argc, char** argv){
	_config.outFileName = argv[0];
	return 0;
}

int extraArgumentFunction(int argc, char** argv){
	printf("Got an extra Argument: %s\n", argv[0]);
	return -1;
}