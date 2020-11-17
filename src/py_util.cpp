#include "py_util.h"
#include "Python.h"
#include <stdio.h>
#define PFILE "nlp_util"
#define U_PFILE L"nlp_util"

static PyObject *tagger = nullptr;
static PyObject *sim = nullptr;

/**
 * Initializing python with specific file
 * **(Avoid using multiple times)**
 */
void PyUtils::py_init() {
  wchar_t *argv[1];
  argv[0] = U_PFILE;
  Py_Initialize();
  PySys_SetArgv(1, argv);
}

PyUtils::PyUtils()
{
  py_init();
}

/**
 * *Calling a function from python module*
 * **(In order to avoid segmentation fault make sure to use
 * `Py_Init();` once at the start of the program
 * and ending the program with `Py_Finalize();`)**
 */
PyObject* PyUtils::nlp_py_function(char *fname, const char *argument, const char *argument2,
                      int argc) {
  PyObject *pValue = nullptr;
  PyObject *pModule = nullptr;
  PyObject *pFunc = nullptr;
  PyObject *pArgs = nullptr;

  if (!sim || !tagger)
  {
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
    Py_XDECREF(pModule);

    if (!strcmp(fname,"compare"))
		  sim = pFunc;
    else
      tagger = pFunc;     
  }
  if (!(argc - 1))
    pArgs = Py_BuildValue("(s)", argument);
  else if (argc - 1)
    pArgs = Py_BuildValue("(ss)", argument, argument2);

  if (!strcmp(fname,"compare") )
		pFunc = sim;
  else
    pFunc = tagger;
  
  /* Calls for function returns to pValue*/
    
    pValue = PyObject_CallObject(pFunc, pArgs);
  if (!pValue) {
    PyErr_Print();
    exit(1);
  }
  Py_XDECREF(pArgs);
  return pValue;
}

PyUtils::~PyUtils()
{
  Py_Finalize();
}

std::shared_ptr<PyUtils> PyUtils::get_py_util()
{
  if (nullptr == _py_util)
  {
    PyUtils* raw_singelton_ptr = new PyUtils();
    _py_util = std::shared_ptr<PyUtils>(raw_singelton_ptr);
  }

  return _py_util;
}