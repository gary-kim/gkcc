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

int main () {
  fn();
  int a = 1;
  a = 1;
  if (a) {
    int b = 1;
    b = 1;
  } else {
    a = 2;
    y->e;
  }
}
