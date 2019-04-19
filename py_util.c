#include "py_util.h"
#include "Python.h"
#include <stdio.h>
#define PFILE "nlp_util"
#define U_PFILE L"nlp_util"



/**
 * Initializing python with specific file
 * **(Avoid using multiple times)**
 */
void Py_Init() {
  wchar_t *argv[1];
  argv[0] = U_PFILE;
  Py_Initialize();
  PySys_SetArgv(1, argv);
}

/**
 * *Calling a function from python module*
 * **(In order to avoid segmentation fault make sure to use
 * `Py_Init();` once at the start of the program
 * and ending the program with `Py_Finalize();`)**
 */
PyObject *py_function(char *fname, const char *argument, const char *argument2,
                      int argc) {
  PyObject *pValue = NULL, *pModule = NULL, *pFunc, *pArgs = NULL;
  pModule = PyImport_Import(PyUnicode_FromString(PFILE));
  if (!pModule) {
    PyErr_Print();
    exit(1);
  }
  pFunc = PyObject_GetAttrString(pModule, fname);
  if (!pFunc) {
    PyErr_Print();
    exit(1);
  }
  Py_DECREF(pModule);
  if (!(argc - 1))
    pArgs = Py_BuildValue("(s)", argument);
  else if (argc - 1)
    pArgs = Py_BuildValue("(ss)", argument, argument2);
  /* Calls for function returns to pValue*/
  pValue = PyObject_CallObject(pFunc, pArgs);
  if (!pValue) {
    PyErr_Print();
    exit(1);
  }
  Py_DECREF(pArgs);
  Py_DECREF(pFunc);
  return pValue;
}
#ifdef INIT
#error ("Multiple use of Py_Init")
#endif
