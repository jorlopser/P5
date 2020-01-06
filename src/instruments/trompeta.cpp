#include <iostream>
#include <math.h>
#include "trompeta.h"
#include "keyvalue.h"

#include <stdlib.h>

using namespace upc;
using namespace std;

/* el constructor declara el instrumento como inactivo y ubica el espacio para
almacenar un tick de señal */
Trompeta::Trompeta(const std::string &param) 
  : adsr(SamplingRate, param) {
  bActive = false;
  x.resize(BSIZE);

  /*
    You can use the class keyvalue to parse "param" and configure your instrument.
    Take a Look at keyvalue.h    
  */

  // analizamos la cadena de parámetros localizando los de interés para el instrumento
  KeyValue kv(param);
  int N;

  if (!kv.to_int("N",N))
    N = 40; //default value
  
  //Create a tbl with one period of a sinusoidal wave
  tbl.resize(N);
  float phase = 0, step = 2 * M_PI /(float) N;
  index = 0;
  for (int i=0; i < N ; ++i) {
    tbl[i] = sin(phase);
    phase += step;
  }
}

/* Cada vez que el programa encuentra un comando MIDI en el fichero score, 
invoca al método command() de la clase, que toma por argumentos los campos del fichero score, 
esto es: el comando, la nota y la velocidad. */
void Trompeta::command(long cmd, long note, long vel) {
    // Si el comando es NoteOn (9), el método declara activo al instrumento
  if (cmd == 9) {		//'Key' pressed: attack begins
    bActive = true;
    adsr.start();
    F0 = (440*pow(2,((float)note - 69.0)/12.0));
		index = 0; // contador, para recorrer la tabla
    salto = (F0*tbl.size())/SamplingRate; 
	  A = vel / 127.; //amplitud
  }
  else if (cmd == 8) {	//'Key' released: sustain ends, release begins
    adsr.stop();
  }
  else if (cmd == 0) {	//Sound extinguished without waiting for release to end
    adsr.end();
  }
}

// síntesis de la señal
const vector<float> & Trompeta::synthesize() {
  // la curva ADSR ya ha llegado a su final
  if (not adsr.active()) {
    x.assign(x.size(), 0); //asignamos a la señal sintetizada el valor de 0
    bActive = false; //marcamos nota como inactiva
    return x;
  }
  // Si ya ha sido marcada como inactiva
  else if (not bActive)
    return x;

  // Si la nota está activa, el método realiza la síntesis
  for (unsigned int i=0; i<x.size(); ++i) {
    x[i] = A * tbl[index];
    index += salto;
    while (index >= tbl.size())
      index -= tbl.size();
  }
  adsr(x); //apply envelope to x and update internal status of ADSR

  return x;
}
