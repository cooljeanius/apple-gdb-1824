void
hello(void)
{
  char *hello = "Hello \\\"!\r\n";
  int i;
  for (i = 0; hello[i]; i++)
    write (1, hello + i, 1);
}

int
main(void)
{
  hello ();
}
/*
Local variables: 
change-log-default-name: "ChangeLog-mi"
End: 
*/

