#include <stdlib.h>
#include <stdio.h>

typedef struct
{
  char* name;
  int matriculation_number;
  int semester; 
  char* program;
} student;

student*
create_student( char* name,
		int matriculation_number,
		int semester,
		char* program )
{
  student* new_student = malloc(sizeof(student)); // es wurde kein Speicherplatz für new_student allokiert
  new_student->name = name;
  new_student->matriculation_number = matriculation_number; // um bei einem struct-Pointer auf ein member zuzugreifen, benutzt man den -> Operator, nicht den . Operator
  new_student->semester = semester; // gleicher Fehler wie in der vorherigen Zeile
  new_student->program  = program;
  return new_student;
}

int main()
{
  student* s1 = create_student( "Max Mustermann", 424242, 1, "Computer Science" );
  free( s1 );
  printf("Bei den drei Fehlern handelte es sich um:\n");
  printf("Zeile 17 (Laufzeitfehler): Es wurde kein Speicher für new_student allokiert. Behebung durch malloc.\n");
  printf("Zeile 19 (Kompilierfehler): Für Zugriff auf members von struct pointers nutzt man den Pfeil-Operator ->.\n");
  printf("Der Punktoperator hingegen wird benutzt beim Zugriff auf members von structs (nicht Pointer auf structs!).\n");
  printf("Zeile 20 (Kompilierfehler): Gleicher Fehler wie in Zeile 19. Behebung ebenso mit Pfeil- statt Punktoperator.\n");
}

