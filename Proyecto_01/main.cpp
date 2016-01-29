#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>
#include <fstream>

//------------------- Variables Globales ------------ //
std::string estadoActual;
std::string estadoAnterior;

//Declaración de la tabla de transiciones
std::string tablaTrans[14][13] = {
                                    {";", "0..9",  ".",   "+",   "-",   "*",   "/",   "=",   "az", "(",   ")",   "^",   " "},
                                    {"q0", "q1",   "q2",  "q3",  "q4",  "_",   "_",   "_",   "q9",   "q10", "_",   "_",   "_"},
                                    {"q1", "q1",   "q2",  "q3",  "q4",  "q6",  "q7",  "q5",  "q9",   "q10", "q11", "q8",  "_"},
                                    {"q2", "q2",   "_",   "q3",  "q4",  "q6",  "q7",  "q5",  "_",    "q10", "q11", "q8",  "_"},
                                    {"q3", "q1",   "q2",  "_",   "_",   "_",   "_",   "_",   "q9",   "q10", "_",   "_",   "q3"},
                                    {"q4", "q1",   "q2",  "_",   "_",   "_",   "_",   "_",   "q9",   "q10", "_",   "_",   "q4"},
                                    {"q5", "q1",   "q2",  "q3",  "q4",  "_",   "_",   "q5",  "q9",   "q10", "_",   "_",   "q5"},
                                    {"q6", "q1",   "q2",  "_",   "_",   "_",   "_",   "_",   "q9",   "q10", "_",   "_",   "q6"},
                                    {"q7", "q1",   "q2",  "_",   "_",   "_",   "q12", "_",   "q9",   "q10", "_",   "_",   "q7"},
                                    {"q8", "q1",   "q2",  "_",   "_",   "_",   "_",   "_",   "q9",   "q10", "_",   "_",   "q8"},
                                    {"q9", "q9",   "_",   "q3",  "q4",  "q6",  "q7",  "q5",  "q9",   "q10", "q11", "q8",  "q9"},
                                    {"q10", "q1",  "q2",  "q3",  "q4",  "_",   "_",   "_",   "q9",   "q10", "q11", "_",   "q10"},
                                    {"q11", "q1",  "q2",  "q3",  "q4",  "q6",  "q7",  "q5",  "q9",   "q10", "q11", "q8",  "q11"},
                                    {"q12", "q12", "q12", "q12", "q12", "q12", "q12", "q12", "q12",  "q12", "q12", "q12", "q12"}
                                 };

//------- Funciones ---------//
void evualuarChar(std::string x);

