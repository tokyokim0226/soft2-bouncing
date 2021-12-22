#include <stdio.h>


typedef struct student
{
  int id;
  char name[100];
  int age;
  double height;
  double weight;
} Student;

typedef struct tagged_student1
{
  int id;
  char name[100];
  int age;
  double height;
  double weight;
  char tag;
} TStudent1;

typedef struct tagged_student2
{
  char tag;
  int id;
  char name[100];
  int age;
  double height;
  double weight;
} TStudent2;

void print_student(char var[], const struct student *s);
void print_tagged_student1(char var[], const struct tagged_student1 *s);
void print_tagged_student2(char var[], const struct tagged_student2 *s);



int main (int argc, char**argv)
{
  Student s_begin;
  Student s1;
  TStudent1 s2;
  TStudent2 s3;
  TStudent2 s_end;
  
  /* 以下に各構造体の先頭アドレスを表示するプログラムを書く */
  /* Hint: printf で %p フォーマットを指定する*/
  /* 逆順に表示（s_end, s3, s2, s1, s_begin の順）*/

  printf("\naddress\n%16s %16s %16s %16s %16s\n", "s_end", "s3", "s2", "s1", "s_begin");
  printf("%16p %16p %16p %16p %16p\n\n", &s_end, &s3, &s2, &s1, &s_begin);

  /* 以下には各構造体のサイズをsizeof演算子で計算し、表示する */
  /* printf 表示には%zu を用いる*/

  printf("sizeof\n%16s %16s %16s\n", "student", "tagged_student1", "tagged_student2");
  printf("%16ld %16ld %16ld\n\n", sizeof(struct student), sizeof(struct tagged_student1), sizeof(struct tagged_student2));
  
  print_tagged_student2("s_end", &s_end);
  print_tagged_student2("s3", &s3);
  print_tagged_student1("s2", &s2);
  print_student("s1", &s1);
  print_student("s_begin", &s_begin);
  printf("\n\n");

  return 0;
}


void print_student(char var[], const struct student *s) {
  printf("%8s: %p %p %p %p %p\n", var, &s->id, &s->name, &s->age, &s->height, &s->weight);
}

void print_tagged_student1(char var[], const struct tagged_student1 *s) {
  printf("%8s: %p %p %p %p %p %p\n", var, &s->id, &s->name, &s->age, &s->height, &s->weight, &s->tag);
}

void print_tagged_student2(char var[], const struct tagged_student2 *s) {
  printf("%8s: %p %p %p %p %p %p\n", var, &s->tag, &s->id, &s->name, &s->age, &s->height, &s->weight);
}