int foo();
int bar();
int baz(int);

int
main(void)
{
  /* Use comma operator to sequence evaluation of bar and foo. */
  return baz((bar(), foo()));
}

int
foo(void)
{
  return 0;
}