int main()
{

  //Declaración de el mapa con los estados para saber que tipo de token representa cada estado
  std::map<std::string, std::string> estados;
  estados["q0"] = "Inicio";
  estados["q1"] = "Entero";
  estados["q2"] = "Real";
  estados["q3"] = "Mas";
  estados["q4"] = "Menos";
  estados["q5"] = "Igual";
  estados["q6"] = "Multiplicación";
  estados["q7"] = "Divisón";
  estados["q8"] = "Potencia";
  estados["q9"] = "Variable";
  estados["q10"] = "Paréntesis Abre";
  estados["q11"] = "Paréntesis Cierra";
  estados["q12"] = "Comentario";


  //Declaración del string que se va a evaluar
  std::string s;

  //Se abre el archivo
  std::ifstream myfile ("test.txt");
  if (myfile.is_open())
  {
    //Ciclo que se ejecuta por cada linea de el archivo de texto
    while ( getline (myfile,s) )
    {
      //Set inicial de los estados actuales
      estadoActual = "q0";
      estadoAnterior = "q0";

      //Tamaño de la expresión a evaluar
      int size = s.size();

      //Se pone ese caracter en el string que se va a imprimir
      std::string e;
      char r = s.at(0);
      std::string stringToPrint;
      std::stringstream aa;
      aa << r;
      aa >> stringToPrint;

      //Variables booleanas para control de la aplicación
      bool first = true;
      bool expErr =false;
      bool isComment =false;

      //Ciclo que se ejecuta durante toda la expresión a evaluar
      for(int i = 0; i < size; i++)
      {
        //Se saca el caracter a evaluar y se convierte a string
        char c = s.at(i);
        std::stringstream ss;
        e = "";
        ss << c;
        ss >> e;

        //Se evalua el caracter para ver a que estado mueve el automata
        evualuarChar(e);

        //Detecta si la expresión llegó a un punto que no se pueda evaluar
        if(estadoActual == "_")
        {
          std::cout << "Expresión Erronea" << std::endl;
          expErr = true;
          break;
        }

        // Detecta si se inició un comentario
        if(estadoActual == "q12")
        {
          isComment = true;
        }

        //Ya solo empieza a guardar el comentario
        if (isComment)
        {
          estadoActual = "q12";
          if (!first)
          {
            stringToPrint.append(e);
          }
        }
        else if(estadoAnterior == "q1" && estadoActual == "q2") //Cambio de numero entero a numero real
        {
          if (!first)
          {
            stringToPrint.append(e);
          }
        }
        else if (estadoAnterior == "q7" && estadoActual == "q12") //Cambio de división a comentario
        {
          if (!first)
          {
            stringToPrint.append(e);
          }
        }
        else if(estadoActual != estadoAnterior) //Revisa que haya habido un cambio de estado para imprimir
        {
          // Imprime los caracteres acumulados
          std::cout << stringToPrint << " ----> " << estados.find(estadoAnterior)->second << std::endl;
          stringToPrint = "";
          stringToPrint.append(e);
        }
        else
        {
          //En caso de que no imprima solo sigue acumulando caracteres en una variable hasta que se cambie de estado
          if (!first)
          {
            stringToPrint.append(e);
          }
        }
        first = false;

      }

      //Se imprime el caracter final
      if (!expErr)
      {
        std::cout << stringToPrint << " ----> " << estados.find(estadoActual)->second << std::endl;
        stringToPrint = "";
      }
      std::cout << "----------------" << std::endl;
    }
    myfile.close(); //Se cierra el archivo de texto
  }
  else
  {
    std::cout << "Unable to open file" << std::endl;
  }




  return 0;
}

//Función que evalua el caracter y hace los cambios de estado
void evualuarChar(std::string x)
{
  int columna = 0;
  int fila = 0;

  //Revisa si se ingresó un espacio
  if (x.empty())
  {
    columna = 12;
  }
  else if (x == "0" || x == "2" || x == "2" || x == "3" || x == "4" || x == "5" || x == "6" || x == "7" || x == "8" || x == "9")
  {
    //Pone la columna en 1 si se detecto un número
    columna = 1;
  }
  else if (x != "+" && x != "-" && x != "*" &&  x != "/" && x != "=" && x != "(" && x != ")" && x != "^" && x != "." && x != " ")
  {
    //pone la columna en 8 si se detecta que es una letra (Variable)
    columna = 8;
  }
  else
  {
    columna = 0; //En caso de que sea un operador se pone la columna en 0
  }

  if (columna == 0)
  {
    //Ciclo que revisa cuál fué el operador que se ingresó
    for(int i = 0; i < 13; i++)
    {
      std::string temp = "";
      temp = tablaTrans[0][i];
      if (x == temp)
      {
        columna = i;
        break;
      }
    }
  }

  //Ys que se sabe cuál fue la entrada se revisa cuál es el estado actual
  //Una vez que se encuentra el estado actual se ve a que estado transiciona con el token que se ingresó
  for(int i = 0; i < 13; i++)
  {
    std::string temp = "";
    temp = tablaTrans[i][0];
    if (estadoActual == temp)
    {
      fila = i;
      break;
    }
  }

  //Asigna los nuevos estados
  estadoAnterior = estadoActual;
  estadoActual = tablaTrans[fila][columna];
  if(estadoAnterior == "q0")
  {
    estadoAnterior = estadoActual;
  }
}
