void
x(void)
{
  void (*fp)() = 0;
  fp();
}

int
main(void)
{
  x();
  return 0;
}
