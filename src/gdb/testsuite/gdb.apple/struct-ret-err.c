struct xx { int x; int y; };

struct xx
f(void)
{
  struct xx x = { 3, 4 };
  return x;
}

int
main(void)
{
  struct xx l = f ();
}
