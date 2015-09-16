static void func1(void (*f)(void))
{
  f();
}
static void func2(void (*f)(void))
{
  func1(f);
}
int main()
{
  func2(0);
}

