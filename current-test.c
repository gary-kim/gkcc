struct a {
  int b;
};

struct c {
  struct a *d;
};

struct e {
  struct f{
    int a;
  };

  struct f f;

  struct e *p;
};

struct e y;

int fn();
int printf(char *str);

int main () {
  fn();
  int a = 1;
HERE:
  a = 1;
  if (a) {
    int b = 1;
    b = 1;
    goto HERE;
  } else {
    a = 2;
    y->e;
  }

  switch (a) {
    case 1:
      printf("");
    case 2:
      printf("");
    default:
      printf("");
  }
}
