# include  <vpi_user.h>

void dumpVpiHandle(vpiHandle obj) {
  
}

static int vscaleAssembler_compiletf(char*user_data)
{
      return 0;
}

static int vscaleAssembler_calltf(char*user_data)
{
      vpi_printf("vscaleAssembler, World!\n");
      vpi_printf("%s\n", user_data);
      return 0;
}

void vscaleAssembler_register()
{
      s_vpi_systf_data tf_data;

      tf_data.type      = vpiSysTask;
      tf_data.tfname    = "$vscaleAssembler";
      tf_data.calltf    = vscaleAssembler_calltf;
      tf_data.compiletf = vscaleAssembler_compiletf;
      tf_data.sizetf    = 0;
      tf_data.user_data = 0;
      vpi_register_systf(&tf_data);
}

void (*vlog_startup_routines[])() = {
    vscaleAssembler_register,
    0
};
