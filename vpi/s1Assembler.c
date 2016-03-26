#include <vpi_user.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "parser.h"

void dumpVpiHandle(vpiHandle obj) {
}

static int s1AssemblerCompiletf(char*user_data)
{
  vpiHandle systf_handle;
  vpiHandle arg_itr;
  vpiHandle arg_handle;
  PLI_INT32 tfarg_type;
  int err_flag = 0;

  systf_handle = vpi_handle(vpiSysTfCall, NULL);
  arg_itr = vpi_iterate(vpiArgument, systf_handle);
  if (arg_itr == NULL) {
    vpi_printf("ERROR: $s1Assember requires 1 argument. has none.\n");
    err_flag = 1;
  }
  arg_handle = vpi_scan(arg_itr);
  tfarg_type = vpi_get(vpiType, arg_handle);
  if (tfarg_type != vpiConstant &&
      tfarg_type != vpiStringVal &&
      tfarg_type != vpiReg) {
    vpi_printf("$s1Assember argument must be a string.\n");
    vpi_printf("            tfarg_type is %0d\n", tfarg_type);
    err_flag = 1;
  }
  if (vpi_scan(arg_itr) != NULL) {
    vpi_printf("ERROR: $s1Assember requires 1 argument. has too many.\n");
  }

  if (err_flag) {
    vpi_control(vpiFinish, 1);
  }

  return 0;
}

static int s1AssemblerCalltf(char*user_data)
{
  s_vpi_value value_s;
  vpiHandle systf_handle;
  vpiHandle arg_itr;
  vpiHandle arg_handle;
  char *s;

  systf_handle = vpi_handle(vpiSysTfCall, NULL);
  if ((arg_itr = vpi_iterate(vpiArgument, systf_handle)) == NULL) {
    vpi_printf("ERROR: $s1Assembler failed to obtain systf arg handle\n");
    return(0);
  }

  arg_handle = vpi_scan(arg_itr);
  value_s.format = vpiStringVal;
  vpi_get_value(arg_handle, &value_s);
  
  //vpi_printf("hello string size: %0d\n", (int)strlen(value_s.value.str));
  s = calloc(strlen(value_s.value.str), sizeof(char));
  memset(s, '\0',strlen(value_s.value.str));
  strncpy(s,value_s.value.str, strlen(value_s.value.str));
  vpi_printf("[ s1Assembler ] argument: %s\n", s);

  {
    parserData *parser;
    parser = ParserNew(s);
    ParserStart(parser);
    ParserDelete(&parser);
  }

  free(s);  
  return 0;
}

void s1Assembler_register()
{
  s_vpi_systf_data tf_data;

  tf_data.type      = vpiSysTask;
  tf_data.sysfunctype = vpiSysFuncSized;
  tf_data.tfname    = "$s1Assembler";
  tf_data.calltf    = s1AssemblerCalltf;
  tf_data.compiletf = s1AssemblerCompiletf;
  tf_data.sizetf    = 0;
  tf_data.user_data = 0;
  vpi_register_systf(&tf_data);
}

void (*vlog_startup_routines[])() = {
  s1Assembler_register,
  0
};
