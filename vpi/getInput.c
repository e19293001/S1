#include <vpi_user.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "parser.h"

void dumpVpiHandle(vpiHandle obj) {
}

static int getInputCompiletf(char*user_data)
{
  vpiHandle systf_handle;
  vpiHandle arg_itr;
  int err_flag = 0;

  systf_handle = vpi_handle(vpiSysTfCall, NULL);
  arg_itr = vpi_iterate(vpiArgument, systf_handle);
  if (arg_itr != NULL) {
    vpi_printf("ERROR: $getInput no arguments required.\n");
    err_flag = 1;
  }

  if (err_flag) {
    vpi_control(vpiFinish, 1);
  }

  return 0;
}

static int getInputCalltf(char*user_data)
{
  s_vpi_value value_s;
  vpiHandle systf_handle;

  char numInput;

  systf_handle = vpi_handle(vpiSysTfCall, NULL);

  scanf("%c", &numInput);

  value_s.format = vpiIntVal;
  value_s.value.integer = (PLI_INT32) numInput;

  vpi_put_value(systf_handle, &value_s, NULL, vpiNoDelay);
  
  return 0;
}

void getInput_register()
{
  s_vpi_systf_data tf_data;

  tf_data.type      = vpiSysFunc;
  tf_data.sysfunctype = vpiSysFuncSized;
  tf_data.tfname    = "$getInput";
  tf_data.calltf    = getInputCalltf;
  tf_data.compiletf = getInputCompiletf;
  tf_data.sizetf    = 0;
  tf_data.user_data = 0;
  vpi_register_systf(&tf_data);
}

void (*vlog_startup_routines[])() = {
  getInput_register,
  0
};
