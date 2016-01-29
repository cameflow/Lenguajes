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



  std::string s;
  std::ifstream myfile ("test.txt");
  if (myfile.is_open())
  {
    while ( getline (myfile,s) )
    {
      estadoActual = "q0";
      estadoAnterior = "q0";
      int size = s.size();
      std::string e;
      char r = s.at(0);
      std::string stringToPrint;
      std::stringstream aa;
      aa << r;
      aa >> stringToPrint;
      bool first = true;
      bool expErr =false;
      bool isComment =false;
      for(int i = 0; i < size; i++)
      {
        char c = s.at(i);
        std::stringstream ss;
        e = "";
        ss << c;
        ss >> e;
        evualuarChar(e);
        if(estadoActual == "_")
        {
          std::cout << "Expresión Erronea" << std::endl;
          expErr = true;
          break;
        }
        if(estadoActual == "q12")
        {
          isComment = true;
        }
        if (isComment)
        {
          estadoActual = "q12";
          if (!first)
          {
            stringToPrint.append(e);
          }
        }
        else if(estadoAnterior == "q1" && estadoActual == "q2")
        {
          if (!first)
          {
            stringToPrint.append(e);
          }
        }
        else if (estadoAnterior == "q7" && estadoActual == "q12")
        {
          if (!first)
          {
            stringToPrint.append(e);
          }
        }
        else if(estadoActual != estadoAnterior)
        {
          // std::cout << "Entra" << std::endl;
          std::cout << stringToPrint << " ----> " << estados.find(estadoAnterior)->second << std::endl;
          stringToPrint = "";
          stringToPrint.append(e);
        }
        else
        {
          if (!first)
          {
            stringToPrint.append(e);
          }
        }
        first = false;

      }
      if (!expErr)
      {
        std::cout << stringToPrint << " ----> " << estados.find(estadoActual)->second << std::endl;
        stringToPrint = "";
      }
      std::cout << "----------------" << std::endl;
    }
    myfile.close();
  }
  else
  {
    std::cout << "Unable to open file" << std::endl;
  }




  return 0;
}

void evualuarChar(std::string x)
{
  int columna = 0;
  int fila = 0;
  if (x.empty())
  {
    columna = 12;
  }
  else if (x == "0" || x == "2" || x == "2" || x == "3" || x == "4" || x == "5" || x == "6" || x == "7" || x == "8" || x == "9")
  {
    columna = 1;
  }
  else if (x != "+" && x != "-" && x != "*" &&  x != "/" && x != "=" && x != "(" && x != ")" && x != "^" && x != "." && x != " ")
  {
    columna = 8;
  }
  else
  {
    columna = 0;
  }

  if (columna == 0)
  {
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

  estadoAnterior = estadoActual;
  estadoActual = tablaTrans[fila][columna];
  if(estadoAnterior == "q0")
  {
    estadoAnterior = estadoActual;
  }
}
