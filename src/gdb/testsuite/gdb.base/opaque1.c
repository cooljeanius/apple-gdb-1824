struct foo {
    int a;
    int b;
} afoo = { 1, 2};

struct foo *
getfoo(void)
{
    return (&afoo);
}

#ifdef PROTOTYPES
void putfoo (struct foo *foop)
#else
void putfoo (foop)
     struct foo *foop;
#endif
{
}
