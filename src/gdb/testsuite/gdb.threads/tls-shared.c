__thread int i_tls = 1;
int
foo(void)
{
  return i_tls;
}

