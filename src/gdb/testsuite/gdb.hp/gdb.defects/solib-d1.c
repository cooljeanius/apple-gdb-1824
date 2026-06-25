static int file_static = 1234;
int global_from_primary = 5678;

int
function_from_primary(void)
{
    garbage();	
}

int
force_generation_of_export_stub(void)
{
    _start();   /* force main module to have an export stub for _start() */
}
